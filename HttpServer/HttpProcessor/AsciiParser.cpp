#include "AsciiParser.h"
#include <iostream>

char AsciiParser::asciiCompliant[255];
bool AsciiParser::isCharTableInitialized = initializeTable();

bool AsciiParser::initializeTable(){
    for(size_t i = 0;i<255;++i){
        asciiCompliant[i] = 0;
    }
    for(char i = 'A';i<='Z';++i){
        asciiCompliant[static_cast<int>(i)] = 1;
    }
    for(char i = '0';i<='9';++i){
        asciiCompliant[static_cast<int>(i)] = 1;
    }
    for(char i = 'a';i<='z';++i){
        asciiCompliant[static_cast<int>(i)] = 1;
    }
    asciiCompliant['.'] = asciiCompliant[','] = asciiCompliant['-'] = asciiCompliant['_'] = 1;
}

void AsciiParser::escapeString(std::string & str){
    for(size_t i = 0;i<str.size();++i){
        if(!isAscii(str[i])){
            unsigned char prev = str[i];
            str[i] = '%';
            str.insert(i+1,1,toHex(prev&15));
            str.insert(i+1,1,toHex(prev>>4));
            i+=2;
        }
    }
}
void AsciiParser::unescapeString(std::string &str){
    for(size_t i = 0;i<str.size();++i){
        if(str[i]=='%'){
            unsigned char val = (fromHex(str[i+1])<<4) + fromHex(str[i+2]);
            str.erase(i+1,2);
            str[i] = val;
        }
    }
}

bool AsciiParser::isAscii(char chr){
    return asciiCompliant[static_cast<unsigned char>(chr)];
}

char AsciiParser::toHex(unsigned char chr){
    return (chr<=9) * (chr+'0') + (chr>9) * ('A' - 10 + chr);
}
unsigned char AsciiParser::fromHex(char str){
    return (str<'9') * (str-'0') + (str>='9') * (10 + str-'A');
}