#pragma once
#ifndef _MYUDPSOCKET_HPP_
#define _MYUDPSOCKET_HPP_

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Winsock2.h>
#include <windows.h>
#pragma comment(lib,"ws2_32.lib")  ///多平台应用，应加载属性中，link-》输入
#else
#include <unistd.h>
#include<arpa/inet.h>
#include <string.h>

#define SOCKET int
#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR            (-1)

#endif

#include <vector>
#include <string>
#include <thread>
#include <stdio.h>  
#include <Ws2tcpip.h>
#include <iostream>
#include <io.h>
#include <map>
#include "../Common/BaseApp.hpp"
#include "../Common/Log4c.hpp"
#include "../ProjectDomain/ProHeader.hpp"
using namespace std;

class MyUdpSocket
{
	SOCKET _sock;
    struct  sockaddr_in serAddr;
    int serAddrlen;
public:
	MyUdpSocket() {
		_sock = INVALID_SOCKET;
        serAddr.sin_family = AF_INET;
        serAddr.sin_port = htons(8888);
        serAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

        serAddrlen = sizeof(serAddr);
	}
    MyUdpSocket(char* servIp,int port) {
        _sock = INVALID_SOCKET;

        serAddr.sin_family = AF_INET;
        serAddr.sin_port = htons(port);
        serAddr.sin_addr.s_addr = inet_addr(servIp);

        serAddrlen = sizeof(serAddr);
    }
	virtual ~MyUdpSocket() {

		close();
	}

    /// <summary>
    /// initSocket
    /// </summary>
    /// <returns>0:ok, other: error</returns>
    int initSocket() {
        int iRet = -1;
        if (INVALID_SOCKET != _sock)
        {
            //close an old socket;
            close();
        }
#ifdef _WIN32
        //init WSA  
        WORD sockVersion = MAKEWORD(2, 2);
        WSADATA wsaData;
        iRet = WSAStartup(sockVersion, &wsaData);
        if (iRet != 0)
        {
            return iRet;
        }
#endif

        //1.create UPD socket
        _sock  = socket(AF_INET, SOCK_DGRAM, 0);
        if (INVALID_SOCKET == _sock)
        {
            //printf("Failed to create socket.\n");
            return _sock;
        }
        return 0;
    }

    /// <summary>
    /// Receive  data
    /// </summary>
    /// <returns>Received data count </returns>
    int recvData(void* recvBuf, int ibufLen) {
        memset(recvBuf, 0xff, ibufLen);
        int recved = 0;
        while (recved < ibufLen) {
            //get header ,get the data length
            int nRet = recvfrom(_sock, (char*)recvBuf + recved, ibufLen, 0, (SOCKADDR*)&serAddr, &serAddrlen);
            if (nRet == 0 || nRet == SOCKET_ERROR) {
                logger.error("Failed to receive data. \n");

                printf("Failed to receive data. \n");
                return 0;
            }
            recved += nRet;

        }
        return recved;
    }

    /// <summary>
    /// send data
    /// </summary>
    /// <param name="pSendData"></param>
    /// <returns></returns>
    int sendData(char* sendBuff, int sendLen) {

        if (_sock == INVALID_SOCKET) return 0;

        int  nRet = sendto(_sock, sendBuff, sendLen, 0, (struct sockaddr*)&serAddr, sizeof(serAddr));

        if (nRet != sendLen)
        {
            logger.error("Failed to send data. \n");
            return SOCKET_ERROR;
        }
        return nRet;
    }

    void close() {
        if (_sock != INVALID_SOCKET) {
#ifdef _WIN32
            closesocket(_sock);
            WSACleanup();
#else
            close(_sock);
#endif
            _sock = INVALID_SOCKET;
        }
    }



private:

};




#endif