//
// Created by rjd67 on 2020/7/25.
//

#ifndef HIGAN_HTTPRESPONSE_H
#define HIGAN_HTTPRESPONSE_H

namespace higan
{
class HttpResponse
{
public:
	explicit HttpResponse(bool keep_connection);
	~HttpResponse();


private:
};
}

#endif //HIGAN_HTTPRESPONSE_H
