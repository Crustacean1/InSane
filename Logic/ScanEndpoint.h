#ifndef SCAN_ENDPOINT
#define SCAN_ENDPOINT

#include "../HttpServer/HttpEndpoint/HttpEndpoint.h" 

class ScannerManager;

class ScanEndpoint :public HttpEndpoint{
    ScannerManager & _scannerManager;
    public:
    ScanEndpoint(ScannerManager & manager,std::unique_ptr<JsonParser> && parser);
    std::string httpGet(HttpContext & context, const KHttp::Route & route);
    std::string httpPost(HttpContext & context, const KHttp::Route & route);
};

#endif /*SCAN_ENDPOINT*/