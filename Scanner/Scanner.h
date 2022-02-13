#ifndef SCANNER
#define SCANNER

#include <sane/sane.h>
#include <vector>
#include <tuple>
#include <iostream>
#include "ScannerOption.h"
#include <map>

class Image;
class ScanStatus;
class ScannerOptionDto;

class Scanner {
  SANE_Handle _scannerHandle;
  SANE_Parameters _params;

  float _progress;

  static constexpr uint32_t CHANNELS[]={4};
  std::vector<std::unique_ptr<ScannerOption>> _options;

  ScannerOption& getRawOption(size_t optionNo);
  void initImage(Image &image);
  void restoreState(SANE_Word & word);
public:
    Scanner(std::string scannerName);
    Scanner(Scanner&& scanner);
    ~Scanner();

    void reloadOptions();
    void reloadParams();

    void debugDump();

    size_t getOptionCount();


    void setOption(size_t optionNo,std::string value);
    ScannerOptionDto getOption(size_t optionNo);

    void scan(Image &image,size_t & status);
};
#endif /*SCANNER*/