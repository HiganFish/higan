//
// Created by rjd67 on 2020/8/2.
//

#include <higan/base/ThreadPool.h>
#include <iostream>
#include <unistd.h>

higan::Mutex mutex;

void Foo()
{
	higan::MutexLockGuard guard(mutex);
	std::cout << "Foo? Foo!" << std::endl;
}

int main()
{
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