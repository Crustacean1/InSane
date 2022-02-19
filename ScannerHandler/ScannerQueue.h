#ifndef SCANNER_QUEUE
#define SCANNER_QUEUE

#include <memory>
#include <mutex>
#include <thread>
#include "../Scanner/Scanner.h"
#include "../Scanner/ScannerOptionDto.h"
#include "../Image.h"

class ScannerOptionDto;

struct ScanningStatus{
    static constexpr const char * STATUS_NAMES[] = {"Ready","Initializing","Scanning","Writing","Completed","Failed"};
    enum class Status : unsigned char{
        Ready,
        Initializing,
        Scanning,
        Writing,
        Completed,
        Failed
    };
    size_t progress;
    Status status;
    std::string statusToString();
};

class ScannerQueue{
    std::string _scannerName;
    std::shared_ptr<Scanner> _scanner;
    std::shared_ptr<Image> _scan;

    std::mutex _scannerMutex;
    std::shared_ptr<std::thread> _scannerThread;

    std::shared_ptr<OptionDtoCollection> _optionCollectionBuffer;

    std::shared_ptr<Image::ImageBuffer> _scanBuffer;

    ScanningStatus _scanningStatus;

    size_t _scannerTimeout;
    size_t _activityTimestamp;

    void reserveScanner();

    void scanImage();
    void refreshOptions();
    void readOptions();
    void readScan(Image & image);
    void updateTimestamp();

    void scan(std::unique_ptr<std::unique_lock<std::mutex>> lock);

    ScannerQueue(const ScannerQueue&);

    public:

    ScannerQueue(const std::string &scannerName, size_t scannerTimeout = 30000);
    ScannerQueue(ScannerQueue&&);

    bool tryScan();
    bool tryRefreshOptions();
    bool trySetOption(size_t optionNo,const std::string &value);
    bool tryResetOptions();
    bool tryReleaseScanner();

    std::vector<ScannerOptionDto> getOptions();
    ScanningStatus getScanningStatus();
    std::shared_ptr<Image::ImageBuffer> getScanResult();

    ~ScannerQueue();
};

#endif /*SCANNER_QUEUE*/