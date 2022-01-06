#ifndef SCANNER_OPTION
#define SCANNER_OPTION

#include <nlohmann/json.hpp>
#include <sane/sane.h>

class Scanner;

class ScannerOption{
    SANE_Handle & _handle;
    const SANE_Option_Descriptor  *_desc;
    const size_t _optionNo;
    static float fixedToFloat(SANE_Fixed val);
    static SANE_Fixed floatToFixed(float val);
    //nlohmann::json _jsonBuffer;
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
    void set(int val);
    void set(std::string val);
    void set(float val);

    friend Scanner;
    ScannerOption(SANE_Handle &handle, size_t option);
    public:

    nlohmann::json serialize() const;

    SANE_Word setOption(std::string param);

};

#endif /*SCANNER_OPTION*/