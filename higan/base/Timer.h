//
// Created by rjd67 on 2020/8/5.
//

#ifndef HIGAN_TIMER_H
#define HIGAN_TIMER_H

#include <functional>
#include <queue>

#include "higan/base/TimeStamp.h"

namespace higan
{

class Timer
{
public:
	typedef std::function<void(const Timer&)> TimerCallback;

	/**
	 * 创建定时器包装类
	 * @param name 定时器名称
	 * @param timeout 超时时间 单位毫秒
	 * @param repeat 是否重复
	 * @param callback 超时后执行的回调函数
	 */
	Timer(const std::string& name, int timeout, bool repeat, const TimerCallback& callback);
	~Timer();

	const std::string& GetName() const;

	void SetName(const std::string& name);

	const TimeStamp& GetExpireTime() const;

	void SetExpireTime(const TimeStamp& expire_time);

	bool Repeat() const;

	void SetRepeat(bool repeat);

	const TimerCallback& GetTimerCallback() const;

	void SetTimerCallback(const TimerCallback& timer_callback);

	/**
	 * 调整定时器的 过期时间 用于调整重复的定时器
	 */
	void AdjustExpireTime();

	bool operator<(const Timer& timer) const
	{
		return GetExpireTime() < timer.GetExpireTime();
	}

private:

	std::string name_;

	/**
	 * 过期时间
	 */
	TimeStamp expire_time_;

	/**
	 * 超时时间 单位毫秒
	 */
	int timeout_;

	bool repeat_;

	TimerCallback timer_callback_;
};




class TimerManager
{
public:

	TimerManager();
	~TimerManager();

	void Insert(const Timer& timer);

	/**
	 * 获得最短超时时间 单位毫秒
	 * @return
	 */
	int GetMinTimeout();

	/**
	 * 处理超时的事件
	 * @param timeval 自上次调用经过的时间
	 */
	int GetTimeoutTimer(std::vector<Timer>* timeout_timers);

private:

	std::priority_queue<Timer> timer_queue_;

	/**
	 * 当CallTimeoutTimer传入的参数 小于min_timeout_ 积累值并返回
	 */
	TimeStamp min_expire_time;
};
}

#endif //HIGAN_TIMER_H
