#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <functional>
#include <memory>
using std::function;
using std::bind;
using std::shared_ptr;
using std::unique_ptr;
//using std::placeholders;
namespace mynet {

class EventLoop;
class TcpConnection;
class Connector;

class TcpClient {
public:
	//typedef void (*Callback)(void *args, TcpConnection *);
	typedef function<void (shared_ptr<TcpConnection>)> Callback;
	TcpClient(EventLoop *, const char *, int, bool = true);
	~TcpClient();
	void setConnectCallback(Callback);
	void setMessageCallback(Callback);
	void setCloseCallback(Callback);
	void setErrorCallback(Callback);
	void setWriteCompleteCallback(Callback);
	void start();
private:
	void handleWrite(shared_ptr<TcpConnection>);
	void handleClose(shared_ptr<TcpConnection>);
	EventLoop *loop_;
	unique_ptr<Connector> connector_;
	shared_ptr<TcpConnection> connection_;
	int fd_;
	bool retry_;
	bool connected_;
	char peerAddr_[50];
	int peerPort_;

	Callback connectCallback_;
	Callback messageCallback_;
	Callback closeCallback_;
	Callback errorCallback_;
	Callback writeCompleteCallback_;
};

}
#endif
