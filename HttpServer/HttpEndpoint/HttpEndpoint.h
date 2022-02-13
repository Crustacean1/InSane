#ifndef HTTP_ENDPOINT
#define HTTP_ENDPOINT

#include "../HttpProcessor/HttpMiddleware.h"

class HttpEndpoint{
    protected:
    public:
    virtual void httpPut(HttpMiddleware::HttpReq& req,HttpMiddleware::HttpRes& res,std::vector<std::string> pathParams);
    virtual void httpDelete(HttpMiddleware::HttpReq& req,HttpMiddleware::HttpRes& res,std::vector<std::string> pathParams);
    virtual void httpGet(HttpMiddleware::HttpReq& req,HttpMiddleware::HttpRes& res,std::vector<std::string> pathParams);
    virtual void httpPost(HttpMiddleware::HttpReq& req,HttpMiddleware::HttpRes& res,std::vector<std::string> pathParams);
};

#endif /*HTTP_ENDPOINT*/