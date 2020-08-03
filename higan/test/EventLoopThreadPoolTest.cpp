//
// Created by rjd67 on 2020/8/3.
//

#include <higan/EventLoopThreadPool.h>
#include <higan/base/Mutex.h>
#include <unistd.h>

higan::Mutex mutex;

void Print()
{
	higan::MutexLockGuard guard(mutex);

	static int times = 0;
	printf("nep? nep! time: %d\n", ++times);
}

int main()
{
	higan::EventLoop base_loop;

	higan::EventLoopThreadPool pool("TestEventLoopThreadPool", &base_loop);

	pool.SetThreadNum(4);
	pool.Start();

	for (int i = 0; i < 10; ++i)
	{
		pool.GetNextEventLoop()->RunInLoop(Print);
	}

	sleep(2);

	return 0;
}