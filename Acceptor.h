#ifndef ACCEPTOR_H
#define ACCEPTOR_H

#include "Channel.h"
#include <functional>
#include <memory>

using std::function;
using std::bind;
using std::shared_ptr;
namespace mynet {

class TcpConnection;
class EventLoop;
class Acceptor {
public:
	// use tr1::function instead 
	//typedef void (*Callback)(void *, TcpConnection *);
	typedef function<void (shared_ptr<TcpConnection>)> ConnectionCallback;	
	Acceptor(EventLoop *, const char *, int);
	~Acceptor();
	void start();
	void setConnectionCallback(ConnectionCallback);
private:
	// we don't need this any more
	//static void readCallback(void *);
	void handleRead();
	char ipAddr_[20];
	int port_;
	EventLoop *loop_;
	int listenfd_;
	Channel channel_;
	ConnectionCallback connectionCallback_;
	// we don't need this any more
	//void *connectionArgs_;
	bool started_;
};
}
#endif
