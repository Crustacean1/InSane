#include "OptionValue.h"
#include <algorithm>
#include <cstring>
#include <exception>
#include <iostream>

StringOptionValue::StringOptionValue(SANE_Word size) {
  _buffer = new char[size];
}
StringOptionValue::~StringOptionValue() { delete[] _buffer; }
void StringOptionValue::load(const std::string &str) {
  strcpy(_buffer, str.c_str());
}
std::string StringOptionValue::get() { return std::string(_buffer); }
void *StringOptionValue::getBuffer() { return _buffer; }

void IntOptionValue::load(const std::string &str) {
  _val = std::atoi(str.c_str());
}
std::string IntOptionValue::get() { return std::to_string(_val); }
void *IntOptionValue::getBuffer() { return &_val; }

float FixedOptionValue::fromFixed(SANE_Word val) {
  float buffer = val;
  return static_cast<SANE_Word>(
      (buffer / (((uint64_t)1) << SANE_FIXED_SCALE_SHIFT)));
}
SANE_Word FixedOptionValue::toFixed(float val) {
  uint64_t buffer = val * ((uint64_t)1 << SANE_FIXED_SCALE_SHIFT);
  return static_cast<SANE_Word>(buffer);
}
void FixedOptionValue::load(const std::string &str) {
  _intRep = toFixed(std::atof(str.c_str()));
}
std::string FixedOptionValue::get() {
  float _floatRep = fromFixed(_intRep);
  return std::to_string(_floatRep);
}
void *FixedOptionValue::getBuffer() { return &_intRep; }

void BoolOptionValue::load(const std::string &str) {
  auto intVal = std::atoi(str.c_str());
  _val = intVal != 0;
}
std::string BoolOptionValue::get() { return std::to_string(_val); }
void *BoolOptionValue::getBuffer() { return &_val; }

OptionValue *OptionFactory::GetOptionValue(SANE_Word varType,
                                           SANE_Word varSize) {
  switch (varType) {
  case SANE_TYPE_INT:
    return new IntOptionValue();
  case SANE_TYPE_FIXED:
    return new FixedOptionValue();
  case SANE_TYPE_STRING:
    return new StringOptionValue(varSize);
  case SANE_TYPE_BOOL:
    return new BoolOptionValue();
  default:
    return nullptr;
  }
}