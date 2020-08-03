//
// Created by rjd67 on 2020/8/3.
//

#ifndef HIGAN_EVENTLOOPTHREAD_H
#define HIGAN_EVENTLOOPTHREAD_H

#include "higan/base/Thread.h"
#include "higan/EventLoop.h"

namespace higan
{
class EventLoopThread
{
public:
	explicit EventLoopThread(const std::string& name);
	~EventLoopThread();

	EventLoop* GetEventLoop();

private:

	std::string name_;
	EventLoop loop_;

	Thread thread_;

};
}

#endif //HIGAN_EVENTLOOPTHREAD_H
