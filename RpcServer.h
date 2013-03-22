#ifndef RPCSERVER_H
#define RPCSERVER_H

#include "Codec.h"
#include "Dispatcher.h"
#include "TcpServer.h"
#include "TcpConnection.h"
#include "base/ThreadPool.h"
#include <functional>
#include <memory>
using std::function;
using std::bind;
using std::shared_ptr;

namespace mynet {

/* using registerRpc to register rpc before calling start()
 * function
 */
class RpcServer {
public:
//	typedef void *(*MessageCallback)(void *, const void *, int, int *);
	typedef function<void *(const void *, int, int *)> MessageCallback;
	RpcServer(EventLoop *, const char *, const char *, int, int);
	~RpcServer();

	void start();
	void registerRpc(int, MessageCallback); 
private:
	void onConnection(shared_ptr<TcpConnection>);
	void onClose(shared_ptr<TcpConnection>);
	void onError(shared_ptr<TcpConnection>);
	
	ThreadPool threadPool_;
	TcpServer server_;
	Codec codec_;
	Dispatcher dispatcher_;
};
}
#endif
