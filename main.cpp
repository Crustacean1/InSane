#include <iostream>
#include "HttpServer/HttpListener/HttpListener.h"
#include "ScannerHandler/ScannerManager.h"
#include "Logic/ScannerProcessorBuilder.h"

int main(int argc, char **argv) {
  /*try{
    int threadNumber = 1;
    boost::asio::io_context ioc{threadNumber};

    ScannerManager scannerManager;
    ScannerProcessorBuilder builder(scannerManager);

    KHttp::HttpListener server(builder,
                      ioc,
                      boost::asio::ip::tcp::endpoint{boost::asio::ip::make_address("127.0.0.1"),2138});
    server.run();
    std::cout<<"server started"<<std::endl;
    std::vector<std::thread> threads;
    int threadCount = 3;
    for(int i = 0;i<threadCount;++i){
        threads.emplace_back([&ioc]{ioc.run();});
    }
    ioc.run();
  }catch(std::exception & e){
      std::cerr<<"Failed to start post server: "<<e.what()<<std::endl;
  }*/
  try{
    /*ScannerManager manager;
    std::cout<<"Manager initialized"<<std::endl;
    auto scanners = manager.getScannerList();
    for(const auto & el : scanners){
      std::cout<<"Scanner found: "<<el<<std::endl;
    }
    auto scanner = manager.getScanner("v4l:/dev/video0");
    std::cout<<"scanner obtained"<<std::endl;
    scanner->tryScan();*/
    ScannerQueue queue(std::make_unique<Scanner>("v4l:/dev/video0"));
    queue.tryScan();
    //while(true){}
  }catch(std::exception &e){

  }
}