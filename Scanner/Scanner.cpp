#include "Scanner.h"

#include <algorithm>
#include <cstring>
#include <exception>
#include <iostream>

#include "../Image.h"
#include "ScannerOptionDto.h"

Scanner::Scanner(std::string scannerName, size_t bufferSize) : _bufferSize(bufferSize) {
    if (sane_open(scannerName.c_str(), &_scannerHandle) != SANE_STATUS_GOOD) {
        throw std::runtime_error("Failed to open device: ~" + std::string(scannerName));
    }
    reloadOptions();
    reloadParams();
}

Scanner::Scanner(Scanner&& scanner) : _scannerHandle(scanner._scannerHandle), _params(scanner._params), _options(std::move(scanner._options)), _bufferSize(scanner._bufferSize) {
    scanner._scannerHandle = nullptr;
    std::cout << "move performed" << std::endl;
}

Scanner::~Scanner() {
    if (_scannerHandle != nullptr) {
        sane_close(_scannerHandle);
    }
}

void Scanner::reloadOptions() {
    SANE_Status status;
    status = sane_get_parameters(_scannerHandle, &_params);
    if (status != SANE_STATUS_GOOD) {
        throw std::runtime_error("Couldn't read parameters: " + std::string(sane_strstatus(status)));
    }
    _options.clear();
    for (SANE_Word i = 1; sane_get_option_descriptor(_scannerHandle, i) != NULL; ++i) {
        std::unique_ptr<ScannerOption> option = std::make_unique<ScannerOption>(_scannerHandle, i);
        _options.push_back(std::move(option));
    }
}

void Scanner::reloadParams() {
    auto status = sane_get_parameters(_scannerHandle, &_params);
    if (status != SANE_STATUS_GOOD) {
        throw std::runtime_error("Failed to read params from scanner" + std::string(sane_strstatus(status)));
    }
}

void Scanner::initImage(Image& image) {
    image.setWidth(_params.pixels_per_line);
    image.setHeight(_params.lines);
    image.setChannelCount(_params.format == SANE_FRAME_RGB ? 3 : 1);
    image.setDepth(_params.depth);
    image.createBuffer();
}

void Scanner::initScanning(ScanningStatus & scanStatus) {
    scanStatus.state = ScanningStatus::State::Initializing;

    auto error_code = sane_start(_scannerHandle);
    if (error_code != SANE_STATUS_GOOD) {
        switch (error_code) {
            case SANE_STATUS_DEVICE_BUSY:
                throw std::runtime_error("Device busy");
            case SANE_STATUS_INVAL:
                std::cerr << "Invalid options detected" << std::endl;
            default:
                throw std::runtime_error("Failed to start scanning: " + std::string(sane_strstatus(error_code)));
        }
    }

    reloadParams();
}

void Scanner::scan(Image& image, ScanningStatus& scanStatus) {  // idea: custom execption type for json wrapping

    initScanning(scanStatus);
    initImage(image);

    size_t expectedPixels = image.getImageSize();
    size_t total = 0;

    SANE_Int bytesRead;
    SANE_Status readStatus;

    unsigned char* buffer = new unsigned char[_bufferSize];

    scanStatus.progress = 0;
    scanStatus.state = ScanningStatus::State::Scanning;

    while ((readStatus = sane_read(_scannerHandle, buffer, _bufferSize, &bytesRead)) == SANE_STATUS_GOOD) {
        image.read(buffer, bytesRead);
        total += bytesRead;
        scanStatus.progress = total * 100 / expectedPixels;
    }

    std::cout << "read: " << total << " bytes+" << bytesRead << std::endl;
    image.flushBuffer();

    delete[] buffer;
    if (readStatus != SANE_STATUS_EOF && readStatus != SANE_STATUS_GOOD) {
        std::cout << readStatus << std::endl;
        throw std::runtime_error("Failed during read: " + std::string(sane_strstatus(readStatus)));
    }
}

void Scanner::debugDump() {
    for (const auto& option : _options) {
        try {
            std::cout << option->getName() << "\t" << option->getTitle() << "\t" << option->getDesc() << "\t" << option->get(NULL) << std::endl;
        } catch (std::runtime_error& e) {
            std::cout << e.what() << "\n\n" << std::endl;
        }
        for (const auto& constr : option->getConstraints()) {
            std::cout << constr << "\t";
        }
        std::cout << "\n\n\n";
    }
}
size_t Scanner::getOptionCount() { return _options.size(); }

ScannerOption& Scanner::getRawOption(size_t optionNo) {
    if (optionNo > _options.size()) {
        throw std::runtime_error("Tried to access nonexistent option");
    }
    return *_options[optionNo];
}

void Scanner::restoreState(SANE_Word& details) {
    if (details & SANE_INFO_RELOAD_OPTIONS) {
        reloadOptions();
    }
    if (details & SANE_INFO_RELOAD_PARAMS) {
        reloadParams();
    }
}

void Scanner::setOption(size_t optionNo, std::string value) {
    auto& option = getRawOption(optionNo);
    SANE_Word details;
    option.set(value, &details);
    std::cout << "Option set" << std::endl;
    restoreState(details);
}

ScannerOptionDto Scanner::getOption(size_t optionNo) {
    const auto& option = getRawOption(optionNo);
    SANE_Word details;
    ScannerOptionDto result(optionNo, option, &details);
    restoreState(details);
    return result;
}

void Scanner::resetOptions() {
    for (auto& option : _options) {
        try {
            option->reset(NULL);
        } catch (std::runtime_error& e) {
            std::cerr << e.what() << std::endl;
        }
    }
}

std::string ScanningStatus::statusToString() { return std::string(STATUS_NAMES[static_cast<unsigned int>(state)]); }