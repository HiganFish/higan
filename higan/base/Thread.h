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
	 * 创建线程包装
	 * @param name 线程名称
	 */
	explicit Thread(const std::string& name);
	~Thread();

	void Join() const;

	/**
	 * 每次调用会创建新的线程 线程执行完func后自动退出
	 * @param func
	 * @return
	 */
	bool CallFunction(const ThreadFunc& func);

private:
	std::string name_;

	pthread_t thread_;

};
}

#endif //HIGAN_THREAD_H
