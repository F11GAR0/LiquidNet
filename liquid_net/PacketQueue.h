#pragma once
#include "main.h"
#include <list>

using std::list;

class OrderedQueue {
public:
	void AddPacket(Packet* p);
	Packet* TakePacket();
	unsigned int GetCount();
private:
	list<Packet*> m_Queue;
};

template <class Queue, class DebugBuffer = NullBuffer>
class PacketQueue : public Queue, public DebugBuffer
{
public:
	void Push(Packet* p);
	Packet Pop();
private:
	using Queue::AddPacket;
	using Queue::TakePacket;
	using DebugBuffer::Send;
	using DebugBuffer::SendByteStream;
};

template<class Queue, class DebugBuffer>
inline Packet PacketQueue<Queue, DebugBuffer>::Pop()
{
	return *TakePacket();
}

template<class Queue, class DebugBuffer>
inline void PacketQueue<Queue, DebugBuffer>::Push(Packet* p)
{
	SendByteStream(&p->GetData());
	AddPacket(p);
}
