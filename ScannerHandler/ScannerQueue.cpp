#include "ScannerQueue.h"
#include "../Scanner/ScannerOptionDto.h"
#include <iostream>

ScannerQueue::ScannerQueue(std::unique_ptr<Scanner> &&scannerPtr) : _scanner(std::move(scannerPtr))
{}

void ScannerQueue::refreshOptions(){
    std::lock_guard<std::mutex> guard(_optionMutex);
    size_t optionCount = _scanner->getOptionCount();
    _scanner->reloadOptions();
    std::cout<<"Detected: "<<optionCount<<" options"<<std::endl;
    for(size_t i =0 ;i<optionCount;++i){
        try{
            _optionBuffer.push_back(_scanner->getOption(i));
            //std::cout<<_optionBuffer[_optionBuffer.size()-1].title<<std::endl;
        }catch(std::runtime_error &e){
            std::cerr<<e.what()<<"\n"<<std::endl;
        }
    }
}

bool ScannerQueue::tryRefreshOptions(){
    std::unique_lock<std::mutex> lock(_scannerMutex);
    if(!lock.owns_lock()){
        return false;
    }
    refreshOptions();
    return true;
}

bool ScannerQueue::trySetOption(size_t optionNo,const std::string &value){
    std::unique_lock<std::mutex> lock(_scannerMutex);
    if(!lock.owns_lock()){
        return false;
    }
    try{
        _scanner->setOption(optionNo,value);
    }catch(std::runtime_error &e){
        std::cerr<<"In ScannerQueue, trySetOption\n" << e.what()<<std::endl;
        return false;
    }
    return true;
}

std::vector<ScannerOptionDto> ScannerQueue::getOptions(){
    std::lock_guard<std::mutex> guard(_optionMutex);
    return  _optionBuffer;
}