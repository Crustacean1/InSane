#ifndef SCANNER_OPTION
#define SCANNER_OPTION

//#include <nlohmann/json.hpp>
#include <sane/sane.h>
#include "OptionValue.h"
#include <memory>

class Scanner;

class ScannerOption{
    SANE_Handle & _handle;
    const SANE_Option_Descriptor  *_desc;
    const size_t _optionNo;
    std::unique_ptr<OptionValue> _optionValue;

    static constexpr const char * UNIT_NAMES[7] ={
        "None",
        "Pixel",
        "Bit",
        "Mm",
        "Dpi",
        "Percent",
        "Microsecond"
    };
    static constexpr const char * TYPE_NAMES[]{
        "Bool",
        "Int",
        "Fixed",
        "String",
        "Button",
        "Group"
    };
    static constexpr const char * CONSTRAINT_NAMES[]{
        "None",
        "Range",
        "List",
        "List"    
    };

    ScannerOption(const ScannerOption & );
    public:
    ScannerOption(SANE_Handle &handle, size_t option);
    ScannerOption(ScannerOption && scanner);

    std::string getName() const;
    std::string getTitle() const;
    std::string getDesc() const;

    SANE_Word getSize() const;

    SANE_Word getUnit() const;
    SANE_Word getType() const;
    SANE_Word getConstraintType() const;

    const char * getUnitName() const;
    const char * getTypeName() const;
    const char * getConstraintTypeName() const;

    std::string get(SANE_Word * details) const;
    void set(std::string param,SANE_Word * details);

    std::vector<std::string> getConstraints() const;
};

#endif /*SCANNER_OPTION*/