#ifndef JSON_PARSER
#define JSON_PARSER

#include <json/json.h>
#include <json/reader.h>
#include <string>
#include <memory>
#include "HttpEndpoint.h"

class ScannerOptionDto;

class JsonParser{
    std::unique_ptr<Json::CharReader> _reader;
    std::unique_ptr<Json::FastWriter>  _writer;
    std::string _errorBuffer;
    JsonParser& operator=(const JsonParser &) = delete;
    JsonParser& operator=(JsonParser &&) = delete;
    JsonParser(const JsonParser &) = delete;
    JsonParser(JsonParser &&) = delete;

    public:
    JsonParser();
    ~JsonParser();
    Json::Value parse(const std::string & data);
    Json::Value parse(const ScannerOptionDto & dto);
    Json::Value parse(const HttpEndpoint::ErrorDto & dto);

    std::string serialize(const Json::Value &val);
};

#endif /*JSON_PARSER*/