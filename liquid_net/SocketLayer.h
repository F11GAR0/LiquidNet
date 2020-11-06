#pragma once
#include "main.h"

#define INVALID_SOCKET (SOCKET)-1
#define DEFAULT_TIMEOUT 120

#ifdef WIN32 || WIN64
#include <Windows.h>
#include <stdio.h>
#include <time.h>
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
    ~SocketLayer();
	//if ip is null - socket will be listen
	SOCKET CreateSocket(unsigned short port, const char* ip, unsigned int timeout);
    static void CloseSocket(SOCKET sock);
	bool Connect(SOCKET s, const char* ip, unsigned short port);
	template <class QueueType, class DebugBuffer = NullBuffer>
	bool Recieve(SOCKET s, PacketQueue<QueueType, DebugBuffer>* OUT queue, unsigned short port = -1);
	void Send(SOCKET s, ByteStream* bs, unsigned int addr, unsigned short port);
private:
    unsigned short m_usPort;
};

template<class QueueType, class DebugBuffer>
inline bool SocketLayer::Recieve(SOCKET s, PacketQueue<QueueType, DebugBuffer>* OUT queue, unsigned short port)
{
    int len;
    socklen_t size = sizeof(sockaddr_in);
    char buf[512];
    sockaddr_in cli;
    cli.sin_family = AF_INET;
    len = recvfrom(s, buf, 512, 0, (sockaddr*)&cli, &size);
    if (len != -1) {
        ByteStream bs;
        bs.Write((unsigned char*)buf, len);
        Packet* p = new Packet(&bs, cli.sin_addr.S_un.S_addr, cli.sin_port);
        queue->Push(p);
        return true;
    }
    return false;
}
