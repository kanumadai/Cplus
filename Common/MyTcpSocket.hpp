#pragma once
#ifndef _MYSOCKET_HPP_
#define _MYSOCKET_HPP_

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


class MyTcpSocket
{
    SOCKET _sock;

public:
    MyTcpSocket() {

        _sock = INVALID_SOCKET;


    }
    virtual ~MyTcpSocket() {
        close();
    }

    SOCKET getSocket() {
        return _sock;
    }

    void setSocket(SOCKET& sock) {
        _sock= sock;
    }

    bool isRun() {
        return INVALID_SOCKET != _sock;
    }

    // virtual void onRun() = 0;

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

        //1.create socket
        _sock = socket(AF_INET,
            SOCK_STREAM,
            IPPROTO_TCP
        );
        if (INVALID_SOCKET == _sock)
        {
            //printf("Failed to create socket.\n");
            return _sock;
        }
        return 0;
    }

    /// <summary>
    /// bind ip and port
    /// </summary>
    /// <returns>0:ok, other: error</returns>
    int bindSocket(unsigned short port) {

        int iRet = -1;
        sockaddr_in sockServer;
        sockServer.sin_family = AF_INET;
        sockServer.sin_port = htons(port);
        sockServer.sin_addr.S_un.S_addr = INADDR_ANY;
        iRet = bind(_sock, (sockaddr*)&sockServer, sizeof(sockServer));
        if (SOCKET_ERROR == iRet)
        {
            return iRet;
        }
        return 0;
    }


    /// <summary>
    /// connect to server
    /// </summary>
    /// <param name="ip">server's ip addr: "0.0.0.0"</param>
    /// <param name="port">server's port: 0000</param>
    /// <returns>0:ok, other: error</returns>
    int connnectToServer(char* ip, unsigned short port) {

        struct  sockaddr_in serAddr;
        serAddr.sin_family = AF_INET;
        serAddr.sin_port = htons(port);
#ifdef _WIN32
        serAddr.sin_addr.S_un.S_addr = inet_addr(ip);
#else
        serAddr.sin_addr.s_addr = inet_addr(ip);
#endif // _WIN32


        int serAddrlen = sizeof(serAddr);

        if (connect(_sock, (SOCKADDR*)&serAddr, sizeof(SOCKADDR)) == SOCKET_ERROR) {
            printf("connect error !");
            return SOCKET_ERROR;
        }

        return 0;
    }

    /// <summary>
    /// Listen 
    /// </summary>
    /// <param name="max">max socket links</param>
    /// <returns>0:ok, other: error</returns>
    int listen4Connect(int max = SOMAXCONN) {

        int nRet = listen(_sock, max);//监听最大值
        if (nRet == SOCKET_ERROR)
        {
            return nRet;
        }
        return 0;
    }

    int acceptClient(MyTcpSocket& clientSock, sockaddr_in& clientAdd) {
        int nSize = sizeof(clientAdd);
        SOCKET linkInSock;
        linkInSock = accept(_sock, (sockaddr*)&clientAdd, &nSize);
        if (linkInSock == SOCKET_ERROR)
        {
            return SOCKET_ERROR;
        }
        clientSock.setSocket(linkInSock);
        return 0;

    }
    
    /// <summary>
    /// Receive  data
    /// </summary>
    /// <returns>Received data count </returns>
    int recvData(void* recvBuf,int ibufLen) {
        memset(recvBuf, 0xff, ibufLen);
        char aryBuff[4096] = { 0 };
        int recved = 0;
        while (recved < ibufLen) {

            int nRet = recv(_sock, (char*)recvBuf+recved, ibufLen-recved, 0);

            if (nRet == 0 || nRet == SOCKET_ERROR) {
                LOG_ERROR("Failed to receive data. \n");

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

        int  nRet = send(_sock, sendBuff, sendLen, 0);
        if (nRet != sendLen)
        {
            LOG_ERROR("Failed to send data. \n");
            return SOCKET_ERROR;
        }
        return nRet;
    }



    /// <summary>
    /// close socket
    /// </summary>
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




};

#endif // !_MYSOCKET_HPP_