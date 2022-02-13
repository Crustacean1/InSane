#include <iostream>
#include "HttpSession.h"
#include <chrono>
#include "../HttpProcessor/Processor.h"
#include "../HttpProcessor/ProcessorBuilder.h"

using namespace KHttp;

HttpSession::HttpSession(boost::asio::ip::tcp::socket && socket,const ProcessorBuilder & processorBuilder) : _tcpStream(std::move(socket)), 
_processorBuilder(processorBuilder){
}

void HttpSession::read(){
    _req = {};
    _tcpStream.expires_after(std::chrono::seconds(30));

    boost::beast::http::async_read(_tcpStream,
    _buffer,
    _req,
    boost::beast::bind_front_handler(
        &HttpSession::onRead,
        shared_from_this()));
}

void HttpSession::run(){
    boost::asio::dispatch(
        _tcpStream.get_executor(),
        boost::beast::bind_front_handler(
            &HttpSession::read,
            shared_from_this()));
}

void HttpSession::onRead(boost::beast::error_code ec,size_t length){
    std::cout<<"length of request: "<<length<<std::endl;
    if(ec){
        std::cerr<<"Error during read: "<<ec<<std::endl;
        return;
    }
    std::cout<<"headers: "<<std::endl;
    for(const auto &header : _req.base()){

        std::cout<<header.name()<<"\t"<<header.name_string()<<std::endl;
    }
    std::cout<<"body: "<<_req.body()<<std::endl;

    auto processor = _processorBuilder.build(_req,_res);
    processor->run();
    write();
}

void HttpSession::onWrite(bool close,boost::beast::error_code ec,size_t length){
    if(ec){
        std::cerr<<"Failed to write response: "<<ec<<std::endl;
        return;
    }
    if(!close){
        read();
    }
}

void HttpSession::write(){
    async_write(_tcpStream,
    _res,
    boost::beast::bind_front_handler(
        &HttpSession::onWrite,
        shared_from_this(),
        _res.need_eof())
    );
}

HttpSession::~HttpSession(){
    std::cout<<"Connection ended"<<std::endl;
}