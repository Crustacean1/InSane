#include "ScannerManager.h"

#include <sane/sane.h>

#include <chrono>
#include <functional>
#include <iostream>
#include <memory>

#include "../Scanner/Scanner.h"
#include "../HttpServer/HttpListener/Scheduler.h"

std::shared_ptr<ScannerManager> ScannerManager::managerInstance = nullptr;

std::shared_ptr<ScannerManager> ScannerManager::createScannerManager(size_t scannerRefreshRate){
  if(managerInstance == nullptr){
    managerInstance = std::shared_ptr<ScannerManager>(new ScannerManager(scannerRefreshRate));
  }
  return managerInstance;
}

ScannerManager::ScannerManager(size_t scannerRefreshRate) : _scannerRefreshRate(scannerRefreshRate){
  SANE_Int saneVersion = 1;

  auto status = sane_init(&saneVersion, NULL);
  if (status != SANE_STATUS_GOOD) {
    throw std::runtime_error("In ScannerManager::ScannerManager\n" +
                             std::string(sane_strstatus(status)));
  }
}

bool ScannerManager::refreshFunction(std::weak_ptr<ScannerManager> manager){
  
  if(auto exisitingManager = manager.lock()){
    exisitingManager->releaseOldScanners();
    return true;
  }
  std::cout<<"Manager destroyed"<<std::endl;
  return false;
}

void ScannerManager::setRefreshScheduler(Scheduler &scheduler){
  scheduler.addRegularTask(std::bind(ScannerManager::refreshFunction,std::weak_ptr<ScannerManager>(shared_from_this())),_scannerRefreshRate);
}

void ScannerManager::releaseOldScanners() {
  std::lock_guard<std::mutex> lock(_mapMutex);
  for (const auto &[key, value] : _scannerQueues) {
    value->tryReleaseScanner();
  }
}

ScannerManager::~ScannerManager() {
  _scannerQueues.clear();
  sane_exit();
}

std::shared_ptr<ScannerQueue>
ScannerManager::getScanner(const std::string &name) {
  std::lock_guard<std::mutex> guard(_mapMutex);
  return getScannerFromMap(name);
}

std::vector<std::string> ScannerManager::getScannerList() {
  std::lock_guard<std::mutex> guard(_mapMutex);
  return _connectedScanners;
}

void ScannerManager::refreshScannerList() {
  std::lock_guard<std::mutex> guard(_mapMutex);
  const SANE_Device **deviceList;
  auto status = sane_get_devices(&deviceList, (SANE_Bool)0);
  if (status != SANE_STATUS_GOOD) {
    throw std::runtime_error("Failed to retrieve device list: " +
                             std::string(sane_strstatus(status)));
  }
  _connectedScanners.clear();
  for (int i = 0; deviceList[i]; ++i) {
    _connectedScanners.push_back(std::string(deviceList[i]->name));
  }
}

std::shared_ptr<ScannerQueue>
ScannerManager::getScannerFromMap(const std::string &name) {
  if (_scannerQueues.find(name) == _scannerQueues.end()) {
    _scannerQueues[name] = createScanner(name);
  }
  return _scannerQueues[name];
}

std::shared_ptr<ScannerQueue>
ScannerManager::createScanner(const std::string &name) {
  auto queue(std::make_shared<ScannerQueue>(name));
  return queue;
}
