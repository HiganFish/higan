//
// Created by rjd67 on 2020/8/2.
//

#ifndef HIGAN_THREAD_H
#define HIGAN_THREAD_H

#include <pthread.h>
#include <functional>
#include <string>

namespace higan
{
class Thread
{
public:
	typedef std::function<void()> ThreadFunc;

	/**
	 * 创建线程
	 * @param name 线程名称
	 * @param function 线程执行函数
	 */
	explicit Thread(const std::string& name, const ThreadFunc& function);
	~Thread();

	/**
	 * 当且仅当线程启动并且 未被Join才能调用 否则出错
	 */
	void Join();

	/**
	 * 启动线程执行设定的函数, 线程只能启动一次 否则出错
	 */
	void Start();

private:
	std::string name_;

	pthread_t thread_;

	ThreadFunc function_;

	bool started;
	bool joined;

};
}

#endif //HIGAN_THREAD_H
