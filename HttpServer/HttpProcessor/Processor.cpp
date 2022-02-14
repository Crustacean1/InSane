#include "Processor.h"
#include "../HttpListener/HttpContext.h"
#include <iostream>

using namespace KHttp;

void Processor::run(HttpContext &context) {
  if (_middlewares.size() != 0) {
    _counter = 0;
    _middlewares[0]->run(context, _nextFnc);
  }
}

Processor::Processor()
    : _nextFnc(std::bind(&Processor::next, this, std::placeholders::_1)) {}

void Processor::next(HttpContext &context) {
  if (!(_counter < _middlewares.size())) {
    executeEndpoint(context);
    return;
  }
  _middlewares[_counter++]->run(context, _nextFnc);
}

void Processor::executeEndpoint(HttpContext &context) {
  Route reqRoute(std::string(context.req.target()));
  size_t maxMatch = 0;
  size_t index = -1;


  for (size_t i = 0; i < _endpoints.size(); ++i) {
    if (_endpoints[i].first <= reqRoute) {
      auto newMatch = _endpoints[i].first.match(reqRoute);
      if (newMatch > maxMatch) {
        index = i;
      }
    }
  }

  if (index != (size_t)-1) {
    auto routedEndpoint = _endpoints[index].first;
    auto routeParams = routedEndpoint.extractParams(reqRoute);
    HttpEndpoint &endpoint = *_endpoints[index].second;
    context.res.body() = executeMethod(context, routeParams, endpoint);
    return;
  }
  context.res.base().result(boost::beast::http::status::not_found);
}
std::string Processor::executeMethod(HttpContext &context, const Route &route,
                                     HttpEndpoint &endpoint) {
  context.res.base().result(boost::beast::http::status::ok);
  switch (context.req.method()) {
  case boost::beast::http::verb::get:
    return endpoint.httpGet(context, route);
  case boost::beast::http::verb::post:
    return endpoint.httpPost(context, route);
  case boost::beast::http::verb::put:
    return endpoint.httpPut(context, route);
  case boost::beast::http::verb::delete_:
    return endpoint.httpDelete(context, route);
  default:
    context.res.base().result(boost::beast::http::status::not_found);
    throw std::runtime_error("Endpoint does not exist");
  }
}
