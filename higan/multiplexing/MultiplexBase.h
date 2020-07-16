//
// Created by lsmg on 7/15/20.
//

#ifndef _MULTIPLEXBASE_H_
#define _MULTIPLEXBASE_H_

#include <vector>
#include <ctime>

namespace higan
{
class Channel;

class MultiplexBase
{
public:

	typedef std::vector<Channel*> ChannelList;


	MultiplexBase();
	~MultiplexBase();

	virtual bool Add(Channel* channel);

	virtual bool Modify(Channel* channel);

	virtual bool Delete(Channel* channel);

	virtual int LoopOnce(int timeout, MultiplexBase::ChannelList* active_channel_list);

private:

	ChannelList channel_list_;
};
}

#endif //_MULTIPLEXBASE_H_
