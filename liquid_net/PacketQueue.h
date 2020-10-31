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

template <class Queue>
class PacketQueue : public Queue
{
public:
	void Push(Packet p);
	Packet Pop();
private:
	using Queue::AddPacket;
	using Queue::TakePacket;
};

template<class Queue>
inline void PacketQueue<Queue>::Push(Packet p)
{
	AddPacket(&p);
}

template<class Queue>
inline Packet PacketQueue<Queue>::Pop()
{
	return *TakePacket();
}
