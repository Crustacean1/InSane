#ifndef SCANNER_VALUE
#define SCANNER_VALUE

#include <sane/sane.h>
#include <string>
#include <vector>

class OptionValue {
public:
  virtual void load(const std::string &str) = 0;
  virtual std::string get() =0;
  virtual void *getBuffer()  = 0;
  ~OptionValue() {}
};

class IntOptionValue : public OptionValue {
  SANE_Word _val;

public:
  void load(const std::string &str);
  std::string get();
  void *getBuffer();
  ~IntOptionValue() {}
};

class FixedOptionValue : public OptionValue {
  SANE_Word toFixed(float val);
  float fromFixed(SANE_Word val);
  SANE_Word _intRep;

public:
  void load(const std::string &str);
  void *getBuffer();
  std::string get();
  ~FixedOptionValue() {}
};

class StringOptionValue : public OptionValue {
  char *_buffer;

public:
  
  StringOptionValue(SANE_Word size);
  void load(const std::string &str);
  std::string get();
  void *getBuffer();
  ~StringOptionValue();
};

class BoolOptionValue : public OptionValue {
  bool _val;

public:
  void load(const std::string &str);
  std::string get();
  void *getBuffer();
  ~BoolOptionValue() {}
};

class OptionFactory {
  OptionFactory() {}

public:
  static OptionValue *GetOptionValue(SANE_Word varType, SANE_Word varSize);
};

#endif /*SCANNER_VALUE*/