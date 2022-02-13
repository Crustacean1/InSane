#ifndef JSON_PARSER
#define JSON_PARSER

#include <json/json.h>
#include <json/reader.h>
#include <string>

class ScannerOptionDto;

class JsonParser{
    Json::CharReader * _reader;
    Json::FastWriter * _writer;
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

    std::string parse(const Json::Value &val);
};

#endif /*JSON_PARSER*/