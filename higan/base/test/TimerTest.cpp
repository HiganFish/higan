//
// Created by rjd67 on 2020/8/5.
//
#include <higan/EventLoop.h>
#include <higan/base/Logger.h>

higan::EventLoop* g_loop;
int64_t g_last_time = 0;

void Foo(const higan::Timer& timer)
{
	int64_t time = higan::TimeStamp::Now();
	LOG_INFO << higan::Fmt("Timer: %s timeout at: %ld, add: %ld", timer.GetName().c_str(),
			time, time - g_last_time);

	g_last_time = time;

	static int count = 0;
	if (++count >= 10)
	{
		g_loop->Quit();
	}
}

int main()
{
	higan::EventLoop loop;
	g_loop = &loop;

	higan::Timer timer("TestTimer", 1000, true, Foo);
	loop.AddTimer(timer);

	LOG_INFO << higan::Fmt("Start Loop at: %ld", g_last_time = higan::TimeStamp::Now());
	loop.Loop();
}