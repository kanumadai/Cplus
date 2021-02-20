
#include<STDIO.H>
#include<iostream>
#include<cstring>
#include <Ws2tcpip.h>
#include <thread>
#include "../Common/Log4c.hpp"
#include "../Common/BaseApp.hpp"
#include "../ProjectDomain/ProHeader.hpp"
#include <map>
#include "../Common/MyTcpSocket.hpp"
using namespace std;
#pragma comment(lib, "ws2_32.lib")


///
/// 
bool onMessage(Header& packageHeader, char* packegeData) {

	return true;
}

void clientRecvThread(map<MyTcpSocket*, WSAEVENT>* vctClients) {

	MyTcpSocket* mySock = vctClients->begin()->first;
    WSAEVENT aryEvent = vctClients->begin()->second;

	while (TRUE)
	{
		int nCount = 0;

		//检测指定的socket
		int nRet = WSAWaitForMultipleEvents(nCount, &aryEvent, FALSE, 1000, FALSE);
		if (nRet == WSA_WAIT_TIMEOUT) //超时继续等待
		{
			continue;
		}

		//处理数据

		WSANETWORKEVENTS workevent;
		WSAEnumNetworkEvents(mySock->getSocket(), aryEvent, &workevent);
		if (workevent.lNetworkEvents & FD_READ)
		{
			Header packageHeader;
			int length = sizeof(Header);
			//get header ,get the data length
			int recvCount = mySock->recvData(&packageHeader, length);
			if (recvCount <= 0) {
				//error;
				continue;

			}
			int dataLen = packageHeader.dataLength;
			char* packegeData = new char[dataLen];

			//recv the last data,get one package;
			int tmpRecvLen = mySock->recvData(&packegeData, dataLen);
			if (tmpRecvLen != dataLen) {
				//error,received length is not equal request length
			}

			bool bRet = onMessage(packageHeader, packegeData);
			if (true != bRet) {
				//error
			}
			delete packegeData;

		}
		else if (workevent.lNetworkEvents & FD_CLOSE)
		{
			mySock->close();
			//  vctClients.erase(itr);

		}

		//g_lock.UnLock();
	}


}


int main()
{
	int iRet = 0;
	logger.setLogger("..\\bin\\x64\\Log\\TcpSocketClient");
	logger.setLogger("..\\bin\\x64\\Log\\TcpSocketClient2");
	//1.create socket
	MyTcpSocket mySock;

	LOG_INFO("hello.");

	iRet = mySock.initSocket();
	if (0 != iRet) {
		LOG_ERROR("Failed to create socket.\n");
	}
	//2.connect to server

	iRet = mySock.connnectToServer((char*)"127.0.0.1", 8888);
	if (0 != iRet) {
		LOG_ERROR("connect error.\n");
		return -1;
	}

	map<MyTcpSocket*, WSAEVENT>vctClients;
	SOCKET* clientSock =(SOCKET*) mySock.getSocket();

	WSAEVENT hEvent = WSACreateEvent();
	WSAEventSelect(*clientSock, hEvent, FD_READ | FD_CLOSE);
	vctClients.insert(make_pair(&mySock, hEvent));

	//thread  recv data
	thread recvTh(clientRecvThread, &vctClients);
	recvTh.detach();

	//client's process;
	while (mySock.isRun()) {
		//onRun();
	}
		
	mySock.close();
	return 0;

}

void sendMsg(const SOCKET& udpSocket, const sockaddr_in& recvAddr) {
	string inputdata;
	string testdata = "Client:";
	const char* sendData;
	while (true)
	{
		cin >> inputdata;
		sendData = (testdata + inputdata).c_str();   //string转const char* 
		sendto(udpSocket, sendData, strlen(sendData) + 1, 0, (struct sockaddr*)&recvAddr, sizeof(recvAddr));

	}

}
