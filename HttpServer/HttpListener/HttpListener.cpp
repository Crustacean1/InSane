#include "HttpListener.h"
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/core.hpp>
#include <iostream>
#include "HttpSession.h"
#include "../HttpProcessor/ProcessorBuilder.h"

using namespace KHttp;

HttpListener::HttpListener(const ProcessorBuilder & procBuilder,boost::asio::io_context & ioc,boost::asio::ip::tcp::endpoint endpoint) : 
_ioc(ioc), _acceptor(boost::asio::make_strand(_ioc)), _processorBuilder(procBuilder){
    boost::beast::error_code ec;
    std::cout<<"Hijinks ensue: "<<std::endl;
    _acceptor.open(endpoint.protocol(),ec);
    if(ec){
        throw std::runtime_error("Failed to open acceptor");
    }
    _acceptor.set_option(boost::asio::socket_base::reuse_address(true), ec);
    if(ec){
        throw std::runtime_error("Failed to set sharing on socket");
    }
    _acceptor.bind(endpoint,ec);
    if(ec){
        throw std::runtime_error("Failed to bind to address");
    }
    _acceptor.listen(boost::asio::socket_base::max_connections,ec);
    if(ec){
        throw std::runtime_error("Failed to start listening");
    }
}
void HttpListener::run(){
    _acceptor.async_accept(boost::asio::make_strand(_ioc),
                        boost::beast::bind_front_handler(&HttpListener::onAccept,this));
}
void HttpListener::onAccept(boost::beast::error_code ec,boost::asio::ip::tcp::socket socket){
    
    if(ec){
        std::cerr<<"failed to establish conection: "<<ec<<std::endl;
        return;
    }
    std::cout<<"connection established"<<std::endl;
    std::make_shared<HttpSession>(std::move(socket),_processorBuilder)->run();
    run();
}