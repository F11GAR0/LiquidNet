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
	Packet* Copy();
	void SetData(ByteStream* bs);
	ByteStream& GetData();
	void SetPacketId(unsigned char packet_id);
	unsigned char GetPacketId() const;
	size_t GetSenderHash();
	void SetSenderInfo(unsigned long ip, unsigned short port);
	void SetPortAbsolute();
	bool IsPortAbsolute();
	std::pair<unsigned long, unsigned short> GetSenderInfo();
	~Packet();
private:
	unsigned char m_bPacketId;
	ByteStream m_bsData;
	unsigned long m_ulSender;
	unsigned short m_usSenderPort;
	bool m_bAbsolutePort;
};

