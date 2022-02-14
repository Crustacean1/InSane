#ifndef HTTP_MIDDLEWARE
#define HTTP_MIDDLEWARE

#include <boost/beast.hpp>
#include <functional>
#include <iostream>

class HttpContext;

class HttpMiddleware{
    public:
    using HttpReq = boost::beast::http::request<boost::beast::http::string_body>;
    using HttpRes = boost::beast::http::response<boost::beast::http::string_body>;
    using HttpMethod = boost::beast::http::verb;
    using NextFnc = std::function<void(HttpContext&)>;

    virtual void run(HttpContext & context, NextFnc & fnc) = 0;
};

class HttpDenierMiddleware : public HttpMiddleware{
    std::vector<boost::beast::http::verb> _verbs;
    public:
    HttpDenierMiddleware(const std::vector<boost::beast::http::verb> &allowedMethods) : _verbs{allowedMethods}{}
    void run(HttpContext & context, NextFnc &fnc) override;
};

class HttpExceptionMiddleware : public HttpMiddleware{
    std::ostream & _logStream;
    public:
    HttpExceptionMiddleware(std::ostream & stream) : _logStream(stream){};
    void run(HttpContext & context, NextFnc & fnc) override;
};

class KeepAliveMiddleware : public HttpMiddleware{
    public:
    void run(HttpContext & context, NextFnc & fnc) override;
};

#endif /*HTTP_MIDDLEWARE*/