//
// Created by rjd67 on 2020/7/22.
//

#include <higan/http/HttpContext.h>

int main()
{
	std::string request = "GET /hello.html HTTP/1.1\r\n"
						  "Connection: Keep-Alive\r\n\r\n";

	higan::HttpContext context;

	context.ParseRequest(request.c_str(), request.size());
}