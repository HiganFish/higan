//
// Created by rjd67 on 2020/8/2.
//

#include <higan/base/Thread.h>
#include <iostream>

void Foo()
{
	std::cout << "Foo? Foo!" << std::endl;
}

int main()
{
	higan::Thread thread("Test Thread");
	thread.CallFunc(Foo);

	thread.Join();

	return 0;
}