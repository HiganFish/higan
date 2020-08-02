//
// Created by rjd67 on 2020/8/2.
//

#ifndef HIGAN_CONDITION_H
#define HIGAN_CONDITION_H

#include "higan/base/Mutex.h"

namespace higan
{
class Condition
{
public:
	Condition(Mutex& mutex);
	~Condition();

	void Wait();

	void Notify();
private:

	Mutex& mutex_;

	pthread_cond_t cond_;
};
}

#endif //HIGAN_CONDITION_H
