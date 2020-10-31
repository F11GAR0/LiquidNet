#pragma once
#include "main.h"

#define INVALID_SOCKET (SOCKET)-1

#ifdef WIN32 || WIN64
#include <WinSock2.h>
#include <stdio.h>
#include <time.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <winsock.h>
#include <stdlib.h>
#pragma comment(lib, "ws2_32.lib")

typedef int socklen_t;
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

typedef int SOCKET;
#define closesocket(x) close(x)
#endif

class SocketLayer
{
public:
    SocketLayer();
	//if ip is null - socket will be listen
	SOCKET CreateSocket(unsigned short port, const char* ip, unsigned int timeout);
	bool Connect(SOCKET s, const char* ip, unsigned short port);
	template <class QueueType>
	bool Recieve(SOCKET s, PacketQueue<QueueType>* OUT queue);
	void Send(SOCKET s, ByteStream* bs, unsigned int addr, unsigned short port);
};

template<class QueueType>
inline bool SocketLayer::Recieve(SOCKET s, PacketQueue<QueueType>* OUT queue)
{
    int len;
    socklen_t size;
    char buf[128];
    sockaddr_in cli;
    len = recvfrom(s, buf, sizeof(buf), 0, (sockaddr*)&cli, &size);
    if (len != -1) {
        ByteStream bs;
        bs.Write(buf, len);
        queue->Push(Packet(&bs, cli.sin_addr.S_un.S_addr, cli.sin_port));
        return true;
    }
    return false;
}
