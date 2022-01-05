#ifndef SCANNERMANAGER
#define SCANNERMANAGER

#include "ExecutionQueue.h"
#include <map>
#include <sane/sane.h>
#include <exception>
#include <memory>
#include <iostream>

class ScannerManager{
    std::map<std::string,std::shared_ptr<ExecutionQueue>> _scanners;
    public:
    
    ScannerManager();
    void refreshDeviceList();
    std::string getScannerOptionsInJson(std::string scannerName);
    std::shared_ptr<ExecutionQueue> getScanner(std::string scannerName);
    void scan(std::string scannerName);

    ~ScannerManager();
};

#endif /*SCANNERMANAGER*/