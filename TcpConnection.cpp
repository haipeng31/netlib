#include "TcpConnection.h"
#include "EventLoop.h"
#include "base/Logging.h"
#include "SockOps.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
using namespace mynet;

TcpConnection::TcpConnection(int sockfd, EventLoop *loop, const char *peerAddr, int peerPort)
 :  fd_(sockfd),
	channel_(sockfd),
	loop_(loop),
	inputBuffer_(),
	outputBuffer_()
{
	memset(peerAddr_, 0, sizeof(peerAddr_));
	strncpy(peerAddr_, peerAddr, strlen(peerAddr));
	peerPort_ = peerPort; 
	channel_.setEvent(Channel::kReadEvent);
	//channel_.setEvent(Channel::kWriteEvent);
	channel_.setReadCallback(bind(&TcpConnection::handleRead, this));
	//channel_.setWriteCallback(writeCallback);
	channel_.setErrorCallback(bind(&TcpConnection::handleError, this));
	LOG_INFO << "Add Channel with fd " << fd_;
	loop_->addChannel(&channel_);
	//disconnecting_ = false;
}

TcpConnection::~TcpConnection()
{
	::close(fd_);
//	loop_->removeChannel(&channel_);
	LOG_TRACE << "In des";
}

void TcpConnection::setMessageCallback(Callback messageCallback)
{
	messageCallback_ = messageCallback;
}

void TcpConnection::setCloseCallback(Callback closeCallback)
{
	closeCallback_ = closeCallback;
}

void TcpConnection::setErrorCallback(Callback errorCallback)
{
	errorCallback_ = errorCallback;
}

void TcpConnection::setWriteCompleteCallback(Callback writeCompleteCallback)
{
	writeCompleteCallback_ = writeCompleteCallback;
}

/* many thread update the eventloop, need lock, fix me */
void TcpConnection::sendMsg(void *data, int len)
{
	//LOG_TRACE << "outputBuffer avail = " << outputBuffer_.avail();
	outputBuffer_.writeBuf((char *)data, len);
	//free(data);
	//LOG_TRACE << "outputBuffer avail = " << outputBuffer_.avail();
	channel_.setEvent(Channel::kWriteEvent);
	channel_.setWriteCallback(bind(&TcpConnection::handleWrite, shared_from_this()));
	loop_->updateChannel(&channel_);
}

Buffer *TcpConnection::getInputBuf()
{
	return &inputBuffer_;
}

Buffer *TcpConnection::getOutputBuf()
{
	return &outputBuffer_;
}

int TcpConnection::fd()
{
	return fd_;
}
const char *TcpConnection::peerAddr()
{
	return peerAddr_;
}

int TcpConnection::peerPort()
{
	return peerPort_;
}

int TcpConnection::handleRead()
{
	int n = inputBuffer_.bufReadFd(fd_);
	if (n == 0) {
		handleClose();
	} else if (n > 0) {
		messageCallback_(shared_from_this());
	} else {
		// do nothing
		handleError();
	}

	return n;

}

void TcpConnection::handleWrite()
{
	outputBuffer_.showContent();
	//LOG_TRACE << "-----------------------------";
	//LOG_TRACE << "-----------------------------";
	//LOG_TRACE << outputBuffer_.avail();
	//LOG_TRACE << "-----------------------------";
	//LOG_TRACE << "-----------------------------";
	int n = outputBuffer_.bufWriteFd(fd_, outputBuffer_.avail());
	if (n <= 0) {
		LOG_ERROR << "socket write error";
	}
	//LOG_TRACE << "write " << n << "bytes";
	outputBuffer_.showContent();
	//LOG_TRACE << outputBuffer_.avail();
	

	if (outputBuffer_.avail() == 0) {
		if (writeCompleteCallback_) {
			writeCompleteCallback_(shared_from_this());
		}
//		if (!disconnecting_) {
			channel_.dissetEvent(Channel::kWriteEvent);
			loop_->updateChannel(&channel_);
//		}
	}
}

void TcpConnection::handleClose()
{
	//assert(disconnecting_ == false);
	loop_->removeChannel(&channel_);
	LOG_TRACE << "remove channel" << channel_.fd();;
//	::close(fd_);
	shared_ptr<TcpConnection> con = shared_from_this();
	closeCallback_(con);
//	disconnecting_ = true;
	LOG_TRACE << "handle close";
}

void TcpConnection::handleError()
{
	if (errorCallback_) {
		errorCallback_(shared_from_this());
	}
}

void TcpConnection::shutdown()
{
	handleClose();
}
