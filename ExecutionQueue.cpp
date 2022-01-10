#include "ExecutionQueue.h"
#include "Image.h"

using nlohmann::json;

ExecutionQueue::ExecutionQueue(SANE_String_Const name) : _scannerName(std::string(name)){
}

json ExecutionQueue::getOptionsInJson(){return _scannerOptions;}

std::tuple<json,size_t> ExecutionQueue::execute(json request){
    if(request["request"] == "get_options"){

    }else if(request["request"] == "set_option"){

    }else if(request["request"] == "scan"){

    }else if(request["request"] == "get_progress"){

    }
}

void ExecutionQueue::refreshJsonOptions(){
    if(_scanner.get() == nullptr){
        _scanner = std::make_shared<Scanner>(_scannerName);
        refreshJsonOptions();
    }
    auto rawOptions = _scanner->getOptions();
    _scannerOptions = json::object();

    for(const auto & [sectionName,sectionElements] : rawOptions){
        _scannerOptions[sectionName] = json::array();
        for(const auto & option : sectionElements){
            _scannerOptions[sectionName].push_back(option.serialize());
        }
    }
}

json ExecutionQueue::scan(std::string filepath){
    try{
        if(_scanner.get() == nullptr){
            _scanner = std::make_shared<Scanner>(_scannerName);
            refreshJsonOptions();
        }
        auto [rawImage,width,height,channels] = _scanner->scan();
        Image image(rawImage,width,height,channels);
        image.save(filepath);
        return json{{"details",""}};
    }catch(std::runtime_error &e){
        return json{{"details",e.what()}};
    }

}
json ExecutionQueue::setOption(uint32_t option,std::string value){
    try{
        if(_scanner.get() == nullptr){
            _scanner = std::make_shared<Scanner>(_scannerName);
            refreshJsonOptions();
        }
        _scanner->setOption(option,value);
        refreshJsonOptions();
        return json{{"details",""}};
    }catch(std::runtime_error & e){
        std::cout<<"wtf?? "<<e.what()<<std::endl;
        return json{{"details",e.what()}};
    }
}

std::string ExecutionQueue::getName(){return _scannerName;}