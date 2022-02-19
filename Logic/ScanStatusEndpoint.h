#ifndef SCANSTATUS_ENDPOINT
#define SCANSTATUS_ENDPOINT

#include "../HttpServer/HttpEndpoint/HttpEndpoint.h" 

class ScannerManager;

class ScanStatusEndpoint :public KHttp::HttpEndpoint{
    static const std::string statusNames[];
    std::shared_ptr<ScannerManager> _scannerManager;
    public:
    ScanStatusEndpoint(std::shared_ptr<ScannerManager>  manager,std::unique_ptr<JsonParser> && parser);
    std::string httpGet(KHttp::Context & context, const KHttp::Route & route);
};

#endif /*SCANSTATUS_ENDPOINT*/