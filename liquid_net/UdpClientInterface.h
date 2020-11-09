#pragma once
#include "main.h"
#include "Packet.h"
#include "PacketQueue.h"
#include "SocketLayer.h"
#include "UdpTraficGuide.h"

class UdpClientInterface
{
public:
	virtual bool Connect(const char* ip, unsigned int timeout) = 0;
	virtual void Send(ByteStream* bs, unsigned long addr, unsigned short remote_port) = 0;
protected:
	virtual ~UdpClientInterface() {}
};

