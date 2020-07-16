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

	const static int EPOLL_MAX_EVNETS = 100;

	MultiplexEpoll();
	~MultiplexEpoll();


	/**
	 * 增加新的Channel到多路复用中, 同时注册入channel_list, 同时更新Channel状态
	 * @param channel 待添加的Channel
	 * @return true for success
	 */
	bool Add(Channel* channel) override;

	/**
	 * 更新Channel
	 * @param channel 待更新的Channel, 同时更新Channel状态
	 * @return true for success
	 */
	bool Modify(Channel* channel) override;

	/**
	 * 从路复用中删除Channel, 同时同channel_list中移除, 同时更新Channel状态
	 * @param channel 待删除的Channel
	 * @return true for success
	 */
	bool Delete(Channel* channel) override;

	/**
	 * 执行一次多路复用
	 * @param active_channel_list 填充并返回就绪的事件
	 * @param timeout 超时时间, 可用于设定定时器事件 单位毫秒
	 * @return 就绪事件数量 -1出错 0超时
	 */
	int LoopOnce(int timeout, MultiplexBase::ChannelList* active_channel_list) override;
private:

	int epollfd_;



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
	bool UpdateChannelEvent(Channel* channel, int option) const;
};
}
#endif //_MULTIPLEXEPOLL_H_
