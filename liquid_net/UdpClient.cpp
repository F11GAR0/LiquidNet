#include "main.h"
#include "UdpClient.h"

UdpClient::~UdpClient() {
	auto threads = this->GetThreadStates();
	*threads.first = false;
	*threads.second = false;
}

bool UdpClient::Connect(const char* ip, unsigned int timeout) {
	Initialize(ip, m_usPort, timeout);
	if (this->GetSocketLayerInstance()->Connect(this->GetSocket(), ip, m_usPort)) {
		//add some crypto init packet trafic
		return true;
	}
	return false;
}

void UdpClient::Send(ByteStream* bs, unsigned long addr, unsigned short remote_port) {
	SendTo(addr == (unsigned long)-1 ? this->GetDestAddr() : addr, remote_port == (unsigned short)-1 ? m_usPort : remote_port, remote_port != (unsigned short)-1, bs);
}
