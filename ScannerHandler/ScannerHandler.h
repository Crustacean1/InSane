#ifndef SCANNER_HANDLER
#define SCANNER_HANDLER

#include <memory>
#include <boost/asio.hpp>

class Scanner;

class ScannerHandler{
    //std::unique_ptr<Scanner> _scanner;
    //boost::asio::strand<boost::asio::io_context::executor_type> _strand;
    public:
    ScannerHandler(Scanner && scanner,boost::asio::io_context &io);
    nlohmann::json setOption(std::string option,std::string value);
    nlohmann::json startScan();
    nlohmann::json getInfo();
    nlohmann::json getStatus();
};

#endif /*SCANNER_HANDLER*/