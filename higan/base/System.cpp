//
// Created by rjd67 on 2020/7/20.
//

#include "higan/utils/System.h"

#include <utility>
#include <unistd.h>
#include <syscall.h>

using namespace higan;


std::string System::RunShellCommand(std::string command, std::initializer_list<std::string> args)
{
	std::string run_command = std::move(command);

	for (const std::string& arg : args)
	{
		run_command.append(" ").append(arg); // difference between operator+ and append
	}

	FILE* ret_stream = popen(run_command.c_str(), "r");

	char buff[1024];
	std::string result;
	while (fgets(buff, sizeof buff, ret_stream) != nullptr)
	{
		result.append(buff);
	}

	return result;
}



static thread_local pid_t g_thread_pid = 0;

pid_t System::GetTid()
{
	if (__builtin_expect(g_thread_pid == 0, 0))
	{
		g_thread_pid = syscall(SYS_gettid);
	}
	return g_thread_pid;
}

