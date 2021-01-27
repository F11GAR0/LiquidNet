#include "main.h"
#include "UdpTraficGuide.h"

void TransformBs(ByteStream* bs, BigInt key) {
#ifdef SECURITY
	unsigned int len = 0;
	unsigned char *data = bs->GetData(&len);
	for (int i = 0; i < len; i++) {
		data[i] ^= key.to_int();
	}
#endif
}

THREAD send_thread(LPVOID arg) {
	UdpTraficGuide* self = (UdpTraficGuide*)arg;
	auto queue = self->GetSendQueue();
	while (*self->GetThreadStates().first) {
		if (queue->GetCount() > 0) {
			ByteStream bs;
			self->GetSendListSection().Lock();
			Packet p = queue->Pop();
			self->GetSendListSection().Unlock();
			bs.Write((BYTE)p.GetPacketId());
			unsigned int len = 0;
			unsigned char *data = p.GetData().GetData(&len);
			bs.Write(data, len);
			SAFE_FREE(data);
			if(self->IsInitializedSecurity(p.GetSenderHash()))
				TransformBs(&bs, self->GetSecurityInfo(p.GetSenderHash()).end_key);
			self->GetSocketLayerInstance()->Send(self->GetSocket(), &bs, p.GetSenderInfo().first, p.GetSenderInfo().second, !p.IsPortAbsolute());
		}
		Sleep(10);
	}
	return 0;
}

THREAD listen_thread(LPVOID arg) {
	UdpTraficGuide* self = (UdpTraficGuide*)arg;
	auto queue = self->GetRecvQueue();
	while (*self->GetThreadStates().second) {
		Packet *p = self->GetSocketLayerInstance()->Recieve(self->GetSocket());
		if (p) {
			//self->GetListenListSection().Lock();
			if (self->IsInitializedSecurity(p->GetSenderHash())) {	
				ByteStream bs;
				bs.Write((BYTE)p->GetPacketId());
				unsigned int len = 0;
				unsigned char* data = p->GetData().GetData(&len);
				bs.Write(data, len);
				TransformBs(&bs, self->GetSecurityInfo(p->GetSenderHash()).end_key);
				Packet* t = new Packet(&bs, p->GetSenderInfo().first, p->GetSenderInfo().second);
				if (self->IsRecieveCallbackRegistrated()) { //system packets
					self->CallbackRecieve(self, &t);
				}
				self->GetListenListSection().Lock();
				if(t) //user packets
					queue->Push(t);
				self->GetListenListSection().Unlock();
				SAFE_DELETE(p);
			}
			else {
				if (self->IsRecieveCallbackRegistrated()) {
					self->CallbackRecieve(self, &p);
				}
				Sleep(10);
				self->GetListenListSection().Lock();
				if (p)
					queue->Push(p);
				self->GetListenListSection().Unlock();
			}
			//self->GetListenListSection().Unlock();
		}
		Sleep(10);
	}
	return 0;
}

void UdpTraficGuide::Initialize(const char* ip, unsigned short port, unsigned int timeout)
{
	m_SockLayer = new SocketLayer();
	if (ip && ip[0]) {
		m_ulAddr = inet_addr(ip);
		m_bIsClient = true;
	}
	m_Socket = m_SockLayer->CreateSocket(port, ip, timeout);
	m_bListenThread = true;
	m_bSendThread = true;
	m_RCA = new std::map<size_t, RCAData>();
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

void UdpTraficGuide::SendTo(unsigned long ip, unsigned short port, bool absolute_port, ByteStream* bs)
{
	Packet* p = new Packet(bs, ip, port);
	if (absolute_port)
		p->SetPortAbsolute();
	// need mutex here
	m_SendQueue->Push(p->Copy());
	delete p;
}

void UdpTraficGuide::CallbackRecieve(UdpTraficGuide *self, Packet** p)
{
	m_RecvCallback(self, p);
}

bool UdpTraficGuide::IsRecieveCallbackRegistrated()
{
	return m_RecvCallback != nullptr;
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

std::map < size_t, RCAData >* UdpTraficGuide::GetRCAMap()
{
	return m_RCA;
}

bool UdpTraficGuide::IsInitializedSecurity(size_t client)
{
	if (m_bIsClient) {
		if (m_RCA->find(CLIENT_HASH) != m_RCA->end())
			return m_RCA->at(CLIENT_HASH).initialized;
		else
			return false;
	}
	if (m_RCA->find(client) != m_RCA->end())
		return m_RCA->at(client).initialized;
	return false;
}

RCAData UdpTraficGuide::GetSecurityInfo(size_t client)
{
	if (m_bIsClient) {
		return m_RCA->at(CLIENT_HASH);
	}
	return m_RCA->at(client);
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
	SocketLayer::CloseSocket(m_Socket);
	SAFE_DELETE(m_SendQueue);
	SAFE_DELETE(m_RecieveQueue);
	SAFE_DELETE(m_SockLayer);
}

CriticalSection UdpTraficGuide::GetListenListSection()
{
	return m_ListenListSection;
}

CriticalSection UdpTraficGuide::GetSendListSection()
{
	return m_SendListSection;
}
