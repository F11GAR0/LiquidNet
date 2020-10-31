#pragma once
#include "main.h"
#include "Packet.h"
#include "PacketQueue.h"
#include "SocketLayer.h"

class UdpClientInterface
{
public:
	virtual bool Connect(const char* ip, unsigned int timeout) = 0;
	virtual void Send(ByteStream* bs) = 0;
protected:
	virtual ~UdpClientInterface() {}
};

