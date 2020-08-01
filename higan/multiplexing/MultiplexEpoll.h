//
// Created by lsmg on 7/15/20.
//

#ifndef _MULTIPLEXEPOLL_H_
#define _MULTIPLEXEPOLL_H_

#include <sys/epoll.h>

#include "higan/multiplexing/MultiplexBase.h"

namespace higan
{
class MultiplexEpoll : public MultiplexBase
{
public:

	const static int EPOLL_MAX_EVNETS;

	MultiplexEpoll();
	~MultiplexEpoll();


	/**
	 * 根据channel状态更新
	 * @param channel
	 */
	void UpdateChannel(Channel* channel) override;

	/**
	 * 执行一次多路复用
	 * @param active_channel_list 填充并返回就绪的事件
	 * @param timeout 超时时间, 可用于设定定时器事件 单位毫秒
	 * @return true 超时 false 正常返回
	 */
	bool LoopOnce(int timeout, MultiplexBase::ChannelList* active_channel_list) override;

private:

	int epollfd_;
	/**
	 * 默认的epoll_event事件
	 */
	const static int EPOLL_DEFAULT_EVENT;

	/**
	 * 存放epoll_wait返回的事件
	 */
	std::vector<epoll_event> epoll_events_;

	/**
	 * 将epoll_events_中就绪事件填充人active_channel_list
	 * @param active_event_num 就绪事件数量
	 * @param active_channel_list 就绪事件保存位置
	 */
	void FillActiveChannelList(int active_event_num, MultiplexBase::ChannelList* active_channel_list);

	/**
	 * 更新channel的注册
	 * @param channel 待更新channel
	 * @param option EPOLL_CTL_ADD EPOLL_CTL_DEL EPOLL_CTL_MOD
	 * @return ok for true
	 */
	bool UpdateChannelEvent(Channel* channel, int event, int option) const;
};
}
#endif //_MULTIPLEXEPOLL_H_
