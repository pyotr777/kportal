#include <string>
#include "KP_Exception.hpp"

using namespace std;

class KP_JobIdNotFoundException : public KP_Exception
{
public:
	KP_JobIdNotFoundException(const std::string &msg)
		: KP_Exception(msg)
	{
	}

	~KP_JobIdNotFoundException()
	{
	}
};

