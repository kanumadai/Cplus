#include <stdio.h>  
#include <winsock2.h>  
#include <Ws2tcpip.h>
#include <thread>
#include <iostream>
#include <io.h>
#include "../Common/MyTcpSocket.hpp"
#include "../Common/BaseApp.hpp"
using namespace std;
#pragma comment(lib,"ws2_32.lib")  


void myListen(MyTcpSocket* servSocket) {
    servSocket->listen4Connect(10);

    sockaddr_in recvAddr;
    char revData[255];
    while (true)
    {

        printf("Waitting for link in ... \n");
        MyTcpSocket clientSocket;
       int ret= servSocket->acceptClient(clientSocket,recvAddr);
       if (0 != ret) {
           //error
       }
        printf("Client %s is link in\n", inet_ntoa(recvAddr.sin_addr));

        string sendStr = "";
        char sendBuf[100];
        sprintf_s(sendBuf, "Welcome to Server: %s\n", inet_ntoa(recvAddr.sin_addr));

        ret = clientSocket.sendData( sendBuf, strlen(sendBuf));
        if (strlen(sendBuf) != ret) {
            //error
        }

        //thread clientTh(linkSocket);
        //clientTh.join();

        recv(linkSocket, revData, 255,0);

        printf("Client %s: %s\n", inet_ntoa(recvAddr.sin_addr),revData);



     closesocket(linkSocket);
    }
}

//void clientTh(const SOCKET& linkSocket) {
//    string inputdata;
//    string testdata = "Server:";
//    const char* sendData;
//    while (true)
//    {
//        cin >> inputdata;
//        sendData = (testdata + inputdata).c_str();   //string转const char* 
//        sendto(linkSocket, sendData, strlen(sendData) + 1, 0, (struct sockaddr*)&recvAddr, sizeof(recvAddr));
//
//    }
//   
//}
int main(int argc, char* argv[])
{
    int iRet = 0;
    logger.setLogger("..\\bin\\x64\\Log\\TcpSocketServer");
    //1.create socket
    MyTcpSocket mySock;

    logger.info("TcpSocketServer.");

    iRet = mySock.initSocket();
    if (0 != iRet) {
        printf("Failed to create socket.\n");
    }
    //2.bind,ip:INADDR_ANY,port,8888
    iRet = mySock.bindSocket(8888);
    if (0 != iRet) {
        printf("Failed to bind socket.\n");
    }


    //listen net
    thread thListen(myListen, &mySock);

    thListen.join();


    mySock.close();
    return 0;
}

