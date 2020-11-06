#pragma once
#include "main.h"
#include "Packet.h"
#include "PacketQueue.h"
#include "SocketLayer.h"

#ifdef WIN32 || WIN64
#define THREAD DWORD WINAPI
#define THREADHANDLE HANDLE
#else
#include <pthread.h>
#define WINAPI
#define LPVOID void*
#define THREAD	void *
#define THREADHANDLE	pthread_t
#endif

typedef void (*RecieveCallback)(Packet*);

class UdpTraficGuide
{
public:
	UdpTraficGuide() : m_RecieveQueue(new PacketQueue<OrderedQueue>), m_SendQueue(new PacketQueue<OrderedQueue>) {}
	void Initialize(const char* ip, unsigned short port, unsigned int timeout);
	void RegisterRecvCallback(RecieveCallback recv_callback);
	void Send(const char* ip, unsigned short port, ByteStream* bs);
	void CallbackRecieve(Packet* p);
	unsigned long GetDestAddr();
	SocketLayer* GetSocketLayerInstance();
	std::pair<bool*, bool*> GetThreadStates();
	SOCKET GetSocket();
	PacketQueue<OrderedQueue>* GetRecvQueue();
	PacketQueue<OrderedQueue>* GetSendQueue();
	~UdpTraficGuide();
private:
	PacketQueue<OrderedQueue>* m_RecieveQueue;
	PacketQueue<OrderedQueue>* m_SendQueue;
	RecieveCallback m_RecvCallback;
	SOCKET m_Socket;
	unsigned long m_ulAddr;
	SocketLayer* m_SockLayer;
	THREADHANDLE m_ListenThread;
	THREADHANDLE m_SendThread;
	bool m_bListenThread;
	bool m_bSendThread;
};

