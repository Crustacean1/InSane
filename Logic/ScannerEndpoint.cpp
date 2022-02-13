#include <iostream>
#include "ScannerEndpoint.h"
#include "../HttpServer/HttpEndpoint/JsonParser.h"
#include "../ScannerHandler/ScannerManager.h"

ScannerEndpoint::ScannerEndpoint(ScannerManager & scannerManager,std::unique_ptr<JsonParser> &&parser) : _scannerManager(scannerManager),_parser(std::move(parser)){}

void ScannerEndpoint::httpGet(HttpMiddleware::HttpReq &req,HttpMiddleware::HttpRes &res,std::vector<std::string> pathParams){
    auto args = _parser->parse(req.body());
    auto scannerId = args["scanner_id"].asString();
    Json::Value response;

    if(scannerId == ""){
        auto scanners = _scannerManager.getScannerList();
        for(const auto & scanner : scanners){
            response["devices"].append(scanner);
        }
        res.body() = _parser->parse(response);
    }else{
        auto scanner = _scannerManager.getScanner(scannerId);
        auto options = scanner->getOptions();
        for(const auto & option: options){
            response["options"].append(_parser->parse(option));
        }
        std::cout<<"all done"<<std::endl;
        res.body() = _parser->parse(response);
    }
    res.base().result(boost::beast::http::status::ok);
}

void ScannerEndpoint::httpPost(HttpMiddleware::HttpReq &req,HttpMiddleware::HttpRes &res,std::vector<std::string> pathParams){
    auto args = _parser->parse(req.body());
    auto scannerId = args["scanner_id"].asString();
    if(scannerId != ""){
        auto scanner = _scannerManager.getScanner(scannerId);
        if(scanner == nullptr){
            res.body() = _parser->parse(_parser->parse((std::string)"{\"details\":\"no such scanner found\"}"));
            return;
        }
    }
}

void ScannerEndpoint::httpPut(HttpMiddleware::HttpReq &req,HttpMiddleware::HttpRes &res,std::vector<std::string> pathParams){
    
}
