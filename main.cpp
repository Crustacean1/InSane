#include <iostream>
//#include "ScannerManager.h"
//#include <nlohmann/json.hpp>
#include "HttpListener.h"


int main(int argc, char ** argv)
{
    /*try{
        int threadNumber = 1;
        boost::asio::io_context ioc{threadNumber};
        HttpListener server(ioc,boost::asio::ip::tcp::endpoint{boost::asio::ip::make_address("127.0.0.1"),2138});
        server.run();
        ioc.run();

    }catch(std::exception & e){
        std::cerr<<"Failed to start post server: "<<e.what()<<std::endl;
    }*/
    
    //ScannerManager manager;


    //try{

    //manager.refreshDeviceList(); Can be used to find devices, but takes some time

    //auto scanner = manager.getScanner("net:192.168.1.9:hpaio:/usb/HP_LaserJet_Pro_MFP_M125a?serial=CNB6H9PBMM");

    //scanner->setOption(2,std::string("Color"));
    //scanner->setOption(3,std::string("1200"));
    //scanner->setOption(8,std::string("None"));

    //std::cout<<scanner->getOptionsInJson().dump()<<std::endl;
    /*if(argc<2){
        return 0;
    }
    //scanner->scan(std::string(argv[1]));

    }catch(std::runtime_error &e){
        std::cerr<<e.what()<<std::endl;
        return -1;
    }*/
}