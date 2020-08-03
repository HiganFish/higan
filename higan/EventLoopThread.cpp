//
// Created by rjd67 on 2020/8/3.
//

#include "EventLoopThread.h"

using namespace higan;

EventLoopThread::EventLoopThread(const std::string& name):
		name_(name),
		loop_(),
		thread_(name_, std::bind(&EventLoop::Loop, &loop_))
{
	thread_.Start();
}

EventLoopThread::~EventLoopThread()
{
	loop_.Quit();
	thread_.Join();
}

EventLoop* EventLoopThread::GetEventLoop()
{
	return &loop_;
}
