#include "EventLoop.h"
#include "TcpConnection.h"
#include "TcpClient.h"
#include "base/Logging.h"
#include <vector>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <memory>

using std::vector;
using std::cout;
using std::endl;
using std::cin;
using std::shared_ptr;
using namespace mynet;

#define MSGLEN 4096
static size_t cnt = 0;
static size_t messageCnt = 0;
static bool stopTest = false;
char repMsg[MSGLEN];

static int nClient, nsec;

void onConnection(shared_ptr<TcpConnection> con)
{
	// do nothing
	cout << "Sucessfully connect to server" << endl;
	cnt++;
	cout << "The number of client are " << cnt << endl;
	char msg[MSGLEN];
	memset(msg, 'a', MSGLEN);
	int len = MSGLEN - 4;
	memcpy(msg, &len, 4);
	con->sendMsg(msg, MSGLEN);
}

void onMessage(shared_ptr<TcpConnection> con)
{
	while (1) {
		Buffer *inputBuf = con->getInputBuf();

		if (inputBuf->avail() >= 4) {
			size_t length = 0;
			memcpy(&length, inputBuf->cStr(), 4);
			if (length + 4 <= inputBuf->avail()) {
			//	char *repMsg = (char *)malloc(length+4);
				inputBuf->readBuf(repMsg, length+4);

				messageCnt++;
				if (!stopTest) {
					con->sendMsg(repMsg, length+4);
				}
			//	free(repMsg);
			} else {
				break;
			}
		} else {
			break;
		}
	}

	if (stopTest) {
		cnt--;
		if (cnt == 0) {
			cout << "Finish test, nclient = " << nClient << " time = " << nsec << " messagecnt = " << messageCnt << endl;
			cout << "Finish test, throughput is " << (double)(messageCnt * 4) / (1024 * nsec) << endl;
		}
	}
}

void onClose(shared_ptr<TcpConnection> con)
{
	LOG_INFO << "client down";
	// do nothing
}

void onError(shared_ptr<TcpConnection> con)
{
	// do nothing
}

void onWriteComplete(shared_ptr<TcpConnection> con)
{
	char msg[50];
	memset(msg, 0, sizeof(msg));
	scanf("%s", msg);
	con->sendMsg(msg, strlen(msg));
}

void stopCallback()
{
	stopTest = true;
}

int main()
{
	EventLoop loop;
	vector<TcpClient *> clients;
	
	cout << "Please enter number of clients" << endl;
	cin >> nClient;
	cout << "please enter test time" << endl;
	cin >> nsec;

	for (int i = 0; i < nClient; i++) {
		TcpClient *client = new TcpClient(&loop, "127.0.0.1", 5555);

		client->setConnectCallback(onConnection);
		client->setMessageCallback(onMessage);
		client->setCloseCallback(onClose);
		client->setErrorCallback(onError);
//		client.setWriteCompleteCallback(onWriteComplete, NULL);
	
		client->start();
		clients.push_back(client);
	}

	struct timespec expire;
	expire.tv_sec = nsec;
	loop.runAfter(expire, stopCallback);
	loop.loop();
}


