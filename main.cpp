#include "Scanner/Scanner.h"
#include "ScannerHandler/ScannerQueue.h"
#include <iostream>
#include "Image.h"
#include <chrono>
#include "HttpServer/HttpListener.h"
#include "Scanner/ScannerManager.h"

int main(int argc, char **argv) {
  try{
      int threadNumber = 1;
      boost::asio::io_context ioc{threadNumber};
      HttpListener
  server(ioc,boost::asio::ip::tcp::endpoint{boost::asio::ip::make_address("127.0.0.1"),2138});
      server.run();
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