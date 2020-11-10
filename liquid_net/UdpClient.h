#pragma once
#include "main.h"
#include "UdpClientInterface.h"

class UdpClient : public UdpClientInterface, private UdpTraficGuide {
public:
	
	UdpClient(unsigned short port) : m_usPort(port) {}
	bool Connect(const char* ip, unsigned int timeout);
	void Send(ByteStream* bs, unsigned long addr = -1, unsigned short remote_port = -1);
	Packet* Recieve();
	void Listen(unsigned int timeout);
	virtual ~UdpClient();
private:
	RCAData m_ClientRCAData;
	unsigned short m_usPort;
};

