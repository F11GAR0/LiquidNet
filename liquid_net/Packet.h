#pragma once
#include "main.h"
#include "ByteStream.h"

class Packet
{
public:
	//Initializate empty packet with 0 packet id and emply bytestream
	Packet();
	//Initializate packet from bytestream with packet id
	Packet(ByteStream* bs, unsigned long sender, unsigned short sender_port);
	void SetData(ByteStream* bs);
	ByteStream& GetData() const;
	void SetPacketId(unsigned short packet_id);
	unsigned short GetPacketId() const;
	~Packet();
private:
	unsigned short m_usPacketId;
	ByteStream* m_bsData;
	unsigned int m_uiSender;
	unsigned long m_ulSenderPort;
};

