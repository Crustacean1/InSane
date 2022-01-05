#ifndef SCANNER
#define SCANNER

#include <exception>
#include <sane/sane.h>
#include <vector>
#include <iostream>
#include "ScannerOption.h"


class Scanner {
  SANE_Handle _scannerHandle;
  SANE_Parameters _params;

  std::vector<ScannerOption> _options;

public:
    Scanner(SANE_String_Const scannerName);
    ~Scanner();

    ScannerOption &getOption(size_t option);

    void scan(std::ostream & stream);
    float getProgress();
    std::string getJsonOptions();
};
#endif /*SCANNER*/