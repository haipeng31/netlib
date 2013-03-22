OBJ = TestClient.o Test.pb.o  TestServer.o ProtobufCodec.o ProtobufDispatcher.o Timer.o TimerQueue.o TcpBlockingClient.o RpcClient.o EchoClient.o Codec.o Dispatcher.o RpcServer.o TcpClient.o Connector.o TcpServer.o Acceptor.o TcpConnection.o SockOps.o EventLoop.o Channel.o Poller.o EPoller.o Buffer.o
BASEOBJ = base/Logging.o base/ThreadPool.o

testprotoclient:$(OBJ) $(BASEOBJ) testprotoclient.o
	g++ $(OBJ) $(BASEOBJ) testprotoclient.o -lpthread -DTRACE -lrt -lprotobuf -o testprotoclient
testprotoserver:$(OBJ) $(BASEOBJ) testprotoserver.o
	g++ $(OBJ) $(BASEOBJ) testprotoserver.o -lpthread -DTRACE -lrt -lprotobuf -o testprotoserver
testblockingclient:$(OBJ) $(BASEOBJ) testblockingclient.o
	g++ $(OBJ) $(BASEOBJ) testblockingclient.o -lpthread -DTRACE -lrt -o testblockingclient
testthroughput:$(OBJ) $(BASEOBJ) testthroughput.o
	g++ $(OBJ) $(BASEOBJ) testthroughput.o -lpthread -DTRACE -lprotoc -lrt -lpthread -o testthroughput
testtimer:$(OBJ) $(BASEOBJ) testtimer.o
	g++ $(OBJ) $(BASEOBJ) testtimer.o -DTRACE -lrt -lpthread -o testtimer
testrpc:$(OBJ) $(BASEOBJ) testrpc.o
	g++ $(OBJ) $(BASEOBJ) testrpc.o -lpthread -o testrpc
testrpcclient:$(OBJ) $(BASEOBJ) testrpcclient.o
	g++ $(OBJ) $(BASEOBJ) testrpcclient.o -o testrpcclient
client:$(OBJ) $(BASEOBJ) testclient.o
	g++ $(OBJ) $(BASEOBJ) testclient.o -lpthread -DTRACE -lrt -o client
server: $(OBJ) $(BASEOBJ) testserver.o
	g++ $(OBJ) $(BASEOBJ) testserver.o -o server

testprotoclient.o:testprotoclient.cpp
	g++ --std=c++11 -c -g testprotoclient.cpp
testprotoserver.o:testprotoserver.cpp
	g++ --std=c++11 -c -g testprotoserver.cpp

testblockingclient.o:testblockingclient.cpp
	g++ --std=c++11 -c -g testblockingclient.cpp
testthroughput.o:testthroughput.cpp
	g++ --std=c++11 -c -g testthroughput.cpp

testsockops.o:testsockops.cpp
	g++ --std=c++11 -c -g testsockops.cpp

testsockopsclient.o:testsockopsclient.cpp
	g++ --std=c++11 -c -g testsockopsclient.cpp

testevent.o:testevent.cpp
	g++ --std=c++11 -c -g testevent.cpp

testrpcclient.o:testrpcclient.cpp
	g++ --std=c++11 -c -g testrpcclient.cpp

testtimer.o:testtimer.cpp
	g++ --std=c++11 -c -g testtimer.cpp
TestClient.o:TestClient.cpp
	g++ --std=c++11 -c -g TestClient.cpp
TestServer.o:TestServer.cpp
	g++ --std=c++11 -c -g TestServer.cpp
Test.pb.o:Test.pb.cc
	g++ --std=c++11 -c -g Test.pb.cc
ProtobufCodec.o:ProtobufCodec.cpp
	g++ --std=c++11 -c -g ProtobufCodec.cpp

ProtobufDispatcher.o:ProtobufDispatcher.cpp
	g++ --std=c++11 -c -g ProtobufDispatcher.cpp

Timer.o:Timer.cpp
	g++ --std=c++11 -c -g Timer.cpp

TimerQueue.o:TimerQueue.cpp
	g++ --std=c++11 -c -g TimerQueue.cpp

TcpBlockingClient.o:TcpBlockingClient.cpp
	g++ --std=c++11 -c -g TcpBlockingClient.cpp

RpcClient.o:RpcClient.cpp
	g++ --std=c++11 -c -g RpcClient.cpp

EchoClient.o:EchoClient.cpp
	g++ --std=c++11 -c -g EchoClient.cpp
testrpc.o:testrpc.cpp
	g++ --std=c++11 -c -g testrpc.cpp

Codec.o:Codec.cpp
	g++ --std=c++11 -c -g Codec.cpp

Dispatcher.o:Dispatcher.cpp
	g++ --std=c++11 -c -g Dispatcher.cpp

RpcServer.o:RpcServer.cpp
	g++ --std=c++11 -c -g RpcServer.cpp

testclient.o:testclient.cpp
	g++ --std=c++11 -c -g testclient.cpp

testserver.o:testserver.cpp
	g++ --std=c++11 -c -g testserver.cpp

TcpClient.o:TcpClient.cpp
	g++ --std=c++11 -c -g TcpClient.cpp

Connector.o:Connector.cpp
	g++ --std=c++11 -c -g Connector.cpp

TcpServer.o:TcpServer.cpp
	g++ --std=c++11 -c -g TcpServer.cpp
Acceptor.o:Acceptor.cpp
	g++ --std=c++11 -c -g Acceptor.cpp
TcpConnection.o:TcpConnection.cpp
	g++ --std=c++11 -c -g TcpConnection.cpp
SockOps.o:SockOps.cpp
	g++ --std=c++11 -c -g SockOps.cpp
testbuffer.o:testbuffer.cpp
	g++ --std=c++11 -c -g testbuffer.cpp

EventLoop.o:EventLoop.cpp
	g++ --std=c++11 -c -g EventLoop.cpp

Channel.o:Channel.cpp
	g++ --std=c++11 -c -g Channel.cpp

Poller.o:Poller.cpp
	g++ --std=c++11 -c -g Poller.cpp

EPoller.o:EPoller.cpp
	g++ --std=c++11 -c -g EPoller.cpp

Buffer.o:Buffer.cpp
	g++ --std=c++11 -c -g Buffer.cpp
clean:
	rm *.o
