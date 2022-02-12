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
}

ScannerManager::~ScannerManager(){
    sane_exit();
}

std::shared_ptr<ScannerQueue> ScannerManager::getScanner(const std::string & name){
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