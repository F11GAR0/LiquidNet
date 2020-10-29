#pragma once
#include "main.h"

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
	//TODO: Socket layer realisation.
	//-SOCKET generator
	//-send func
	//-recv func
};

