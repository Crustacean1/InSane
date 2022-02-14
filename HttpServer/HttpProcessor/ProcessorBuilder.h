#ifndef ENDPOINT_BUILDER
#define ENDPOINT_BUILDER

#include <memory>
#include "../HttpProcessor/Processor.h"

class HttpContext;

namespace KHttp{

class ProcessorBuilder{
    public:
    virtual std::unique_ptr<Processor> build() const = 0;
};
}

#endif /*ENDPOINT_BUILDER*/