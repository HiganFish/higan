//
// Created by rjd67 on 2020/8/2.
//

#ifndef HIGAN_MUTEX_H
#define HIGAN_MUTEX_H

#include <pthread.h>
#include <cassert>

/**
 * __builtin_expect(exp, x) exp的结果很可能是
 * x = 0时, 很可能为假
 * x = 1时, 很可能为真
 */

#define PTHREAD_CHECK(ret)	\
	if (__builtin_expect((ret) != 0, 0))	\
		__assert_perror_fail((ret), __FILE__, __LINE__, __func__)


namespace higan
{
class Mutex
{
public:
	Mutex();
	~Mutex();

	void Lock();

	void UnLock();

	pthread_mutex_t* GetMutexT();
private:

	pthread_mutex_t mutex_;
};

class MutexLockGuard
{
public:
	explicit MutexLockGuard(Mutex& mutex);
	~MutexLockGuard();

private:
	Mutex& mutex_;
};
}

#endif //HIGAN_MUTEX_H
