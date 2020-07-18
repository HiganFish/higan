//
// Created by lsmg on 7/15/20.
//

#include "higan/Channel.h"
#include "higan/EventLoop.h"

using namespace higan;

Channel::Channel(EventLoop* loop, int fd):
		loop_(loop),
		fd_(fd),
		register_readable_(false),
		register_writable_(false),
		channel_readable_(false),
		channel_writable_(false),
		channel_error_(false),
		channel_status_(CHANNEL_NOT_ADDED)
{

}

Channel::~Channel()
{

}

int Channel::GetFd() const
{
	return fd_;
}

bool Channel::GetRegisterReadable() const
{
	return register_readable_;
}

bool Channel::GetRegisterWritable() const
{
	return register_writable_;
}

void Channel::EnableReadable()
{
	register_readable_ = true;
	loop_->UpdateChannel(this);
}

void Channel::DisableReadable()
{
	register_readable_ = false;
	loop_->UpdateChannel(this);
}

void Channel::EnableWritable()
{
	register_writable_ = true;
	loop_->UpdateChannel(this);
}

void Channel::DisableWritable()
{
	register_writable_ = false;
	loop_->UpdateChannel(this);
}

void Channel::DisableAll()
{
	register_writable_ = false;
	register_readable_ = false;
	loop_->UpdateChannel(this);
}

void Channel::SetChannelReadable(bool ready)
{
	channel_readable_ = ready;
}

void Channel::SetChannelWritable(bool ready)
{
	channel_writable_ = ready;
}

void Channel::SetChannelError(bool ready)
{
	channel_error_ = ready;
}

void Channel::HandleEvent()
{
	if (channel_error_)
	{
		if (error_callback_)
		{
			error_callback_();
		}
		return;
	}

	if (channel_readable_)
	{
		if (readable_callback_)
		{
			readable_callback_();
		}
	}

	if (channel_writable_)
	{
		if (writable_callback_)
		{
			writable_callback_();
		}
	}
}

void Channel::SetErrorCallback(const ChannelCallbackFunc& cb)
{
	error_callback_ = cb;
}

void Channel::SetWritableCallback(const ChannelCallbackFunc& cb)
{
	writable_callback_ = cb;

}

void Channel::SetReadableCallback(const ChannelCallbackFunc& cb)
{
	readable_callback_ = cb;
}

Channel::ChannelStatus Channel::GetChannelStatus() const
{
	return channel_status_;
}

void Channel::SetChannelStatus(Channel::ChannelStatus status)
{
	channel_status_ = status;
}
