#ifndef SCANNER
#define SCANNER

#include <sane/sane.h>

#include <iostream>
#include <map>
#include <tuple>
#include <vector>

#include "ScannerOption.h"

class Image;
class ScanStatus;
class ScannerOptionDto;

struct ScanningStatus {
    static constexpr const char* STATUS_NAMES[] = {"Ready", "Initializing", "Scanning", "Writing", "Completed", "Failed"};
    enum class State : unsigned char { Ready, Initializing, Scanning, Writing, Completed, Failed };
    size_t progress;
    State state;
    std::string statusToString();
};

class Scanner {
    SANE_Handle _scannerHandle;
    SANE_Parameters _params;

    static constexpr uint32_t CHANNELS[] = {4};
    std::vector<std::unique_ptr<ScannerOption>> _options;

    const size_t _bufferSize;

    ScannerOption& getRawOption(size_t optionNo);
    void initImage(Image& image);
    void initScanning(ScanningStatus & scanStatus);

    void restoreState(SANE_Word& word);

   public:
    Scanner(std::string scannerName, size_t bufferSize = 1024);
    Scanner(Scanner&& scanner);
    ~Scanner();

    void reloadOptions();
    void reloadParams();

    void debugDump();

    size_t getOptionCount();

    void setOption(size_t optionNo, std::string value);
    void resetOptions();
    ScannerOptionDto getOption(size_t optionNo);

    void scan(Image& image, ScanningStatus& status);
};
#endif /*SCANNER*/