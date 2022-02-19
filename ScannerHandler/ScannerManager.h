#ifndef SCANNER_MANAGER
#define SCANNER_MANAGER

#include <map>
#include <string>
#include <memory>
#include <mutex>
#include "ScannerQueue.h"
#include <vector>
#include <chrono>

class Scanner;
class Scheduler;

class ScannerManager : public std::enable_shared_from_this<ScannerManager>{
    static bool refreshFunction(std::weak_ptr<ScannerManager> manager);
    static std::shared_ptr<ScannerManager> managerInstance;

    std::mutex _mapMutex;
    std::map<std::string,std::shared_ptr<ScannerQueue>> _scannerQueues;

    std::vector<std::string> _connectedScanners;

    std::shared_ptr<ScannerQueue> createScanner(const std::string & name);
    std::shared_ptr<ScannerQueue> getScannerFromMap(const std::string& map);
    
    size_t _scannerRefreshRate;

    ScannerManager(const ScannerManager&);
    ScannerManager(size_t scannerRefreshRate = 30);
    ScannerManager(ScannerManager&&);

    void releaseOldScanners();

    public:
    static std::shared_ptr<ScannerManager> createScannerManager(size_t scannerRefreshRate);

    ~ScannerManager();

    void setRefreshScheduler(Scheduler & scheduler);
    std::shared_ptr<ScannerQueue> getScanner(const std::string &name);
    std::vector<std::string> getScannerList();
    void refreshScannerList();
};

#endif /*SCANNER_MANAGER*/