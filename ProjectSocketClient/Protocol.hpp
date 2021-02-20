#include "..\Common\MyTcpSocket.hpp"
#include "..\Common\Log4c.hpp"


class MyProtocol
{
public:
	MyProtocol();
	~MyProtocol();

    bool onSelect(MyTcpSocket& mySock) {

        SOCKET* sock = (SOCKET*)mySock.getSocket();

        if (!mySock.isRun()) {
            return false;
        }

        fd_set fdReads;
        FD_ZERO(&fdReads);
        FD_SET(*sock, &fdReads);
        timeval t = { 1,0 };
        int ret = select(*sock + 1, &fdReads, 0, 0, &t);
        if (ret < 0) {
            return false;

        }
        if (FD_ISSET(*sock, &fdReads)) {
            FD_CLR(*sock, &fdReads);
            if (NULL == mySock.recvData()) {
                return false;
            }
        }
        return true;
    }


private:

};

MyProtocol::MyProtocol()
{
}

MyProtocol::~MyProtocol()
{
}