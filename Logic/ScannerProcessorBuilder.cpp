#include "ScannerProcessorBuilder.h"
#include "../HttpServer/HttpEndpoint/JsonParser.h"

ScannerProcessorBuilder::ScannerProcessorBuilder(ScannerManager & scannerManager) : _scannerManager(scannerManager){}

std::unique_ptr<KHttp::Processor> ScannerProcessorBuilder::build(HttpMiddleware::HttpReq & req,HttpMiddleware::HttpRes & res) const{
    auto processor = std::make_unique<KHttp::Processor>(req,res);
    processor->addMiddleware<HttpExceptionMiddleware,std::ostream&>(std::cerr);
    processor->addMiddleware<HttpDenierMiddleware,std::vector<boost::beast::http::verb>>(std::vector<boost::beast::http::verb>{boost::beast::http::verb::post,
    boost::beast::http::verb::get,
    boost::beast::http::verb::options});
    
    processor->addEndpoint<ScannerEndpoint, ScannerManager&,std::unique_ptr<JsonParser>&&>("scanner", _scannerManager,std::move(std::make_unique<JsonParser>()));
    return processor;
}