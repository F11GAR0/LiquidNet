#pragma once
#include "main.h"
#include "UdpClientInterface.h"

class UdpClient : public UdpClientInterface {
public:
	UdpClient(unsigned short port) : m_usPort(port) {}
	bool Connect(const char* ip, unsigned int timeout);
	void Send(ByteStream* bs);
	virtual ~UdpClient();
private:
	unsigned short m_usPort;
};

