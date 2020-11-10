#include "main.h"
#include "PacketQueue.h"

void OrderedQueue::AddPacket(Packet* p)
{
	m_Queue.push_back(p);
}

Packet* OrderedQueue::TakePacket()
{
	if (m_Queue.size() <= 0) return nullptr;
	Packet* p = m_Queue.front();
	m_Queue.pop_front();
	return p;
}

Packet* OrderedQueue::TakeBegin()
{
	return m_Queue.front();
}

void OrderedQueue::PopFront()
{
	m_Queue.pop_front();
}

unsigned int OrderedQueue::GetCount()
{
	return m_Queue.size();
}
