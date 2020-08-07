//
// Created by rjd67 on 2020/7/18.
//

#ifndef HIGAN_NONCOPYABLE_H
#define HIGAN_NONCOPYABLE_H

namespace higan
{
class noncopyable
{
public:

	noncopyable(const noncopyable&) = delete;

	void operator=(const noncopyable&) = delete;

protected:
	/**
	 * 防止外部直接构造noncopyable 设置为protected
	 */

	noncopyable() = default;

	~noncopyable() = default;

};
}
#endif //HIGAN_NONCOPYABLE_H
