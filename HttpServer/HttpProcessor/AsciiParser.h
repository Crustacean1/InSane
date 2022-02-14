#ifndef ASCII_PARSER
#define ASCII_PARSER

#include <string>

class AsciiParser {
  static char asciiCompliant[255];
  static bool isCharTableInitialized;
  static bool initializeTable();

  static bool isAscii(char chr);
  static char toHex(unsigned char chr);
  static unsigned char fromHex(char chr);

public:
  AsciiParser();
  static void escapeString(std::string &str);
  static void unescapeString(std::string &str);
};

#endif /*ASCII_PARSER*/