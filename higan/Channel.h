//
// Created by lsmg on 7/15/20.
//

#ifndef _CHANNEL_H_
#define _CHANNEL_H_

namespace higan
{

class Channel
{
public:

	explicit Channel(int fd);
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

	/**
	 * @return 是否返回了可读事件
	 */
	bool GetChannelReadable() const;

	/**
	 * @return 是否返回了可写事件
	 */
	bool GetChannelWritable() const;

	/**
	 * @return 是否返回了错误事件
	 */
	bool GetChannelError() const;

	void SetChannelReadable(bool ready);

	void SetChannelWritable(bool ready);

	void SetChannelError(bool ready);

	/**
	 * EventLoop调用, 用来调用对应的事件处理函数
	 */
	void HandleEvent();

private:

	int fd_;

	bool register_readable_;
	bool register_writable_;

	bool channel_readable_;
	bool channel_writable_;
	bool channel_error_;
};
}

#endif //_CHANNEL_H_
