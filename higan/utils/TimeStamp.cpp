//
// Created by rjd67 on 2020/8/5.
//

#include "TimeStamp.h"

using namespace higan;

const int TimeStamp::MICROSECOND_PER_SECOND = 1000000;
const int TimeStamp::MICROSECOND_PER_MILLISECOND = 1000;


TimeStamp::TimeStamp():
		TimeStamp(0)
{

}

TimeStamp::TimeStamp(int64_t time):
		microsecond_time_(time)
{

}

TimeStamp::~TimeStamp()
{

}

TimeStamp TimeStamp::Now()
{
	struct timeval now{};

	gettimeofday(&now, nullptr);

	return TimeStamp(now.tv_sec * MICROSECOND_PER_SECOND + now.tv_usec);
}

bool TimeStamp::operator<(const TimeStamp& stamp) const
{
	return GetMicroSecond() < stamp.GetMicroSecond();
}

bool TimeStamp::operator<=(const TimeStamp& stamp) const
{
	return GetMicroSecond() <= stamp.GetMicroSecond();
}


TimeStamp& TimeStamp::operator-(const TimeStamp& stamp)
{
	microsecond_time_ -= stamp.GetMicroSecond();

	return *this;
}

int64_t TimeStamp::GetMicroSecond() const
{
	return microsecond_time_;
}

void TimeStamp::AddMillisecond(int millisecond)
{
	microsecond_time_ += millisecond * MICROSECOND_PER_MILLISECOND;
}

int64_t TimeStamp::GetMilliSecond() const
{
	return microsecond_time_ / MICROSECOND_PER_MILLISECOND;
}
