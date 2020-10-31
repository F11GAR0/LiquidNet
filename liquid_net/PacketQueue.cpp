#include "main.h"
#include "PacketQueue.h"

void OrderedQueue::AddPacket(Packet* p)
{
	m_Queue.push_back(p);
}

Packet* OrderedQueue::TakePacket()
{
	Packet* p = *m_Queue.begin();
	m_Queue.pop_front();
	return p;
}

unsigned int OrderedQueue::GetCount()
{
	return m_Queue.size();
}
