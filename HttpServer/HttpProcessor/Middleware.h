#ifndef HTTP_MIDDLEWARE
#define HTTP_MIDDLEWARE

#include "../HttpProcessor/Route.h"
#include <boost/beast.hpp>
#include <functional>
#include <iostream>

namespace KHttp {

class Context;

class Middleware {
public:
  using HttpReq = boost::beast::http::request<boost::beast::http::string_body>;
  using HttpRes = boost::beast::http::response<boost::beast::http::string_body>;
  using HttpMethod = boost::beast::http::verb;
  using NextFnc = std::function<void(Context &)>;

  virtual void run(Context &context, NextFnc &fnc) = 0;
};

class DenierMiddleware : public Middleware {
  std::vector<boost::beast::http::verb> _verbs;

public:
  DenierMiddleware(
      const std::vector<boost::beast::http::verb> &allowedMethods)
      : _verbs{allowedMethods} {}
  void run(Context &context, NextFnc &fnc) override;
};

class ExceptionMiddleware : public Middleware {
  std::ostream &_logStream;

public:
  ExceptionMiddleware(std::ostream &stream) : _logStream(stream){};
  void run(Context &context, NextFnc &fnc) override;
};

class KeepAliveMiddleware : public Middleware {
public:
  void run(Context &context, NextFnc &fnc) override;
};

class CorsMiddleware : public Middleware {
public:
  using MethodRule = boost::beast::http::verb;
  using DomainRule = std::string;

private:
  std::vector<DomainRule> _domainRules;
  std::vector<MethodRule> _methodRules;
  template<typename T> std::string join(const std::vector<T> & vec);
public:
  CorsMiddleware(const std::vector<DomainRule> &domainRules, const std::vector<MethodRule> &methodRules);
  void run(Context &context, NextFnc &fnc) override;
};
} // namespace KHttp
#endif /*HTTP_MIDDLEWARE*/