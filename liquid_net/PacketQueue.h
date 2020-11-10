#pragma once
#include "main.h"
#include <list>

using std::list;

class OrderedQueue {
public:
	void AddPacket(Packet* p);
	Packet* TakePacket();
	Packet* TakeBegin();
	void PopFront();
	unsigned int GetCount();
private:
	list<Packet*> m_Queue;
};

template <class Queue, class DebugBuffer = NullBuffer>
class PacketQueue : private Queue, private DebugBuffer
{
public:
	void Push(Packet* p);
	Packet Pop();
	void PopFront();
	Packet Begin();
	unsigned int GetCount();
private:
	using Queue::GetCount;
	using Queue::PopFront;
	using Queue::AddPacket;
	using Queue::TakePacket;
	using Queue::TakeBegin;
	using DebugBuffer::Send;
	using DebugBuffer::SendByteStream;
};

template<class Queue, class DebugBuffer>
inline Packet PacketQueue<Queue, DebugBuffer>::Pop()
{
	if (Queue::GetCount() > 0)
		return *Queue::TakePacket();
	else
		throw new std::exception("pop when queue is empty");
}

template<class Queue, class DebugBuffer>
inline void PacketQueue<Queue, DebugBuffer>::PopFront()
{
	Queue::PopFront();
}

template<class Queue, class DebugBuffer>
inline Packet PacketQueue<Queue, DebugBuffer>::Begin()
{
	return *TakeBegin();
}

template<class Queue, class DebugBuffer>
inline unsigned int PacketQueue<Queue, DebugBuffer>::GetCount()
{
	return Queue::GetCount();
}

template<class Queue, class DebugBuffer>
inline void PacketQueue<Queue, DebugBuffer>::Push(Packet* p)
{
	SendByteStream(&p->GetData());
	AddPacket(p);
}
