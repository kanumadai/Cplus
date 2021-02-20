#include <Winsock2.h>
#include <windows.h>
#include <vector>
#include <thread>
#include "../Common/MyTcpSocket.hpp"
#include "../Common/Log4c.hpp"
using namespace std;
#pragma comment(lib,"ws2_32.lib")  



bool HandleData(const SOCKET& sockClient)
{
    // 5) 收发数据
    Sleep(5000);
    char aryBuff[MAXWORD] = { 0 };
    int nRet = recv(sockClient, aryBuff, sizeof(aryBuff), 0);
    if (nRet == SOCKET_ERROR)
    {
        printf("接受数据失败 \n");
        return false;
    }
    if (nRet > 0) {

        printf("收到数据: %s \n", aryBuff);
    }

    char szBuff[] = { "recv OK \r\n" };
    nRet = send(sockClient, szBuff, sizeof(szBuff), 0);
    if (nRet == SOCKET_ERROR)
    {
        printf("数据发送失败 \n");
        return false;
    }

    return true;
}

//线程, 用来处理客户端, 和客户端进行数据的收发
DWORD WINAPI HandleClientsThread(const map<SOCKET, WSAEVENT>& vctClients)
{
    //vector<SOCKET>& vctClients =*(vector<SOCKET>*)pParam;
    //vector<pair<SOCKET, WSAEVENT>>& vctClients = *(vector<pair<SOCKET, WSAEVENT>>*)pParam;

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
        int nRet = WSAWaitForMultipleEvents(nCount, aryEvents, FALSE, 1000, FALSE);
        if (nRet == WSA_WAIT_TIMEOUT) //超时继续等待
        {
            continue;
        }

        //处理数据

        //g_lock.Lock();
        for (auto itr = vctClients.begin(); itr != vctClients.end(); itr++)
        {
            //判断sock是否可以读数据
            //if (FD_ISSET(*itr, &fdRead))
            //判断socket是否是可以读数据了
            if (itr->second == aryEvents[nRet])
            {
                WSANETWORKEVENTS workevent;
                WSAEnumNetworkEvents(itr->first, itr->second, &workevent);
                if (workevent.lNetworkEvents & FD_READ)
                {
                    //if (itr->second == aryEvents[nRet]);
                    if (!HandleData(itr->first))
                    {
                        //连接断开
                      //  vctClients.erase(itr);
                        break;
                    }
                }
                else if (workevent.lNetworkEvents & FD_CLOSE)
                {
                  //  vctClients.erase(itr);
                }
            }
        }
        //g_lock.UnLock();
    }

    return 0;
}


int main()
{
    int iRet = 0;
    //1.创建socket
    MyTcpSocket mySock;
    logger.setLogger("..\\bin\\x64\\Log\\EventClient");

    logger.info("hello.");

    iRet = mySock.initSocket();
    if (0 != iRet) {
        printf("Failed to create socket.\n");
    }
    //2.绑定端口

    iRet = mySock.connnectToServer((char*)"127.0.0.1",9527);
    if (0 != iRet) {
        printf("connect error.\n");
        return -1;
    }

    char sendData[100];
    char revData[255];
    //创建线程，检测socket是否有数据可读并处理
    //vector<SOCKET> vctClients;
    map<SOCKET, WSAEVENT>vctClients;
    SOCKET clientSock = mySock.getSocket();

    WSAEVENT hEvent = WSACreateEvent();
    WSAEventSelect(clientSock, hEvent, FD_READ | FD_CLOSE);
    vctClients.insert(make_pair(clientSock, hEvent));

    thread th(HandleClientsThread, vctClients);
    th.detach();

    while (1) {

    }

    closesocket(clientSock);
    mySock.close();
    return 0;
}