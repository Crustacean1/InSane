#ifndef HTTP_ENDPOINT
#define HTTP_ENDPOINT

#include <string>
#include <memory>

namespace KHttp{
    class Route;
}
class JsonParser;
class HttpContext;

class HttpEndpoint{
    public:

    class ErrorDto{
        public:
        std::string details;
    };

    protected:
    
    std::string internal_error(HttpContext &context, const ErrorDto& dto);
    std::string not_found(HttpContext &context, const ErrorDto& dto);
    std::string unauthorized(HttpContext &context, const ErrorDto& dto);

    std::unique_ptr<JsonParser> _parser;

    HttpEndpoint();
    public:
    
    HttpEndpoint(std::unique_ptr<JsonParser> &&parser);

    virtual std::string httpPut(HttpContext & context,const KHttp::Route & route);
    virtual std::string httpDelete(HttpContext & context,const KHttp::Route & route);
    virtual std::string httpGet(HttpContext & context,const KHttp::Route & route);
    virtual std::string httpPost(HttpContext & context,const KHttp::Route & route);
};

#endif /*HTTP_ENDPOINT*/