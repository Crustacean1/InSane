#ifndef SCANNER_PROCESSOR_BUILDER
#define SCANNER_PROCESSOR_BUILDER

#include "../ScannerHandler/ScannerManager.h"
#include "ScannerEndpoint.h"
#include "../HttpServer/HttpProcessor/ProcessorBuilder.h"


class ScannerProcessorBuilder : public KHttp::ProcessorBuilder{
    ScannerManager & _scannerManager;

    public:
    ScannerProcessorBuilder(ScannerManager & scannerManager);
    std::unique_ptr<KHttp::Processor> build(HttpMiddleware::HttpReq & req,HttpMiddleware::HttpRes & res) const override;
};

#endif /*SCANNER_PROCESSOR_BUILDER*/