//
// Created by rjd67 on 2020/8/2.
//

#include "ThreadPool.h"

using namespace higan;

ThreadPool::ThreadPool(const std::string& name):
		name_(name),
		mutex_(),
		condition_(mutex_),
		threads_(),
		pending_functions_(),
		running_(false)
{

}

ThreadPool::~ThreadPool()
{
	if (running_)
	{
		Stop();
	}
}

void ThreadPool::Init(int thread_num)
{
	running_ = true;

	for (int i = 0; i < thread_num; ++i)
	{
		threads_.emplace_back(new Thread(name_ + "@thead-" + std::to_string(i),
				std::bind(&ThreadPool::ThreadFunctionInternal, this)));
		threads_[i]->Start();
	}
}

void ThreadPool::CallFunction(const Thread::ThreadFunc& func)
{
	{
		MutexLockGuard guard(mutex_);
		pending_functions_.push_back(func);
		condition_.Notify();
	}
}


void ThreadPool::ThreadFunctionInternal()
{
	/**
	 * 当不存在执行任务的时, 线程Wait. 新的任务加入后唤醒线程执行任务
	 */
	while (running_)
	{
		Thread::ThreadFunc func;
		{
			MutexLockGuard guard(mutex_);
			while (pending_functions_.empty() && running_)
			{
				condition_.Wait();
			}

			if (!pending_functions_.empty())
			{
				func = pending_functions_.back();
				pending_functions_.pop_back();
				condition_.Notify();
			}
		}

		if (func)
		{
			func();
		}
	}
}

void ThreadPool::Stop()
{
	/**
	 * 正在执行任务的线程 执行完后立即退出
	 * 正处于等待状态的线程 唤醒后 如果存在未执行任务 执行一次任务后也退出
	 */
	running_ = false;
	condition_.NotifyAll();

	for (const auto& thread : threads_)
	{
		thread->Join();
	}

	threads_.clear();
	pending_functions_.clear();
}
