#include "ScannerOption.h"
#include "OptionValue.h"
#include <cstring>
#include <iostream>

ScannerOption::ScannerOption(SANE_Handle &handle, size_t option)
    : _handle(handle), _desc(sane_get_option_descriptor(_handle, option)),
      _optionNo(option),
      _optionValue(OptionFactory::GetOptionValue(getType(), getSize())) {}
ScannerOption::ScannerOption(ScannerOption &&scanner)
    : _handle(scanner._handle), _desc(scanner._desc),
      _optionNo(scanner._optionNo),
      _optionValue(std::move(scanner._optionValue)) {}

std::string ScannerOption::getName() const {
  return std::string(_desc->name == NULL ? "" : _desc->name);
}
std::string ScannerOption::getTitle() const {
  return std::string(_desc->title == NULL ? "" : _desc->title);
}
std::string ScannerOption::getDesc() const {
  return std::string(_desc->desc == NULL ? "" : _desc->desc);
}

SANE_Word ScannerOption::getSize() const { return _desc->size; }

SANE_Word ScannerOption::getUnit() const { return _desc->unit; }
SANE_Word ScannerOption::getType() const { return _desc->type; }
SANE_Word ScannerOption::getConstraintType() const {
  return _desc->constraint_type;
}

std::string ScannerOption::get(SANE_Word *details) const {
  if (_optionValue == nullptr || !SANE_OPTION_IS_ACTIVE(_desc->cap)) {
    return "";
  }
  if (auto status =
          sane_control_option(_handle, _optionNo, SANE_ACTION_GET_VALUE,
                              _optionValue->getBuffer(), details);
      status != SANE_STATUS_GOOD) {
    throw std::runtime_error("Failed to retrieve option value: " +
                             std::string(sane_strstatus(status) +
                                         std::string(" with option number: ") +
                                         std::to_string(_optionNo)));
  }
  return _optionValue->get();
}

void ScannerOption::set(std::string newValue, SANE_Word *details) {
  if (_optionValue == nullptr || !SANE_OPTION_IS_SETTABLE(_desc->cap) ||
      !SANE_OPTION_IS_ACTIVE(_desc->cap)) {
    throw std::runtime_error("Attempt to write to virtual parameter");
  }
  _optionValue->load(newValue);
  if (auto status =
          sane_control_option(_handle, _optionNo, SANE_ACTION_SET_VALUE,
                              _optionValue->getBuffer(), details);
      status != SANE_STATUS_GOOD) {
    throw std::runtime_error("Failed to set option: " +
                             std::string(sane_strstatus(status)));
  }
}

void ScannerOption::reset(SANE_Word *details) {
  if (_optionValue == nullptr || !SANE_OPTION_IS_SETTABLE(_desc->cap) ||
      !SANE_OPTION_IS_ACTIVE(_desc->cap)) {
    return;
  }
  auto status = sane_control_option(_handle, _optionNo, SANE_ACTION_SET_AUTO,
                                    NULL, details);
  if (status != SANE_STATUS_GOOD) {
    throw std::runtime_error(
        "In ScannerOption::reset: Failed to reset option " +
        std::to_string(_optionNo) + "(" + getTitle() + ")" + "\n" +
        std::string(sane_strstatus(status)));
  }
}

std::vector<std::string> ScannerOption::getConstraints() const {
  if (!SANE_OPTION_IS_ACTIVE(_optionNo)) {
    return std::vector<std::string>{};
  }

  std::vector<std::string> result;
  size_t size;

  switch (getConstraintType()) {
  case SANE_CONSTRAINT_NONE:
    break;
  case SANE_CONSTRAINT_WORD_LIST:
    size = _desc->constraint.word_list[0] + 1;
    for (size_t i = 1; i < size; ++i) {
      (*static_cast<SANE_Word *>(_optionValue->getBuffer())) =
          _desc->constraint.word_list[i];
      result.push_back(_optionValue->get());
    }
    break;
  case SANE_CONSTRAINT_STRING_LIST:
    for (int i = 0; _desc->constraint.string_list[i] != NULL; ++i) {
      result.push_back(std::string(_desc->constraint.string_list[i]));
    }
    break;
  case SANE_CONSTRAINT_RANGE:
    (*static_cast<SANE_Word *>(_optionValue->getBuffer())) =
        _desc->constraint.range->min;
    result.push_back(_optionValue->get());
    (*static_cast<SANE_Word *>(_optionValue->getBuffer())) =
        _desc->constraint.range->max;
    result.push_back(_optionValue->get());
    break;
  }
  return result;
}