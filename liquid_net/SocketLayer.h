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
    Packet* Recieve(SOCKET s);
	void Send(SOCKET s, ByteStream* bs, unsigned long addr, unsigned short port, bool auto_htons = true);
private:
    unsigned short m_usPort;
};