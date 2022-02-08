#include "ScannerOptionDto.h"
#include "ScannerOption.h"

ScannerOptionDto::ScannerOptionDto(size_t optionNo, const ScannerOption& option, SANE_Word * details) :
title(option.getTitle()),
value(option.get(details)),
valueList(option.getConstraints()),
unit(UNIT_NAMES[option.getUnit()]),
constraint(CONSTRAINT_NAMES[option.getConstraintType()]),
type(TYPE_NAMES[option.getType()])
{
}