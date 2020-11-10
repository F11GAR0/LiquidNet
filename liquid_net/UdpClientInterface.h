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
	virtual void Listen(unsigned int timeout) = 0;
	//virtual void RegisterRecvCallback(RecieveCallback recv_callback) = 0;
	virtual Packet* Recieve() = 0;
	virtual void Send(ByteStream* bs, unsigned long addr, unsigned short remote_port) = 0;
protected:
	virtual ~UdpClientInterface() {}
};

