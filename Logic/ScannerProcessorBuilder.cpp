#include "ScannerProcessorBuilder.h"
#include "../HttpServer/HttpEndpoint/JsonParser.h"
#include "../HttpServer/HttpListener/HttpContext.h"
#include "ScannerEndpoint.h"
#include "ScanEndpoint.h"
#include "ScanStatusEndpoint.h"

ScannerProcessorBuilder::ScannerProcessorBuilder(ScannerManager & scannerManager) : _scannerManager(scannerManager){}

std::unique_ptr<KHttp::Processor> ScannerProcessorBuilder::build() const{
    auto processor = std::make_unique<KHttp::Processor>();
    processor->addMiddleware<KeepAliveMiddleware>();
    processor->addMiddleware<HttpExceptionMiddleware,std::ostream&>(std::cerr);
    processor->addMiddleware<HttpDenierMiddleware,std::vector<boost::beast::http::verb>>(std::vector<boost::beast::http::verb>{boost::beast::http::verb::post,
    boost::beast::http::verb::get,
    boost::beast::http::verb::put});
    
    processor->addEndpoint<ScannerEndpoint, ScannerManager&, std::unique_ptr<JsonParser>&&>("/scanner", _scannerManager, std::make_unique<JsonParser>());
    processor->addEndpoint<ScanEndpoint, ScannerManager&, std::unique_ptr<JsonParser>&&>("/scan", _scannerManager, std::make_unique<JsonParser>());
    processor->addEndpoint<ScanStatusEndpoint, ScannerManager&, std::unique_ptr<JsonParser>&&>("/scan/status", _scannerManager, std::make_unique<JsonParser>());
    return processor;
}