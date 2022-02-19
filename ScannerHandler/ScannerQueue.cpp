#include "ScannerQueue.h"

#include <stdio.h>

#include <chrono>
#include <fstream>
#include <iostream>
#include <mutex>

#include "../Image.h"
#include "../Scanner/ScannerOptionDto.h"

ScannerQueue::ScannerQueue(const std::string &scannerName, size_t scannerTimeout)
    : _scannerName(scannerName),
      _scanner(nullptr),
      _scannerThread(std::make_shared<std::thread>([]() {})),
      _optionCollectionBuffer(std::make_shared<OptionDtoCollection>()),
      _scanBuffer(std::make_shared<Image::ImageBuffer>()),
      _scanningStatus{0, ScanningStatus::Status::Ready},
      _scannerTimeout(scannerTimeout),
      _activityTimestamp(std::chrono::system_clock::now().time_since_epoch().count()) {
    tryRefreshOptions();
}

ScannerQueue::~ScannerQueue() { _scannerThread->join(); }

bool ScannerQueue::tryRefreshOptions() {
    updateTimestamp();
    std::unique_lock<std::mutex> lock(_scannerMutex,std::try_to_lock);
    if (!lock.owns_lock()) {
        return false;
    }
    reserveScanner();
    _scanner->reloadOptions();
    readOptions();
    return true;
}

bool ScannerQueue::trySetOption(size_t optionNo, const std::string &value) {
    updateTimestamp();
    std::unique_lock<std::mutex> lock(_scannerMutex,std::try_to_lock);
    if (!lock.owns_lock()) {
        std::cerr << "Couldn't secure lock to try set option" << std::endl;
        return false;
    }

    try {
        std::cout << "setting option: " << optionNo << "\t" << value << std::endl;
        reserveScanner();
        _scanner->setOption(optionNo, value);
        _scanner->reloadOptions();
        readOptions();

    } catch (std::runtime_error &e) {
        std::cerr << "In ScannerQueue, trySetOption\n" << e.what() << std::endl;
        return false;
    }
    return true;
}

bool ScannerQueue::tryResetOptions() {
    updateTimestamp();
    std::unique_lock<std::mutex> lock(_scannerMutex,std::try_to_lock);
    if (!lock.owns_lock()) {
        std::cerr << "Couldn't secure lock to reset options" << std::endl;
        return false;
    }
    try {
        reserveScanner();
        _scanner->resetOptions();
        _scanner->reloadOptions();
        readOptions();
    } catch (std::runtime_error &e) {
        std::cerr << "In ScannerQueue::tryResetOptions:\n" << e.what() << std::endl;
        return false;
    }
    return true;
}

bool ScannerQueue::tryScan() {
    updateTimestamp();
    std::unique_lock<std::mutex> lock(_scannerMutex,std::try_to_lock);
    if (!lock.owns_lock()) {
        return false;
    }
    reserveScanner();
    _scannerThread->join();
    _scannerThread = std::make_unique<std::thread>(&ScannerQueue::scan, this);
    return true;
}

ScanningStatus ScannerQueue::getScanningStatus() {
    updateTimestamp();
    auto oldState = _scanningStatus;
    if (_scanningStatus.status == ScanningStatus::Status::Completed) {
        _scanningStatus.status = ScanningStatus::Status::Ready;
    }
    return oldState;
}

std::vector<ScannerOptionDto> ScannerQueue::getOptions() {
    updateTimestamp();
    reserveScanner();
    return _optionCollectionBuffer->options;
}
std::shared_ptr<Image::ImageBuffer> ScannerQueue::getScanResult() {
    updateTimestamp();
    return _scanBuffer;
}

bool ScannerQueue::tryReleaseScanner() {
    std::cout << "\t\tTrying to release scanner" << std::endl;
    std::unique_lock<std::mutex> lock(_scannerMutex,std::try_to_lock);
    if (!lock.owns_lock()) {
        return false;
    }
    auto currentTime = std::chrono::seconds(std::time(NULL)).count();
    std::cout << "Time delta: " << (currentTime - _activityTimestamp) << "\t" << _scannerTimeout << std::endl;
    if ((currentTime - _activityTimestamp) > _scannerTimeout) {
        std::cerr << "\t\tReleasing scanner: " << std::endl;
        _scanner = nullptr;
        return true;
    }
    return false;
}

void ScannerQueue::updateTimestamp() { _activityTimestamp = std::chrono::seconds(std::time(NULL)).count(); }

void ScannerQueue::readOptions() {
    size_t optionCount = _scanner->getOptionCount();
    std::shared_ptr<OptionDtoCollection> dtoCollection = std::make_shared<OptionDtoCollection>();

    for (size_t i = 0; i < optionCount; ++i) {
        try {
            auto option = _scanner->getOption(i);
            dtoCollection->options.push_back(option);
        } catch (std::runtime_error &e) {
            std::cerr << "In ScannerQueue::readOptions: " << e.what() << "\n" << std::endl;
        }
    }
    _optionCollectionBuffer.swap(dtoCollection);
}

void ScannerQueue::scan() {
    try {
        std::lock_guard<std::mutex> lock(_scannerMutex);
        _scanningStatus.progress = 0;
        _scanningStatus.status = ScanningStatus::Status::Scanning;
        PngImage newImage;
        _scanner->scan(newImage, _scanningStatus.progress);
        readScan(newImage);
    } catch (std::runtime_error &e) {
        _scanningStatus.status = ScanningStatus::Status::Failed;
        std::cerr << "From ScannerQueue::scan:\n" << e.what() << std::endl;
    }
}

void ScannerQueue::readScan(Image &image) {
    _scanningStatus.status = ScanningStatus::Status::Writing;
    auto newBuffer = std::make_shared<Image::ImageBuffer>();
    image.writeToBuffer(*newBuffer);
    _scanBuffer.swap(newBuffer);
    _scanningStatus.status = ScanningStatus::Status::Completed;
}

void ScannerQueue::reserveScanner() {
    if (_scanner == nullptr) {
        _scanner = std::make_shared<Scanner>(_scannerName);
        _scanner->reloadOptions();
        readOptions();
    }
}

std::string ScanningStatus::statusToString() { return std::string(STATUS_NAMES[static_cast<unsigned int>(status)]); }