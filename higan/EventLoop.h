//
// Created by rjd67 on 2020/7/16.
//

#ifndef HIGAN_EVENTLOOP_H
#define HIGAN_EVENTLOOP_H

#include <memory>
#include <functional>
#include <queue>

#include "higan/multiplexing/MultiplexBase.h"

namespace higan
{

class Channel;
/**
 * 事件循环类统一所有不同的事件循环
 */
class EventLoop : public noncopyable
{
public:

	typedef std::function<void()> PendingFunc;

	EventLoop();
	~EventLoop();

	void Loop();

	void UpdateChannel(Channel* channel);

	/**
	 * 在事件循环中执行函数  当前如果正在调用事件处理函数 则延迟调用 否则立即调用
	 * @param func
	 */
	void RunInLoop(const PendingFunc& func);
private:

	std::unique_ptr<MultiplexBase> multiplex_base_;

	/**
	 * 就绪Channel列表
	 */
	MultiplexBase::ChannelList active_channel_list_;

	bool handling_pending_event_;
	bool looping_;

	std::queue<PendingFunc> pending_func_queue_;

	void HandleActiveEvent(int active_event_num);

	void HandleTimeoutEvent(int timeout);

	void CallPendingFunc();
};
}
#endif //HIGAN_EVENTLOOP_H
