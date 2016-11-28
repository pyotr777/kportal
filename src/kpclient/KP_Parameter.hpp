#ifndef KP_PARAMETER_HPP
#define KP_PARAMETER_HPP

#include <string>
#include <iostream>

#include "KP_Entry.hpp"
using namespace std;

class KP_Parameter
{
public:
	std::string name;
    enum Type { Int, Long, Float, Double, String, Entry, Option } type;
	union
	{
		int intValue;
		long longValue;
		float floatValue;
		double doubleValue;
        std::string * stringValue;
        KP_Entry * entryValue;
    } value;

    //std::string stringValue;

public:
    KP_Parameter(string name);
    KP_Parameter(int value, string name);
    KP_Parameter(long value, string name);
    KP_Parameter(float value, string name);
    KP_Parameter(double value, string name);
    KP_Parameter(std::string value, string name);
    KP_Parameter(KP_Entry value, string name);
    KP_Parameter(const KP_Parameter& copy);
    KP_Parameter& operator = (const KP_Parameter& copy);

    ~KP_Parameter();
	
};


#endif //KP_PARAMETER_HPP
