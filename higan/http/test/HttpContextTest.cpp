//
// Created by rjd67 on 2020/7/22.
//

#include <higan/http/HttpContext.h>
#include <higan/utils/System.h>

int no = 0;

void TestParse(higan::HttpContext* context, const std::string& request)
{
	context->Reset();

	size_t len = request.size();
	ssize_t result = context->ParseRequest(request.c_str(), len);

	if (result == -1)
	{
		printf("parse no: %d error\n", no);
	}
	else if (len != static_cast<size_t>(result))
	{
		printf("parse no: %d over\n", no);
	}

	no++;
}

int main()
{

	std::string request = "GET /hello.html HTTP/1.1\r\n"
						  "Connection: Keep-Alive\r\n\r\n"
						  "a=1&b=2&c=3";

	std::string request1 = "GET /hello.htmlHTTP/1.1\r\n"
						  "Connection: Keep-Alive\r\n\r\n"
						  "a=1&b=2&c=3";

	std::string request2 = "GET /hello.html HTTP/1.1\n"
						  "Connection: Keep-Alive\r\n"
						  "a=1&b=2&c=3";

	std::string request3 = "GET /hello.html HTTP/1.1\r\n"
						  "Connection: \r\nKeep-Alive\r\n\r\n"
						  "a=1&b=2&c=3";

	higan::HttpContext context;

	TestParse(&context, request);
	TestParse(&context, request1);
	TestParse(&context, request2);
	TestParse(&context, request3);
}