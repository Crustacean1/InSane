#include "Scanner.h"
#include <exception>
#include <iostream>

Scanner::Scanner(std::string scannerName) {
    if (sane_open(scannerName.c_str(),&_scannerHandle) != SANE_STATUS_GOOD){
        throw std::runtime_error("Failed to open device: ~"+std::string(scannerName));
    }
    const SANE_Option_Descriptor *optionCountDesc = sane_get_option_descriptor(_scannerHandle, 0);
    if (optionCountDesc == NULL) {
    throw std::runtime_error(
        "Failed to initialize device: can't access property 0");
    }
    // Reading options
    reloadOptions();
    reloadParams();
}
Scanner::~Scanner() {
    sane_close(_scannerHandle);
}

void Scanner::setOption(size_t option,std::string val){
    if(option>=_options.size()){
        throw std::runtime_error("Option access out of bounds");
    }
    auto resp = _options[option].setOption(val);
    if(resp & SANE_INFO_RELOAD_OPTIONS){
        reloadOptions();
    }else if(resp & SANE_INFO_RELOAD_PARAMS){
        reloadParams();
    }
}

void Scanner::reloadOptions(){
    if(auto status = sane_get_parameters(_scannerHandle,&_params);status != SANE_STATUS_GOOD){
        throw std::runtime_error("Couldn't read parameters" + std::string(sane_strstatus(status)));
    }
    SANE_Word optionCount;
    if(auto status = sane_control_option(_scannerHandle,0,SANE_ACTION_GET_VALUE,&optionCount,NULL); status != SANE_STATUS_GOOD){
        throw std::runtime_error("Couldn't read parameters" + std::string(sane_strstatus(status)));
    }
    _options.clear();
    for(SANE_Word i = 0;i<optionCount;++i){
        _options.push_back(ScannerOption(_scannerHandle,i));
    }

}
void Scanner::reloadParams(){
    auto sizeOptionDescription = sane_get_option_descriptor(_scannerHandle,0);
    if(sizeOptionDescription == NULL){
        throw std::runtime_error("Couldn't read options");
    }
}

std::map<std::string,std::vector<ScannerOption>> Scanner::getOptions(){
    std::map<std::string,std::vector<ScannerOption>> formattedOptions;
    std::string prevGroup = "Nondescript";
    for(size_t i = 1;i<_options.size();++i){
        if(_options[i]._desc->type == SANE_TYPE_GROUP){
            prevGroup = std::string(_options[i]._desc->title);
            formattedOptions[prevGroup] = std::vector<ScannerOption>();
            continue;
        }
        formattedOptions[prevGroup].push_back(_options[i]);
    }
    return formattedOptions;
}

std::tuple<unsigned char*,size_t,size_t,size_t> Scanner::scan(){ // idea: custom execption type for json wrapping
    if(sane_get_parameters(_scannerHandle,&_params) != SANE_STATUS_GOOD){
        throw std::runtime_error("Failed to read scanner parameters");
    }
    if(auto error_code = sane_start(_scannerHandle); error_code != SANE_STATUS_GOOD){
        switch(error_code){
            case SANE_STATUS_DEVICE_BUSY:
                throw std::runtime_error("Device busy");
            default:
                throw std::runtime_error("Failed to start scanning" + std::to_string(error_code));
        }
    }
    uint64_t lines = _params.lines;
    uint64_t width = _params.bytes_per_line; 

    uint64_t scanSize = lines*width;

    size_t bufferSize = 1024; 
    size_t total = 0;
    
    unsigned char* file = new unsigned char[scanSize + lines*3]; 
    unsigned char* buffer = new unsigned char[bufferSize];
    SANE_Int bytesRead;

    std::cout<<"Scan params: "<<_params.bytes_per_line<<"\t"<<_params.pixels_per_line<<"\t"<<_params.lines<<"\t"<<_params.format<<"\t"<<_params.depth<<"\t"<<_params.last_frame<<std::endl;

    SANE_Status status;

    while((status = sane_read(_scannerHandle,buffer,bufferSize,&bytesRead)) == SANE_STATUS_GOOD){
        memcpy(file + total,buffer,bytesRead);
        total += bytesRead;
    }
    std::cout<<"read: "<<total<<" bytes"<<std::endl;
    if(status != SANE_STATUS_EOF && status != SANE_STATUS_GOOD){
        std::cout<<status<<std::endl;
        throw std::runtime_error("Failed during read");
    }
    delete[] buffer;
    return std::make_tuple(file,_params.pixels_per_line,_params.lines,_params.format == SANE_FRAME_GRAY ? 1: 3);
}
float Scanner::getProgress(){return _progress;}