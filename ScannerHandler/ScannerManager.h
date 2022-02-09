#ifndef SCANNER_MANAGER
#define SCANNER_MANAGER

#include <map>
#include <string>
#include <memory>
#include "ScannerQueue.h"

class Scanner;

class ScannerManager{
    std::map<std::string,std::shared_ptr<ScannerQueue>> _scannerQueues;

    ScannerManager(const ScannerManager&);
    public:

    ScannerManager();
    ScannerManager(ScannerManager&&);

    ~ScannerManager();

    std::shared_ptr<ScannerQueue> getScanner(const std::string &name);

};

#endif /*SCANNER_MANAGER*/