#include "Dispatcher.h"
#include "base/Logging.h"
#include <cstdlib>
#include <cstring>
#include <assert.h>

using namespace mynet;

void Dispatcher::registerMsgCallback(int msgType, MessageCallback messageCallback)
{
	assert(messageCallbacks_.find(msgType) == messageCallbacks_.end());
	messageCallbacks_[msgType] = messageCallback;
}

void *Dispatcher::dispatchMessage(const void *msg, int msgLen, int *resMsgLen)
{
	int msgType = -1;
	memcpy(&msgType, msg, 4);
	LOG_INFO << msgType;
	msg = (void *)((char *)msg + 4);
	msgLen -= 4;
	LOG_INFO << (char *)msg;
	MessageCallback messageCallback = messageCallbacks_[msgType];

	return messageCallback(msg, msgLen, resMsgLen);
}
