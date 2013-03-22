#include "RpcServer.h"
#include "base/Logging.h"

using namespace mynet;

RpcServer::RpcServer(EventLoop *loop, const char *name, const char *ipAddr, int port, int nthread)
 :  threadPool_(nthread),
	server_(loop, name, ipAddr, port),
	codec_(4, &threadPool_),
	dispatcher_()
{
	LOG_INFO << "RPC SERVER " << name << " initialized";
}

RpcServer::~RpcServer()
{
}

void RpcServer::start()
{
	threadPool_.start();
	codec_.setMessageCallback(bind(&Dispatcher::dispatchMessage, &dispatcher_, std::placeholders::_1,
					std::placeholders::_2, std::placeholders::_3));
	server_.setMessageCallback(bind(&Codec::dealMsg, &codec_, std::placeholders::_1));
	server_.setConnectionCallback(bind(&RpcServer::onConnection, this, std::placeholders::_1));
	server_.setCloseCallback(bind(&RpcServer::onClose, this, std::placeholders::_1));
	//server_.setErrorCallback(onError, NULL);

	server_.start();
}

void RpcServer::registerRpc(int messageType, MessageCallback messageCallback)
{
	dispatcher_.registerMsgCallback(messageType, messageCallback);
}

void RpcServer::onConnection(shared_ptr<TcpConnection> con)
{
	LOG_INFO << "New connection from " << con->peerAddr() << ":" << con->peerPort();
}

void RpcServer::onClose(shared_ptr<TcpConnection> con)
{
	LOG_INFO << "Connection " << con->peerAddr() << ":" << con->peerPort() << " down";
}

void RpcServer::onError(shared_ptr<TcpConnection> con)
{
	LOG_INFO << "Connection " << con->peerAddr() << ":" << con->peerPort() << " error";
}
