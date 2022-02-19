#include <chrono>
#include <iostream>
#include <thread>

#include "HttpServer/HttpListener/HttpListener.h"
#include "HttpServer/HttpProcessor/AsciiParser.h"
#include "HttpServer/HttpProcessor/Route.h"
#include "Image.h"
#include "Logic/ScannerProcessorBuilder.h"
#include "ScannerHandler/ScannerManager.h"
#include "HttpServer/HttpListener/Scheduler.h"

int main(int argc, char **argv) {
    try {
        if(argc != 2){
          std::cerr<<"Invalid amount of arguments specified, syntacx i ./Insane [ip address of server]"<<std::endl;
        }
        int threadNumber = 1;
        boost::asio::io_context ioc{threadNumber};

        Scheduler scheduler(ioc);
        auto scannerManager = ScannerManager::createScannerManager(60000);
        scannerManager->setRefreshScheduler(scheduler);

        ScannerProcessorBuilder builder(scannerManager);

        KHttp::HttpListener server(builder, ioc, boost::asio::ip::tcp::endpoint{boost::asio::ip::make_address(argv[1]), 2138});
        server.run();
        std::cout << "server started" << std::endl;
        std::vector<std::thread> threads;
        int threadCount = 3;

        for (int i = 1; i < threadCount; ++i) {
            threads.emplace_back([&ioc] { ioc.run(); });
        }
        ioc.run();
    } catch (std::exception &e) {
        std::cerr << "Failed to start post server: " << e.what() << std::endl;
    }
}