#ifndef HTTP_MIDDLEWARE
#define HTTP_MIDDLEWARE

#include <boost/beast.hpp>
#include <functional>
#include <iostream>

class HttpMiddleware{
    public:
    using HttpReq = boost::beast::http::request<boost::beast::http::string_body>;
    using HttpRes = boost::beast::http::response<boost::beast::http::string_body>;
    using HttpMethod = boost::beast::http::verb;
    using NextFnc = std::function<void()>;

    virtual void run(HttpReq & req,
    HttpRes & res,
    NextFnc & fnc) = 0;
};

class HttpDenierMiddleware : public HttpMiddleware{
    std::vector<boost::beast::http::verb> _verbs;
    public:
    HttpDenierMiddleware(const std::vector<boost::beast::http::verb> &allowedMethods) : _verbs{allowedMethods}{}
    void run(HttpReq & req,HttpRes &res, NextFnc &fnc) override;
};

class HttpExceptionMiddleware : public HttpMiddleware{
    std::ostream & _logStream;
    public:
    HttpExceptionMiddleware(std::ostream & stream) : _logStream(stream){};
    void run(HttpReq & req,HttpRes &res,NextFnc & fnc) override;
};

#endif /*HTTP_MIDDLEWARE*/