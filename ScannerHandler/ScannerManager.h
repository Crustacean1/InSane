#ifndef SCANNER_MANAGER
#define SCANNER_MANAGER

#include <map>
#include <string>
#include <memory>
#include <mutex>
#include "ScannerQueue.h"
#include <vector>

class Scanner;

class ScannerManager{
    std::mutex _mutex;
    std::map<std::string,std::shared_ptr<ScannerQueue>> _scannerQueues;
    std::vector<std::string> _connectedScanners;

    ScannerManager(const ScannerManager&);
    public:

    ScannerManager();
    ScannerManager(ScannerManager&&);

    ~ScannerManager();

    std::shared_ptr<ScannerQueue> getScanner(const std::string &name);
    std::vector<std::string> getScannerList();
    void refreshScannerList();
};

#endif /*SCANNER_MANAGER*/