#ifndef SCANNERMANAGER
#define SCANNERMANAGER

#include <map>
#include <sane/sane.h>
#include <exception>
#include <memory>
#include <iostream>

class ScannerManager{
    public:
    
    ScannerManager();
    void refreshDeviceList();
    ~ScannerManager();
};

#endif /*SCANNERMANAGER*/