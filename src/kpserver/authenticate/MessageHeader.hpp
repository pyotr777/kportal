#ifndef MESSAGEHEADER_HPP
#define MESSAGEHEADER_HPP
#include <string>
#include "ReqType.hpp"
using namespace std;

class MessageHeader
{
public:
    MessageHeader(){}
	~MessageHeader(){}
	MessageHeader(string owner): _owner(owner){}
    MessageHeader(string owner, RequestType type): _owner(owner), _type(type){}
    MessageHeader(const MessageHeader& copy)
    {
        this -> _owner = copy._owner;
        this -> _type = copy._type;
    }
    string getRequestID() { return _owner; }
    RequestType getType()  { return _type; }

    MessageHeader& operator= (const MessageHeader& copy)
    {
        this -> _owner = copy._owner;
        this -> _type = copy._type;
        return *this;
    }
	void setRequestId(std::string requestId){_owner = requestId;}
private:
    string _owner;
    RequestType _type;
};

#endif // MESSAGEHEADER_HPP
