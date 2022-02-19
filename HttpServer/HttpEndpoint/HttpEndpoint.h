#ifndef HTTP_ENDPOINT
#define HTTP_ENDPOINT

#include <memory>
#include <string>

class JsonParser;

namespace KHttp {

class Route;
class Context;

class HttpEndpoint {
public:
  class ErrorDto {
  public:
    std::string details;
  };

protected:
  std::string internal_error(Context &context, const ErrorDto &dto);
  std::string not_found(Context &context, const ErrorDto &dto);
  std::string unauthorized(Context &context, const ErrorDto &dto);

  std::unique_ptr<JsonParser> _parser;

  HttpEndpoint();

public:
  HttpEndpoint(std::unique_ptr<JsonParser> &&parser);

  virtual std::string httpPut(Context &context, const KHttp::Route &route);
  virtual std::string httpDelete(Context &context,
                                 const KHttp::Route &route);
  virtual std::string httpGet(Context &context, const KHttp::Route &route);
  virtual std::string httpPost(Context &context, const KHttp::Route &route);
};

} // namespace KHttp
#endif /*HTTP_ENDPOINT*/