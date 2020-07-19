//
// Created by lsmg on 7/15/20.
//

#ifndef _CHANNEL_H_
#define _CHANNEL_H_

#include <functional>

#include "higan/utils/noncopyable.h"

namespace higan
{

class EventLoop;
class Channel : public noncopyable
{
public:

	enum ChannelStatus
	{
		CHANNEL_NOT_ADDED,
		CHANNEL_ADDED,
	};

	typedef std::function<void()> ChannelCallback;

	/**
	 * 提供事件循环与channel所属的socket描述符
	 * @param loop
	 * @param fd
	 */
	Channel(EventLoop* loop, const std::string& connection_name, int fd);
	~Channel();

	int GetFd() const;

	/**
	 * @return 是否注册可读事件
	 */
	bool GetRegisterReadable() const;

	/**
	 * @return 是否注册可写事件
	 */
	bool GetRegisterWritable() const;

	void EnableReadable();
	void DisableReadable();

	void EnableWritable();
	void DisableWritable();

	void DisableAll();

	void SetChannelReadable(bool ready);

	void SetChannelWritable(bool ready);

	void SetChannelError(bool ready);

	/**
	 * EventLoop调用, 用来调用对应的事件处理函数
	 */
	void HandleEvent();

	void SetErrorCallback(const ChannelCallback& cb);
	void SetWritableCallback(const ChannelCallback& cb);
	void SetReadableCallback(const ChannelCallback& cb);

	ChannelStatus GetChannelStatus() const;

	void SetChannelStatus(ChannelStatus status);
private:

	EventLoop* loop_;
	std::string connection_name_;
	int fd_;

	bool connecting_;

	/**
	 * 是否注册可读事件
	 */
	bool register_readable_;

	/**
	 * 是否注册可写事件
	 */
	bool register_writable_;

	/**
	 * 返回了可读事件
	 */
	bool channel_readable_;

	/**
	 * 返回了可写事件
	 */
	bool channel_writable_;

	/**
	 * 返回了错误事件
	 */
	bool channel_error_;

	ChannelCallback error_callback_;
	ChannelCallback writable_callback_;
	ChannelCallback readable_callback_;

	ChannelStatus channel_status_;
};
}

#endif //_CHANNEL_H_
