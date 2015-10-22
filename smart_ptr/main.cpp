#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include "boost/scoped_ptr.hpp"
#include "boost/smart_ptr/intrusive_ptr.hpp"
#include "boost/smart_ptr/shared_array.hpp"
#include "testObject.h"
//不能建立auto_ptr的容器！！！！！
using namespace std;

auto_ptr<testObject> test_get_auto_ptr()
{
    auto_ptr<testObject> pt1(new testObject("ptr get"));
    return pt1;
}

void test_auto_ptr()
{
	auto_ptr<testObject> pt1(new testObject("pt1"));
	//pt2拥有对象， pt1不再拥有该对象
	//auto_ptr是独占的，无法让两个auto_ptr同时拥有某个对象
	auto_ptr<testObject> pt2(pt1);
	pt2->name();

	testObject* pObject2 = new testObject("pt2");
	auto_ptr<testObject> pt3(pObject2);
	//在析构时会导致异常
	//auto_ptr<testObject> pt4(pObject2);

	pt3->name();
	//pt4->name();

    //get ptr
    auto_ptr<testObject> pt_get = test_get_auto_ptr();
    pt_get->name();
}

void test_unique_ptr()
{
	unique_ptr<testObject> pt1(new testObject("pt1"));
	//pt2拥有对象， pt1不再拥有该对象
	//auto_ptr是独占的，无法让两个auto_ptr同时拥有某个对象
    //无法编译
    //unique_ptr<testObject> pt2(pt1);
	//pt2->name();

	testObject* pObject2 = new testObject("pt2");
    unique_ptr<testObject> pt3(pObject2);
	//在析构时会导致异常
	//auto_ptr<testObject> pt4(pObject2);

	pt3->name();
	//pt4->name();

    vector<unique_ptr<testObject>>v;
    unique_ptr<testObject> p1(new testObject("abc"));
    v.push_back(std::move(p1));
}

shared_ptr<testObject> test_get_shared_ptr()
{
    shared_ptr<testObject> pt1(new testObject("ptr get"));
    return pt1;
}

void test_shared_ptr()
{
    shared_ptr<testObject> pt1(new testObject("pt1"));
    cout <<"unique:"<<pt1.unique() << endl;
    cout << "use_count:" << pt1.use_count() << endl;
    shared_ptr<testObject> pt2 = pt1;
    cout << "unique:" << pt1.unique() << endl;
    cout << "use_count:" << pt1.use_count() << endl;
    pt1->name();

    vector<shared_ptr<testObject>> vs;
    shared_ptr<testObject> pt3(new testObject("pt2"));
    vs.push_back(pt2);
    cout << "pt3 unique:" << pt3.unique() << endl;
    cout << "pt3 use_count:" << pt3.use_count() << endl;

    //get ptr
    shared_ptr<testObject> pt_get = test_get_shared_ptr();
    pt_get->name();
    cout << "pt_get unique:" << pt_get.unique() << endl;
    cout << "pt_get use_count:" << pt_get.use_count() << endl;

    testObject* pt5 = new testObject("pt5");
    {
        shared_ptr<testObject> p(pt5);
    }
}

void test_weak_ptr()
{
    std::shared_ptr<Man> m(new Man());
    std::shared_ptr<Woman> w(new Woman());
    if (m && w) {
        m->setWife(w);
        w->setHusband(m);
    }

    cout << "m unique:" << m.unique() << endl;
    cout << "m use_count:" << m.use_count() << endl;

    cout << "w unique:" << w.unique() << endl;
    cout << "w use_count:" << w.use_count() << endl;
}

/* 编译不通过
boost::scoped_ptr<testObject> test_get_scoped_ptr()
{
    boost::scoped_ptr<testObject> pt1(new testObject("ptr get"));
    return pt1;
}
*/
void test_scoped_ptr()
{
    boost::scoped_ptr<testObject> ptr1(new testObject("scoped"));
    ptr1->name();

    //无法编译
    //boost::scoped_ptr<testObject> ptr2(ptr1);
}

void test_shared_array()
{
    boost::shared_array<testObject> ptArray(new testObject[2]);
    cout << "ptArray unique:" << ptArray.unique() << endl;
    cout << "ptArray use_count:" << ptArray.use_count() << endl;
    ptArray[0].name();

    boost::shared_array<testObject> p1(ptArray);
    cout << "ptArray unique:" << ptArray.unique() << endl;
    cout << "ptArray use_count:" << ptArray.use_count() << endl;
}

void test_intrusive_ptr()
{
    testObject* pOb = new testObject("intrusive");
    boost::intrusive_ptr<testObject> ptr1(pOb);
    boost::intrusive_ptr<testObject> ptr2(pOb);
    ptr1->name();
    cout << "count:" <<ptr1->use_count()<<endl;
}

void main()
{
    //test_auto_ptr();
    //test_unique_ptr();
    //test_shared_ptr();
    //test_weak_ptr();
    //test_scoped_ptr();
    //test_shared_array();
    test_intrusive_ptr();
}