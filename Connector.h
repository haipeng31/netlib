#ifndef CONNECTOR_H
#define CONNECTOR_H

#include <functional>
#include <memory>

using std::function;
using std::bind;
using std::shared_ptr;
using std::unique_ptr;
namespace mynet {

class EventLoop;
class Channel;
class TcpConnection;

class Connector {
public:
	//typedef void (*Callback)(void *args, TcpConnection *);
	typedef function<void (shared_ptr<TcpConnection>)> ConnectionCallback;
	Connector(EventLoop *, const char *, int, bool = true);
	~Connector();

	void setConnectionCallback(ConnectionCallback);
	void start();
	void connecting();
	void retry();
	int fd();
private:
	void handleWrite();
	EventLoop *loop_;
	unique_ptr<Channel> channel_;
	int fd_;
	char peerAddr_[50];
	int peerPort_;
	bool connected_;
	bool retry_;
	size_t retryInterval_;
	const static size_t kMaxRetryInterval;
	ConnectionCallback connectionCallback_;
};
}
#endif
