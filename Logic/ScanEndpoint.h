#ifndef SCAN_ENDPOINT
#define SCAN_ENDPOINT

#include "../HttpServer/HttpEndpoint/HttpEndpoint.h" 

class ScannerManager;

class ScanEndpoint :public KHttp::HttpEndpoint{
    std::shared_ptr<ScannerManager> _scannerManager;
    public:
    ScanEndpoint(std::shared_ptr<ScannerManager>  manager,std::unique_ptr<JsonParser> && parser);
    std::string httpGet(KHttp::Context & context, const KHttp::Route & route);
    std::string httpPost(KHttp::Context & context, const KHttp::Route & route);
};

#endif /*SCAN_ENDPOINT*/