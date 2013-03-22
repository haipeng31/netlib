#ifndef TCP_CONNECTION
#define TCP_CONNECTION

#include "Channel.h"
#include "Buffer.h"
#include <functional>
#include <memory>

using std::function;
using std::bind;
using std::shared_ptr;
using std::enable_shared_from_this;
namespace mynet {

class EventLoop;
class Channel;
class Buffer;

class TcpConnection : public enable_shared_from_this<TcpConnection> {
public:
	TcpConnection(int sockfd, EventLoop *, const char *peerAddr, int peerPort);
	~TcpConnection();
	//typedef void (*Callback)(void *, TcpConnection *);
	typedef function<void (shared_ptr<TcpConnection>)> Callback;
	void setMessageCallback(Callback);
	void setCloseCallback(Callback);
	void setErrorCallback(Callback);
	void setWriteCompleteCallback(Callback);
	void sendMsg(void *, int);
	Buffer *getInputBuf();
	Buffer *getOutputBuf();
	int fd();
	const char *peerAddr();
	int peerPort();
	void shutdown();
private:
	int fd_;
	int handleRead();
	void handleWrite();
	void handleClose();
	void handleError();
	EventLoop *loop_;
	Channel channel_;
	Buffer inputBuffer_;
	Buffer outputBuffer_;
	Callback messageCallback_;
	Callback closeCallback_;
	Callback errorCallback_;
	Callback writeCompleteCallback_;
	char peerAddr_[50];
	int peerPort_;
//	bool disconnecting_;
};

}
#endif
