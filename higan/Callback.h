//
// Created by rjd67 on 2020/7/21.
//

#ifndef HIGAN_CALLBACK_H
#define HIGAN_CALLBACK_H

#include <functional>
#include <memory>
namespace higan
{

using std::placeholders::_1;
using std::placeholders::_2;
using std::placeholders::_3;

class Buffer;
class TcpConnection;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef std::function<void(const TcpConnectionPtr&)> TcpConnectionCallback;
typedef std::function<void(const TcpConnectionPtr&, Buffer&)> MessageCallback;
}
#endif //HIGAN_CALLBACK_H
