#ifndef EXECUTION_QUEUE
#define EXECUTION_QUEUE

#include "Scanner/Scanner.h"
#include <memory>
#include <string>

class ExecutionQueue{

    const std::string _scannerName;

    std::shared_ptr<Scanner> _scanner;

    void refreshJsonOptions();

    void scan(std::string filpath = "/tmp/scan01.png");
    void setOption(uint32_t options,std::string t);
    size_t getProgess();

    friend int main(int,char**);

    public:

    ExecutionQueue(SANE_String_Const name);

    std::string getName();
};

#endif /*EXECUTION_QUEUE*/