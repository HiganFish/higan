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
		quit_(false),
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

	while (!quit_)
	{
		active_channel_list_.clear();
		bool loop_timeout = multiplex_base_->LoopOnce(timeout, &active_channel_list_);

		if (loop_timeout)
		{
			HandleTimeoutEvent(timeout);
		}
		else
		{
			HandleActiveEvent();
		}

		CallPendingFunc();
	}

	looping_ = false;
}

void EventLoop::HandleActiveEvent()
{
	handling_pending_event_ = true;
	for (Channel* channel : active_channel_list_)
	{
		channel->HandleEvent();
	}
	handling_pending_event_ = false;
}

void EventLoop::HandleTimeoutEvent(int timeout)
{

}

void EventLoop::CallPendingFunc()
{
	while (!pending_func_queue_.empty())
	{
		pending_func_queue_.front()();
		pending_func_queue_.pop();
	}
}

void EventLoop::RunInLoop(const EventLoop::PendingFunc& func)
{
	pending_func_queue_.push(func);
}

void EventLoop::UpdateChannel(Channel* channel)
{
	multiplex_base_->UpdateChannel(channel);
}

void EventLoop::Quit()
{
	quit_ = true;
}
