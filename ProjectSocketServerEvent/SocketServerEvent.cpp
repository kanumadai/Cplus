#define WIN32_LEAN_AND_MEAN

#ifdef _WIN32
   #include <Winsock2.h>
   #include <windows.h>
#else
   #include <unistd.h>
   #include<arpa/inet.h>
#endif
#include <vector>
#include <string>
#include <thread>
#include <stdio.h>  
#include <Ws2tcpip.h>
#include <iostream>
#include <io.h>

#include <ComDef.h>

#include "../Common/BaseApp.hpp"
#include "../Common/MyTcpSocket.hpp"
using namespace std;
#pragma comment(lib,"ws2_32.lib")  ///多平台应用，应加载属性中，link-》输入



bool HandleData(const SOCKET& sockClient)
{
    // 5) 发数据


    char szBuff[100];
    static int count = 0;
    Sleep(5000);
    sprintf_s(szBuff, "Try to send ,NO. %d \r\n",count++);

    int   nRet = send(sockClient, szBuff, sizeof(szBuff), 0);
    if (nRet == SOCKET_ERROR)
    {
        printf("数据发送失败 \n");
        return false;
    }

    //recv
    char aryBuff[MAXWORD] = { 0 };
     nRet = recv(sockClient, aryBuff, sizeof(aryBuff), 0);
    if (nRet == SOCKET_ERROR)
    {
        printf("接受数据失败 \n");
       // return false;
    }
    if (nRet > 0) {
        printf("收到数据: %s \n", aryBuff);

    }

    return true;
}

//线程, 用来处理客户端, 和客户端进行数据的收发
void  HandleClientsThread(LPVOID pParam)
{
    //vector<SOCKET>& vctClients =*(vector<SOCKET>*)pParam;
    map<SOCKET, WSAEVENT>& vctClients = *(map<SOCKET, WSAEVENT>*)pParam;
    
    while (TRUE)
    {
        /*
        fd_set fdRead;
        FD_ZERO(&fdRead); //初始化
        */
        WSAEVENT aryEvents[WSA_MAXIMUM_WAIT_EVENTS];
        int nCount = 0;

        //把所有客户端加入数组

   /*     g_lock.Lock();*/

        if (vctClients.size() == 0) {
            continue;
        }

        for (auto& pairSockEvent : vctClients)
        {
            //FD_SET(sock, &fdRead);
            aryEvents[nCount++] = pairSockEvent.second;
        }
        //g_lock.UnLock();


        /*
        timeval tv = { 1, };
        int nRet = select(fdRead.fd_count,
            &fdRead,
            NULL,
            NULL,
            &tv);

        if (nRet == 0 || nRet==SOCKET_ERROR)
        {
            continue;
        }
        */
        //检测指定的socket
        //int nRet = WSAWaitForMultipleEvents(nCount, aryEvents, FALSE, 10000, FALSE);
        //if (nRet == WSA_WAIT_TIMEOUT) //超时继续等待
        //{
        //    continue;
        //}

        //处理数据

        //g_lock.Lock();
        for (auto itr = vctClients.begin(); itr != vctClients.end(); itr++)
        {
            if (!HandleData(itr->first))
            {
                //连接断开
                vctClients.erase(itr);
                break;
            }


            //判断sock是否可以读数据
            //if (FD_ISSET(*itr, &fdRead))
            //判断socket是否是可以读数据了
            //if (itr->second == aryEvents[nRet])
            //{
            //    WSANETWORKEVENTS workevent;
            //    WSAEnumNetworkEvents(itr->first, itr->second, &workevent);
            //    if (workevent.lNetworkEvents & FD_WRITE)
            //    {
            //        //if (itr->second == aryEvents[nRet]);
            //        if (!HandleData(itr->first))
            //        {
            //            //连接断开
            //            vctClients.erase(itr);
            //            break;
            //        }
            //    }
            //    else if (workevent.lNetworkEvents & FD_CLOSE)
            //    {
            //        vctClients.erase(itr);
            //    }
            //}
        }
        //g_lock.UnLock();
    }

}


int main()
{ 
    int iRet = 0;
    //1.创建socket
    MyTcpSocket mySock;
    logger.setLogger("..\\bin\\x64\\Log\\EventServer");

    iRet = mySock.initSocket();
    if (0 != iRet) {
        printf("Failed to create socket.\n");
    }
    //2.绑定端口

    iRet = mySock.bindSocket(9527);
    if (0 != iRet) {
        printf("Failed to bind socket\n");
    }


    //3.监听
    iRet = mySock.myListen();
    if (0 != iRet) {
        printf("Failed to listen. \n");
    }
    //创建线程，检测socket是否有数据可读并处理
    //vector<SOCKET> vctClients;
    map<SOCKET, WSAEVENT>vctClients;
    
    thread th(HandleClientsThread, (LPVOID)&vctClients);
    //th.join();
    //HANDLE hTread = CreateThread(NULL, 0, HandleClientsThread, (LPVOID)&vctClients, 0, NULL);
    //CloseHandle(hTread);

    while (true)
    {
        // 4) 接受连接
        SOCKET clientSock;
        sockaddr_in clientAddr;

        iRet = mySock.acceptClient(clientSock, clientAddr);
        if (0 != iRet) {
            printf("Failed to accept. \n");
        }
        char logStr[100] = {NULL};
        sprintf_s(logStr,"IP: %s port: %d link into server. \r\n",
            inet_ntoa(clientAddr.sin_addr),
            ntohs(clientAddr.sin_port));

        logger.info(logStr);

        //g_lock.Lock();
        //create an event bind to the client socket.
        WSAEVENT hEvent = WSACreateEvent();
        WSAEventSelect(clientSock, hEvent, FD_WRITE | FD_CLOSE);
        vctClients.insert(make_pair(clientSock, hEvent));
        //g_lock.UnLock();

    }

    mySock.close();
    return 0;
}