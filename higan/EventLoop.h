//
// Created by rjd67 on 2020/7/16.
//

#ifndef HIGAN_EVENTLOOP_H
#define HIGAN_EVENTLOOP_H

#include <memory>
#include <functional>
#include <queue>

#include "higan/multiplexing/MultiplexBase.h"
#include "higan/base/Mutex.h"
#include "higan/utils/Timer.h"

namespace higan
{

class Channel;
/**
 * 事件循环类统一所有不同的事件循环
 */
class EventLoop : public noncopyable
{
public:

	typedef std::function<void()> EventLoopFunc;

	EventLoop();
	~EventLoop();

	void Loop();

	void UpdateChannel(Channel* channel);

	/**
	 * 在事件循环中执行函数  当前如果正在调用事件处理函数 则延迟调用 否则立即调用
	 * @param func
	 */
	void RunInLoop(const EventLoopFunc& func);

	void Quit();

	void AddTimer(const Timer& timer);

private:

	std::unique_ptr<MultiplexBase> multiplex_base_;

	/**
	 * 就绪Channel列表
	 */
	MultiplexBase::ChannelList active_channel_list_;
	std::vector<Timer> timeout_timers_;

	bool handling_pending_event_;
	bool looping_;
	bool quit_ = false;

	std::queue<EventLoopFunc> pending_func_queue_;

	Mutex mutex_;

	TimerManager timer_manager_;

	void HandleActiveEvent();

	void HandleTimeoutEvent();
	void CallPendingFunc();
};
}
#endif //HIGAN_EVENTLOOP_H
