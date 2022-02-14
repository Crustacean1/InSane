#ifndef SCANNER_ENDPOINT
#define SCANNER_ENDPOINT

#include "../HttpServer/HttpProcessor/HttpMiddleware.h"
#include "../HttpServer/HttpEndpoint/HttpEndpoint.h"
#include <string>

class ScannerManager;
class JsonParser;
class ScannerQueue;
namespace Json{
    class Value;
}

class ScannerEndpoint : public HttpEndpoint{
    ScannerManager & _scannerManager;

    static const std::string scannerIdField;
    static const std::string optionIdField;
    static const std::string valueField;
    static const std::string statusField;

    std::shared_ptr<ScannerQueue> getScanner(const std::string & scannerId);
    public:
    ScannerEndpoint(ScannerManager& scannerManager,std::unique_ptr<JsonParser> &&_parser);
    std::string httpPost(HttpContext & context, const KHttp::Route & route) override;
    std::string httpPut(HttpContext & context, const KHttp::Route & route) override;
    std::string httpGet(HttpContext & context, const KHttp::Route & route) override;
};

#endif /*SCANNER_ENDPOINT*/