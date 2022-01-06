#ifndef EXECUTION_QUEUE
#define EXECUTION_QUEUE

#include "Scanner.h"
#include <memory>
#include <functional>
#include <string>
#include <queue>

class ExecutionQueue{

    //std::queue<std::function<void()>> _stack;
    //std::string _auth;
    //uint32_t _authTimeout;

    const std::string _scannerName;

    std::shared_ptr<std::pair<Scanner> _scanner;
    nlohmann::json _scannerOptions;

    nlohmann::json authenticate();
    nlohmann::json scan(std::string filpath = "/tmp/scan01.png");
    nlohmann::json setOption(uint32_t options,std::string t);
    nlohmann::json getOptionsInJson();
    nlohmann::json getProgressInJson();

    friend int main(int,char**);

    public:

    ExecutionQueue(SANE_String_Const name);
    std::tuple<nlohmann::json,size_t> execute(nlohmann::json request); //Immediate return is granteed

};

#endif /*EXECUTION_QUEUE*/