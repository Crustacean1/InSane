#include "ScannerManager.h"
#include "../Scanner/Scanner.h"
#include <sane/sane.h>
#include <iostream>

ScannerManager::ScannerManager(){
    SANE_Int saneVersion = 1;

    auto status = sane_init(&saneVersion,NULL);
    if(status != SANE_STATUS_GOOD){
        throw std::runtime_error("In ScannerManager::ScannerManager\n" + 
        std::string(sane_strstatus(status)));
    }
    refreshScannerList();
}

ScannerManager::~ScannerManager(){
    _scannerQueues.clear();
    sane_exit();
}

std::shared_ptr<ScannerQueue> ScannerManager::getScanner(const std::string & name){
    std::lock_guard<std::mutex> guard(_mutex);
    if(_scannerQueues.find(name) == _scannerQueues.end()){
        try{
        auto scanner = std::make_unique<Scanner>(name);
        auto scannerQueue = std::make_shared<ScannerQueue>(std::move(scanner));
        _scannerQueues[name] = std::move(scannerQueue);
        }catch(std::runtime_error & e){
            std::cerr<<"Failed to open scanner: "<<name<<std::endl;
            return nullptr;
        }
    }
    return _scannerQueues[name];
}

std::vector<std::string> ScannerManager::getScannerList(){
    std::lock_guard<std::mutex> guard(_mutex);
    return _connectedScanners;
}

void ScannerManager::refreshScannerList(){
    std::lock_guard<std::mutex> guard(_mutex);
    const SANE_Device ** deviceList;
    auto status = sane_get_devices(&deviceList,(SANE_Bool)0);
    if(status != SANE_STATUS_GOOD){
        throw std::runtime_error("Failed to retrieve device list: " + std::string(sane_strstatus(status)));
    }
    _connectedScanners.clear();
    for(int i = 0;deviceList[i];++i){
        _connectedScanners.push_back(std::string(deviceList[i]->name));
    }
}