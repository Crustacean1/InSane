#ifndef ENDPOINT_BUILDER
#define ENDPOINT_BUILDER

#include <memory>
#include "../HttpProcessor/Processor.h"

namespace KHttp{

class ProcessorBuilder{
    public:
    virtual std::unique_ptr<Processor> build(HttpMiddleware::HttpReq & req,HttpMiddleware::HttpRes & res) const = 0;
};
}

#endif /*ENDPOINT_BUILDER*/