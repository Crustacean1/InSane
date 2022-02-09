#ifndef SCANNER_QUEUE
#define SCANNER_QUEUE

#include <memory>
#include <mutex>
#include <thread>
#include "../Scanner/Scanner.h"
#include "../Scanner/ScannerOptionDto.h"

class ScannerOptionDto;
class Image;

struct ScanningStatus{
    enum class Status : unsigned char{
        Ready,
        Waiting,
        Pending,
        Scanning,
        Completed,
        Failed
    };
    size_t progress;
    Status status;
};

class ScannerQueue{
    std::unique_ptr<Scanner> _scanner;
    std::shared_ptr<Image> _scan;

    std::mutex _scannerMutex;
    std::unique_ptr<std::thread> _scannerThread;

    std::unique_ptr<OptionDtoCollection> _optionCollectionBuffer;

    std::shared_ptr<Image> _scannedImage;

    ScanningStatus _scanningStatus;

    void scanImage();
    void refreshOptions();
    void readOptions();

    void scan(std::unique_ptr<std::unique_lock<std::mutex>> lock);

    ScannerQueue(const ScannerQueue&);

    public:

    ScannerQueue(std::unique_ptr<Scanner> &&scannerPtr);
    ScannerQueue(ScannerQueue&&);

    bool tryScan();
    bool tryRefreshOptions();
    bool trySetOption(size_t optionNo,const std::string &value);

    std::vector<ScannerOptionDto> getOptions();
    ScanningStatus getScanningStatus();
    std::shared_ptr<Image> getScanResult();

    ~ScannerQueue();
};

#endif /*SCANNER_QUEUE*/