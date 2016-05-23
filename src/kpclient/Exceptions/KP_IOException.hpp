#include <string>
#include "KP_Exception.hpp"

using namespace std;

class KP_IOException : public KP_Exception
{
public:
	KP_IOException(const std::string &msg)
		: KP_Exception(msg)
	{
	}

	~KP_IOException()
	{
	}
};
