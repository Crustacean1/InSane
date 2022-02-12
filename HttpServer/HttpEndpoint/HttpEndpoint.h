#ifndef HTTP_ENDPOINT
#define HTTP_ENDPOINT

#include "../HttpProcessor/HttpMiddleware.h"

class HttpEndpoint{
    protected:
    public:
    virtual void httpPut(HttpMiddleware::HttpReq& req,HttpMiddleware::HttpRes& res);
    virtual void httpDelete(HttpMiddleware::HttpReq& req,HttpMiddleware::HttpRes& res);
    virtual void httpGet(HttpMiddleware::HttpReq& req,HttpMiddleware::HttpRes& res);
    virtual void httpPost(HttpMiddleware::HttpReq& req,HttpMiddleware::HttpRes& res);
};

#endif /*HTTP_ENDPOINT*/