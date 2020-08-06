//
// Created by rjd67 on 2020/8/5.
//

#include <higan/http/HttpServer.h>
#include <signal.h>
#include "ElectricityBill.h"

std::string root = "/usr/local/web/df-5100/";

ElectricityBill* g_bill;

void OnHttpRequest(const higan::TcpConnectionPtr& conn, const higan::HttpRequest& request, higan::HttpResponse& response)
{
	if (request.GetMethod() != higan::HttpRequest::HTTP_REQUEST_GET)
	{
		response.SetStatusCode(higan::HttpResponse::STATUS_400_BAD_REQUEST);
		response.SetCloseConnection();
		return;
	}

	const std::string& url = request.GetUrl();

	if (url.length() > 15)
	{
		response.SetStatusCode(higan::HttpResponse::STATUS_400_BAD_REQUEST);
		response.AppendBody("为什么你的链接可以这么长?");
		response.SetCloseConnection();
		return;
	}

	if (url.find('?') != std::string::npos)
	{
		response.SetStatusCode(higan::HttpResponse::STATUS_400_BAD_REQUEST);
		response.AppendBody("你的链接带参数了... 估计是你使用的浏览器会自动传参吧");
		response.SetCloseConnection();
		return;
	}

	if (url == "/" || url == "/index.html")
	{
		response.SetStatusCode(higan::HttpResponse::STATUS_200_OK);
		response.SetFileToResponse(root + "index.html");
	}
	else
	{
		std::string path;
		if (g_bill->GetRoomFilePath(url, &path))
		{
			response.SetStatusCode(higan::HttpResponse::STATUS_200_OK);
			response.SetFileToResponse(path);
		}
		else
		{
			response.SetStatusCode(higan::HttpResponse::STATUS_400_BAD_REQUEST);
			response.AppendBody("不要访问一些奇奇怪怪的链接....");
			response.CloseConnection();
		}
	}
}

int main()
{
	signal(SIGPIPE, SIG_IGN);

	higan::EventLoop loop;

	ElectricityBill bill(&loop, root);
	g_bill = &bill;

	higan::InetAddress address{4000};

	higan::HttpServer server{&loop, address, "ElectricityBill"};
	server.SetHttpRequestCallback(OnHttpRequest);

	server.Start();
	loop.Loop();

	return 0;
}