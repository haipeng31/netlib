#ifndef DISPATCHER_H
#define DISPATCHER_H

#include <map>
#include <functional>
using std::function;
using std::bind;
using std::map;
namespace mynet {

class Dispatcher {
public:
	//typedef void *(*MessageCallback)(void *, const void *, int, int *);
	typedef function<void *(const void *, int, int *)> MessageCallback;
	void registerMsgCallback(int, MessageCallback);
	
	void *dispatchMessage(const void *, int, int *);
private:
	typedef map<int, MessageCallback> CallbackMap;
	CallbackMap messageCallbacks_;
};
}
#endif
