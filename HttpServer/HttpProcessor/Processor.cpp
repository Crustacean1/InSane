#include "Processor.h"
#include <iostream>

using namespace KHttp;

void Processor::run(HttpMiddleware::HttpReq & req,HttpMiddleware::HttpRes & res){
    if(_middlewares.size()!=0){
        _counter = 0;
        _middlewares[0]->run(req,res,_nextFnc);
    }
}

Processor::Processor() : 
    _nextFnc(std::bind(&Processor::next,this,std::placeholders::_1,std::placeholders::_2))
{}

void Processor::next(HttpMiddleware::HttpReq & req,HttpMiddleware::HttpRes & res){
    if(!(_counter<_middlewares.size())){
        executeEndpoint(req,res); 
        return;
    }
    _middlewares[_counter++]->run(req,res,_nextFnc);
}

void Processor::executeEndpoint(HttpMiddleware::HttpReq & req,HttpMiddleware::HttpRes & res){
    Route reqRoute(std::string(req.target()));
    for(const auto & endpoint : _endpoints){
        if(endpoint.first <= reqRoute){
            executeMethod(*endpoint.second,req.method(),req,res);
        }
    }
    res.base().result(boost::beast::http::status::not_found);
}

void Processor::executeMethod(HttpEndpoint & endpoint,const HttpMiddleware::HttpMethod &method,HttpMiddleware::HttpReq &req,HttpMiddleware::HttpRes & res){
    switch(method){
        case boost::beast::http::verb::get:
            std::cout<<"Get request received"<<std::endl;
            endpoint.httpGet(req,res);
            return;
        case boost::beast::http::verb::post:
            std::cout<<"Post request received"<<std::endl;
            endpoint.httpPost(req,res);
            return;
        case boost::beast::http::verb::put:
            std::cout<<"Put request received"<<std::endl;
            endpoint.httpPut(req,res);
            return;
        default:
            throw std::runtime_error("Can't handle method of this type");
    }
}
