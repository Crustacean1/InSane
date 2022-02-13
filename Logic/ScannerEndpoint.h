#ifndef SCANNER_ENDPOINT
#define SCANNER_ENDPOINT

#include "../HttpServer/HttpProcessor/HttpMiddleware.h"
#include "../HttpServer/HttpEndpoint/HttpEndpoint.h"

class ScannerManager;
class JsonParser;

class ScannerEndpoint : public HttpEndpoint{
    ScannerManager & _scannerManager;
    std::unique_ptr<JsonParser> _parser;
    public:
    ScannerEndpoint(ScannerManager& scannerManager,std::unique_ptr<JsonParser> &&_parser);
    void httpPost(HttpMiddleware::HttpReq & req,HttpMiddleware::HttpRes& res,std::vector<std::string> pathParams) override;
    void httpPut(HttpMiddleware::HttpReq & req, HttpMiddleware::HttpRes & res,std::vector<std::string> pathParams) override;
    void httpGet(HttpMiddleware::HttpReq & req, HttpMiddleware::HttpRes & res,std::vector<std::string> pathParams) override;
};

#endif /*SCANNER_ENDPOINT*/