#ifndef TESTSERVER_H
#define TESTSERVER_H

#include "TcpServer.h"
#include "ProtobufCodec.h"
#include "ProtobufDispatcher.h"
#include "Test.pb.h"
#include <functional>
#include <memory>
#include <google/protobuf/message.h>
using std::shared_ptr;
using std::function;
using std::bind;
using google::protobuf::Message;

namespace mynet {
class TestServer {
public:
	typedef shared_ptr<Message> MessagePtr;
	TestServer(EventLoop *loop, const char *serverName, const char *ipAddr, int port);
	
	void start();
	void onTestRequest(TestRequest *,shared_ptr<TcpConnection>);
	void onConnection(shared_ptr<TcpConnection>);
	void onClose(shared_ptr<TcpConnection>);
private:
	TcpServer tcpServer_;
	ProtobufDispatcher dispatcher_;
	ProtobufCodec codec_;
};
}
#endif
