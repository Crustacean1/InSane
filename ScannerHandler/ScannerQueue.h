#ifndef SCANNER_QUEUE
#define SCANNER_QUEUE

#include <memory>
#include <mutex>
#include <thread>
#include "../Scanner/Scanner.h"
#include "../Scanner/ScannerOptionDto.h"

class ScannerOptionDto;
class Image;

class ScannerQueue{
    std::unique_ptr<Scanner> _scanner;
    std::shared_ptr<Image> _scan;

    std::mutex _scannerMutex;
    std::thread * _scannerThread;

    std::mutex _optionMutex;
    std::vector<ScannerOptionDto> _optionBuffer;

    void scanImage();
    void refreshOptions();

    public:

    ScannerQueue(std::unique_ptr<Scanner> &&scannerPtr);
    bool tryStartScanning();
    bool tryRefreshOptions();
    bool trySetOption(size_t optionNo,const std::string &value);
    std::vector<ScannerOptionDto> getOptions();
    std::shared_ptr<Image> tryGetScanResult();
};

#endif /*SCANNER_QUEUE*/