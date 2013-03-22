#ifndef CHANNEL_H
#define CHANNEL_H

#include <functional>
using std::function;
using std::bind;

namespace mynet {

class Channel {
public:
//	typedef void (*Callback)(void *);
	typedef function<void ()> Callback;
	static const int kReadEvent;
	static const int kWriteEvent;
	static const int kNoEvent;
	Channel(int fd);
	void setEvent(int);
	void dissetEvent(int);
	int getEvent();
	void setRevent(int);
	void handleEvent();
	void setReadCallback(const Callback &);
	void setWriteCallback(const Callback &);
	void setCloseCallback(const Callback &);
	void setErrorCallback(const Callback &);
	int fd();
private:
	int fd_;
	int events_;
	int revents_;
	Callback readCallback_;
	Callback writeCallback_;
	Callback closeCallback_;
	Callback errorCallback_;
};

}
#endif
