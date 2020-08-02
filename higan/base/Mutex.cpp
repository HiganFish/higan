//
// Created by rjd67 on 2020/8/2.
//

#include "higan/base/Mutex.h"

using namespace higan;

Mutex::Mutex()
{
	pthread_mutexattr_t attr;
	PTHREAD_CHECK(pthread_mutex_init(&mutex_, &attr));
}

Mutex::~Mutex()
{
	PTHREAD_CHECK(pthread_mutex_destroy(&mutex_));
}

void Mutex::Lock()
{
	PTHREAD_CHECK(pthread_mutex_lock(&mutex_));
}

void Mutex::UnLock()
{
	PTHREAD_CHECK(pthread_mutex_unlock(&mutex_));
}

pthread_mutex_t* Mutex::GetMutexT()
{
	return &mutex_;
}


MutexLockGuard::MutexLockGuard(Mutex& mutex):
		mutex_(mutex)
{
	mutex_.Lock();
}

MutexLockGuard::~MutexLockGuard()
{
	mutex_.UnLock();
}
