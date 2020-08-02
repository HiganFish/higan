//
// Created by rjd67 on 2020/8/2.
//

#include "Condition.h"

using namespace higan;

Condition::Condition(Mutex& mutex):
		mutex_(mutex),
		cond_()
{
	pthread_condattr_t cond_attr;
	PTHREAD_CHECK(pthread_cond_init(&cond_, &cond_attr));
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
