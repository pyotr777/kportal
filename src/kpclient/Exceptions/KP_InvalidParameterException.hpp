#ifndef KP_INVALIDPARAMETEREXCEPTION_HPP
#define KP_INVALIDPARAMETEREXCEPTION_HPP

#include <string>
#include "KP_Exception.hpp"

using namespace std;

class KP_InvalidParameterException : public KP_Exception
{
public:
    KP_InvalidParameterException(const std::string &msg)
        : KP_Exception(msg)
    {
    }

    ~KP_InvalidParameterException()
    {
    }
};


#endif // KP_INVALIDPARAMETEREXCEPTION_HPP
