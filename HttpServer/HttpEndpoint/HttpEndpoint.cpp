#include "HttpEndpoint.h"
#include "../HttpProcessor/Route.h"
#include "JsonParser.h"
#include "../HttpListener/Context.h"

using namespace KHttp;

HttpEndpoint::HttpEndpoint(std::unique_ptr<JsonParser> &&parser) : _parser(std::move(parser)){}

std::string HttpEndpoint::not_found(Context & context,const ErrorDto& dto){
    context.res.base().result(boost::beast::http::status::not_found);
    return _parser->serialize(_parser->parse(dto));
}
std::string HttpEndpoint::internal_error(Context & context,const ErrorDto& dto){
    context.res.base().result(boost::beast::http::status::internal_server_error);
    return _parser->serialize(_parser->parse(dto));
}
std::string HttpEndpoint::unauthorized(Context & context,const ErrorDto& dto){
    context.res.base().result(boost::beast::http::status::unauthorized);
    return _parser->serialize(_parser->parse(dto));
}


std::string HttpEndpoint::httpGet(Context & context,const Route & route){return "";}
std::string HttpEndpoint::httpPost(Context & context,const Route & route){return "";}
std::string HttpEndpoint::httpPut(Context & context,const Route & route){return "";}
std::string HttpEndpoint::httpDelete(Context & context,const Route & route){return "";}
