#include "Scanner.h"
#include <exception>
#include <iostream>
#include <algorithm>
#include <cstring>
#include "../Image.h"

Scanner::Scanner(std::string scannerName) {
    if (sane_open(scannerName.c_str(),&_scannerHandle) != SANE_STATUS_GOOD){
        throw std::runtime_error("Failed to open device: ~"+std::string(scannerName));
    }
    // Reading options
    reloadOptions();
}
Scanner::~Scanner() {
    sane_close(_scannerHandle);
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
        std::unique_ptr<ScannerOption> option = std::make_unique<ScannerOption>(_scannerHandle,i);
        _options.push_back(std::move(option));
    }
}
void Scanner::reloadParams(){
    if(auto status = sane_get_parameters(_scannerHandle,&_params);
    status != SANE_STATUS_GOOD){
        throw std::runtime_error("Failed to read params from scanner" + 
                                std::string(sane_strstatus(status)));
    }
}
void Scanner::initImage(Image & image){
    image.setWidth(_params.pixels_per_line);
    image.setHeight(_params.lines);
    image.setChannelCount(_params.format == SANE_FRAME_RGB ? 3 : 1);
}
void Scanner::scan(Image& image){ // idea: custom execption type for json wrapping
    if(auto error_code = sane_start(_scannerHandle); error_code != SANE_STATUS_GOOD){
        switch(error_code){
            case SANE_STATUS_DEVICE_BUSY:
                throw std::runtime_error("Device busy");
            default:
                throw std::runtime_error("Failed to start scanning" + std::to_string(error_code));
        }
    }
    reloadParams();
    initImage(image);
    image.createBuffer();

    size_t bufferSize = 1024; 
    size_t total = 0;
    
    unsigned char* buffer = new unsigned char[bufferSize];
    SANE_Int bytesRead;

    std::cout<<"Scan params: "<<_params.bytes_per_line<<"\t"<<_params.pixels_per_line<<"\t"<<_params.lines<<"\t"<<_params.format<<"\t"<<_params.depth<<"\t"<<_params.last_frame<<std::endl;

    SANE_Status status;

    while((status = sane_read(_scannerHandle,buffer,bufferSize,&bytesRead)) == SANE_STATUS_GOOD){
        image.write(buffer,bytesRead);
        total += bytesRead;
    }
    std::cout<<"read: "<<total<<" bytes+"<<bytesRead<<std::endl;
    delete[] buffer;
    if(status != SANE_STATUS_EOF && status != SANE_STATUS_GOOD){
        std::cout<<status<<std::endl;
        throw std::runtime_error("Failed during read");
    }
}

void Scanner::debugDump(){
    for(const auto & option : _options){
        try{

        std::cout<<option->getName()<<"\t"<<option->getTitle()<<"\t"<<option->getDesc()<<"\t"<<option->get(NULL)<<std::endl;
        }catch(std::runtime_error &e){
            std::cout<<e.what()<<"\n\n"<<std::endl;
        }
        for(const auto & constr : option->getConstraints()){
            std::cout<<constr<<"\t";
        }
        std::cout<<"\n\n\n";
    }
}
ScannerOption& Scanner::getOption(std::string title){
    if(auto option = std::find_if(_options.begin(),_options.end(),
    [&title](const std::unique_ptr<ScannerOption>& option)
    {return option->getName() == title;}); 
    option != _options.end()){
    return *(*option).get();
    }
    throw std::runtime_error("Tried to access non existent option");
}

std::vector<std::string> Scanner::scanForScanners(){
    std::vector<std::string> result;

    const SANE_Device ** deviceList;
    if(auto status = sane_get_devices(&deviceList,(SANE_Bool)0);status != SANE_STATUS_GOOD){
        throw std::runtime_error("Failed to retrieve device list: " + std::string(sane_strstatus(status)));
    }
    for(int i = 0;deviceList[i];++i){
        result.push_back(std::string(deviceList[i]->name));
    }
    return result;
}
void Scanner::init(){
    SANE_Int version = 1;
    if(auto status = sane_init(&version,NULL); status != SANE_STATUS_GOOD){
        throw std::runtime_error("Failed to initialize sane: " + std::string(sane_strstatus(status)));
    }
}