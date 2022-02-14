#ifndef HTTP_CONTEXT
#define HTTP_CONTEXT

#include <boost/beast.hpp>

class HttpContext{
    public:
    using Request = boost::beast::http::request<boost::beast::http::string_body>;
    using Response = boost::beast::http::response<boost::beast::http::string_body>;

    Request req;
    Response res;
};

#endif /*HTTP_CONTEXT*/