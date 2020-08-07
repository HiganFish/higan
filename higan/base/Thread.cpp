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

	LOG_DEBUG << higan::Fmt("Thread: %s, call start", attr->name.c_str());
	attr->func();
	LOG_DEBUG << higan::Fmt("Thread: %s, call end", attr->name.c_str());

	return nullptr;
}

Thread::Thread(const std::string& name, const ThreadFunc& function):
		name_(name),
		thread_(),
		function_(function),
		started(false),
		joined(false)
{

}

Thread::~Thread()
{
	/**
	 * 当线程未已经启动且未被Join detach线程继续运行 否则直接退出
	 */
	if (started && !joined)
	{
		LOG_INFO << higan::Fmt("detach thread: %s", name_.c_str());
		pthread_detach(thread_);
	}
}

void Thread::Start()
{
	if (started)
	{
		LOG_FATAL << higan::Fmt("thread: %s had started", name_.c_str());
	}

	started = true;

	ThreadInternalAttr *attr = new ThreadInternalAttr(name_, function_);

	if (pthread_create(&thread_, nullptr, ThreadInternalFunction, static_cast<void*>(attr)) != 0)
	{
		LOG_FATAL << higan::Fmt("thread: %s pthread_create error", name_.c_str());
	}

}

void Thread::Join()
{
	if (!started)
	{
		LOG_FATAL << higan::Fmt("Thread: %s hadn't start", name_.c_str());
	}
	if (joined)
	{
		LOG_FATAL << higan::Fmt("Thread: %s had joined", name_.c_str());
	}

	joined = true;
	pthread_join(thread_, nullptr);
	LOG_INFO << higan::Fmt("Thread: %s exit", name_.c_str());
}