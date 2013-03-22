#include "TestClient.h"
#include "EventLoop.h"

using namespace mynet;

int main()
{
	EventLoop loop;
	TestClient testClient(&loop, "127.0.0.1", 5555);
	loop.loop();
	return 0;
}
