#include "Scanner/Scanner.h"
#include "ScannerHandler/ScannerQueue.h"
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

  try {

    std::string scannerName(argv[1]);
    std::cout<<"starting scanner initialization"<<std::endl;

    Scanner::init();
    //ScannerQueue queue(std::make_unique<Scanner>(scannerName));
    //queue.tryRefreshOptions();
    auto scanner = new Scanner(scannerName);
    auto optionCount = scanner->getOptionCount();
    for(int i = 0;i<optionCount;++i) {
      std::cout<<scanner->getOption(i).title<<std::endl;
    }


    //scanner->getOption("resolution").set("1200",NULL);

    //scanner->setOption(0,"Color");
    //scanner->debugDump();

    /*PngImage newImage;
    size_t progress;
    scanner->scan(newImage,progress);
    newImage.save("image.png");*/

  } catch (std::runtime_error &e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }
}