#include "TcpBlockingClient.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
using std::cout;
using std::endl;
using namespace mynet;

#define  CNT 10000

double time_diff(struct timeval *start, struct timeval *end)
{
	int sec_dif = end->tv_sec - start->tv_sec;
	int usec_dif = end->tv_usec - start->tv_usec;

	if (usec_dif < 0) {
		usec_dif += 1000000;
		sec_dif--;
	}

	return sec_dif + (double)usec_dif / 1000000;
}

int main()
{
	TcpBlockingClient client("test", "127.0.0.1", 5555);
	client.start();
	char msg[4096];
	memset(msg, 'a', 4096);

	struct timeval start, end;
	gettimeofday(&start, NULL);
	for (int i = 0; i < CNT; i++) {
		client.sendMsg(msg, 4096);
		void *repMsg;
		int repMsgLen;
		repMsg = client.recvMsg(&repMsgLen);
		free(repMsg);
	}
	gettimeofday(&end, NULL);

	cout << "through %lf\n" << CNT * 4 * 8 / (1024 * time_diff(&start, &end)) << endl;
}
