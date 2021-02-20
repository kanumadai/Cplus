#include <stdio.h>
#include<iostream>
#include <Ws2tcpip.h>
#include <thread>
#include <cstring>
#include <string>
#include <map>
using namespace std;
#pragma comment(lib, "ws2_32.lib")

void sendMsg(const SOCKET& udpSocket, const sockaddr_in& recvAddr) {
	string inputdata;
	string testdata = "Client:";
	const char* sendData;
	while (true)
	{
		printf("Please input :\n");
		cin >> inputdata;
		sendData = (testdata + inputdata).c_str();   //string转const char* 
		sendto(udpSocket, sendData, strlen(sendData) + 1, 0, (struct sockaddr*)&recvAddr, sizeof(recvAddr));

	}

}


int main()
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return 0;
	}
	//while (true) {
		SOCKET sclient = socket(AF_INET, SOCK_DGRAM, 0);
		if (sclient == INVALID_SOCKET)
		{
			printf("invalid socket!");
			return 0;
		}

		
		struct  sockaddr_in serAddr;
		serAddr.sin_family = AF_INET;
		serAddr.sin_port = htons(8888);
		serAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

		map<SOCKET, WSAEVENT>vctClients;
		WSAEVENT hEvent = WSACreateEvent();
		WSAEventSelect(sclient, hEvent, FD_WRITE | FD_CLOSE);
		vctClients.insert(make_pair(sclient, hEvent));

		int serAddrlen = sizeof(serAddr);
		short recvLen;
		char recvLenData[2];
		char *revData;
		string inputdata;
		string testdata = "Client:";
		const char* sendData;
		int dataLen;
		int nCount = 0;

		while (true)
		{
			
			//检测指定的socket
			int nRet = WSAWaitForMultipleEvents(nCount, &hEvent, FALSE, 10, FALSE);
			if (nRet == WSA_WAIT_TIMEOUT) //超时继续等待
			{
				continue;
			}

			//get header ,get the data length
			dataLen= recvfrom(sclient, recvLenData, 2, 0, (SOCKADDR*)&serAddr, &serAddrlen);
			if (dataLen <= 0) {
				//error;
				continue;

			}
			revData = new char[dataLen];

			//recv the last data,get one package;
			int tmpRecvLen=0;
			while (dataLen - tmpRecvLen > 0) {
				tmpRecvLen = recvfrom(sclient, &revData[tmpRecvLen], dataLen, 0, (SOCKADDR*)&serAddr, &serAddrlen);
				//data is not all received.
				dataLen -= tmpRecvLen;

			}


			//printf("Please input :\n");
			//getline(cin,inputdata);
			//sendData = inputdata.c_str();   //string转const char* 
			//sendto(sclient, sendData, strlen(sendData) + 1, 0, (struct sockaddr*)&serAddr, sizeof(serAddr));

			//recvfrom(sclient, revData, 255, 0, (SOCKADDR*)&serAddr, &serAddrlen);
			//if ('q' == revData[0]) {
			//	sendData = "q";
			//	sendto(sclient, sendData, strlen(sendData) + 1, 0, (struct sockaddr*)&serAddr, sizeof(serAddr));

			//	break;
			//}
			//printf("Server %s: %s\n", inet_ntoa(serAddr.sin_addr), revData);


			//thread thSendMsgClient(sendMsg, sclient, serAddr);
			//thSendMsgClient.join();

		}

		closesocket(sclient);
	//	break;
	//}

	WSACleanup();
	return 0;

}
