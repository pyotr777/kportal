#include "KP_Parameter.hpp"


KP_Parameter::KP_Parameter(string name)
{
    this -> type = Option;
    this -> name = name;
}

KP_Parameter::KP_Parameter(int value, string name)
{
    this -> type = Int;
    this -> name = name;
    this -> value.intValue = value;
}

KP_Parameter::KP_Parameter(long value, string name)
{
    this -> type = Long;
    this -> name = name;
    this -> value.longValue = value;
}

KP_Parameter::KP_Parameter(float value, string name)
{
    this -> type = Float;
    this -> name = name;
    this -> value.floatValue = value;
}

KP_Parameter::KP_Parameter(double value, string name)
{
    this -> type = Double;
    this -> name = name;
    this -> value.doubleValue = value;
}

KP_Parameter::KP_Parameter(std::string value, string name)
{
    this -> type = String;
    this -> name = name;
    this -> value.stringValue = new string(value.c_str());
}

KP_Parameter::KP_Parameter(KP_Entry value, string name)
{
    this -> type = Entry;
    this -> name = name;
    this -> value.entryValue = new KP_Entry(value);
}

KP_Parameter& KP_Parameter::operator = (const KP_Parameter& copy)
{
    this -> type = copy.type;
    this -> name = copy.name;
    switch((int) type)
    {
    case (int)Int:
        this -> value.intValue = copy.value.intValue;
        break;
    case (int)Long:
        this -> value.longValue = copy.value.longValue;
        break;
    case (int)Float:
        this -> value.floatValue = copy.value.floatValue;
        break;
    case (int)Double:
        this -> value.doubleValue = copy.value.doubleValue;
        break;
    case (int)String:
        this -> value.stringValue = new string((*copy.value.stringValue).c_str());
        break;
    case (int)Entry:
        this ->value.entryValue = new KP_Entry(*copy.value.entryValue);
        break;
    }
}

KP_Parameter::KP_Parameter(const KP_Parameter & copy)
{
    this -> type = copy.type;
    this -> name = copy.name;
    switch((int) type)
    {
    case (int)Int:
        this -> value.intValue = copy.value.intValue;
        break;
    case (int)Long:
        this -> value.longValue = copy.value.longValue;
        break;
    case (int)Float:
        this -> value.floatValue = copy.value.floatValue;
        break;
    case (int)Double:
        this -> value.doubleValue = copy.value.doubleValue;
        break;
    case (int)String:
        this -> value.stringValue = new string((*copy.value.stringValue).c_str());
        break;
    case (int)Entry:
        this ->value.entryValue = new KP_Entry(*copy.value.entryValue);
        break;
    }
}

KP_Parameter::~KP_Parameter()
{
    if(type == String)
    {
        if(this->value.stringValue != NULL)
        {
            delete this->value.stringValue;
            this->value.stringValue = NULL;
        }
    }
    else if(type == Entry)
    {
        if(this -> value.entryValue != NULL)
        {
            delete this->value.entryValue;
            this -> value.entryValue = NULL;
        }
    }
}
