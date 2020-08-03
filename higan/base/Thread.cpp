//
// Created by rjd67 on 2020/8/2.
//

#include "Thread.h"
#include "higan/base/Mutex.h"
#include "higan/utils/Logger.h"

using namespace higan;

struct ThreadInternalAttr
{
	ThreadInternalAttr(const std::string& n, const Thread::ThreadFunc& fc):
			name(n),
			func(fc)
	{

	}
	std::string name;
	Thread::ThreadFunc func;
};

void* ThreadInternalFunction(void* args)
{
	ThreadInternalAttr* attr = static_cast<ThreadInternalAttr*>(args);
	if (!attr)
	{
		return nullptr;
	}

	LOG("Thread: %s, call start", attr->name.c_str());
	attr->func();
	LOG("Thread: %s, call end", attr->name.c_str());

	return nullptr;
}

Thread::Thread(const std::string& name):
		name_(name),
		thread_()
{
	LOG("create thread: %s", name_.c_str());
}

Thread::~Thread()
{
	LOG("destroy thread: %s", name_.c_str());
}

bool Thread::CallFunction(const ThreadFunc& func)
{
	ThreadInternalAttr *attr = new ThreadInternalAttr(name_, func);

	if (pthread_create(&thread_, nullptr, ThreadInternalFunction, static_cast<void*>(attr)) != 0)
	{
		LOG_IF(true, "thread: %s pthread_create error", name_.c_str());
		return false;
	}

	return true;
}

void Thread::Join() const
{
	pthread_join(thread_, nullptr);
}

