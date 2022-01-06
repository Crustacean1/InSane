#include "ScannerOption.h"
#include <cstring>
#include <iostream>

using nlohmann::json; 

ScannerOption::ScannerOption(SANE_Handle & handle,size_t option) : _handle(handle), _desc(sane_get_option_descriptor(_handle,option)),_optionNo(option){

}
float ScannerOption::fixedToFloat(SANE_Fixed val){
    return float(val) / (int64_t(1)<<SANE_FIXED_SCALE_SHIFT);
}
SANE_Fixed ScannerOption::floatToFixed(float val){
    return int64_t((int64_t(1)<<SANE_FIXED_SCALE_SHIFT)*val);
}
void ScannerOption::set(int val){
    sane_control_option(_handle,_optionNo,SANE_ACTION_SET_VALUE,&val,NULL);
}
void ScannerOption::set(float val){
    auto buffer = floatToFixed(val);
    sane_control_option(_handle,_optionNo,SANE_ACTION_SET_VALUE,&buffer,NULL);
}
void ScannerOption::set(std::string val){
    char * buffer = new char[val.size()+1];
    strcpy(buffer,val.c_str());
    buffer[val.size()] = 0;
    sane_control_option(_handle,_optionNo,SANE_ACTION_SET_VALUE,buffer,NULL);
    delete[] buffer;
}
json ScannerOption::serialize()const{
    json optionJson = json::object();    

    optionJson["name"] =_desc->name != NULL ? std::string(_desc->name) : "";
    optionJson["title"] =_desc->title != NULL ? std::string(_desc->title) : "";
    optionJson["desc"] =_desc->desc != NULL ? std::string(_desc->desc) : "";
    optionJson["unit"] = std::string(UNIT_NAMES[_desc->unit]);
    optionJson["type"] =  std::string(TYPE_NAMES[_desc->type]);
    optionJson["bound_type"] = std::string(CONSTRAINT_NAMES[_desc->constraint_type]);
    optionJson["id"] = _optionNo;
    auto numConverter = _desc->type == SANE_TYPE_FIXED ? [](SANE_Word val)->std::string{return std::to_string(fixedToFloat(val));}:
                                                            [](SANE_Word val)->std::string{return std::to_string(val);};
    switch(_desc->constraint_type){
        case SANE_CONSTRAINT_RANGE:
            optionJson["bound_range"][0] = numConverter(_desc->constraint.range->min); 
            optionJson["bound_range"][1] = numConverter(_desc->constraint.range->max); 
        break;
        case SANE_CONSTRAINT_WORD_LIST:
           for(SANE_Int i = 0;i<*_desc->constraint.word_list;++i){
               optionJson["bound_range"][i] = numConverter(_desc->constraint.word_list[i+1]);
           }
        break;
        case SANE_CONSTRAINT_STRING_LIST:
            for(size_t i = 0;_desc->constraint.string_list[i] != NULL;++i){
                optionJson["bound_range"][i] = std::string(_desc->constraint.string_list[i]);
            }
        default:
        break;
    }
    switch(_desc->type){
        case SANE_TYPE_INT:
        case SANE_TYPE_BOOL:
        case SANE_TYPE_FIXED:
        {
            SANE_Word word;
            if(sane_control_option(_handle,_optionNo,SANE_ACTION_GET_VALUE,&word,NULL) == SANE_STATUS_GOOD){
                optionJson["value"] = numConverter(word);
            }
            break;
        }
        case SANE_TYPE_STRING:
        {
            SANE_String str = new SANE_Char[_desc->size+100];
            if(sane_control_option(_handle,_optionNo,SANE_ACTION_GET_VALUE,str,NULL) == SANE_STATUS_GOOD){
                str[_desc->size] = 0;
                optionJson["value"] = std::string(str);
            }
            break;
        }
        default:
        break;
    }

    return optionJson;
}

SANE_Word ScannerOption::setOption(std::string val){
    SANE_Word word = 0;
    SANE_Word details;
    
    switch(_desc->type){
        case SANE_TYPE_BOOL:
        case SANE_TYPE_INT:
            word = std::atoi(val.c_str());
            if(auto status = sane_control_option(_handle,_optionNo,SANE_ACTION_SET_VALUE,&word,&details); status != SANE_STATUS_GOOD){
                throw std::runtime_error("Couldn't set option to value: "+ val + " error: "+sane_strstatus(status));
            }
            break;
        case SANE_TYPE_FIXED: 
            word = floatToFixed(std::atof(val.c_str()));
            if(auto status = sane_control_option(_handle,_optionNo,SANE_ACTION_SET_VALUE,&word,&details);status != SANE_STATUS_GOOD){
                throw std::runtime_error("Couldn't set option to value: "+ val + " error: "+sane_strstatus(status));
            }
            break;
        case SANE_TYPE_STRING:
            {
                char * buffer = new char[_desc->size];
                memcpy(buffer,val.c_str(),_desc->size);
                if(auto status = sane_control_option(_handle,_optionNo,SANE_ACTION_SET_VALUE,buffer,&details); status != SANE_STATUS_GOOD){
                    throw std::runtime_error("Couldn't set option to value: "+ val + " error: "+sane_strstatus(status));
                }
                delete[] buffer;
            }
            break;
        default:
            break;
    }
    return details;
}