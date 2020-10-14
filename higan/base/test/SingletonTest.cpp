//
// Created by rjd67 on 2020/10/14.
//
#include <higan/base/Singleton.h>
#include <iostream>

class Foo
{
public:
	Foo():
			time_(0)
	{
		std::cout << "Call Constructor\n";
	}

	void Bar()
	{
		std::cout << "Bar: " << ++time_ << std::endl;
	}

	int time_;
};

int main()
{
	higan::Singleton<Foo>::Instance().Bar();
	higan::Singleton<Foo>::Instance().Bar();
	higan::Singleton<Foo>::Instance().Bar();
	higan::Singleton<Foo>::Instance().Bar();

	/*
	Call Constructor
	Bar: 1
	Bar: 2
	Bar: 3
	Bar: 4
	 * */

	return 0;
}