#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <map>
#include <functional>
#include <memory>
using std::function;
using std::bind;
using std::map;
using std::shared_ptr;
using std::unique_ptr;
namespace mynet {

class EventLoop;
class Acceptor;
class TcpConnection;

class TcpServer {
public:
	//typedef void (*MessageCallback)(void *arg, TcpConnection *);
	//typedef void (*ConnectionCallback)(void *arg, TcpConnection *);
	//typedef void (*CloseCallback)(void *arg, TcpConnection *);
	typedef function<void (shared_ptr<TcpConnection>)> MessageCallback;
	typedef function<void (shared_ptr<TcpConnection>)> ConnectionCallback;
	typedef function<void (shared_ptr<TcpConnection>)> CloseCallback;
	TcpServer(EventLoop *loop, const char *, const char *, int);
	~TcpServer();
	void start();
	void setMessageCallback(MessageCallback);
	void setConnectionCallback(ConnectionCallback);
	void setCloseCallback(ConnectionCallback);
private:
	typedef map<int, shared_ptr<TcpConnection>> ConnectionMap;

	void handleNewConnection(shared_ptr<TcpConnection>);
	void handleClose(shared_ptr<TcpConnection>);
	EventLoop *loop_;
	unique_ptr<Acceptor> acceptor_;	
	ConnectionMap connections_;
	char name_[50];
	char ipAddr_[20];
	int port_;
	MessageCallback messageCallback_;
	ConnectionCallback connectionCallback_;
	CloseCallback closeCallback_;
	bool started_;
};
}
#endif
