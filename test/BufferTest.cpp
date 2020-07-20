//
// Created by rjd67 on 2020/7/20.
//

#include <string>
#include <fcntl.h>
#include <higan/Buffer.h>
#include <higan/utils/System.h>
#include <iostream>

int main()
{
	std::string in_file = "/root/jdk-8u261-windows-x64.exe";
	std::string out_file = in_file + ".copy";

	higan::System::RunShellCommand("rm", {"-f", out_file});

	int in_fd = open(in_file.c_str(), O_RDONLY);
	int out_fd = open(out_file.c_str(), O_WRONLY | O_CREAT);

	if (in_fd < 0 || out_fd < 0)
	{
		return -1;
	}

	higan::Buffer buffer;

	while (buffer.ReadFromFd(in_fd) > 0)
	{
		ssize_t write_size = write(out_fd, buffer.ReadBegin(), buffer.ReadableSize());
		buffer.AddReadIndex(write_size);
	}

	std::cout << higan::System::RunShellCommand("md5sum", {in_file});
	std::cout << higan::System::RunShellCommand("md5sum", {out_file});

	return 0;
}