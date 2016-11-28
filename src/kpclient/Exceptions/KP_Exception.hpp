#ifndef KP_EXCEPTION_HPP
#define KP_EXCEPTION_HPP

#include <string>

using namespace std;

class KP_Exception
{
private:
	std::string msg;

public:
	KP_Exception(const std::string &msg)
	{
		this->msg = msg;
	}

	virtual ~KP_Exception()
	{
	}

	const std::string & getMessage() const
	{
		return this->msg;
	}
};

#endif //KP_EXCEPTION_HPP
