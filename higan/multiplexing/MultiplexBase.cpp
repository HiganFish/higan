//
// Created by lsmg on 7/15/20.
//

#include "higan/multiplexing/MultiplexBase.h"

using namespace higan;

MultiplexBase::MultiplexBase()
{

}
MultiplexBase::~MultiplexBase()
{

}
bool MultiplexBase::Add(Channel* channel)
{
	return false;
}
bool MultiplexBase::Modify(Channel* channel)
{
	return false;
}

bool MultiplexBase::Delete(Channel* channel)
{
	return false;
}

int MultiplexBase::LoopOnce(int timeout, MultiplexBase::ChannelList* active_channel_list)
{
	return 0;
}
