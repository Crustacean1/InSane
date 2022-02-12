#ifndef SCANNER_ENDPOINT
#define SCANNER_ENDPOINT

#include "../HttpProcessor/HttpMiddleware.h"
#include "HttpEndpoint.h"

class ScannerManager;
class JsonParser;

class ScannerEndpoint : public HttpEndpoint{
    ScannerManager & _scannerManager;
    public:
    ScannerEndpoint(ScannerManager& scannerManager);
    void httpPost(HttpMiddleware::HttpReq & req,HttpMiddleware::HttpRes& res) override;
    void httpPut(HttpMiddleware::HttpReq & req, HttpMiddleware::HttpRes & res) override;
    void httpGet(HttpMiddleware::HttpReq & req, HttpMiddleware::HttpRes & res) override;
};

#endif /*SCANNER_ENDPOINT*/