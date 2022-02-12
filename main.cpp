#include "Scanner/Scanner.h"
#include "ScannerHandler/ScannerQueue.h"
#include <iostream>
#include "Image.h"
#include <chrono>
#include "HttpServer/HttpListener/HttpListener.h"
#include "ScannerHandler/ScannerManager.h"
#include "Logic/ScannerProcessorBuilder.h"

int main(int argc, char **argv) {
  try{
    int threadNumber = 1;
    boost::asio::io_context ioc{threadNumber};

    ScannerManager scannerManager;
    std::cout<<"scanner manager created"<<std::endl;
    ScannerProcessorBuilder builder(scannerManager);
    std::cout<<"processor configuration created"<<std::endl;

    KHttp::HttpListener server(builder,
                      ioc,
                      boost::asio::ip::tcp::endpoint{boost::asio::ip::make_address("127.0.0.1"),2138});
    std::cout<<"server created"<<std::endl;
    server.run();
    std::cout<<"run started"<<std::endl;
    std::vector<std::thread> threads;
    int threadCount = 3;
    for(int i = 0;i<threadCount;++i){
        threads.emplace_back([&ioc]{ioc.run();});
    }
    ioc.run();
  }catch(std::exception & e){
      std::cerr<<"Failed to start post server: "<<e.what()<<std::endl;
  }
}