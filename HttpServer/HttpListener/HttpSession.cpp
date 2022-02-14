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
    _context.req = {};
    _tcpStream.expires_after(std::chrono::seconds(30));

    boost::beast::http::async_read(_tcpStream,
    _buffer,
    _context.req,
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
    std::cout<<"Received request of length: "<<length<<std::endl;
    if(ec){
        std::cerr<<"Error during read: "<<ec<<std::endl;
        return;
    }

    auto processor = _processorBuilder.build();
    processor->run(_context);
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
    _context.res,
    boost::beast::bind_front_handler(
        &HttpSession::onWrite,
        shared_from_this(),
        _context.res.need_eof())
    );
}

HttpSession::~HttpSession(){
    std::cout<<"Connection ended"<<std::endl;
}