#ifndef HTTP_PROCESSOR
#define HTTP_PROCESSOR

#include "HttpMiddleware.h"
#include "../HttpEndpoint/HttpEndpoint.h"
#include <vector>
#include <memory>
#include "Route.h"

namespace KHttp{

class Processor{
    std::vector<std::unique_ptr<HttpMiddleware>> _middlewares;
    std::vector<std::pair<Route,std::unique_ptr<HttpEndpoint>>> _endpoints;

    HttpMiddleware::HttpReq &_req;
    HttpMiddleware::HttpRes &_res;

    size_t _counter;
    HttpMiddleware::NextFnc _nextFnc;

    void next();

    void executeMethod(HttpEndpoint & endpoint, const HttpMiddleware::HttpMethod &method, const std::vector<std::string>& pathParams);
    void executeEndpoint();

    public:

    Processor(HttpMiddleware::HttpReq &req,HttpMiddleware::HttpRes &res);

    template<typename H,typename... Args> void addMiddleware(Args... args);
    template<typename E,typename... Args>
    void addEndpoint(const std::string &path,Args... args);

    virtual void run();
};

template<typename H,typename... Args>
void Processor::addMiddleware(Args... args){
    _middlewares.push_back(std::make_unique<H>(std::forward<Args>(args)...));
}
template<typename E,typename... Args>
void Processor::addEndpoint(const std::string &route,Args... args){
    try{
        Route parsedRoute(route);
        auto preexistingRoute = std::find_if(_endpoints.begin(),_endpoints.end(),[&parsedRoute](const std::pair<Route,std::unique_ptr<HttpEndpoint>> & orig){return orig.first == parsedRoute;});

        if(preexistingRoute != _endpoints.end()){
            throw std::runtime_error("Attempt to assign multiple endpoints to the same route: " + route);
        }

        auto newEndpoint = std::make_unique<E>(std::forward<Args>(args)...);
        _endpoints.push_back(std::pair<Route,std::unique_ptr<HttpEndpoint>>(parsedRoute,std::move(newEndpoint)));
    }catch(std::runtime_error &e){
        std::cerr<<"In Processor::addEndpoint: Invalid route passed: " << route<<"\n"<<e.what();
    }
}
}
#endif /*HTTP_PROCESSOR*/