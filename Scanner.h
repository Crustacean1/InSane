#ifndef SCANNER
#define SCANNER

#include <sane/sane.h>
#include <vector>
#include <tuple>
#include <iostream>
#include "ScannerOption.h"


class Scanner {
  SANE_Handle _scannerHandle;
  SANE_Parameters _params;

  std::vector<ScannerOption> _options;
  float _progress;

  static constexpr uint32_t CHANNELS[]={4};
public:
    Scanner(std::string scannerName);
    ~Scanner();

    void setOption(size_t option,std::string val);
    void reloadOptions();
    void reloadParams();

    std::tuple<unsigned char*,size_t,size_t,size_t> scan();
    float getProgress();
    std::map<std::string,std::vector<ScannerOption>> getOptions();
};
#endif /*SCANNER*/