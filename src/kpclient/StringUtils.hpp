#ifndef STRINGUTILS_HPP
#define STRINGUTILS_HPP
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <limits.h>

class StringUtils
{
public:
    static unsigned int Size_tToUint( size_t what );
    static unsigned int Split(const std::string &txt, std::vector<std::string> &strs, char ch);
    static unsigned int Split(const std::string &work, std::vector<std::string> &flds, char delim, char startChar, char endChar);
    static std::string IntToStr(int value);
    static std::string UintToStr(unsigned int value);
};
#endif // STRINGUTILS_HPP
