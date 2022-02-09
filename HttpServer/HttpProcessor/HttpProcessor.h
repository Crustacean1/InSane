#ifndef HTTP_PROCESSOR
#define HTTP_PROCESSOR

#include "HttpMiddleware.h"
#include <vector>
#include <memory>

class HttpProcessor{
    std::vector<std::shared_ptr<HttpMiddleware>> _middlewares;
    size_t _counter;
    void next(HttpMiddleware::HttpReq &req,HttpMiddleware::HttpRes & res);
    HttpMiddleware::NextFnc _nextFnc;

    protected:
    template<typename H,typename... Args> void addMiddleware(Args... args);

    public:

    HttpProcessor();
    virtual void run(HttpMiddleware::HttpReq &req,HttpMiddleware::HttpRes &res);
};

template<typename H,typename... Args>
void HttpProcessor::addMiddleware(Args... args){
    _middlewares.push_back(std::make_shared<H>(args...));
}

class ScannerHttpProcessor : public HttpProcessor{
    public:
    ScannerHttpProcessor();
};

#endif /*HTTP_PROCESSOR*/