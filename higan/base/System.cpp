//
// Created by rjd67 on 2020/7/20.
//

#include "higan/base/System.h"
#include "System.h"

#include <utility>
#include <unistd.h>
#include <syscall.h>
#include <cstdlib>
#include <sys/stat.h>
#include <csignal>
#include <fcntl.h>

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

bool System::MakeDirIfNotExist(const std::string& dir_path, bool loop_create)
{
	if (dir_path.empty())
	{
		return false;
	}

	int result = mkdir(dir_path.c_str(), S_IRWXU | S_IRGRP | S_IROTH);
	if (result == -1)
	{
		if (errno == EEXIST)
		{
			return true;
		}
		else if (errno == ENOENT)
		{
			if (!loop_create)
			{
				return false;
			}

			std::string parent_path = dir_path.substr(0, dir_path.find_last_of('/'));

			if (MakeDirIfNotExist(parent_path, false))
			{
				return MakeDirIfNotExist(dir_path, false);
			}
		}
		else
		{
			return false;
		}
	}

	return true;
}

void System::DaemonRun()
{
	signal(SIGCHLD, SIG_IGN);
	int pid = fork();
	if (pid <  0)
	{
		fprintf(stderr, "fork error\n");
		exit(-1);
	}
	else if (pid > 0)
	{
		exit(0);
	}

	setsid();
	int fd = open("/dev/null", O_RDWR, 0);
	if (fd != -1)
	{
		dup2(fd, STDIN_FILENO);
		dup2(fd, STDOUT_FILENO);
		dup2(fd, STDERR_FILENO);
	}
	if (fd > 2)
	{
		close(fd);
	}
}

