#include "main.h"
#include "Packet.h"

Packet::Packet()
{
	m_bPacketId = 0;
	m_ulSender = 0;
	m_usSenderPort = 0;
	m_bAbsolutePort = false;
}

Packet::Packet(ByteStream* bs, unsigned long sender, unsigned short sender_port)
{
	bs->Read(m_bPacketId);
	m_bAbsolutePort = false;
	unsigned int data_len = bs->GetLength() - sizeof(unsigned char);
	if (data_len > 0 && data_len != -1) {
		unsigned char* data = (unsigned char*)malloc(data_len);
		bs->Read(data, data_len);
		m_bsData.Write(data, data_len);
		SAFE_FREE(data);
	}
	bs->ResetReadPointer();
	m_ulSender = sender;
	m_usSenderPort = sender_port;
}

Packet* Packet::Copy()
{
	Packet* ret = new Packet();
	
	if (this->m_bAbsolutePort) ret->SetPortAbsolute();
	ret->SetData(&m_bsData.Copy());
	ret->SetPacketId(this->m_bPacketId);
	ret->SetSenderInfo(this->m_ulSender, this->m_usSenderPort);

	return ret;
}

Packet::~Packet()
{
//
}

void Packet::SetData(ByteStream* bs)
{
	m_bsData.Clear();
	unsigned int len = 0;
	unsigned char* data = bs->GetData(&len);
	m_bsData.Write(data, len);
}

ByteStream& Packet::GetData()
{
	return m_bsData;
}

void Packet::SetPacketId(unsigned char packet_id)
{
	m_bPacketId = packet_id;
}

unsigned char Packet::GetPacketId() const
{
	return m_bPacketId;
}

size_t Packet::GetSenderHash()
{
	return m_usSenderPort * (m_usSenderPort >> 2) % m_ulSender;
}

void Packet::SetSenderInfo(unsigned long ip, unsigned short port)
{
	m_ulSender = ip;
	m_usSenderPort = port;
}

void Packet::SetPortAbsolute()
{
	m_bAbsolutePort = true;
}

bool Packet::IsPortAbsolute()
{
	return m_bAbsolutePort;
}

std::pair<unsigned long, unsigned short> Packet::GetSenderInfo()
{
	return std::pair<unsigned long, unsigned short>(m_ulSender, m_usSenderPort);
}
