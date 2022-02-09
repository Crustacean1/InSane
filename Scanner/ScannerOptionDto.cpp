#include <iostream>
#include "ScannerOptionDto.h"
#include "ScannerOption.h"

ScannerOptionDto::ScannerOptionDto(size_t no, const ScannerOption& option, SANE_Word * details) :
title(option.getTitle()),
desc(option.getDesc()),
unit(UNIT_NAMES[option.getUnit()]),
constraint(CONSTRAINT_NAMES[option.getConstraintType()]),
type(TYPE_NAMES[option.getType()]),
optionNo(std::to_string(no))
{
    if(option.getType()!=SANE_TYPE_GROUP){
        value = option.get(details);
        valueList = option.getConstraints();
    }
}

void ScannerOptionDto::debug(){
    if(type == "Group"){
        std::cout<<"\n\t"<<title<<"\n\n";
        return;
    }
    std::cout<<optionNo<<"\t"<<title<<"\t"<<value<<":"<<unit<<"\t"<<constraint<<"\n";
    for(const auto & el : valueList){
      std::cout<<el<<"\t";
    }
    std::cout<<"\n\n";
}