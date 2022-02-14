#include "ScanEndpoint.h"
#include "../HttpServer/HttpProcessor/Route.h"
#include "../HttpServer/HttpEndpoint/JsonParser.h"
#include "../ScannerHandler/ScannerManager.h"
#include "../Image.h"

using namespace KHttp;

ScanEndpoint::ScanEndpoint(ScannerManager &manager,
                           std::unique_ptr<JsonParser> &&parser)
    : HttpEndpoint(std::move(parser)), _scannerManager(manager) {}

std::string ScanEndpoint::httpGet(HttpContext & context,const Route & route) {
    if(route.size() != 1){
        return not_found(context,ErrorDto{"Invalid get syntax"});
    }
    auto scannerId = route[0];
    auto scanner = _scannerManager.getScanner(scannerId);
    if(scanner == nullptr){
        return not_found(context,ErrorDto{"Scanner not found"});
    }
    auto scan = scanner->getScanResult();
    if(scan!= nullptr){
        scan->writeToFile("dummy.png");
    }
    //TODO: Add multipart form
    return "";
}
//TODO: Fix bug preventing scanning more than one time
std::string ScanEndpoint::httpPost(HttpContext &context,
                                      const Route &route) {
  if (route.size() != 1) {
    return not_found(context, ErrorDto{"Invalid post syntax"});
  }
  auto scanner = _scannerManager.getScanner(route[0]);
  if(scanner == nullptr){
      return not_found(context,ErrorDto{"Scanner not found"});
  }
  Json::Value result;

  std::cout<<"staring scanning"<<std::endl;
  if(scanner->tryScan()){
      result["status"] = "success";
  }else{
      result["status"] = "device_busy";
  }
  return _parser->serialize(result);
}