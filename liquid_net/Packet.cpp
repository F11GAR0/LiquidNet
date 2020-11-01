#include "main.h"
#include "Packet.h"

Packet::Packet()
{
	m_usPacketId = 0;
	m_ulSender = 0;
	m_usSenderPort = 0;
	m_bsData = new ByteStream();
}

Packet::Packet(ByteStream* bs, unsigned long sender, unsigned short sender_port)
{
	bs->Read(m_usPacketId);
	unsigned int data_len = bs->GetLength() - sizeof(unsigned short);
	unsigned char* data = (unsigned char*)malloc(data_len);
	bs->Read(&data, data_len);
	bs->ResetReadPointer();
	m_bsData = new ByteStream();
	m_bsData->Write(data, data_len);
	SAFE_FREE(data);
	m_ulSender = sender;
	m_usSenderPort = sender_port;
}

Packet* Packet::Copy()
{
	Packet* ret = new Packet();
	
	ret->SetData(&m_bsData->Copy());
	ret->SetPacketId(this->m_usPacketId);
	ret->SetSenderInfo(this->m_ulSender, this->m_usSenderPort);

	return ret;
}

Packet::~Packet()
{
	SAFE_DELETE(m_bsData);
}

void Packet::SetData(ByteStream* bs)
{
	SAFE_DELETE(m_bsData);
	m_bsData = new ByteStream();
	unsigned int len = 0;
	unsigned char* data = bs->GetData(&len);
	m_bsData->Write(data, len);
}

ByteStream& Packet::GetData() const
{
	return m_bsData->Copy();
}

void Packet::SetPacketId(unsigned short packet_id)
{
	m_usPacketId = packet_id;
}

unsigned short Packet::GetPacketId() const
{
	return m_usPacketId;
}

void Packet::SetSenderInfo(unsigned long ip, unsigned short port)
{
	m_ulSender = ip;
	m_usSenderPort = port;
}
