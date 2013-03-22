#include "ProtobufCodec.h"
#include "base/Logging.h"
#include "TcpConnection.h"
#include <google/protobuf/descriptor.h>
#include <google/protobuf/message.h>


using namespace mynet;

ProtobufCodec::ProtobufCodec(const ProtobufMsgCb &protobufMsgCb)
 :	protobufMsgCb_(protobufMsgCb),
	errorCb_(defaultErrorCb)
{
}


ProtobufCodec::ProtobufCodec(const ProtobufMsgCb &protobufMsgCb, const ErrorCb &errorCb)
 :	protobufMsgCb_(protobufMsgCb),
	errorCb_(errorCb)
{
}

void ProtobufCodec::onMessage(const TcpConnectionPtr &con)
{
	Buffer *buffer = con->getInputBuf();
	int length = -1;
	while (buffer->avail() >= kLengthSize) {
		if (length == -1) {
			memcpy(&length, buffer->cStr(), kLengthSize);
			if (length < kMinMessageLen || length > kMaxMessageLen) {
				errorCb_(con, kInvalidLength);
			}
			LOG_TRACE << length;
		}
		
		LOG_TRACE << length + kLengthSize << buffer->avail();
		if (length + kLengthSize <= buffer->avail()) {
			ErrorCode error = kNoError;
			LOG_TRACE << "start parsing";
			MessagePtr message = parse(buffer->cStr() + 4, length, &error);
			LOG_TRACE << "finish parse";
			if (!error) {
				protobufMsgCb_(con, message);
			} else {
				errorCb_(con, error);
			}
			buffer->eraseBuf(length + kLengthSize);
			length = -1;
		} else {
			// do nothing
			LOG_TRACE << "breaking";
			break;
		}
	}
}

void ProtobufCodec::send(const TcpConnectionPtr & con, const MessagePtr &message) const
{
	Buffer buffer;
	string msgTypeName = message->GetTypeName();
	size_t nameLen = msgTypeName.size();
	size_t msgLen = sizeof(size_t) + nameLen + message->ByteSize();
	buffer.expandBuf(msgLen + sizeof(size_t));
	buffer.writeBuf(reinterpret_cast<const char *>(&msgLen), sizeof(size_t));
	buffer.writeBuf(reinterpret_cast<const char *>(&nameLen), sizeof(size_t));
	buffer.writeBuf(msgTypeName.c_str(), nameLen);
	message->SerializeToArray(reinterpret_cast<void *>(buffer.cStr() + buffer.avail()), message->ByteSize());
	buffer.updateBuf(message->ByteSize());
	con->sendMsg(buffer.cStr(), buffer.avail());
}

ProtobufCodec::MessagePtr ProtobufCodec::parse(const void *data, int dataLen, ErrorCode *error)
{
	size_t nameLen = -1;
	memcpy(&nameLen, data, sizeof(size_t));
	LOG_TRACE << nameLen;
	if (nameLen < 2 || (nameLen > dataLen - kLengthSize)) {
		*error = kInvalidNameLen;
		return NULL;
	}
	string msgTypeName((const char *)data + sizeof(size_t), nameLen);
	LOG_TRACE << msgTypeName;
	MessagePtr message = createMsg(msgTypeName);
	if (!message) {
		*error = kUnknownMessageType;
		return NULL;
	}
	if (!message->ParseFromArray((const char *)data + sizeof(size_t) + nameLen, dataLen - sizeof(size_t) - nameLen)) {
		*error = kParseError;
		return NULL;
	}

	return message;
}

ProtobufCodec::MessagePtr ProtobufCodec::createMsg(const string &msgTypeName)
{
	Message *message = NULL;
	const google::protobuf::Descriptor *descriptor = 
		google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(msgTypeName);
	if (descriptor) {
		const google::protobuf::Message *prototype = 
			google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
		if (prototype) {
			message = prototype->New();
		}
	}
	return MessagePtr(message);
}

namespace {
	const string kNoErrorStr = "NoError";
	const string kInvalidLengthStr = "InvalidLength";
	const string kInvalidNameLenStr = "InvalidNameLen";
	const string kUnknownMessageTypeStr = "UnknownMessageType";
	const string kParseErrorStr = "ParseError";
}

const string &ProtobufCodec::errorToStr(ErrorCode error)
{
	switch(error) {
	case kNoError:
		return kNoErrorStr;

	case kInvalidLength:
		return kInvalidLengthStr;

	case kInvalidNameLen:
		return kInvalidNameLenStr;

	case kUnknownMessageType:
		return kUnknownMessageTypeStr;

	case kParseError:
		return kParseErrorStr;
	}
}

void ProtobufCodec::defaultErrorCb(const TcpConnectionPtr &con, ErrorCode error)
{
	LOG_TRACE << "Connection from " << con->peerAddr() << ":"
		<< con->peerPort() << "send a wrong message with error " << errorToStr(error);
	con->shutdown();
}

