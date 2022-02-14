#ifndef SCANNER_PROCESSOR_BUILDER
#define SCANNER_PROCESSOR_BUILDER

#include "../ScannerHandler/ScannerManager.h"
#include "../HttpServer/HttpProcessor/ProcessorBuilder.h"
#include "../HttpServer/HttpEndpoint/JsonParser.h"

class ScannerProcessorBuilder : public KHttp::ProcessorBuilder{
    ScannerManager & _scannerManager;
    public:
    ScannerProcessorBuilder(ScannerManager & scannerManager);
    std::unique_ptr<KHttp::Processor> build() const override;
};

#endif /*SCANNER_PROCESSOR_BUILDER*/