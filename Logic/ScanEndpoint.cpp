#include "ScanEndpoint.h"

#include <stdio.h>

#include <boost/beast/http/fields.hpp>
#include <fstream>
#include <iostream>

#include "../HttpServer/HttpEndpoint/JsonParser.h"
#include "../HttpServer/HttpListener/Context.h"
#include "../HttpServer/HttpProcessor/Route.h"
#include "../Image.h"
#include "../ScannerHandler/ScannerManager.h"

using namespace KHttp;

ScanEndpoint::ScanEndpoint(std::shared_ptr<ScannerManager> manager, std::unique_ptr<JsonParser> &&parser) : HttpEndpoint(std::move(parser)), _scannerManager(manager) {}

std::string ScanEndpoint::httpGet(Context &context, const Route &route) {
    if (route.size() > 2) {
        return not_found(context, ErrorDto{"Invalid get syntax"});
    }
    auto scannerId = route[0];
    auto scanner = _scannerManager->getScanner(scannerId);
    if (scanner == nullptr) {
        return not_found(context, ErrorDto{"Scanner not found"});
    }
    auto scan = scanner->getScanResult();
    if (scan != nullptr) {
        context.res.set(boost::beast::http::field::content_disposition, "inline; attachment; filename=\"scan.png\"");
        context.res.set(boost::beast::http::field::content_type, "image/png");
        return std::string(reinterpret_cast<const char *>(scan->data()), scan->size());
    }
    return not_found(context, ErrorDto{"No recents scans, did you forget to post to this endpoint first?"});
}

std::string ScanEndpoint::httpPost(Context &context, const Route &route) {
    if (route.size() != 1) {
        return not_found(context, ErrorDto{"Invalid post syntax"});
    }
    auto scanner = _scannerManager->getScanner(route[0]);
    if (scanner == nullptr) {
        return not_found(context, ErrorDto{"Scanner not found"});
    }
    Json::Value result;

    std::cout << "staring scanning" << std::endl;
    if (scanner->tryScan()) {
        result["status"] = "success";
    } else {
        result["status"] = "failure";
    }
    return _parser->serialize(result);
}