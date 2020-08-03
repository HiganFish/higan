//
// Created by rjd67 on 2020/8/2.
//

#include "Condition.h"

using namespace higan;

Condition::Condition(Mutex& mutex):
		mutex_(mutex),
		cond_()
{
	/**
	 * 如果不设置其他参数 应该传入nullptr 如果传入未初始化的
	 * pthread_cond_attr 会出错
	 */
	PTHREAD_CHECK(pthread_cond_init(&cond_, nullptr));
}

Condition::~Condition()
{
	PTHREAD_CHECK(pthread_cond_destroy(&cond_));
}

void Condition::Wait()
{
	PTHREAD_CHECK(pthread_cond_wait(&cond_, mutex_.GetMutexT()));
}

void Condition::Notify()
{
	PTHREAD_CHECK(pthread_cond_signal(&cond_));
}

void Condition::NotifyAll()
{
	PTHREAD_CHECK(pthread_cond_broadcast(&cond_));
}
