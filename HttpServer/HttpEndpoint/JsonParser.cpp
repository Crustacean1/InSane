#include "JsonParser.h"
#include <json/reader.h>
#include "../../Scanner/ScannerOptionDto.h"
#include "HttpEndpoint.h"
#include <iostream>

JsonParser::JsonParser(){
    Json::CharReaderBuilder builder;
    builder["collectComments"] = false;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    _reader = std::move(reader);
    std::unique_ptr<Json::FastWriter> writer(new Json::FastWriter());
    _writer = std::move(writer);
}
Json::Value JsonParser::parse(const std::string &data){
    _errorBuffer.clear();
    Json::Value result;
    std::cout<<"trying to parse..."<<data<<"\t"<<data.length()<<std::endl;
    if(!_reader->parse(data.c_str(),data.c_str()+data.length(),&result,&_errorBuffer)){
        std::cout<<"error"<<std::endl;
        throw std::runtime_error("In JsonParser;:parse: failed to parse value: " + _errorBuffer);
    }
    return result;
}
std::string JsonParser::serialize(const Json::Value &val){
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

Json::Value JsonParser::parse(const HttpEndpoint::ErrorDto & dto){
    Json::Value error;
    error["details"] = dto.details;
    return error;
}

JsonParser::~JsonParser(){}