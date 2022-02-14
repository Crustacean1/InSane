#ifndef SCANSTATUS_ENDPOINT
#define SCANSTATUS_ENDPOINT

#include "../HttpServer/HttpEndpoint/HttpEndpoint.h" 

class ScannerManager;

class ScanStatusEndpoint :public HttpEndpoint{
    static const std::string statusNames[];
    ScannerManager & _scannerManager;
    public:
    ScanStatusEndpoint(ScannerManager & manager,std::unique_ptr<JsonParser> && parser);
    std::string httpGet(HttpContext & context, const KHttp::Route & route);
};

#endif /*SCANSTATUS_ENDPOINT*/