//
// Created by rjd67 on 2020/7/16.
//

#include "higan/utils/Logger.h"
#include "higan/EventLoop.h"
#include "higan/multiplexing/MultiplexEpoll.h"
#include "higan/Channel.h"

using namespace higan;

EventLoop::EventLoop():
		multiplex_base_(new MultiplexEpoll()),
		active_channel_list_(MultiplexEpoll::EPOLL_MAX_EVNETS),
		timeout_timers_(),
		looping_(false),
		quit_(false),
		handling_pending_event_(false),
		mutex_(),
		timer_manager_()
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

		/**
		 * 从定时器队列中得到 最近定时器到现在的时间
		 * 最小为10ms
		 */
		timeout = std::max(timer_manager_.GetMinTimeout(), 10);
		bool loop_timeout = multiplex_base_->LoopOnce(timeout, &active_channel_list_);

		if (loop_timeout)
		{
			HandleTimeoutEvent();
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

void EventLoop::HandleTimeoutEvent()
{
	timeout_timers_.clear();
	timer_manager_.GetTimeoutTimer(&timeout_timers_);

	for (const Timer& timer : timeout_timers_)
	{
		LOG_INFO << higan::Fmt("Timer: %s timeout", timer.GetName().c_str());
		timer.GetTimerCallback()(timer);
	}
}

void EventLoop::CallPendingFunc()
{
	std::queue<EventLoopFunc> pending;

	{
		/**
		 * muduo这里太妙了 swap之后 立刻释放锁 对CallPendingFunc和RunInLoop影响都很小
		 * 如果不进行拷贝 而是加锁后直接处理pending_func_queue_会对RunInLoop产生较大影响
		 */
		MutexLockGuard guard(mutex_);
		pending.swap(pending_func_queue_);
	}

	while (!pending.empty())
	{
		pending.front()();
		pending.pop();
	}
}

void EventLoop::RunInLoop(const EventLoop::EventLoopFunc& func)
{
	{
		MutexLockGuard guard(mutex_);
		pending_func_queue_.push(func);
	}
}

void EventLoop::UpdateChannel(Channel* channel)
{
	multiplex_base_->UpdateChannel(channel);
}

void EventLoop::Quit()
{
	quit_ = true;
}

void EventLoop::AddTimer(const Timer& timer)
{
	LOG_INFO << higan::Fmt("add new timer: %s expire: %s", timer.GetName().c_str(),
				timer.GetExpireTime().FormatToString().c_str());
	timer_manager_.Insert(timer);
}
