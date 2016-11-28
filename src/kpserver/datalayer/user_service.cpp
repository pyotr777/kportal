#include "user_service.hpp"

User_Service::User_Service() {
}

User_Service::User_Service(std::string email, std::string id, StatusUSer st) {
    user_email = email;
    serviceId = id;
    status = st;
}

void User_Service::setUserEmail(std::string email) {
    user_email = email;
}

std::string User_Service::getEmail() {
    return user_email;
}

void User_Service::setServiceId(std::string id) {
    serviceId = id;
}

std::string User_Service::getServiceId() {
    return serviceId;
}

void User_Service::setStatus(StatusUSer st) {
    status = st;
}

StatusUSer User_Service::getStatus() {
    return status;
}
