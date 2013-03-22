#include "TestClient.h"
#include "base/Logging.h"

using namespace mynet;

TestClient::TestClient(EventLoop *loop, const char *ipAddr, int port)
 :	tcpClient_(loop, ipAddr, port),
	dispatcher_(),
	codec_(bind(&ProtobufDispatcher::onProtobufMsg, &dispatcher_, std::placeholders::_1, std::placeholders::_2))
{
	tcpClient_.setMessageCallback(bind(&ProtobufCodec::onMessage, &codec_, std::placeholders::_1));
	tcpClient_.setConnectCallback(bind(&TestClient::onConnection, this, std::placeholders::_1));
	tcpClient_.setCloseCallback(bind(&TestClient::onClose, this, std::placeholders::_1));
	tcpClient_.setErrorCallback(bind(&TestClient::onError, this, std::placeholders::_1));
	//CallbackT<TestRequest>::MessageCallback callback = bind(&TestClient::onTestRequest, this, std::placeholders::_1, std::placeholders::_2);
	//dispatcher_.registerCallback(callback);
	dispatcher_.registerCallback<TestResponse>(bind(&TestClient::onTestResponse, this, std::placeholders::_1, std::placeholders::_2));
	
	tcpClient_.start();
}

void TestClient::sendTestRequest(TcpConnectionPtr con)
{
	TestRequest *testrequest = new TestRequest();
	testrequest->set_num(1);

	codec_.send(con, MessagePtr(testrequest));
}

void TestClient::onTestResponse(TestResponse *testResponse, shared_ptr<TcpConnection> connection)
{
	int num = testResponse->num();
	int returnval = testResponse->returnval();
	
	LOG_TRACE << "num = " << num << "returnval = " << returnval;
}

void TestClient::onConnection(shared_ptr<TcpConnection> con)
{
	LOG_TRACE << "connection up";
	sendTestRequest(con);
}

void TestClient::onClose(shared_ptr<TcpConnection>)
{
	LOG_TRACE << "connection down";
}

void TestClient::onError(shared_ptr<TcpConnection>)
{
	LOG_TRACE << "error";
}
