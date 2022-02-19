#include "ScannerProcessorBuilder.h"

#include "../HttpServer/HttpEndpoint/JsonParser.h"
#include "../HttpServer/HttpProcessor/Middleware.h"
#include "ScanEndpoint.h"
#include "ScanStatusEndpoint.h"
#include "ScannerEndpoint.h"

using method = boost::beast::http::verb;

ScannerProcessorBuilder::ScannerProcessorBuilder(std::shared_ptr<ScannerManager> scannerManager) : _scannerManager(scannerManager) {}

std::unique_ptr<KHttp::Processor> ScannerProcessorBuilder::build() const {
    auto processor = std::make_unique<KHttp::Processor>();
    processor->addMiddleware<KHttp::CorsMiddleware, std::vector<KHttp::CorsMiddleware::DomainRule>, std::vector<KHttp::CorsMiddleware::MethodRule>>({"*"}, {method::get, method::post, method::put});

    processor->addMiddleware<KHttp::KeepAliveMiddleware>();
    processor->addMiddleware<KHttp::ExceptionMiddleware, std::ostream &>(std::cerr);
    processor->addMiddleware<KHttp::DenierMiddleware, std::vector<boost::beast::http::verb>>(std::vector<boost::beast::http::verb>{method::post, method::get, method::put});

    processor->addEndpoint<ScannerEndpoint>("/scanners", _scannerManager, std::make_unique<JsonParser>());
    processor->addEndpoint<ScanEndpoint>("/scan", _scannerManager, std::make_unique<JsonParser>());
    processor->addEndpoint<ScanStatusEndpoint>("/scan/status", _scannerManager, std::make_unique<JsonParser>());
    return processor;
}