#include "HttpMiddleware.h"
#include <chrono>
#include <ctime>
#include <iomanip>

void HttpDenierMiddleware::run(HttpReq & req,HttpRes &res,NextFnc & fnc){
    for(size_t i = 0;i<_verbs.size();++i){
        if(_verbs[i] == req.method()){
            fnc(req,res);
        }
    }
    res.base().result(boost::beast::http::status::not_found);
}

void HttpExceptionMiddleware::run(HttpReq & req,HttpRes & res,NextFnc &fnc){
    try{
        fnc(req,res);
    }catch(std::exception & e){
        auto currentTime = std::chrono::system_clock::now().time_since_epoch().count();
        _logStream<<std::put_time(std::localtime(&currentTime),"%F %T ")<<": "<<e.what()<<std::endl;
    }
}