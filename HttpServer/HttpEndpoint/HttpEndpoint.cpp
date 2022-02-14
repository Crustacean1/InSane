#include "HttpEndpoint.h"
#include "../HttpProcessor/Route.h"
#include "JsonParser.h"
#include "../HttpListener/HttpContext.h"

using namespace KHttp;

HttpEndpoint::HttpEndpoint(std::unique_ptr<JsonParser> &&parser) : _parser(std::move(parser)){}

std::string HttpEndpoint::not_found(HttpContext & context,const ErrorDto& dto){
    context.res.base().result(boost::beast::http::status::not_found);
    return _parser->serialize(_parser->parse(dto));
}
std::string HttpEndpoint::internal_error(HttpContext & context,const ErrorDto& dto){
    context.res.base().result(boost::beast::http::status::internal_server_error);
    return _parser->serialize(_parser->parse(dto));
}
std::string HttpEndpoint::unauthorized(HttpContext & context,const ErrorDto& dto){
    context.res.base().result(boost::beast::http::status::unauthorized);
    return _parser->serialize(_parser->parse(dto));
}


std::string HttpEndpoint::httpGet(HttpContext & context,const Route & route){return "";}
std::string HttpEndpoint::httpPost(HttpContext & context,const Route & route){return "";}
std::string HttpEndpoint::httpPut(HttpContext & context,const Route & route){return "";}
std::string HttpEndpoint::httpDelete(HttpContext & context,const Route & route){return "";}
