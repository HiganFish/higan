//
// Created by lsmg on 7/15/20.
//

#ifndef _MULTIPLEXBASE_H_
#define _MULTIPLEXBASE_H_

#include <queue>
#include <ctime>

namespace higan
{
class Channel;

class MultiplexBase
{
public:

	typedef std::queue<Channel*> ChannelList;

	enum class LoopResult
	{
		LOOP_ERROR = -1, /* 多路复用出错*/
		LOOP_TIMEUP = 0, /* 多路复用超时*/
		LOOP_ACTIVE = 1 /* 多路复用返回就绪事件*/
	};

	MultiplexBase();
	~MultiplexBase();

	virtual bool Add(Channel* channel);

	virtual bool Modify(Channel* channel);

	virtual bool Delete(Channel* channel);

	virtual MultiplexBase::LoopResult LoopOnce(int timeout, MultiplexBase::ChannelList* active_channel_list);

private:

	ChannelList channel_list_;
};
}

#endif //_MULTIPLEXBASE_H_
