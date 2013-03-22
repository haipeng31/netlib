#include "TestServer.h"
#include "base/Logging.h"

using namespace mynet;

TestServer::TestServer(EventLoop *loop, const char *serverName, const char *ipAddr, int port)
 :	tcpServer_(loop, serverName, ipAddr, port),
	dispatcher_(),
	codec_(bind(&ProtobufDispatcher::onProtobufMsg, &dispatcher_, std::placeholders::_1, std::placeholders::_2))
{
	tcpServer_.setMessageCallback(bind(&ProtobufCodec::onMessage, &codec_, std::placeholders::_1));
	tcpServer_.setConnectionCallback(bind(&TestServer::onConnection, this, std::placeholders::_1));
	tcpServer_.setCloseCallback(bind(&TestServer::onClose, this, std::placeholders::_1));
	
	//CallbackT<TestRequest>::MessageCallback callback = bind(&TestServer::onTestRequest, this, std::placeholders::_1, std::placeholders::_2);
	//dispatcher_.registerCallback(callback);
	dispatcher_.registerCallback<TestRequest>(bind(&TestServer::onTestRequest, this, std::placeholders::_1, std::placeholders::_2));
	
//	tcpServer_.start();
}

void TestServer::start()
{
	tcpServer_.start();
}
void TestServer::onTestRequest(TestRequest *testRequest, shared_ptr<TcpConnection> connection)
{
	int num = testRequest->num();
	TestResponse *response = new TestResponse();;
	response->set_num(num);
	response->set_returnval(0);

	codec_.send(connection, MessagePtr(response));
}

void TestServer::onConnection(shared_ptr<TcpConnection>)
{
	LOG_TRACE << "connection up";
}

void TestServer::onClose(shared_ptr<TcpConnection>)
{
	LOG_TRACE << "connection down";
}
