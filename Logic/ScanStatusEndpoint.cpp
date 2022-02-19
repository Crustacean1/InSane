#include "ScanStatusEndpoint.h"
#include "../HttpServer/HttpProcessor/Route.h"
#include "../ScannerHandler/ScannerManager.h"
#include "../HttpServer/HttpEndpoint/JsonParser.h"

ScanStatusEndpoint::ScanStatusEndpoint(std::shared_ptr<ScannerManager>  manager,std::unique_ptr<JsonParser> && parser)
: HttpEndpoint(std::move(parser)), _scannerManager(manager){}

std::string ScanStatusEndpoint::httpGet(KHttp::Context &context, const KHttp::Route &route){
    if(route.size() != 1){
        return not_found(context,ErrorDto{"Malformed GET request"});
    }
    auto scannerId = route[0];
    auto scanner = _scannerManager->getScanner(scannerId);
    if(scanner == nullptr){
        return not_found(context,ErrorDto{"Scanner ont found"});
    }
    auto status = scanner->getScanningStatus();
    Json::Value result;
    result["progress"] = status.progress;
    result["status"] = status.statusToString(); 
    return _parser->serialize(result);
}
