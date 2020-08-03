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

	// LOG("Thread: %s, call start", attr->name.c_str());
	attr->func();
	// LOG("Thread: %s, call end", attr->name.c_str());

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
		LOG("detach thread: %s", name_.c_str());
		pthread_detach(thread_);
	}
}

void Thread::Start()
{
	EXIT_IF(started, "Thread: %s had started", name_.c_str());

	started = true;

	ThreadInternalAttr *attr = new ThreadInternalAttr(name_, function_);

	if (pthread_create(&thread_, nullptr, ThreadInternalFunction, static_cast<void*>(attr)) != 0)
	{
		LOG_IF(true, "thread: %s pthread_create error", name_.c_str());
	}

	// LOG("create thread: %s", name_.c_str());

}

void Thread::Join()
{
	EXIT_IF(!started, "Thread: %s hadn't start", name_.c_str());
	EXIT_IF(joined, "Thread: %s had joined", name_.c_str());

	joined = true;
	pthread_join(thread_, nullptr);
	LOG("exit thread: %s", name_.c_str());
}

