#include <string>
#include "KP_Exception.hpp"

using namespace std;

class KP_NetworkException : public KP_Exception
{
public:
	KP_NetworkException(const std::string &msg)
		: KP_Exception(msg)
	{
	}

	~KP_NetworkException()
	{
	}
};
