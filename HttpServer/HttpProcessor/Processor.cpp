#include "Processor.h"
#include <iostream>

using namespace KHttp;

void Processor::run(){
    if(_middlewares.size()!=0){
        _counter = 0;
        _middlewares[0]->run(_req,_res,_nextFnc);
    }
}

Processor::Processor(HttpMiddleware::HttpReq & req,HttpMiddleware::HttpRes & res) : 
    _req(req),_res(res),
    _nextFnc(std::bind(&Processor::next,this))
{}

void Processor::next(){
    if(!(_counter<_middlewares.size())){
        executeEndpoint(); 
        return;
    }
    _middlewares[_counter++]->run(_req,_res,_nextFnc);
}

void Processor::executeEndpoint(){
    Route reqRoute(std::string(_req.target()));
    for(const auto & endpoint : _endpoints){
        if(endpoint.first <= reqRoute){
            executeMethod(*endpoint.second,_req.method(),reqRoute.extractParams(endpoint.first));
        }
    }
    _res.base().result(boost::beast::http::status::not_found);
}

void Processor::executeMethod(HttpEndpoint & endpoint,const HttpMiddleware::HttpMethod & method, const std::vector<std::string>& pathParams){
    switch(method){
        case boost::beast::http::verb::get:
            std::cout<<"Get request received"<<std::endl;
            endpoint.httpGet(_req,_res,pathParams);
            return;
        case boost::beast::http::verb::post:
            std::cout<<"Post request received"<<std::endl;
            endpoint.httpPost(_req,_res,pathParams);
            return;
        case boost::beast::http::verb::put:
            std::cout<<"Put request received"<<std::endl;
            endpoint.httpPut(_req,_res,pathParams);
            return;
        case boost::beast::http::verb::delete_:
            std::cout<<"Delete request received"<<std::endl;
            endpoint.httpDelete(_req,_res,pathParams);
            return;
        default:
            _res.base().result(boost::beast::http::status::not_found);
            throw std::runtime_error("Can't handle method of this type");
    }
}
