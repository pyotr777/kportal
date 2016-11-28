#ifndef USERINFOOBJECT_H
#define USERINFOOBJECT_H

#include <string>

using namespace std;

class UserInfoObject
{
public:
    UserInfoObject(){}
    ~UserInfoObject(){}
    UserInfoObject(string email, string userType)
    {
        _email = email;
        _userType = userType;
    }
    UserInfoObject(const UserInfoObject& copy)
    {
        this -> _email = copy._email;
        this -> _userType = copy._userType;
    }
    string getEmail() { return _email; }
    string getUserType() { return _userType; }
private:
    string _email;
    string _userType;
};
#endif // USERINFOOBJECT_H
