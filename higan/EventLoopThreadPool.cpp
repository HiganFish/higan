//
// Created by rjd67 on 2020/8/3.
//

#include "higan/EventLoopThreadPool.h"
#include "higan/utils/Logger.h"

using namespace higan;

EventLoopThreadPool::EventLoopThreadPool(const std::string& name, EventLoop* base_loop):
		name_(name),
		threads_(),
		event_loops_(),
		running_(false),
		thread_num_(0)

{
	event_loops_.push_back(base_loop);
}

EventLoopThreadPool::~EventLoopThreadPool()
{
	running_ = false;
}

void EventLoopThreadPool::Init(int thread_num)
{
	EXIT_IF(running_, "EventLoopThreadPool %s is running", name_.c_str());

	running_ = true;

	for (int i = 0; i < thread_num; ++i)
	{
		threads_.emplace_back(new EventLoopThread(name_ + "@thread-" + std::to_string(thread_num)));
		event_loops_.push_back(threads_[i]->GetEventLoop());
	}

	thread_num_ = thread_num;
}

EventLoop* EventLoopThreadPool::GetNextEventLoop()
{
	EXIT_IF(!running_, "EventLoopThreadPool %s hadn't started", name_.c_str());

	EventLoop* result = event_loops_[0];

	if (thread_num_ != 0)
	{
		static size_t event_loop_sub = 1;
		result = event_loops_[event_loop_sub];

		event_loop_sub = event_loop_sub % thread_num_ + 1;
	}

	return result;
}
