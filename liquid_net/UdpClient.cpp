#include "main.h"
#include "UdpClient.h"

UdpClient::~UdpClient() {
	auto threads = this->GetThreadStates();
	*threads.first = false;
	*threads.second = false;
	SocketLayer::CloseSocket(this->GetSocket());
}

bool UdpClient::Connect(const char* ip, unsigned int timeout) {
	Initialize(ip, m_usPort, timeout);

	return false;
}

void UdpClient::Send(ByteStream* bs, unsigned long addr) {
	Packet* p = new Packet(bs, addr == -1 ? this->GetDestAddr() : addr, m_usPort);
	this->GetSendQueue()->Push(p->Copy());
	delete p;
}
