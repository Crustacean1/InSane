#include "Scanner.h"
#include <iostream>
#include <nlohmann/json.hpp>

Scanner::Scanner(SANE_String_Const scannerName) {
    if (sane_open(scannerName,&_scannerHandle) != SANE_STATUS_GOOD){
        throw std::runtime_error("Failed to open device: "+std::string(scannerName));
    }
    const SANE_Option_Descriptor *optionCountDesc = sane_get_option_descriptor(_scannerHandle, 0);
    if (optionCountDesc == NULL) {
    throw std::runtime_error(
        "Failed to initialize device: can't access property 0");
    }
    // Reading options
    uint64_t optionCount = 0;// rather won't overflow
    SANE_Int details;
    if(sane_control_option(_scannerHandle,0,SANE_ACTION_GET_VALUE,static_cast<void*>(&optionCount),&details) != SANE_STATUS_GOOD){
        throw std::runtime_error("Failed to read device options: "+std::string(scannerName)); 
    }
    for(uint64_t i = 0;i<optionCount;++i){
        _options.push_back(ScannerOption(_scannerHandle,i));
    }
    if(sane_get_parameters(_scannerHandle,&_params) != SANE_STATUS_GOOD){
        throw std::runtime_error("Failed to read scanner status");
    }
}
Scanner::~Scanner() {
    sane_close(_scannerHandle);
}

ScannerOption & Scanner::getOption(size_t option){
    if(option>=_options.size()){
        throw std::runtime_error("Access to invalid option no.");
    }
    return _options[option];
}

std::string Scanner::getJsonOptions(){
    nlohmann::json optionsJson = nlohmann::json::object();
    std::string prevGroup = "Nondescript";
    for(size_t i = 1,j = 0;i<_options.size();++i){
        if(_options[i]._desc->type == SANE_TYPE_GROUP){
            j = 0;
            prevGroup = std::string(_options[i]._desc->title);
            optionsJson[prevGroup] = nlohmann::json::array();
            continue;
        }
        optionsJson[prevGroup][j++] = _options[i].serialize();
    }
    return optionsJson.dump();
}

void Scanner::scan(std::ostream & stream){
    if(sane_start(_scannerHandle) != SANE_STATUS_GOOD){
        throw std::runtime_error("Failed to start scanning");
        return;
    }
    std::cout<<"height: "<<_params.lines<<"\twidth: "<<_params.pixels_per_line<<std::endl;
    std::cout<<"estimated size: "<<_params.lines * _params.bytes_per_line*3<<std::endl;

    size_t bufferSize = 1024;
    size_t total = 0;

    unsigned char * file = new unsigned char[_params.bytes_per_line*_params.lines*3];
    unsigned char * buffer = new unsigned char[bufferSize];
    SANE_Int bytesRead;
    while(sane_read(_scannerHandle,buffer,bufferSize,&bytesRead) == SANE_STATUS_GOOD){
        //std::cout<<"JFC I'm reading something: "<<bytesRead<<std::endl;
        memcpy(file+total,buffer,bufferSize);
        total += bytesRead;
    }
    std::cout<<"read: "<<total<<" bytes"<<std::endl;

}