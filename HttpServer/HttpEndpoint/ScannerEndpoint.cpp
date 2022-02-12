#include "ScannerEndpoint.h"

ScannerEndpoint::ScannerEndpoint(ScannerManager & scannerManager) : _scannerManager(scannerManager){}

void ScannerEndpoint::httpGet(HttpMiddleware::HttpReq &req,HttpMiddleware::HttpRes &res){
    res.body() = ("It's true");
}

void ScannerEndpoint::httpPost(HttpMiddleware::HttpReq &req,HttpMiddleware::HttpRes &res){
   std::cerr<<"So far so good"<<std::endl;
   throw std::runtime_error("Exception occured") ;
}

void ScannerEndpoint::httpPut(HttpMiddleware::HttpReq &req,HttpMiddleware::HttpRes &res){
    
}
