//
// Created by rjd67 on 2020/7/20.
//

#ifndef HIGAN_SYSTEM_H
#define HIGAN_SYSTEM_H

#include <string>

#define CHECK_IF_RETURN(check, result, ret)	\
do	\
{	\
	if ((check) == result)	\
	{	\
		return ret;	\
	}	\
}while(0)	\

namespace higan
{
class System
{
public:
	/**
	 * 运行shell命令 并获得输出
	 * @param command shell命令
	 * @param args 参数列表 如{"参数1", "参数2"}
	 * @return shell命令返回结果
	 */
	static std::string RunShellCommand(std::string command, std::initializer_list<std::string> args);
};
}
#endif //HIGAN_SYSTEM_H
