#include "Channel.h"
#include <iostream>
#include <stdio.h>
#include "base/Logging.h"
using namespace mynet;

const int Channel::kReadEvent = 0x1;
const int Channel::kWriteEvent = 0x2;
const int Channel::kNoEvent = 0x0;


Channel::Channel(int fd)
  :	fd_(fd),
	events_(kNoEvent),
	revents_(kNoEvent)
{
}

void Channel::setEvent(int event)
{
	events_ |= event;
}

void Channel::dissetEvent(int event)
{
	events_ &= ~event;
}

int Channel::getEvent()
{
	return events_;
}

void Channel::setRevent(int revents)
{
	revents_ = revents;
}

void Channel::handleEvent()
{
	if (revents_ & kReadEvent) {
#ifdef TRACE
		//LOG_TRACE << "handle read";
#endif
		readCallback_();
	}
	if (revents_ & kWriteEvent) {
		//printf("handlewrite in Channel\n");
		writeCallback_();
	}
}

int Channel::fd()
{
	return fd_;
}

void Channel::setReadCallback(const Callback &readCallback)
{
	readCallback_ = readCallback;
}

void Channel::setWriteCallback(const Callback &writeCallback)
{
	writeCallback_ = writeCallback;
}

void Channel::setCloseCallback(const Callback &closeCallback)
{
	closeCallback_ = closeCallback;
}

void Channel::setErrorCallback(const Callback &errorCallback)
{
	errorCallback_ = errorCallback;
}

