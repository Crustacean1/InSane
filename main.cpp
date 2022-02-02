#include "Scanner/Scanner.h"
#include <iostream>
#include "Image.h"
//#include <nlohmann/json.hpp>
#include "HttpServer/HttpListener.h"
#include "Scanner/ScannerManager.h"

int main(int argc, char **argv) {
  /*try{
      int threadNumber = 1;
      boost::asio::io_context ioc{threadNumber};
      HttpListener
  server(ioc,boost::asio::ip::tcp::endpoint{boost::asio::ip::make_address("127.0.0.1"),2138});
      server.run();
      ioc.run();

  }catch(std::exception & e){
      std::cerr<<"Failed to start post server: "<<e.what()<<std::endl;
  }*/

    if(argc<2){return -1;}
  try {

//ScannerManager manager;
//manager.refreshDeviceList();
//std::cout<<manager.getScannerListInJson();
    // manager.refreshDeviceList(); Can be used to find devices, but takes some
    // time

    /*scanner->setOption(2,std::string("Color"));
    scanner->setOption(3,std::string("1200"));
    scanner->setOption(8,std::string("None"));*/
    /*Scanner::init();
    auto scanners = Scanner::scanForScanners();
    for(const auto &scannerName : scanners ){
      std::cout<<"scanner found: "<<scannerName<<std::endl;
    }*/
    std::string scannerName(argv[1]);
    std::cout<<"starting scanner initialization"<<std::endl;
    auto scanner = new Scanner(scannerName);
    // std::cout<<scanner->getOptionsInJson().dump()<<std::endl;
    scanner->getOption("resolution").set("1200",NULL);
    scanner->getOption("mode").set("Color",NULL);
    scanner->debugDump();

    PngImage newImage;;
    scanner->scan(newImage);
    newImage.save("image.png");

  } catch (std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }
}