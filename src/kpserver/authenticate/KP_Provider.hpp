#ifndef KP_PROVIDER_HPP
#define KP_PROVIDER_HPP

#include <string>
class KP_Provider
{
public:
    KP_Provider();
    void setEmail(std::string& email){ _email = email; }
    std::string getEmail(){ return _email; }
    void setPhone(std::string& phone){ _phone = phone; }
    std::string getPhone(){ return _phone; }
    void setAddress(std::string& address){ _address = address; }
    std::string getAddress(){ return _address; }
    void setDescription(std::string& description){ _description = description; }
    std::string getDescription(){ return _description; }
private:
    std::string _email;
    std::string _phone;
    std::string _address;
    std::string _description;
};

#endif // KP_PROVIDER_HPP
