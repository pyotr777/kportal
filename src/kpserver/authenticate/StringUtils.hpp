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
    static unsigned int Split(const std::string &work, std::vector<std::string> &array_str, std::string delimiter);
    static unsigned int Split(const std::string &work, std::vector<std::string> &flds, char delim, char startChar, char endChar);
    static std::string IntToStr(int value);
    static std::string genIdString();
    static void convertStandardStr(std::string &string_src);
    static bool GetSecondsFromDuration(const std::string duration_str, double& seconds);
    static void ReplaceAll(std::string& str, const std::string& from, const std::string& to);
    static bool Replace(std::string& str, const std::string& from, const std::string& to);
};
#endif // STRINGUTILS_HPP
