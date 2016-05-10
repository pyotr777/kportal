#ifndef LOGINMANAGER_H
#define LOGINMANAGER_H

#include <map>
#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include "UserInfoObject.hpp"

class LoginManager
{
public:
    LoginManager();
    ~LoginManager();
    void addLoginInfo(const std::string loginId, const UserInfoObject userInfo);
    UserInfoObject* getUserInfo(const std::string loginId);
    bool removeUserInfo(const std::string loginId);

private:
    //static LoginManager* instance = NULL;
    std::map<std::string, UserInfoObject> m_users;
    boost::mutex                m_lock;
    boost::condition_variable   m_cond;

};

#endif // LOGINMANAGER_H
