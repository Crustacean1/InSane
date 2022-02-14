#include "ScannerQueue.h"
#include "../Scanner/ScannerOptionDto.h"
#include <iostream>
#include "../Image.h"

ScannerQueue::ScannerQueue(std::unique_ptr<Scanner> &&scannerPtr) : 
_scanner(std::move(scannerPtr)),
_scannerThread{nullptr},
_optionCollectionBuffer{nullptr},
_scannedImage{nullptr},
_scanningStatus{0,ScanningStatus::Status::Ready}
{
    tryRefreshOptions();
}

void ScannerQueue::readOptions(){
    size_t optionCount = _scanner->getOptionCount();
    std::unique_ptr<OptionDtoCollection> dtoCollection = std::make_unique<OptionDtoCollection>();

    for(size_t i =0 ;i<optionCount;++i){
        try{
            auto option = _scanner->getOption(i);
            //std::cout<<option.title<<"\t"<<option.value<<std::endl;
            dtoCollection->options.push_back(option);
        }catch(std::runtime_error &e){
            std::cerr<<e.what()<<"\n"<<std::endl;
        }
    }
    _optionCollectionBuffer.swap(dtoCollection);
}

bool ScannerQueue::tryRefreshOptions(){
    std::unique_lock<std::mutex> lock(_scannerMutex);
    if(!lock.owns_lock()){
        return false;
    }
    _scanner->reloadOptions();
    readOptions();
    return true;
}

bool ScannerQueue::trySetOption(size_t optionNo,const std::string &value){
    std::unique_lock<std::mutex> lock(_scannerMutex);

    if(!lock.owns_lock()){
        std::cerr<<"Couldn't secure lock to try set option"<<std::endl;
        return false;
    }

    try{
        std::cout<<"setting option: "<<optionNo<<"\t"<<value<<std::endl;
        _scanner->setOption(optionNo,value);
        _scanner->reloadOptions();
        readOptions();
        
    }catch(std::runtime_error &e){
        std::cerr<<"In ScannerQueue, trySetOption\n" << e.what()<<std::endl;
        return false;
    }
    return true;
}


std::vector<ScannerOptionDto> ScannerQueue::getOptions(){
    return _optionCollectionBuffer->options; 
}

void ScannerQueue::scan(std::unique_ptr<std::unique_lock<std::mutex>> lock){
    try{
        auto newImage = std::make_shared<PngImage>();
        _scanningStatus.progress = 0;
        _scanningStatus.status = ScanningStatus::Status::Scanning;
        _scanner->scan(*newImage,_scanningStatus.progress);
        _scannedImage = newImage;
    }catch(std::runtime_error & e){
        _scanningStatus.status = ScanningStatus::Status::Failed;
        std::cerr<<"From ScannerQueue::scan:\n"<<e.what()<<std::endl;
        return;
    }
    _scanningStatus.status = ScanningStatus::Status::Completed;
    _scanningStatus.progress = 0;
}

bool ScannerQueue::tryScan(){
    std::unique_ptr<std::unique_lock<std::mutex>> lock = std::make_unique<std::unique_lock<std::mutex>>(_scannerMutex);
    if(!lock->owns_lock()){
        return false;
    }
    _scanningStatus.progress = 0;
    _scanningStatus.status = ScanningStatus::Status::Pending;
    _scannerThread = std::make_unique<std::thread>(&ScannerQueue::scan,this,std::move(lock));
    return true;
}

ScanningStatus ScannerQueue::getScanningStatus(){
    return _scanningStatus;
}

std::shared_ptr<Image> ScannerQueue::getScanResult(){
    _scanningStatus.status = ScanningStatus::Status::Waiting;
    return _scannedImage;
}

ScannerQueue::~ScannerQueue(){
    if(_scannerThread != nullptr){
        std::cout<<"Come, join us"<<std::endl;
        _scannerThread->join();
    }
}