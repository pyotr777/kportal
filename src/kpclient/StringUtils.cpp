#include "StringUtils.hpp"

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

std::string StringUtils::UintToStr(unsigned int value)
{
    std::string istr;
    std::ostringstream convert;
    convert << value;
    istr = convert.str();
    return istr;
}
