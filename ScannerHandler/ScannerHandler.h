#ifndef SCANNER_HANDLER
#define SCANNER_HANDLER

#include <memory>
#include <boost/asio.hpp>
#include <json/json.h>
#include "ScannerManager.h"

class Scanner;

class ScannerHandler{
    //std::unique_ptr<Scanner> _scanner;
    //boost::asio::strand<boost::asio::io_context::executor_type> _strand;
    public:
    ScannerHandler(std::string scannerName,ScannerManager & manager);

    Json::Value setOption(std::string option,std::string value);
    Json::Value startScan(std::string);
    Json::Value getInfo();
    Json::Value getStatus();
};

#endif /*SCANNER_HANDLER*/