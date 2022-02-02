#ifndef SCANNERMANAGER
#define SCANNERMANAGER

#include "../ExecutionQueue.h"
#include <map>
#include <sane/sane.h>
#include <exception>
#include <memory>
#include <iostream>
#include <nlohmann/json.hpp>

class ScannerManager{
    std::map<std::string,std::shared_ptr<ExecutionQueue>> _scanners;
    public:
    
    ScannerManager();
    void refreshDeviceList();
    nlohmann::json getScannerOptionsInJson(std::string scannerName);
    nlohmann::json getScannerListInJson();

    std::shared_ptr<ExecutionQueue> getScanner(std::string scannerName);

    ~ScannerManager();
};

#endif /*SCANNERMANAGER*/