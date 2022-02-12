#include "ScannerProcessorBuilder.h"


ScannerProcessorBuilder::ScannerProcessorBuilder(ScannerManager & scannerManager) : _scannerManager(scannerManager){}

std::unique_ptr<KHttp::Processor> ScannerProcessorBuilder::build() const{
    auto processor = std::make_unique<KHttp::Processor>();
    processor->addMiddleware<HttpExceptionMiddleware,std::ostream&>(std::cerr);
    processor->addMiddleware<HttpDenierMiddleware>(std::vector<boost::beast::http::verb>{boost::beast::http::verb::post,
    boost::beast::http::verb::get,
    boost::beast::http::verb::options});
    
    processor->addEndpoint<ScannerEndpoint, ScannerManager&>("scanner", _scannerManager);
    return processor;
}