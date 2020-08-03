//
// Created by rjd67 on 2020/8/2.
//

#ifndef HIGAN_THREADPOOL_H
#define HIGAN_THREADPOOL_H

#include <vector>
#include <memory>
#include "higan/base/Thread.h"
#include "higan/base/Condition.h"

namespace higan
{
class ThreadPool
{
public:
	/**
	 * 创建线程池
	 * @param name 线程池名称
	 */
	explicit ThreadPool(const std::string& name);
	~ThreadPool();

	/**
	 * 初始化线程池
	 * @param thread_num 线程数量
	 */
	void Init(int thread_num);

	/**
	 * 从线程池中选取线程 调用函数
	 * @param func
	 */
	void CallFunction(const Thread::ThreadFunc& func);

	void Stop();
private:
	std::string name_;

	Mutex mutex_;
	Condition condition_;

	/**
	 * 这里一开始没有想到使用unique_ptr
	 * emplace_back 之后里就会发生析构????
	 */
	std::vector<std::unique_ptr<Thread>> threads_;
	std::vector<Thread::ThreadFunc> pending_functions_;

	bool running_;

	void ThreadFunctionInternal();
};
}

#endif //HIGAN_THREADPOOL_H
