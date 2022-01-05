#ifndef EXECUTION_QUEUE
#define EXECUTION_QUEUE

#include "Scanner.h"
#include <memory>
#include <functional>
#include <string>
#include <queue>

class ExecutionQueue{

    std::queue<std::function<void()>> _stack;
    std::string _auth;
    uint32_t _authTimeout;

    SANE_String_Const _scannerName;
    std::shared_ptr<Scanner> _scanner;

    

    public:

    ExecutionQueue(SANE_String_Const name);

    std::string authenticate();

    void scan(std::string filpath = "/tmp/scan01.png");
    void setOption(uint32_t options,std::string t);
    std::string getOptionsInJson();
    std::string getProgressInJson();
};

#endif /*EXECUTION_QUEUE*/