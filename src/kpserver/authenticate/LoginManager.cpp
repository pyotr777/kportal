#include "LoginManager.hpp"

LoginManager::LoginManager()
{
}

LoginManager::~LoginManager()
{
    m_users.clear();
}

/*
 * Add UserInfoObject by loginId
 */
void LoginManager::addLoginInfo(const std::string loginId, const UserInfoObject userInfo){
    boost::unique_lock<boost::mutex> lock(m_lock);
    m_users.insert(std::pair<std::string, UserInfoObject>(loginId, userInfo));
    //std::cout << "Bp: addLoginInfo(): m_users.size() = " << m_users.size() << std::endl;
    lock.unlock();
    m_cond.notify_one();
}

/*
 * Get UserInfoObject by loginId
 */
UserInfoObject* LoginManager::getUserInfo(const std::string loginId)
{
    boost::unique_lock<boost::mutex> lock(m_lock);

    map<std::string, UserInfoObject>::iterator it = m_users.find(loginId);
    if(it != m_users.end())
    {
        return &m_users[loginId];
    }
    lock.unlock();
    return NULL;
}

/*
 * Remove UserInfoObject by loginId
 */
bool LoginManager::removeUserInfo(const std::string loginId)
{
    boost::unique_lock<boost::mutex> lock(m_lock);
    map<std::string, UserInfoObject>::iterator it = m_users.find(loginId);
    if(it != m_users.end())
    {
        m_users.erase(loginId);
    }
    lock.unlock();
}

/*
    void get_request(request<endpoint_type>& value) {
        boost::unique_lock<boost::mutex> lock(m_lock);

        while (m_requests.empty()) {
            m_cond.wait(lock);
        }

        value = m_requests.front();
        m_requests.pop();
    }
    */
