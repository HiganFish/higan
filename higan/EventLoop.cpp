//
// Created by rjd67 on 2020/7/16.
//

#include "higan/EventLoop.h"
#include "higan/multiplexing/MultiplexEpoll.h"
#include "higan/Channel.h"

using namespace higan;

EventLoop::EventLoop():
multiplex_base_(new MultiplexEpoll()),
active_channel_list_(MultiplexEpoll::EPOLL_MAX_EVNETS),
looping_(false),
handling_pending_event_(false)
{

}

EventLoop::~EventLoop()
{

}

void EventLoop::Loop()
{
	int timeout = 10;

	if (!multiplex_base_)
	{
		return;
	}

	looping_ = true;

	while (looping_)
	{
		int multiplex_result = multiplex_base_->LoopOnce(timeout, &active_channel_list_);

		if (multiplex_result == -1)
		{
			looping_ = false;
		}
		else if (multiplex_result == 0)
		{
			HandleTimeoutEvent(timeout);
		}
		else
		{
			HandleActiveEvent(multiplex_result);
		}

		HandlePendingFunc();
	}
}

void EventLoop::HandleActiveEvent(int active_event_num)
{
	handling_pending_event_ = true;
	for (int i = 0; i < active_event_num; ++i)
	{
		active_channel_list_[i]->HandleEvent();
	}
	handling_pending_event_ = false;
}

void EventLoop::HandleTimeoutEvent(int timeout)
{

}

void EventLoop::HandlePendingFunc()
{
	while (!pending_func_queue_.empty())
	{
		pending_func_queue_.front()();
		pending_func_queue_.pop();
	}
}

void EventLoop::RunInLoop(const EventLoop::PendingFunc& func)
{
	/**
	 * 如果循环未开始则加入队列等待循环开始后 第一次返回时执行
	 * 如果循环已经开始则
	 * 	在调用事件处理函数时 放入等待队列
	 * 	未调用时 立即执行
	 */
	if (looping_)
	{
		if (handling_pending_event_)
		{
			pending_func_queue_.push(func);
		}
		else
		{
			func();
		}
	}
	else
	{
		pending_func_queue_.push(func);
	}
}
