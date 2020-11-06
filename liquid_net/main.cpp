#include "main.h"

UdpClient* g_Server;

void OnPacketRecieve(Packet* p) {
	std::cout << "recieved packet\n";
	std::cout << GetByteStreamStr(&p->GetData());
	//g_Server->Send(&p->GetData(), inet_addr("192.168.0.107"));
}

int main() {
	g_Server = new UdpClient(7772);
	g_Server->Initialize("192.168.0.107", 7772, 100);
	g_Server->RegisterRecvCallback(OnPacketRecieve);
	ByteStream bs;
	const char* str = "Houston, we've had a problem!";
	bs.Write(str, strlen(str));
	g_Server->Send(&bs);
	std::cin.get();
	delete g_Server;
	return 0;
}