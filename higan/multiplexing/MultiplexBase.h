//
// Created by lsmg on 7/15/20.
//

#ifndef _MULTIPLEXBASE_H_
#define _MULTIPLEXBASE_H_

#include <vector>
#include <ctime>
#include "higan/base/noncopyable.h"

namespace higan
{
class Channel;

class MultiplexBase : public noncopyable
{
public:

	typedef std::vector<Channel*> ChannelList;


	MultiplexBase();
	~MultiplexBase();

	virtual void UpdateChannel(Channel* channel);

	virtual bool LoopOnce(int timeout, MultiplexBase::ChannelList* active_channel_list);

private:

	ChannelList channel_list_;
};
}

#endif //_MULTIPLEXBASE_H_
