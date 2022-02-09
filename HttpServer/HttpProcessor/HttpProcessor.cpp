#include "HttpProcessor.h"
#include <iostream>

void HttpProcessor::run(HttpMiddleware::HttpReq & req,HttpMiddleware::HttpRes & res){
    if(_middlewares.size()!=0){
        _counter = 0;
        _middlewares[0]->run(req,res,_nextFnc);
    }
    res.body() = "It's true";
    res.base().result(boost::beast::http::status::ok);
}

HttpProcessor::HttpProcessor() : 
    _nextFnc([this](HttpMiddleware::HttpReq& req,HttpMiddleware::HttpRes &res){
        this->next(req,res);
    })
{}

void HttpProcessor::next(HttpMiddleware::HttpReq & req,HttpMiddleware::HttpRes & res){
    if(!(_counter<_middlewares.size())){
        return;
    }
    _middlewares[_counter++]->run(req,res,_nextFnc);
}

ScannerHttpProcessor::ScannerHttpProcessor() : HttpProcessor(){
    addMiddleware<HttpExceptionMiddleware,std::ostream&>(std::cerr);
    addMiddleware<HttpDenierMiddleware>(std::vector<boost::beast::http::verb>{boost::beast::http::verb::post,
    boost::beast::http::verb::get,
    boost::beast::http::verb::options});
}