智能指针类型:
STL:  auto_ptr、unique_ptr、shared_ptr和weak_ptr
Boost:  scoped_ptr、shared_ptr、scoped_array、shared_array、weak_ptr、intrusive_ptr

#STL
##auto_ptr
auto_ptr对象所有权是独占。 注意事项：
 - 不能同时出现两个auto_ptr指向同一个object。
 - 不能把同一个object赋值给不同的auto_ptr。
 -	不能将auto_ptr放入容器。
 -不能通过复制操作来初始化auto_ptr
 
 ```c
		std::auto_ptr<int> p(new int(42)); //OK
		std::atuo_ptr<int>p = new int(42);//Error
	```
	
	这是因为auto_ptr的构造函数被定义了explicit
 - auto_ptr不能指向数组
       因为auto_ptr的析构函数中删除指针用的是delete,而不是delete [],所以我们不应该用auto_ptr来管理一个数组指针。

##unique_ptr
PS: std::auto_ptr在最新的c++11标准草案中被std::unique_ptr取代, 
两者区别：
 -auto_ptr有拷贝语义，拷贝后源对象变得无效；unique_ptr则无拷贝语义，但提供了移动语义
 -auto_ptr不可作为容器元素，unique_ptr可以作为容器元素
 -auto_ptr不可指向动态数组(尽管不会报错，但不会表现出正确行为)，unique_ptr可以指向动态数组
 -unique_ptr默认的资源删除操作是delete/delete[]，若需要，可以进行自定义
 -void end_connection(connection *p) { disconnect(*p); } //资源清理函数  
 -unique_ptr<connection, decltype(end_connection)*> //资源清理器的“类型”  
 -p(&c, end_connection);// 传入函数名，会自动转换为函数指针 

使用例子：
```c
	//容器
	//vector<unique_ptr<string>> vs { new string{“Doug”}, new string{“Adams”} };
	vector<unique_ptr<string>>v;  
	unique_ptr<string> p1(new string("abc"));  
	v.push_back(std::move(p1));//这里需要显式的移动语义，因为unique_ptr并无copy语义  
	//数组
	unique_ptr<int[]> p (new int[3]{1,2,3});  
	p[0] = 0;// 重载了operator[]  
```

##shared_ptr
PS：同boost:shared_ptr, 不许能循环引用，此为引用计数的通病。
对被管理的资源进行引用计数，当一个shared_ptr对象要共享这个资源的时候，该资源的引用计数加1，当这个对象生命期结束的时候，再把该引用技术减少1。这样当最后一个引用它的对象被释放的时候，资源的引用计数减少到0，此时释放该资源。

##Weak_ptr
该智能指针是为了shared_ptr循环引用问题而引入的。
可以看到，weak_ptr必须从一个share_ptr或另一个weak_ptr转换而来，这也说明，进行该对象的内存管理的是那个强引用的share_ptr。weak_ptr只是提供了对管理对象的一个访问手段。

weak_ptr除了对所管理对象的基本访问功能（通过get()函数）外，还有两个常用的功能函数：expired()用于检测所管理的对象是否已经释放；lock()用于获取所管理的对象的强引用指针。


#Boost
##Scoped_ptr
 -	boost::scoped_ptr所管理的对象生命周期仅仅局限于一个区间（该指针所在的"{}"之间），无法传到区间之外，这就意味着boost::scoped_ptr对象是不能作为函数的返回值的（std::auto_ptr可以）

 -不能共享所有权
auto_ptr<testObject> pt2(pt1); //可以编译，赋值后pt1为empty, pt2为原pt1指向的object
boost::scoped_ptr<testObject> ptr2(ptr1); //无法编译
 -不能管理数组对象，原因与auto_ptr类似

boost::scoped_ptr和std::auto_ptr的功能和操作都非常类似，如何在他们之间选取取决于是否需要转移所管理的对象的所有权（如是否需要作为函数的返回值）。如果没有这个需要的话，大可以使用boost::scoped_ptr，让编译器来进行更严格的检查，来发现一些不正确的赋值操作。

##Scoped_array
  类似于c风格的数组，使用方式也基本一致，不支持动态增长，也没有迭代器支持，不能搭配STL算法，仅有一个纯粹的"裸"数组接口， 有点就是速度快。
  不支持边界检查，越界时行为未知

 -构造函数接受的指针p必须是new[]的结果，而不是new表达式的结果；
 -没有*、->操作符重载，scoped_array持有的不是一个普通指针；
 -析构函数使用delete[]，而不是delete；
 -提供operator[]重载，可以像普通数组一样使用下标访问元素；
 -没有begin(),end()等类似容器迭代器操作函数。

##Shared_array
shared_array与shared_ptr的区别如下：

 -构造函数接受的指针p必须是new[]的结果，而不能是new表达式。
 -提供operator[]操作符重载，可以像普通数组一样用下标访问元素。
 -没有*、->操作符重载，因为shared_array持有的不是一个普通指针。
 -析构函数使用delete[]释放资源，而不是delete。

```c
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
```

在使用shared_array重载的operator[]要注意，shared_array不提供数组索引的范围检查，如果超过了动态数组大小的索引或者是负数索引将引发未定义行为。
shared_array能力有限，大多情况下可以用shared_ptr<std::vector>或者std::vector<shared_ptr>代替。

##intrusive_ptr
boost::intrusive_ptr一种“侵入式”的引用计数指针，它实际并不提供引用计数功能，而是要求被存储的对象自己实现引用计数功能，并提供intrusive_ptr_add_ref和intrusive_ptr_release函数接口供boost::intrusive_ptr调用。

Boost提供了intrusive_ref_counter基类， 用户类只要继承此类即可支持intrusive_ptr_add_ref和intrusive_ptr_release函数， 并提供了两种计数方式：线程安全与非线程安全。
该指针支持将同一个object托管到多个intrusive_ptr。
