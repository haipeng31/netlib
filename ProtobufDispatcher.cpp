#include "ProtobufDispatcher.h"

using namespace mynet;

void ProtobufDispatcher::onProtobufMsg(const TcpConnectionPtr & con, const MessagePtr &msg)
{
	const Descriptor *descriptor = msg->GetDescriptor();
	callbacks_[descriptor]->onMessage(msg, con);
}


