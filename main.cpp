#include <iostream>
#include "HttpServer/HttpListener/HttpListener.h"
#include "ScannerHandler/ScannerManager.h"
#include "Logic/ScannerProcessorBuilder.h"
#include "Image.h"
#include "HttpServer/HttpProcessor/Route.h"
#include "HttpServer/HttpProcessor/AsciiParser.h"

#include <thread>
#include <chrono>


int main(int argc, char **argv) {
  try{
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
  }
}