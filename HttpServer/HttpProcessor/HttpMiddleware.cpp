#include "HttpMiddleware.h"
#include <chrono>
#include <ctime>
#include <iomanip>

void HttpDenierMiddleware::run(HttpReq & req,HttpRes &res,NextFnc & next){
    for(size_t i = 0;i<_verbs.size();++i){
        if(_verbs[i] == req.method()){
            next();
            return;
        }
    }
    res.base().result(boost::beast::http::status::not_found);
}

void HttpExceptionMiddleware::run(HttpReq & req,HttpRes & res,NextFnc &next){
    try{
        next();
    }catch(std::exception & e){
        std::time_t exceptionTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        _logStream<<std::put_time(std::localtime(&exceptionTime),"%F %T")<<": "<<"Exception thrown: "<<e.what()<<std::endl;
        res.base().result(boost::beast::http::status::internal_server_error);
    }
}