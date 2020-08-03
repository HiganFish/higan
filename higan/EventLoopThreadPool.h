//
// Created by rjd67 on 2020/8/3.
//

#ifndef HIGAN_EVENTLOOPTHREADPOOL_H
#define HIGAN_EVENTLOOPTHREADPOOL_H

#include "higan/EventLoopThread.h"

namespace higan
{
class EventLoopThreadPool
{
public:

	/**
	 * 创建线程池
	 * @param name 线程池名称
	 * @param base_loop 线程池的默认返回循环
	 */
	EventLoopThreadPool(const std::string& name, EventLoop* base_loop);
	~EventLoopThreadPool();

	/**
	 * 初始化线程池 创建线程
	 * @param thread_num 线程数量
	 */
	void Init(int thread_num);

	/**
	 * 获取下一个EventLoop
	 * @return 当线程池没有线程的时候返回base_loop 否则从线程池中选择一个线程的EventLoop返回
	 */
	EventLoop* GetNextEventLoop();

private:
	std::string name_;
	std::vector<std::unique_ptr<EventLoopThread>> threads_;
	std::vector<EventLoop*> event_loops_;

	bool running_;

	int thread_num_;
};
}

#endif //HIGAN_EVENTLOOPTHREADPOOL_H
