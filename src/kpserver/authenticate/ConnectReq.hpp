#ifndef CONNECT_REQ_HPP
#define CONNECT_REQ_HPP

#include <string>

using namespace std;
class ConnectReq
{
public:
	ConnectReq ();
	ConnectReq (std::string userId, std::string pwd);
	~ConnectReq();
	std::string getUserId();
	std::string getPwd();
	void setUserId(std::string userId);
	void setPwd(std::string pwd);
private:
	std::string _userId;
	std::string _pwd;
};

#endif //CONNECT_REQ_HPP
