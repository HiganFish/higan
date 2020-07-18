//
// Created by lsmg on 7/15/20.
//

#include <cerrno>
#include <unistd.h>

#include "higan/multiplexing/MultiplexEpoll.h"
#include "higan/Channel.h"
#include "higan/utils/Logger.h"

using namespace higan;

const int MultiplexEpoll::EPOLL_MAX_EVNETS = 100;
const int MultiplexEpoll::EPOLL_DEFAULT_EVENT = EPOLLET;

MultiplexEpoll::MultiplexEpoll():
		epollfd_(epoll_create(5)),
		epoll_events_(EPOLL_MAX_EVNETS)
{
	
}

MultiplexEpoll::~MultiplexEpoll()
{
	close(epollfd_);
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
		return 0;
	}
	else if (epoll_ret == 0)
	{

	}
	else
	{
		FillActiveChannelList(epoll_ret, active_channel_list);
	}

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

bool MultiplexEpoll::UpdateChannelEvent(Channel* channel, int event, int option) const
{
	if (channel == nullptr)
	{
		return false;
	}
	struct epoll_event ev{};
	ev.events = event;

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

void MultiplexEpoll::UpdateChannel(Channel* channel)
{
	if (!channel)
	{
		return;
	}

	int event = EPOLL_DEFAULT_EVENT;
	event |= channel->GetRegisterWritable() ? EPOLLOUT : 0;
	event |= channel->GetRegisterReadable() ? EPOLLIN : 0;

	/**
	 * 当Channel未被注册时 如果没有设定事件则错误 设定了事件则添加Channel
	 * 当Channel注册后, 如果事件为空则清除Channel, 否则修改注册事件
	 */

	switch (channel->GetChannelStatus())
	{
		case Channel::CHANNEL_NOT_ADDED:
		{
			if (event == EPOLL_DEFAULT_EVENT)
			{
				LOG_IF(true, "CHANNEL_NOT_ADDED but no event");
			}
			else
			{
				UpdateChannelEvent(channel, event, EPOLL_CTL_ADD);
				channel->SetChannelStatus(Channel::CHANNEL_ADDED);
			}
			break;
		}
		case Channel::CHANNEL_ADDED:
		{
			if (event == EPOLL_DEFAULT_EVENT)
			{
				UpdateChannelEvent(channel, event, EPOLL_CTL_MOD);
			}
			else
			{
				UpdateChannelEvent(channel, 0, EPOLL_CTL_DEL);
				channel->SetChannelStatus(Channel::CHANNEL_NOT_ADDED);
			};
			break;
		}
	}
}
