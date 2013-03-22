#include "TestServer.h"
#include "EventLoop.h"
using namespace mynet;

int main()
{
	EventLoop loop;
	TestServer server(&loop, "testserver", "127.0.0.1", 5555);
	server.start();
	loop.loop();
	return 0;
}
