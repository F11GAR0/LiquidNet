#include "main.h"
#include "SocketLayer.h"

SocketLayer::SocketLayer()
{
	m_usPort = 0;
#ifdef WIN32 || WIN64
	WSADATA dat;
	WSAStartup(MAKEWORD(2, 2), &dat);
#endif
}

SocketLayer::~SocketLayer()
{
#ifdef WIN32 || WIN64
	WSACleanup();
#endif
}

SOCKET SocketLayer::CreateSocket(unsigned short port, const char* ip, unsigned int timeout = DEFAULT_TIMEOUT)
{
	SOCKET ret = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	unsigned long opt = 1;
	if (setsockopt(ret, SOL_SOCKET, SO_REUSEADDR, (char*)&opt, sizeof(long)) != 0) {
		throw new std::exception("Cant set sock opt reuseaddr");
        return INVALID_SOCKET;
	}

	opt = timeout;
	if (setsockopt(ret, SOL_SOCKET, SO_SNDTIMEO, (char*)&opt, sizeof(long)) != 0) {
		throw new std::exception("Cant set sock opt sndtimeo");
		return INVALID_SOCKET;
	}
	if (setsockopt(ret, SOL_SOCKET, SO_RCVTIMEO, (char*)&opt, sizeof(long)) != 0) {
		throw new std::exception("Cant set sock opt rcvtimeo");
		return INVALID_SOCKET;
	}
	unsigned long nonblocking = 1;
	if (ioctlsocket(ret, FIONBIO, &nonblocking) != 0)
	{
		throw new std::exception("Cant unblock socket");
		return INVALID_SOCKET;
	}

    sockaddr_in srv, cli;
	
    srv.sin_family = AF_INET;
    srv.sin_port = htons(port);
	m_usPort = port;

	if (ip && ip[0]) {
		srv.sin_addr.s_addr = inet_addr(ip);
	}
	else {
		srv.sin_addr.s_addr = INADDR_ANY;
		if (bind(ret, (sockaddr*)&srv, sizeof(sockaddr)) < 0) {
			throw new std::exception("Cant bind socket");
			return INVALID_SOCKET;
		}
	}
	
	return ret;
}

void SocketLayer::CloseSocket(SOCKET sock)
{
	closesocket(sock);
}

bool SocketLayer::Connect(SOCKET s, const char* ip, unsigned short port)
{
	sockaddr_in sock_addr;

	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(port);
	sock_addr.sin_addr.s_addr = inet_addr(ip);

	return !(connect(s, (struct sockaddr*)&sock_addr, sizeof(struct sockaddr)) != 0);
}

void SocketLayer::Send(SOCKET s, ByteStream* bs, unsigned int addr, unsigned short port)
{
	sockaddr_in sa;
	sa.sin_port = htons(port);
	sa.sin_addr.s_addr = addr;
	sa.sin_family = AF_INET;
	unsigned int len = 0;
	unsigned int res = 0;
	unsigned char* data = bs->GetData(&len);
	do
	{
		res = sendto(s, (char*)data, len, 0, (const sockaddr*)&sa, sizeof(struct sockaddr_in));
	} while (res == 0);
}
