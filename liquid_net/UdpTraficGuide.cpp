#include "main.h"
#include "UdpTraficGuide.h"

THREAD send_thread(LPVOID arg) {
	UdpTraficGuide* self = (UdpTraficGuide*)arg;
	auto queue = self->GetSendQueue();
	while (*self->GetThreadStates().first) {
		if (queue->GetCount() > 0) {
			ByteStream bs;
			Packet p = queue->Pop();
			bs.Write((BYTE)p.GetPacketId());
			unsigned int len = 0;
			unsigned char *data = p.GetData().GetData(&len);
			bs.Write(data, len);
			self->GetSocketLayerInstance()->Send(self->GetSocket(), &bs, p.GetSenderInfo().first, p.GetSenderInfo().second);
		}
		Sleep(10);
	}
	return 0;
}

THREAD listen_thread(LPVOID arg) {
	UdpTraficGuide* self = (UdpTraficGuide*)arg;
	auto queue = self->GetRecvQueue();
	while (*self->GetThreadStates().second) {
		self->GetSocketLayerInstance()->Recieve(self->GetSocket(), self->GetRecvQueue());
		if (queue->GetCount() > 0) {
			Packet p = queue->Pop();
			self->CallbackRecieve(&p);
		}
		Sleep(10);
	}
	return 0;
}

void UdpTraficGuide::Initialize(const char* ip, unsigned short port, unsigned int timeout)
{
	m_SockLayer = new SocketLayer();
	if(ip && ip[0])
		m_ulAddr = inet_addr(ip);
	m_Socket = m_SockLayer->CreateSocket(port, ip, timeout);
	m_bListenThread = true;
	m_bSendThread = true;
#ifdef WIN32
	m_SendThread = CreateThread(0, 0, send_thread, this, 0, 0);
	m_ListenThread = CreateThread(0, 0, listen_thread, this, 0, 0);
#else
	pthread_create(&m_SendThread, NULL, send_thread, this);
	pthread_create(&m_ListenThread, NULL, listen_thread, this);
#endif
}

void UdpTraficGuide::RegisterRecvCallback(RecieveCallback recv_callback)
{
	m_RecvCallback = recv_callback;
}

void UdpTraficGuide::Send(const char* ip, unsigned short port, ByteStream* bs)
{
	Packet* p = new Packet(bs, inet_addr(ip), port);
	// need mutex here
	m_SendQueue->Push(p->Copy());
	delete p;
}

void UdpTraficGuide::CallbackRecieve(Packet* p)
{
	m_RecvCallback(p);
}

unsigned long UdpTraficGuide::GetDestAddr()
{
	return m_ulAddr;
}

SocketLayer* UdpTraficGuide::GetSocketLayerInstance()
{
	return m_SockLayer;
}

std::pair<bool*, bool*> UdpTraficGuide::GetThreadStates()
{
	return std::pair<bool*, bool*>(&m_bListenThread, &m_bSendThread);
}

SOCKET UdpTraficGuide::GetSocket()
{
	return m_Socket;
}

PacketQueue<OrderedQueue>* UdpTraficGuide::GetRecvQueue()
{
	return m_RecieveQueue;
}

PacketQueue<OrderedQueue>* UdpTraficGuide::GetSendQueue()
{
	return m_SendQueue;
}

UdpTraficGuide::~UdpTraficGuide()
{
	if (m_bListenThread && m_bSendThread) {
		m_bListenThread = false;
		m_bSendThread = false;
		TerminateThread(m_ListenThread, 0);
		TerminateThread(m_SendThread, 0);
		CloseHandle(m_ListenThread);
		CloseHandle(m_SendThread);
	}
}
