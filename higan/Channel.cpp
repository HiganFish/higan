//
// Created by lsmg on 7/15/20.
//

#include "higan/Channel.h"

using namespace higan;

Channel::Channel(int fd):
fd_(fd),
register_readable_(false),
register_writable_(false),
channel_readable_(false),
channel_writable_(false),
channel_error_(false)
{

}

Channel::~Channel()
{

}

bool Channel::GetChannelReadable() const
{
	return channel_readable_;
}

bool Channel::GetChannelWritable() const
{
	return channel_writable_;
}

bool Channel::GetChannelError() const
{
	return channel_error_;
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
}

void Channel::DisableReadable()
{
	register_readable_ = false;
}

void Channel::EnableWritable()
{
	register_writable_ = true;
}

void Channel::DisableWritable()
{
	register_writable_ = false;
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

}
