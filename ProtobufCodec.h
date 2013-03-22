#ifndef PROTOBUFCODEC_H
#define PROTOBUFCODEC_H

#include <functional>
#include <memory>
#include <string>
using std::function;
using std::shared_ptr;
using std::string;

#include <google/protobuf/message.h>
using google::protobuf::Message;

namespace mynet {

class TcpConnection;

class ProtobufCodec {
public:
	enum ErrorCode {
		kNoError = 0,
		kInvalidLength = 1,
		kInvalidNameLen = 2,
		kUnknownMessageType = 3,
		kParseError = 4,
	};
	
	typedef shared_ptr<Message> MessagePtr;
	typedef shared_ptr<TcpConnection> TcpConnectionPtr;
	typedef function<void (const TcpConnectionPtr &, const MessagePtr &)> ProtobufMsgCb;
	typedef function<void (const TcpConnectionPtr &, ErrorCode)> ErrorCb;

	explicit ProtobufCodec(const ProtobufMsgCb &protobufMsgCb);
	ProtobufCodec(const ProtobufMsgCb &protobufMsgCb, const ErrorCb &errorCb);
	void onMessage(const TcpConnectionPtr &con);
	void send(const TcpConnectionPtr &con, const MessagePtr &message) const;
private:
	const static size_t kLengthSize = 4;
	const static size_t kMinMessageLen = sizeof(size_t);
	const static size_t kMaxMessageLen = 64 * 1024 * 1024;
	MessagePtr parse(const void *, int, ErrorCode *);
	MessagePtr createMsg(const string &msgTypeName);
	static const string &errorToStr(ErrorCode error);
	static void defaultErrorCb(const TcpConnectionPtr &, ErrorCode error);
	ProtobufMsgCb protobufMsgCb_;
	ErrorCb errorCb_;
};
}
#endif
