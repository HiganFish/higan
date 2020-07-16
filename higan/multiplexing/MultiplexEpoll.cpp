//
// Created by lsmg on 7/15/20.
//

#include <cerrno>
#include <unistd.h>

#include "higan/multiplexing/MultiplexEpoll.h"
#include "higan/Channel.h"
#include "higan/utils/Logger.h"

using namespace higan;

const int MultiplexEpoll::EPOLL_MAX_EVNETS;

MultiplexEpoll::MultiplexEpoll():
epollfd_(epoll_create(5)),
epoll_events_(EPOLL_MAX_EVNETS)
{
	
}

MultiplexEpoll::~MultiplexEpoll()
{
	close(epollfd_);
}

bool MultiplexEpoll::Add(Channel* channel)
{

	return UpdateChannelEvent(channel, EPOLL_CTL_ADD);
}

bool MultiplexEpoll::Delete(Channel* channel)
{
	return UpdateChannelEvent(channel, EPOLL_CTL_DEL);
}

bool MultiplexEpoll::Modify(Channel* channel)
{

	return UpdateChannelEvent(channel, EPOLL_CTL_MOD);
}

int MultiplexEpoll::LoopOnce(int timeout, MultiplexBase::ChannelList* active_channel_list)
{

	int epoll_ret = epoll_wait(epollfd_, &epoll_events_[0], EPOLL_MAX_EVNETS, timeout);

	if (epoll_ret < 0)
	{
		if (errno != EINTR)
		{
			LOG_IF(true, "epoll_wait error");
			return -1;
		}
	}

	if (epoll_ret == 0)
	{
		return 0;
	}

	FillActiveChannelList(epoll_ret, active_channel_list);

	return epoll_ret;
}

void MultiplexEpoll::FillActiveChannelList(int active_event_num, MultiplexBase::ChannelList* active_channel_list)
{
	if (active_channel_list == nullptr)
	{
		return;
	}

	for (int i = 0; i < active_event_num; ++i)
	{

		Channel* channel = reinterpret_cast<Channel*>(epoll_events_[i].data.ptr);

		if (channel == nullptr)
		{
			continue;
		}

		channel->SetChannelReadable(epoll_events_[i].events & EPOLLIN);
		channel->SetChannelWritable(epoll_events_[i].events & EPOLLOUT);
		channel->SetChannelError(epoll_events_[i].events & (EPOLLERR | EPOLLHUP));

		(*active_channel_list)[i] = channel;
	}
}

bool MultiplexEpoll::UpdateChannelEvent(Channel* channel, int option) const
{
	if (channel == nullptr)
	{
		return false;
	}
	struct epoll_event ev{};

	uint32_t event = EPOLLET;
	event |= channel->GetRegisterReadable() ? EPOLLIN : 0;
	event |= channel->GetRegisterWritable() ? EPOLLOUT : 0;

	ev.data.ptr = reinterpret_cast<void*>(channel);
	ev.events = event;

	bool result = epoll_ctl(epollfd_, option, channel->GetFd(), &ev);
	LOG_IF(result, "EPOLL_CTL_%s error, fd: %d",
			option == EPOLL_CTL_ADD ? "ADD" :
			option == EPOLL_CTL_DEL ? "DEL" :
			option == EPOLL_CTL_MOD ? "MOD" : "UNKNOWN",
			channel->GetFd());

	return result;
}
