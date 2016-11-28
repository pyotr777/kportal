#ifndef USER_SERVICE_HPP
#define USER_SERVICE_HPP
#include <string>
enum StatusUSer {
    READY,
    WAITING,
    NO_STATUS
};

class User_Service {
private:
    std::string user_email;
    std::string serviceId;
    StatusUSer  status;
public:
    User_Service();
    User_Service(std::string email, std::string id, StatusUSer st);

    void setUserEmail(std::string email);
    std::string getEmail();

    void setServiceId(std::string id);
    std::string getServiceId();

    void setStatus(StatusUSer st);
    StatusUSer getStatus();
};

#endif
