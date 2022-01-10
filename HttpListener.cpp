#include "HttpListener.h"
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/strand.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/core.hpp>
#include <iostream>

HttpListener::HttpListener(boost::asio::io_context & ioc,boost::asio::ip::tcp::endpoint endpoint) : _ioc(ioc), _acceptor(boost::asio::make_strand(_ioc)){
    boost::beast::error_code ec;
    _acceptor.open(endpoint.protocol(),ec);
    if(ec){
        throw std::runtime_error("Failed to open acceptor");
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
    
    std::cout<<"Connection established"<<std::endl;
    if(ec){
        return;
    }
    run(); 
}