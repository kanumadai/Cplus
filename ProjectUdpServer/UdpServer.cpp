#include <stdio.h>  
#include <winsock2.h>  
#include <Ws2tcpip.h>
#include <thread>
#include <iostream>
#include <io.h>
#include <cstring>
#include <string>
#include <vector>
using namespace std;
#pragma comment(lib,"ws2_32.lib")  

//临界区 线程同步
CRITICAL_SECTION g_section;

void sendMsgServ(const SOCKET& udpSocket, sockaddr_in recvAddr) {
    string inputdata;
    string testdata = "Server:";
    const char* sendData;
    while (true)
    {
        printf("Please input :\n");
        getline(cin, inputdata);
        sendData = (testdata + inputdata).c_str();   //string转const char* 
        sendto(udpSocket, sendData, strlen(sendData) + 1, 0, (struct sockaddr*)&recvAddr, sizeof(recvAddr));

    }

}

void myListen(const SOCKET& udpSocket) {


    sockaddr_in recvAddr;
    int nAddrlen = sizeof(recvAddr);
    char revData[255];

    string inputdata;
    string testdata = "Server:";
    const char* sendData;
    while (true)
    {

        EnterCriticalSection(&g_section);
        recvfrom(udpSocket, revData, 255,0, (SOCKADDR*)&recvAddr, &nAddrlen);

        printf("Client %s: %s\n", inet_ntoa(recvAddr.sin_addr),revData);
        if ('q' == revData[0]) {
            sendData = "q";
            sendto(udpSocket, sendData, strlen(sendData) + 1, 0, (struct sockaddr*)&recvAddr, sizeof(recvAddr));
           //continue;
           //break;
        }
        else {


            printf("Please input :\n");
            getline(cin, inputdata);
            sendData = inputdata.c_str();   //string转const char* 
            sendto(udpSocket, sendData, strlen(sendData) + 1, 0, (struct sockaddr*)&recvAddr, sizeof(recvAddr));

        }
     /*   thread thSendMsg(sendMsgServ,udpSocket, recvAddr);
        thSendMsg.join();*/
        LeaveCriticalSection(&g_section);
    }
    // closesocket(udpSocket);
}



int main(int argc, char* argv[])
{

    //初始化WSA  
    WORD sockVersion = MAKEWORD(2, 2);
    WSADATA wsaData;
    if (WSAStartup(sockVersion, &wsaData) != 0)
    {
        return 0;
    }

    cout << LOBYTE(wsaData.wVersion) << " " << HIBYTE(wsaData.wVersion) << endl;



    //创建套接字  
    SOCKET slisten = socket(AF_INET, SOCK_DGRAM, 0);
    if (slisten == INVALID_SOCKET)
    {
        printf("socket error !");
        return 0;
    }
   // 
   // HWND hwnd;
   //int ret= WSAAsyncSelect(slisten, NULL, 0, 0);

    //绑定IP和端口  
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(8888);
    sin.sin_addr.S_un.S_addr = INADDR_ANY;
    if (bind(slisten, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
    {
        printf("bind error !");
    }
    //初始化临界区
    InitializeCriticalSection(&g_section);
    //监听线程，接受数据线程
    thread thListen(myListen, slisten);
    thListen.join();

 /*   vector<SOCKET> vctClients;
    HANDLE hTread = CreateThread(NULL, 0, HandleClientsThread, (LPVOID)&vctClients, 0, NULL);
    CloseHandle(hTread);*/

    Sleep(50000); 
    //删除临界区
    DeleteCriticalSection(&g_section);
    closesocket(slisten);
    WSACleanup();
    return 0;
}

