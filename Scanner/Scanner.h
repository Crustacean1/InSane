#ifndef SCANNER
#define SCANNER

#include <sane/sane.h>
#include <vector>
#include <tuple>
#include <iostream>
#include "ScannerOption.h"
#include <map>

class Image;

class Scanner {
  SANE_Handle _scannerHandle;
  SANE_Parameters _params;

  float _progress;

  static constexpr uint32_t CHANNELS[]={4};
  std::vector<std::unique_ptr<ScannerOption>> _options;
public:
    static void init();
    static std::vector<std::string> scanForScanners();
    Scanner(std::string scannerName);
    ~Scanner();


    void reloadOptions();
    void reloadParams();

    void debugDump();
    ScannerOption& getOption(std::string name);

    void initImage(Image &image);
    void scan(Image &image);
};
#endif /*SCANNER*/