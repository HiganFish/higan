//
// Created by rjd67 on 2020/10/14.
//

#ifndef HIGAN_SINGLETON_H
#define HIGAN_SINGLETON_H

#include <pthread.h>
#include "noncopyable.h"

namespace higan
{
template<class T>
class Singleton : noncopyable
{
public:
	Singleton() = delete;

	~Singleton() = delete;

	static T& Instance()
	{
		pthread_once(&ponce_, &Singleton::Initialize);
		return *value_;
	}

private:

	static void Initialize()
	{
		value_ = new T;
	}

	static pthread_once_t ponce_;
	static T* value_;
};

template <class T>
pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;

template <class T>
T* Singleton<T>::value_ = nullptr;
}
#endif //HIGAN_SINGLETON_H
