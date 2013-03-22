#ifndef CODEC_H
#define CODEC_H

#include "base/Mutex.h"
#include <functional>
#include <memory>
using std::function;
using std::bind;
using std::shared_ptr;

namespace mynet {

class TcpConnection;
class ThreadPool;

struct TaskFuncArg {
	TcpConnection *con;
	const void *msg;
	int length;
};

class Codec {
public:
//	typedef void *(*MessageCallback)(void *objArg, const void *, int, int *);
	typedef function<void *(const void *, int, int *)> MessageCallback;
	Codec(int lengthSize = kLengthSize, ThreadPool *threadPool = NULL);
	
	static void taskFunc(void *objArg, void *arg);	
	void setMessageCallback(MessageCallback messageCallback);
	void dealMsg(shared_ptr<TcpConnection> connection);

public:
	void *encode(void *, int *);
	void encode(void *data, int len);
	static const int kLengthSize;
	int lengthSize_;
	MessageCallback messageCallback_;

	ThreadPool *threadPool_;
	MutexLock sendLock_;
};
}
#endif
