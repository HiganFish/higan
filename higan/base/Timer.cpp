//
// Created by rjd67 on 2020/8/5.
//

#include "Timer.h"

using namespace higan;

Timer::Timer(const std::string& name, int timeout, bool repeat, const TimerCallback& callback):
		name_(name),
		expire_time_(TimeStamp::Now()),
		timeout_(timeout),
		repeat_(repeat),
		timer_callback_(callback)
{
	expire_time_.AddMillisecond(timeout);
}

Timer::~Timer()
{

}

const std::string& Timer::GetName() const
{
	return name_;
}

void Timer::SetName(const std::string& name)
{
	name_ = name;
}

bool Timer::Repeat() const
{
	return repeat_;
}

void Timer::SetRepeat(bool repeat)
{
	repeat_ = repeat;
}

const Timer::TimerCallback& Timer::GetTimerCallback() const
{
	return timer_callback_;
}

void Timer::SetTimerCallback(const Timer::TimerCallback& timer_callback)
{
	timer_callback_ = timer_callback;
}

const TimeStamp& Timer::GetExpireTime() const
{
	return expire_time_;
}

void Timer::SetExpireTime(const TimeStamp& expire_time)
{
	expire_time_ = expire_time;
}

void Timer::AdjustExpireTime()
{
	expire_time_.SetMicrosecond(TimeStamp::Now());
	expire_time_.AddMillisecond(timeout_);

}

TimerManager::TimerManager():
		timer_queue_(),
		min_expire_time()
{

}

TimerManager::~TimerManager()
{

}

void TimerManager::Insert(const Timer& timer)
{
	timer_queue_.push(timer);
	min_expire_time = timer_queue_.top().GetExpireTime();
}

int TimerManager::GetMinTimeout()
{
	if (timer_queue_.empty())
	{
		return 0;
	}
	else
	{
		return static_cast<int>(min_expire_time.GetMilliSecond() - TimeStamp::Now() / TimeStamp::MICROSECOND_PER_MILLISECOND - 1);
	}
}

int TimerManager::GetTimeoutTimer(std::vector<Timer>* timeout_timers)
{
	if (timer_queue_.empty() || timeout_timers == nullptr)
	{
		return 0;
	}

	TimeStamp now(TimeStamp::Now());
	min_expire_time = timer_queue_.top().GetExpireTime();

	if (now < min_expire_time)
	{
		return 0;
	}

	int result_count = 0;

	while (!timer_queue_.empty() && min_expire_time <= now)
	{
		result_count++;

		const Timer& timer = timer_queue_.top();
		min_expire_time = timer.GetExpireTime();

		timeout_timers->push_back(timer);

		if (timer.Repeat())
		{
			Timer new_timer(timer);
			new_timer.AdjustExpireTime();
			Insert(new_timer);
		}

		timer_queue_.pop();
	}

	return result_count;
}
