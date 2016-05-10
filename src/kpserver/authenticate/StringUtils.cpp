#include "StringUtils.hpp"
#include <stdlib.h>
#include <sstream>
unsigned int StringUtils::Size_tToUint( size_t what )
{
    if( what > UINT_MAX ) {
       return 0;
    }
    return static_cast<unsigned int>( what );
}

unsigned int StringUtils::Split(const std::string &work, std::vector<std::string> &flds, char delim)
{
    unsigned int count = 0;
    //std::cout << "work:" << work << std::endl;
    if (!flds.empty()) flds.clear();  // empty vector if necessary
    std::string buf = "";
    unsigned int i = 0;
    while (i < work.length()) {
        if(work[i] == '\n')
        {}
        else if (work[i] != delim)
            buf += work[i];

        else if (buf.length() > 0)
        {
            count ++;
            flds.push_back(buf);
            buf = "";
        }
        i++;
    }
    if (!buf.empty())
    {
        flds.push_back(buf);
        count ++;
    }
    return count;
}

unsigned int StringUtils::Split(const std::string &work, std::vector<std::string> &array_str, std::string delimiter)
{
    std::string str = work;
    size_t pos = 0;
    std::string token;
    while ((pos = str.find(delimiter)) != std::string::npos) {
        token = str.substr(0, pos);
        if (token.length() > 0)
            array_str.push_back(token);
        str.erase(0, pos + delimiter.length());
    }
    if (str.length() > 0) {
        array_str.push_back(str);
    }
    return array_str.size();
}

unsigned int StringUtils::Split(const std::string &work, std::vector<std::string> &flds, char delim, char startChar, char endChar)
{
    unsigned int count = 0;
    //std::cout << "work:" << work << std::endl;
    if (!flds.empty()) flds.clear();  // empty vector if necessary
    std::string buf = "";
    unsigned int i = 0;
    bool isAbort = false;
    while (i < work.length()) {
        if(work[i] == '\n')
        {}
        else if(work[i] == startChar)
        {
            if(buf.length() > 0)
            {
                count ++;
                flds.push_back(buf);
                buf = "";
            }
            buf += startChar;
            isAbort = true;
        }

        else if(work[i] == endChar)
        {
            if(buf.length() > 0)
            {
                buf += endChar;
                count ++;
                flds.push_back(buf);
                buf = "";
            }
            isAbort = false;
        }

        else if (work[i] != delim)
            buf += work[i];

        else if (buf.length() > 0)
        {
            if(!isAbort)
            {
                count ++;
                flds.push_back(buf);
                buf = "";
            }
        }
        i++;
    }
    if (!buf.empty())
    {
        flds.push_back(buf);
        count ++;
    }
    return count;
}

std::string StringUtils::IntToStr(int value)
{
    std::string istr;
    std::ostringstream convert;
    convert << value;
    istr = convert.str();
    return istr;
}
/*
string convertInt(int number)
{
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}*/

std::string StringUtils::genIdString() {
    std::string appendStr;

    for(int i = 0; i < 10; i++)
    {
        int re = rand() % 10;

        std::stringstream ssret;

        ssret.str("");

        ssret << re;

        appendStr += ssret.str();
    }

    long seconds = time (NULL);
    std::stringstream ss;

    ss.str("");
    ss << seconds++;

    std::string id = appendStr + ss.str();

    return id;
}

/*
 * Convert to standard string for message
 */
void StringUtils::convertStandardStr(std::string &string_src) {
    std::string listStr[8] = {"\\","\"","/","\b", "\f", "\n", "\r", "\t"};
    std::string listReplaceStr[8] = {"\\\\","\\\"","\\/","\\b", "\\f", "\\n", "\\r", "\\t"};
    for (int i = 0; i < 8; i++) {
        int pos = 0;
        while (pos != -1) {
            pos = string_src.find(listStr[i],pos);
            if (pos != -1) {
                string_src.replace(pos,listStr[i].length(),listReplaceStr[i]);
                pos += listReplaceStr[i].length();
            }
        }
    }
}

bool StringUtils::GetSecondsFromDuration(const std::string duration_str, double& seconds){
  seconds = 0;
  return 0;
}
