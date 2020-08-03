//
// Created by rjd67 on 2020/8/2.
//

#include <higan/base/ThreadPool.h>
#include <iostream>
#include <unistd.h>

void Foo()
{
	std::cout << "Foo? Foo!" << std::endl;

	sleep(1);
}

int main()
{

	{
		/**
		 * 测试detach 未被Join线程
		 */
		higan::Thread thread("TestThread", Foo);
		thread.Start();
	}


	higan::ThreadPool pool("TestThreadPool");
	pool.Init(2);

	pool.CallFunction(Foo);
	pool.CallFunction(Foo);
	pool.CallFunction(Foo);
	pool.CallFunction(Foo);
	pool.CallFunction(Foo);
	pool.CallFunction(Foo);
	pool.CallFunction(Foo);

	sleep(1);

	return 0;
}