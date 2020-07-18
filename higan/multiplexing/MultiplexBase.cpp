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
void MultiplexBase::UpdateChannel(Channel *channel)
{

}

int MultiplexBase::LoopOnce(int timeout, MultiplexBase::ChannelList* active_channel_list)
{
	return 0;
}
