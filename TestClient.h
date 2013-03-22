#ifndef TESTCLIENT_H
#define TESTCLIENT_H

#include "TcpClient.h"
#include "ProtobufCodec.h"
#include "ProtobufDispatcher.h"
#include "Test.pb.h"
#include <functional>
#include <memory>
#include <google/protobuf/message.h>
#include "TcpConnection.h"
using std::shared_ptr;
using std::function;
using std::bind;
using google::protobuf::Message;

namespace mynet {
class TestClient {
public:
	typedef shared_ptr<Message> MessagePtr;
	typedef shared_ptr<TcpConnection> TcpConnectionPtr;
	TestClient(EventLoop *loop, const char *ipAddr, int port);
	
	void sendTestRequest(TcpConnectionPtr con);
	void onTestResponse(TestResponse *,shared_ptr<TcpConnection>);
	void onConnection(shared_ptr<TcpConnection>);
	void onClose(shared_ptr<TcpConnection>);
	void onError(shared_ptr<TcpConnection>);
private:
	TcpClient tcpClient_;
	ProtobufDispatcher dispatcher_;
	ProtobufCodec codec_;
};
}
#endif
