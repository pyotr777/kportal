#ifndef KP_INVALIDACCESSEXCEPTION_HPP
#define KP_INVALIDACCESSEXCEPTION_HPP

#include <string>
#include "KP_Exception.hpp"

using namespace std;

class KP_InvalidAccessException : public KP_Exception
{
public:
    KP_InvalidAccessException(const std::string &msg)
        : KP_Exception(msg)
    {
    }

    ~KP_InvalidAccessException()
    {
    }
};


#endif // KP_INVALIDACCESSEXCEPTION_HPP
