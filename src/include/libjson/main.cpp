#include<iostream>
#include "../../include/libjson/libjson.h"
//#include "libjson/libjson.h"
//#include <libjson/libjson.h>
//#include <libjson.h>
//#include "Building.h"
//#include "libjson.h"
//#include "JSONNode.h"
#include <list>
#include <string>
using namespace std;   
class A
{
	string a;
	int b;
	public:
	A(string _a, int _b)
	{
		a = _a;
		b = _b;
	}
};
class B
{
	list<A> as;
		public:
	void add(string str, int i)
	{
		A a = A(str, i);
		as.push_back(a);
	}
};
int main()
{

	list<string> strs;
	strs.push_back(string("heheh"));
	
	B * b = new B();
	b -> add(string("hehe"), 1);
	delete b;
	
	
	
//  json::JSONNode n(JSON_NODE);
//	json::JSONNode c(JSON_ARRAY);
    JSONNode n(JSON_NODE);
    
    n.push_back(JSONNode("hehe", "hoho"));
    
    std::string str = "monkey", name = "annimal";
    n.push_back(JSONNode(name, str));
    
	JSONNode c(JSON_ARRAY);
	
    c.push_back(JSONNode("", 16));
    c.push_back(JSONNode("", 43));
    c.push_back(JSONNode("", 69));
    	
    c.push_back(JSONNode("", "jeje"));

    n.push_back(c);

    std::string jc = n.write_formatted();
    std::cout<<jc<<std::endl;

    return 0;
}

