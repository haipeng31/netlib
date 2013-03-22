#ifndef PROTOBUFDISPATCHER_H
#define PROTOBUFDISPATCHER_H

#include <functional>
#include <memory>
#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>
#include <map>
#include "TcpConnection.h"
using std::function;
using std::shared_ptr;
using google::protobuf::Message;
using google::protobuf::Descriptor;
using std::map;

namespace mynet {
class Callback {
public:
	typedef shared_ptr<Message> MessagePtr;
	typedef shared_ptr<TcpConnection> TcpConnectionPtr;
	virtual void onMessage(MessagePtr, TcpConnectionPtr) = 0;
	virtual ~Callback() { }
};

template <typename T>
class CallbackT : public Callback {
public:
	typedef function<void (T *, TcpConnectionPtr)> MessageCallback;
	CallbackT(const MessageCallback &messageCallback);
	void onMessage(MessagePtr ptr, TcpConnectionPtr);

private:
	MessageCallback messageCallback_;
};

template <typename T>
CallbackT<T>::CallbackT(const MessageCallback &messageCallback)
 :	messageCallback_(messageCallback)
{
}

template <typename T>
void CallbackT<T>::onMessage(MessagePtr ptr, TcpConnectionPtr con)
{
	T *message = dynamic_cast<T *>(ptr.get());
	messageCallback_(message, con);
}

class ProtobufDispatcher {
public:
//	template <typename T> 
//	typedef function<void (T *)> MessageCallback;
	typedef shared_ptr<TcpConnection> TcpConnectionPtr;
	typedef shared_ptr<Message> MessagePtr;

//	ProtobufDispatcher();

	template <typename T>
	void registerCallback(const typename CallbackT<T>::MessageCallback &);

	void onProtobufMsg(const TcpConnectionPtr &, const MessagePtr &);
private:
	map<const Descriptor *, Callback *> callbacks_;
};

template <typename T>
void ProtobufDispatcher::registerCallback(const typename CallbackT<T>::MessageCallback &cb)
{
	Callback *callback = new CallbackT<T>(cb);
	callbacks_[T::descriptor()] = callback;
}

}
#endif
