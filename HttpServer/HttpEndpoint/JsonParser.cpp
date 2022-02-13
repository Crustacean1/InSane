#include "JsonParser.h"
#include <json/reader.h>
#include "../../Scanner/ScannerOptionDto.h"

JsonParser::JsonParser(){
    Json::CharReaderBuilder builder;
    builder["collectComments"] = false;
    _reader = builder.newCharReader();
    _writer = new Json::FastWriter();
}
Json::Value JsonParser::parse(const std::string &data){
    _errorBuffer.clear();
    Json::Value result;
    if(!_reader->parse(data.c_str(),data.c_str()+data.size(),&result,&_errorBuffer)){
        throw std::runtime_error("In JsonParser;:parse: failed to parse value: " + _errorBuffer);
    }
    return result;
}
std::string JsonParser::parse(const Json::Value &val){
    return _writer->write(val);
}

Json::Value JsonParser::parse(const ScannerOptionDto & dto){
    Json::Value option;
    option["title"] = dto.title;
    option["desc"] = dto.desc;
    option["value"] = dto.value;
    option["unit"] = dto.unit;
    option["constraint"] = dto.constraint;
    option["type"] = dto.type;
    option["optionNo"] = dto.optionNo;

    for(const auto &val : dto.valueList){
        option["value_range"].append(val);
    }
    return option;
}

JsonParser::~JsonParser(){
    delete _reader;
    delete _writer;
}