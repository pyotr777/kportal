#ifndef ID_RESPONSE_HPP
#define ID_RESPONSE_HPP

#include <string>
#include "Utils/TagKeys.hpp"

class IdRes
{
public:
    enum FinishStates {
        FINISH_FAIL = 0,
        FINISH_SUCCESS = 1
    };

    enum Types
    {
        TYPE_UNKNOW,
        TYPE_ACK,
        TYPE_FINISH,
        TYPE_DATA,
        TYPE_PATH
    };

    IdRes(const IdRes& copy)
    {
        this -> _owner = copy._owner;
        this -> _ack = copy._ack;
        this -> _type = copy._type;
        this -> _result = copy._result;
        this -> _path = copy._path;
        this -> _data = copy._data;
    }

    IdRes(std::string owner, int result)
    {
        this -> _owner = owner;
        this -> _type = TYPE_FINISH;
        this -> _result = result;
    }

    IdRes(std::string owner, std::string pathOackOdata, Types type)
    {
        this -> _owner = owner;
        this -> _type = type;
        switch((int)type)
        {
        case (int)TYPE_PATH:
            this -> _path = pathOackOdata;
            break;
        case (int)TYPE_ACK:
            this -> _ack = pathOackOdata;
            break;
        case (int)TYPE_DATA:
            this -> _data = pathOackOdata;
            break;
        }
    }

    bool isFinish() { return _result == (int)FINISH_SUCCESS; }

    FinishStates getFinish() { return (FinishStates)_result; }

    std::string& getData() { return _data; }

    std::string& getPath() { return _path; }

    Types getType(){ return _type; }

    std::string getOwner() { return _owner; }

    void setOwner(std::string owner) { this -> _owner = owner; }

    std::string getAck() { return this -> _ack; }

private:
    std::string _owner;
    std::string _ack;
    int _result;
    std::string _data;
    std::string _path;
    Types _type;
};

#endif //ID_RESPONSE_HPP
