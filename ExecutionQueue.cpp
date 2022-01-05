#include "ExecutionQueue.h"

ExecutionQueue::ExecutionQueue(SANE_String_Const name) : _scannerName(name){
}

std::string ExecutionQueue::getOptionsInJson(){
    if(_scanner.get() == nullptr){
        _scanner = std::make_shared<Scanner>(_scannerName);
    }
    return _scanner->getJsonOptions();
}

void ExecutionQueue::scan(std::string filepath){
    if(_scanner.get() == nullptr){
        _scanner = std::make_shared<Scanner>(_scannerName);
    }
   _scanner->scan(std::cout);
}
void ExecutionQueue::setOption(uint32_t option,std::string value){
    if(_scanner.get() == nullptr){
        _scanner = std::make_shared<Scanner>(_scannerName);
    }
    _scanner->getOption(option).setOption(value);
}

