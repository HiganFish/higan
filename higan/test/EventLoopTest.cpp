//
// Created by rjd67 on 2020/7/17.
//

#include <unistd.h>
#include <sys/time.h>
#include <higan/utils/Logger.h>
#include "higan/EventLoop.h"

int g_looptime = 0;

higan::EventLoop* g_event_loop;

void PendingFunc()
{

	struct timeval now{};
	gettimeofday(&now, nullptr);
	LOG_INFO << higan::Fmt("%ld:%ld\n", now.tv_sec, now.tv_usec);

	sleep(1);
	LOG_INFO << higan::Fmt("%d\n", ++g_looptime);

	g_event_loop->RunInLoop(PendingFunc);
}

int main()
{

	higan::EventLoop event_loop;
	g_event_loop = &event_loop;

	event_loop.RunInLoop(PendingFunc);
	event_loop.Loop();
}