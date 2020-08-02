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
	std::string in_file = "/tmp/c088313f-7ad5-446c-bdef-522df94cde81.txt";
	higan::System::RunShellCommand("rm", {"-f", in_file});

	int in_fd = open(in_file.c_str(), O_WRONLY | O_CREAT);

	char random_buffer[1024];
	int file_size = 100; // 单位k
	for (int i = 0; i < file_size; ++i)
	{
		for (char& c : random_buffer)
		{
			c = static_cast<char>(random() % 26 + 65);
		}
		write(in_fd, random_buffer, sizeof random_buffer);
	}


	std::string out_file = in_file + ".copy";
	higan::System::RunShellCommand("rm", {"-f", out_file});

	int out_fd = open(out_file.c_str(), O_WRONLY | O_CREAT);
	close(in_fd);
	in_fd = open(in_file.c_str(), O_RDONLY);

	higan::Buffer buffer;
	while (buffer.ReadFromFd(in_fd) > 0)
	{
		ssize_t write_size = write(out_fd, buffer.ReadBegin(), buffer.ReadableSize());
		buffer.AddReadIndex(write_size);
	}

	std::cout << higan::System::RunShellCommand("md5sum", {in_file});
	std::cout << higan::System::RunShellCommand("md5sum", {out_file});

	close(in_fd);
	close(out_fd);

	return 0;
}