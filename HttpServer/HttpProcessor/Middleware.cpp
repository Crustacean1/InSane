#include "Middleware.h"

#include <boost/beast/http/fields.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "../HttpListener/Context.h"

using namespace KHttp;

void DenierMiddleware::run(Context &context, NextFnc &next) {
    for (size_t i = 0; i < _verbs.size(); ++i) {
        if (_verbs[i] == context.req.method()) {
            next(context);
            return;
        }
    }
    context.res.base().result(boost::beast::http::status::not_found);
}

void ExceptionMiddleware::run(Context &context, NextFnc &next) {
    try {
        next(context);
    } catch (std::exception &e) {
        std::time_t exceptionTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        _logStream << std::put_time(std::localtime(&exceptionTime), "%F %T") << ": "
                   << "Exception thrown: " << e.what() << std::endl;
        context.res.base().result(boost::beast::http::status::internal_server_error);
    }
}
void KeepAliveMiddleware::run(Context &context, NextFnc &next) {
    context.res.keep_alive(context.req.keep_alive());
    context.res.set(boost::beast::http::field::content_type, "application/json");
    next(context);
    context.res.prepare_payload();
}

CorsMiddleware::CorsMiddleware(const std::vector<DomainRule> &domainRules, const std::vector<MethodRule> &methodRules) : _domainRules(domainRules), _methodRules(methodRules) {}

template <typename T>
std::string CorsMiddleware::join(const std::vector<T> &vec) {
    std::ostringstream sstream;
    for (const auto &el : vec) {
        sstream << el << ", ";
    }
    auto result = sstream.str();
    return result.substr(0, result.size() - 2);
}

void CorsMiddleware::run(Context &context, NextFnc &fnc) {
    std::string domainList = join(_domainRules);
    std::string methodList = join(_methodRules);
    context.res.base().set(boost::beast::http::field::access_control_allow_origin, domainList);
    if (context.req.method() == boost::beast::http::verb::options) {
        context.res.base().set(boost::beast::http::field::access_control_allow_headers, "Content-Type");
        context.res.base().set(boost::beast::http::field::access_control_allow_methods, methodList);
        context.res.base().result(boost::beast::http::status::ok);
    } else {
        fnc(context);
    }
}