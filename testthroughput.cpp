#include <iostream>
#include "EventLoop.h"
#include "TcpConnection.h"
#include "TcpServer.h"
#include "base/ThreadPool.h"
#include "Codec.h"
#include <stdlib.h>
#include <string.h>
#include <functional>
#include <memory>

using namespace mynet;
using std::cout;
using std::endl;
using std::function;
using std::bind;
using std::shared_ptr;
size_t messageCnt = 0;

void *echoCallback(const void *msg, int msgLen, int *repMsgLen)
{
	char *repMsg = (char *)malloc(msgLen);
	int realLen = msgLen - 4;
	memcpy(repMsg, &realLen, 4);
	memcpy(repMsg+4, msg, msgLen-4);
	*repMsgLen = msgLen;
	messageCnt++;
	if (messageCnt % 1000 == 0) {
		cout << messageCnt << endl;
	}
	return repMsg;
}

void connectionCallback(shared_ptr<TcpConnection> con)
{
	cout << "Connection from " << con->peerAddr() << ":" << con->peerPort() << endl; 
}

void closeCallback(shared_ptr<TcpConnection> con)
{
	cout << "Connection from " << con->peerAddr() << ":" << con->peerPort() << "down" << endl;
}

int main()
{
	EventLoop loop;
//	ThreadPool threadPool(10);
	Codec codec(4, NULL);
	codec.setMessageCallback(echoCallback);
	TcpServer server(&loop, "EchoServer", "127.0.0.1", 5555);
	server.setMessageCallback(bind(&Codec::dealMsg, &codec, std::placeholders::_1));
	server.setConnectionCallback(connectionCallback);
	server.setCloseCallback(closeCallback);
	
	server.start();
	loop.loop();

	return 0;
}
