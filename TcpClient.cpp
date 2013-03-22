#include "TcpClient.h"
#include "EventLoop.h"
#include "TcpConnection.h"
#include "Connector.h"
#include "base/Logging.h"

#include <stdlib.h>
#include <string.h>
#include <assert.h>

using namespace mynet;

TcpClient::TcpClient(EventLoop *loop, const char *peerAddr, int peerPort, bool retry)
 :	loop_(loop),
	connector_(new Connector(loop, peerAddr, peerPort, retry)),
	connection_()
{
	fd_ = connector_->fd();
	retry_ = retry;
	connected_ = false;
	memset(peerAddr_, 0, sizeof(peerAddr_));
	strncpy(peerAddr_, peerAddr, strlen(peerAddr));
	peerPort_ = peerPort;
	
	LOG_INFO << "Client initialized";
}

TcpClient::~TcpClient()
{
}

void TcpClient::setConnectCallback(Callback connectCallback)
{
	connectCallback_ = connectCallback;
}

void TcpClient::setMessageCallback(Callback messageCallback)
{
	messageCallback_ = messageCallback;
}

void TcpClient::setCloseCallback(Callback closeCallback)
{
	closeCallback_ = closeCallback;
}

void TcpClient::setErrorCallback(Callback errorCallback)
{
	errorCallback_ = errorCallback;
}

void TcpClient::setWriteCompleteCallback(Callback writeCompleteCallback)
{
	writeCompleteCallback_ = writeCompleteCallback;
}

void TcpClient::start()
{
	assert(connected_ == false);
	connector_->setConnectionCallback(bind(&TcpClient::handleWrite, this, std::placeholders::_1));
	//connector_->setConnectionCallback([this](TcpConnection *con){ this->handleWrite(con);} );
	connector_->start();

	LOG_INFO << "Client Try to connect Server " << peerAddr_ << ":" << peerPort_;
}

void TcpClient::handleWrite(shared_ptr<TcpConnection> connection)
{
	connection_ = connection;
	connectCallback_(connection_);
	connected_ = true;
	LOG_INFO << "Successfully connect to server";

	connection_->setMessageCallback(messageCallback_);
	connection_->setErrorCallback(errorCallback_);
	connection_->setCloseCallback(bind(&TcpClient::handleClose, this, std::placeholders::_1));
	connection_->setWriteCompleteCallback(writeCompleteCallback_);
}

void TcpClient::handleClose(shared_ptr<TcpConnection> connection)
{
	assert(connected_ == true);
	connected_ = false;

	closeCallback_(connection);
	//connection_.reset();
	connector_->retry();
}


// to do list
// TcpConnection: add writeComplete callback, done
// TcpClient: add writeComplete callback
// testclient: finish the test code
