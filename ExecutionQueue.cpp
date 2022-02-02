#include "ExecutionQueue.h"
#include "Image.h"

ExecutionQueue::ExecutionQueue(SANE_String_Const name) : _scannerName(std::string(name)){
}


void ExecutionQueue::scan(std::string filepath){
    try{
        if(_scanner.get() == nullptr){
            _scanner = std::make_shared<Scanner>(_scannerName);
        }
        /*auto [rawImage,width,height,channels] = _scanner->scan();
        Image image(rawImage,width,height,channels);
        image.save(filepath);*/
    }catch(std::runtime_error &e){
    }

}
void ExecutionQueue::setOption(uint32_t option,std::string value){
    try{
        if(_scanner.get() == nullptr){
            _scanner = std::make_shared<Scanner>(_scannerName);
        }
        //_scanner->setOption(option,value);
    }catch(std::runtime_error & e){
        std::cout<<"wtf?? "<<e.what()<<std::endl;
    }
}

std::string ExecutionQueue::getName(){return _scannerName;}