#include "TcpServer.h"
#include "EventLoop.h"
#include "Acceptor.h"
#include "TcpConnection.h"
#include "TcpServer.h"
#include "base/Logging.h"
#include <string.h>
#include <assert.h>

using namespace mynet;

TcpServer::TcpServer(EventLoop *loop, const char *name, const char *ipAddr, int port)
 :	loop_(loop),
	acceptor_(new Acceptor(loop, ipAddr, port))
{
	memset(name_, 0, 50);
	strncpy(name_, name, strlen(name));
	port_ = port;
	started_ = false;
	LOG_INFO << "server " << name_ << " initialized on port " << port_;
}

TcpServer::~TcpServer()
{
	connections_.clear();
}

void TcpServer::start()
{
	assert(started_ == false);
	started_ = true;

	acceptor_->setConnectionCallback(bind(&TcpServer::handleNewConnection, this, std::placeholders::_1));
	acceptor_->start();
	LOG_INFO << "server " << name_ << " started";
}

void TcpServer::setMessageCallback(MessageCallback messageCallback)
{
	messageCallback_ = messageCallback;
}

void TcpServer::setCloseCallback(CloseCallback closeCallback)
{
	closeCallback_ = closeCallback;
}


void TcpServer::handleClose(shared_ptr<TcpConnection> con)
{
	assert(connections_[con->fd()] == con);
	connections_.erase(con->fd());

	if (closeCallback_) {
		closeCallback_(con);
	}
	LOG_TRACE << "handle close";
	//delete con;
}
void TcpServer::setConnectionCallback(ConnectionCallback connectionCallback)
{
	connectionCallback_ = connectionCallback;
}



void TcpServer::handleNewConnection(shared_ptr<TcpConnection> connection)
{
	assert(connection != NULL);
	connection->setMessageCallback(messageCallback_);
	connection->setCloseCallback(bind(&TcpServer::handleClose, this, std::placeholders::_1));
	assert(connections_.find(connection->fd()) == connections_.end());
	connections_[connection->fd()] = connection;
	
	connectionCallback_(connection);
	//LOG_INFO << "New Connection from " << peerAddr << ":" << peerPort;
}

// to do list
// implement the func to deal with new connection establishing,
// using Acceptor to bind
// bind messageCallback in the implemented fucntion,
// maybe we should change the definiton of TcpConnection's messageCallback
