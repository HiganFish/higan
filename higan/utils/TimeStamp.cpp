//
// Created by rjd67 on 2020/8/5.
//

#include <cstdio>
#include <ctime>
#include <string>
#include <cstring>
#include "TimeStamp.h"

using namespace higan;

const int64_t TimeStamp::MICROSECOND_PER_SECOND = 1000000;
const int64_t TimeStamp::MICROSECOND_PER_MILLISECOND = 1000;


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

int64_t TimeStamp::Now()
{
	struct timeval now{};

	gettimeofday(&now, nullptr);

	return now.tv_sec * MICROSECOND_PER_SECOND + now.tv_usec;
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

void TimeStamp::SetMicrosecond(int64_t microsecond)
{
	microsecond_time_ = microsecond;
}


static time_t g_last_second = 0;
static char g_time_buffer[32];
static size_t g_time_buffer_len;

size_t TimeStamp::FormatToBuffer(char* buffer, size_t len) const
{
	time_t now_second = microsecond_time_ / MICROSECOND_PER_SECOND;
	if (g_last_second != now_second)
	{
		struct tm* now_tm = localtime(&now_second);
		g_time_buffer_len = strftime(g_time_buffer, sizeof g_time_buffer, "%Y-%m-%d %H:%M:%S", now_tm);
	}

	strcpy(buffer, g_time_buffer);
	int s_len = snprintf(buffer + g_time_buffer_len, len - g_time_buffer_len,
			".%06ld", microsecond_time_ % MICROSECOND_PER_SECOND);

	return g_time_buffer_len + s_len;
}

std::string TimeStamp::FormatToString() const
{
	char buffer[32];
	FormatToBuffer(buffer, sizeof buffer);
	return buffer;
}
