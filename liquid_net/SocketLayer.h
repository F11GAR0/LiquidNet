#pragma once
#include "main.h"

#define INVALID_SOCKET (SOCKET)-1
#define DEFAULT_TIMEOUT 120

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
	template <class QueueType, class DebugBuffer = NullBuffer>
	bool Recieve(SOCKET s, unsigned short port, PacketQueue<QueueType, DebugBuffer>* OUT queue);
	void Send(SOCKET s, ByteStream* bs, unsigned int addr, unsigned short port);
};

template<class QueueType, class DebugBuffer>
inline bool SocketLayer::Recieve(SOCKET s, unsigned short port, PacketQueue<QueueType, DebugBuffer>* OUT queue)
{
    int len;
    socklen_t size = sizeof(sockaddr_in);
    char buf[128];
    sockaddr_in cli;
    cli.sin_family = AF_INET;
    cli.sin_addr.S_un.S_addr = INADDR_ANY;
    cli.sin_port = htons(port);
    len = recvfrom(s, buf, 128, 0, (sockaddr*)&cli, &size);
    if (len != -1) {
        ByteStream bs;
        bs.Write((unsigned char*)buf, len);
        Packet* p = new Packet(&bs, cli.sin_addr.S_un.S_addr, cli.sin_port);
        queue->Push(p);
        return true;
    }
    return false;
}
