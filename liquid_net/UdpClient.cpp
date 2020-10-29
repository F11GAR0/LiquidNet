#include "main.h"
#include "UdpClient.h"

UdpClient::~UdpClient() {}

bool UdpClient::Connect(const char* ip, unsigned int timeout) {
	return false;
}

void UdpClient::Send(ByteStream* bs) {
}
