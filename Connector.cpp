#include "Connector.h"
#include "base/Logging.h"
#include "Channel.h"
#include "EventLoop.h"
#include "TcpConnection.h"
#include "SockOps.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
using namespace mynet;

const size_t Connector::kMaxRetryInterval = 8;
Connector::Connector(EventLoop *loop, const char *peerAddr, int peerPort, bool retry)
 :	loop_(loop),
	fd_(-1),
	channel_()
{
	memset(peerAddr_, 0, sizeof(peerAddr_));
	strncpy(peerAddr_, peerAddr, strlen(peerAddr));
	peerPort_ = peerPort;

	connected_ = false;
	retry_ = retry;
	retryInterval_ = 2;
}

Connector::~Connector()
{
}

void Connector::setConnectionCallback(ConnectionCallback connectionCallback)
{
	connectionCallback_ = connectionCallback;
}

void Connector::start()
{
	assert(connected_ == false);
	fd_ = socket();
	channel_.reset(new Channel(fd_));
	int n = connect(fd_, peerAddr_, peerPort_);
	
	if (n == 0) {
		shared_ptr<TcpConnection> connection(new TcpConnection(fd_, loop_, peerAddr_, peerPort_));
		connected_ = true;
		connectionCallback_(connection);
	} else {
		int saveErrNo = errno;
		switch (saveErrNo) {
			case EINPROGRESS:
			case EINTR:
			case EISCONN:
				connecting();
				break;

			case EAGAIN:
			case EADDRINUSE:
			case EADDRNOTAVAIL:
			case ECONNREFUSED:
			case ENETUNREACH:
				retry();
				break;

			default:
				LOG_ERROR << "connect error" << saveErrNo;
				::close(fd_);
		}
	}
}

void Connector::connecting()
{
	assert(connected_ == false);
	channel_->setEvent(Channel::kWriteEvent);
	channel_->setWriteCallback(bind(&Connector::handleWrite, this));
	loop_->addChannel(channel_.get());
	LOG_TRACE << "I'm connecting";
}

void Connector::retry()
{
	//channel_.reset();
	connected_ = false;
	struct timespec expire;
	expire.tv_sec = (retryInterval_ < kMaxRetryInterval ? retryInterval_ : kMaxRetryInterval);
	expire.tv_nsec = 0;
	loop_->runAfter(expire, bind(&Connector::start, this));
	LOG_TRACE << "Retry after " << expire.tv_sec;
	retryInterval_ *= 2;
}
int Connector::fd()
{
	return fd_;
}
/* fix me
 * need to deal with the situation that connect fail
 */
void Connector::handleWrite()
{
	LOG_TRACE << "writable in connector";
	loop_->removeChannel(channel_.get());
	shared_ptr<TcpConnection> connection(new TcpConnection(fd_, loop_, peerAddr_, peerPort_));
	connected_ = true;
	retryInterval_ = 2;
	connectionCallback_(connection);
}
