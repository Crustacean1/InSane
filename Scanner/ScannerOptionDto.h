#ifndef SCANNER_OPTION_DTO
#define SCANNER_OPTION_DTO

#include <string>
#include <vector>
#include <sane/sane.h>

class ScannerOption;

class ScannerOptionDto{
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
    public:
    ScannerOptionDto(size_t optionNo, const ScannerOption& scannerOption, SANE_Word * ptr);
    std::string title;
    std::string value;
    std::vector<std::string> valueList;
    std::string unit;
    std::string constraint; 
    std::string type;
    std::string optionNo;
    std::string active;
};

#endif /*SCANNER_OPTION_DTO*/