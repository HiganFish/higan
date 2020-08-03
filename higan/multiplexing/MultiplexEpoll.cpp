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
const int MultiplexEpoll::EPOLL_DEFAULT_EVENT = 0;

MultiplexEpoll::MultiplexEpoll():
		epollfd_(epoll_create(5)),
		epoll_events_(EPOLL_MAX_EVNETS)
{
	
}

MultiplexEpoll::~MultiplexEpoll()
{
	close(epollfd_);
}

bool MultiplexEpoll::LoopOnce(int timeout, MultiplexBase::ChannelList* active_channel_list)
{
	// EPOLLET 模式下如果maxevents太小 导致无法接受全部event 并不会丢弃 只有接收来的才会被丢弃
	int event_num = epoll_wait(epollfd_, &*epoll_events_.begin(), static_cast<int>(epoll_events_.size()), timeout);

	if (event_num > 0)
	{
		static int sum_event = 0;
		sum_event += event_num;

		printf("sum event: %d\r\n", sum_event);

		FillActiveChannelList(event_num, active_channel_list);
		if(static_cast<size_t>(event_num) == epoll_events_.size())
		{
			epoll_events_.resize(epoll_events_.size() * 2);
		}
	}
	else if (event_num == 0)
	{
		return true;
	}
	else if (event_num < 0)
	{
		if (errno != EINTR)
		{
			LOG_IF(true, "epoll_wait error");
			exit(errno);
		}
	}

	return false;
}

void MultiplexEpoll::FillActiveChannelList(int active_event_num, MultiplexBase::ChannelList* active_channel_list)
{
	if (active_channel_list == nullptr)
	{
		return;
	}

	for (int i = 0; i < active_event_num; ++i)
	{

		Channel* channel = static_cast<Channel*>(epoll_events_[i].data.ptr);

		if (channel == nullptr)
		{
			continue;
		}

		channel->SetChannelReadable(epoll_events_[i].events & EPOLLIN);
		channel->SetChannelWritable(epoll_events_[i].events & EPOLLOUT);
		channel->SetChannelError(epoll_events_[i].events & (EPOLLERR | EPOLLHUP));

		active_channel_list->push_back(channel);
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

	ev.data.ptr = static_cast<void*>(channel);
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
