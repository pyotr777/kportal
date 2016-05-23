#include "ConnectReq.hpp"

ConnectReq::ConnectReq()
{
}

ConnectReq::ConnectReq(std::string userId, std::string pwd)
{
	_userId = userId;
	_pwd = pwd;
}

ConnectReq::~ConnectReq()
{
}

void ConnectReq::setUserId(std::string userId)
{
	_userId = userId;
}

void ConnectReq::setPwd(std::string pwd)
{
	_pwd = pwd;
}

std::string ConnectReq::getUserId()
{
	return _userId;
}

std::string ConnectReq::getPwd()
{
	return _pwd;
}
