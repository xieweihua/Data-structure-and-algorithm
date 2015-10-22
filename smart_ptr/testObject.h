#include <iostream>
#include <memory>
#include <string>
#include <boost/smart_ptr/intrusive_ref_counter.hpp>
using namespace std;

class Woman;
class Man {
private:
    std::weak_ptr<Woman> _wife;
    //std::shared_ptr<Woman> _wife;  
public:
    void setWife(std::shared_ptr<Woman> woman) {
        _wife = woman;
    }

    void doSomthing() {
        if (_wife.lock()) {
        }
    }

    ~Man() {
        std::cout << "kill man\n";
    }
};

class Woman {
private:
    //std::weak_ptr<Man> _husband;  
    std::shared_ptr<Man> _husband;
public:
    void setHusband(std::shared_ptr<Man> man) {
        _husband = man;
    }
    ~Woman() {
        std::cout << "kill woman\n";
    }
};

class testObject: public boost::intrusive_ref_counter<testObject, boost::thread_safe_counter>
{
public:
    testObject()
    {
        this->_name = "defaut";
        cout << "init object:" << this->_name << endl;
    }
	testObject(string name)
	{
		this->_name = name;
		cout<<"init object:"<<name<<endl;
	}
	 ~testObject()
	{
		cout<<"destroy object:"<<this->_name<<endl;
	}

	void name()
	{
		cout<<"call name:"<<this->_name<<endl;
	}
private:
	string _name;
};