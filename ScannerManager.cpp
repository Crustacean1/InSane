#include "ScannerManager.h"

ScannerManager::ScannerManager(){
    SANE_Int version = 1;
    if(sane_init(&version,NULL) != SANE_STATUS_GOOD){
        throw std::runtime_error("Failed to initialize sane backend");
    }
}

void ScannerManager::refreshDeviceList(){
    const SANE_Device ** devices;
    if(sane_get_devices(&devices,(SANE_Bool)0) != SANE_STATUS_GOOD){
        throw std::runtime_error("Failed to read device list");
    }
    std::map<std::string,std::shared_ptr<ExecutionQueue>> newScanners;
    for(auto ptr = devices;*ptr != NULL;++ptr){
        if(_scanners.find((*ptr)->name) != _scanners.end()){
            newScanners[(*ptr)->name] = _scanners[(*ptr)->name];
        }
        else{
            std::cout<<"Found: "<<(*ptr)->name<<std::endl;
            newScanners[(*ptr)->name] = std::make_shared<ExecutionQueue>((*ptr)->name);
        }
    }
    _scanners = std::move(newScanners);
}
std::shared_ptr<ExecutionQueue> ScannerManager::getScanner(std::string scannerName){
    if(_scanners.find(scannerName) == _scanners.end()){
        std::cout<<"Scanner not found in buffer, trying to open by name"<<std::endl;
        try{
            auto device = std::make_shared<ExecutionQueue>(scannerName.c_str());
            _scanners[scannerName] = std::move(device);
            return _scanners[scannerName];
        }catch(std::runtime_error &e){
            std::cerr<<e.what()<<std::endl;
        }
    }
    std::cout<<"buffered access"<<std::endl;
    return _scanners[scannerName];
}
nlohmann::json ScannerManager::getScannerListInJson(){
    nlohmann::json scannerList = nlohmann::json::array();
    for(const auto  & [name,scanner] : _scanners){
        scannerList.push_back(name);
    }
    return scannerList;
}

ScannerManager::~ScannerManager(){
    _scanners.clear();
    sane_exit();
}