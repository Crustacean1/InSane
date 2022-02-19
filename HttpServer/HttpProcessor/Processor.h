#ifndef HTTP_PROCESSOR
#define HTTP_PROCESSOR

#include "../HttpEndpoint/HttpEndpoint.h"
#include "./Middleware.h"
#include "Route.h"
#include <memory>
#include <vector>
#include "../HttpEndpoint/JsonParser.h"

class HttpContext;

namespace KHttp {

class Processor {
  std::vector<std::unique_ptr<Middleware>> _middlewares;
  std::vector<std::pair<Route, std::unique_ptr<HttpEndpoint>>> _endpoints;

  size_t _counter;
  Middleware::NextFnc _nextFnc;

  void next(Context &context);

  using HttpHandler = std::string (HttpEndpoint::*)(Context &,
                                                    const KHttp::Route &);
  std::string executeMethod(Context & context,const Route & route, HttpEndpoint & endpoint);
  void executeEndpoint(Context &context);

public:
  Processor();

  template <typename H, typename... Args> void addMiddleware(Args... args);
  template <typename E, typename... Args>
  void addEndpoint(const std::string &path, Args... args);

  virtual void run(Context &context);
};

template <typename H, typename... Args>
void Processor::addMiddleware(Args... args) {
  _middlewares.push_back(std::make_unique<H>(std::forward<Args>(args)...));
}

template <typename E, typename... Args>
void Processor::addEndpoint(const std::string &route, Args... args) {
  try {
    Route parsedRoute(route);
    auto preexistingRoute = std::find_if(
        _endpoints.begin(), _endpoints.end(),
        [&parsedRoute](
            const std::pair<Route, std::unique_ptr<HttpEndpoint>> &orig) {
          return orig.first >= parsedRoute && parsedRoute <= orig.first;
        });

    if (preexistingRoute != _endpoints.end()) {
      throw std::runtime_error(
          "Attempt to assign multiple endpoints to the same route: " + route);
    }

    auto newEndpoint = std::make_unique<E>(std::forward<Args>(args)...);
    _endpoints.push_back(std::pair<Route, std::unique_ptr<HttpEndpoint>>(
        parsedRoute, std::move(newEndpoint)));
  } catch (std::runtime_error &e) {
    std::cerr << "In Processor::addEndpoint: Invalid route passed: " << route
              << "\n"
              << e.what();
  }
}
} // namespace KHttp
#endif /*HTTP_PROCESSOR*/