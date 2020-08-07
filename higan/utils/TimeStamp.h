//
// Created by rjd67 on 2020/8/5.
//

#ifndef HIGAN_TIMESTAMP_H
#define HIGAN_TIMESTAMP_H

#include <sys/time.h>
#include <cstdint>
#include <string>

namespace higan
{
class TimeStamp
{
public:
	/**
	 * 微秒每秒
	 */
	const static int64_t MICROSECOND_PER_SECOND;

	/**
	 * 微妙每毫秒
	 */
	const static int64_t MICROSECOND_PER_MILLISECOND;

	/**
	 * 默认时间戳 时间为0
	 */
	TimeStamp();

	/**
	 * 创建TimeStamp
	 * @param micro_second 微秒数
	 */
	explicit TimeStamp(int64_t micro_second);
	~TimeStamp();

	bool operator<(const TimeStamp& stamp) const;
	bool operator<=(const TimeStamp& stamp) const;
	TimeStamp& operator-(const TimeStamp& stamp);

	/**
	 * 获取当前时间对应的微秒
	 * @return 获取当前时间对应的微秒
	 */
	static int64_t Now();

	/**
	 * 时间戳时间单位转换为微秒
	 * @return 返回时间戳时间单位转换为微秒
	 */
	int64_t GetMicroSecond() const;

	/**
	 * 时间戳时间单位转换为毫秒
	 * @return 返回时间戳时间单位转换为毫秒
	 */
	int64_t GetMilliSecond() const;

	/**
	 * 增加时间戳的时间
	 * @param millisecond 单位毫秒
	 */
	void AddMillisecond(int millisecond);

	void SetMicrosecond(int64_t microsecond);

	size_t FormatToBuffer(char* buffer, size_t len) const;
	std::string FormatToString() const;

private:

	int64_t microsecond_time_;
};
}

#endif //HIGAN_TIMESTAMP_H
