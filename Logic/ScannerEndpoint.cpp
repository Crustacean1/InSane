#include "ScannerEndpoint.h"
#include "../HttpServer/HttpEndpoint/JsonParser.h"
#include "../HttpServer/HttpListener/HttpContext.h"
#include "../HttpServer/HttpProcessor/Route.h"
#include "../ScannerHandler/ScannerManager.h"
#include <iostream>

using namespace KHttp;

const std::string ScannerEndpoint::scannerIdField = "scanner_id";
const std::string ScannerEndpoint::optionIdField = "option_id";
const std::string ScannerEndpoint::valueField = "value";
const std::string ScannerEndpoint::statusField = "status";

ScannerEndpoint::ScannerEndpoint(ScannerManager &scannerManager,
                                 std::unique_ptr<JsonParser> &&parser)
    : HttpEndpoint(std::move(parser)), _scannerManager(scannerManager) {}

std::shared_ptr<ScannerQueue>
ScannerEndpoint::getScanner(const std::string &scannerId) {
  auto scanner = _scannerManager.getScanner(scannerId);
  if (scanner == nullptr) {
    throw std::runtime_error(
        "In ScannerEndpoint::getScannerNo scanner with such id found");
  }
  return scanner;
}

std::string ScannerEndpoint::httpGet(HttpContext &context, const Route &route) {

  Json::Value response;

  if (route.size() == 0) {
    auto scanners = _scannerManager.getScannerList();
    for (const auto &scanner : scanners) {
      response["devices"].append(scanner);
    }
    return _parser->serialize(response);

  } else if (route.size() == 1) {
    auto scannerId = route[0];
    std::cout << "Accessing scanner: " << scannerId << std::endl;
    auto scanner = _scannerManager.getScanner(scannerId);
    if (scanner == nullptr) {
      return not_found(context, ErrorDto{"Scanner not found"});
    }
    auto options = scanner->getOptions();
    for (const auto &option : options) {
      response["options"].append(_parser->parse(option));
    }
    return _parser->serialize(response);
  }
  return not_found(context, ErrorDto{"Invalid scanner name"});
}

std::string ScannerEndpoint::httpPost(HttpContext &context, const Route &route) {
    _scannerManager.refreshScannerList();
    Json::Value result;
    result["status"] = "success";
    return _parser->serialize(result);
}

std::string ScannerEndpoint::httpPut(HttpContext &context, const Route &route) {
  if(route.size() != 1){
      return not_found(context,ErrorDto{"Invalid Put syntax"});
  }
  auto scanner = _scannerManager.getScanner(route[0]);
  if(scanner == nullptr){
      return not_found(context,ErrorDto{"Couldn't find scanner"});
  }
  Json::Value result;
  Json::Value args = _parser->parse(context.req.body());

  size_t optionId = std::atoi(args[optionIdField].asString().c_str());
  auto success = scanner->trySetOption(optionId,args[valueField].asString());
  if(success){
      result["status"] = "success";
  }else{
      result["status"] = "device_busy";
  }
  return _parser->serialize(result);
}
