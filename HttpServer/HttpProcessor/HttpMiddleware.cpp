#include "HttpMiddleware.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include "../HttpListener/HttpContext.h"

void HttpDenierMiddleware::run(HttpContext & context,NextFnc & next){
    for(size_t i = 0;i<_verbs.size();++i){
        if(_verbs[i] == context.req.method()){
            next(context);
            return;
        }
    }
    context.res.base().result(boost::beast::http::status::not_found);
}

void HttpExceptionMiddleware::run(HttpContext & context,NextFnc &next){
    try{
        next(context);
    }catch(std::exception & e){
        std::time_t exceptionTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        _logStream<<std::put_time(std::localtime(&exceptionTime),"%F %T")<<": "<<"Exception thrown: "<<e.what()<<std::endl;
        context.res.base().result(boost::beast::http::status::internal_server_error);
    }
}
void KeepAliveMiddleware::run(HttpContext & context,NextFnc &next){
    context.res.keep_alive(context.req.keep_alive());
    next(context);
    context.res.set(boost::beast::http::field::content_type,"application/json");
    context.res.prepare_payload();
}