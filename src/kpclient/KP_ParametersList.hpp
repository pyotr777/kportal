#ifndef KP_PARAMETERS_LIST_HPP
#define KP_PARAMETERS_LIST_HPP

#include <string>
#include <list>
#include <iostream>

#include "KP_Parameter.hpp"

using namespace std;

class KP_ParametersList
{
public:
    KP_ParametersList();
	
    ~KP_ParametersList();

    void add(string name);

	void add(int i, std::string name);
	
	void add(long l, std::string name);
	
	void add(float f, std::string name);

    void add(double d, std::string name);

    void add(std::string s, std::string name);

    void add(KP_Entry &entry, std::string);

    std::list<KP_Parameter> * getParameters();

private:
    std::list<KP_Parameter> _parameters;
};


#endif //KP_PARAMETERS_LIST_HPP
