#include <string>
#include "KP_Exception.hpp"

using namespace std;

class KP_AuthorizationException : public KP_Exception
{
public:
	KP_AuthorizationException(const std::string &msg)
		: KP_Exception(msg)
	{
	}

	~KP_AuthorizationException()
	{
	}
};
