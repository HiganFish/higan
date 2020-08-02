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

	explicit Thread(const std::string& name);
	~Thread();

	void Join() const;

	bool CallFunc(const ThreadFunc& func);

private:
	std::string name_;

	pthread_t thread_;

};
}

#endif //HIGAN_THREAD_H
