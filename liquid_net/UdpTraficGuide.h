#pragma once
#include "main.h"
#include "Packet.h"
#include "PacketQueue.h"
#include "SocketLayer.h"
#include "CriticalSection.h"
#include <map>
#include <functional>

#define CLIENT_HASH -1

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

struct RCAData {
	bool initialized;
	int p;
	BigInt g;
	int private_num;
	BigInt public_srv;
	BigInt public_cli;
	BigInt end_key;
	RCAData() {
		initialized = false;
		p = -1;
		private_num = -1;
	}
};

class UdpTraficGuide;

typedef void (*RecieveCallback)(UdpTraficGuide*, Packet**);

void TransformBs(ByteStream* bs, BigInt key);

class UdpTraficGuide
{
public:
	UdpTraficGuide() : m_RecieveQueue(new PacketQueue<OrderedQueue>), m_SendQueue(new PacketQueue<OrderedQueue>) {}
	void Initialize(const char* ip, unsigned short port, unsigned int timeout);
	void RegisterRecvCallback(RecieveCallback recv_callback);
	void SendTo(unsigned long ip, unsigned short port, bool absolute_port, ByteStream* bs);
	void CallbackRecieve(UdpTraficGuide* self, Packet** p);
	bool IsRecieveCallbackRegistrated();
	unsigned long GetDestAddr();
	SocketLayer* GetSocketLayerInstance();
	std::pair<bool*, bool*> GetThreadStates();
	SOCKET GetSocket();
	PacketQueue<OrderedQueue>* GetRecvQueue();
	PacketQueue<OrderedQueue>* GetSendQueue();
	std::map < size_t, RCAData >* GetRCAMap();
	bool IsInitializedSecurity(size_t client);
	RCAData GetSecurityInfo(size_t client);
	~UdpTraficGuide();
	CriticalSection GetListenListSection();
	CriticalSection GetSendListSection();
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
	//Cryptography
	std::map < size_t, RCAData >* m_RCA;
	bool m_bIsClient;
	CriticalSection m_ListenListSection;
	CriticalSection m_SendListSection;
};

