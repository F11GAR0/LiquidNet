#include "main.h"

UdpClient* g_Server;

void OnClientPacketRecieve(Packet* p) {
	std::cout << "recieved packet\n";
	std::cout << GetByteStreamStr(&p->GetData());
}
void OnServerPacketRecieve(Packet* p) {
	std::cout << "recieved packet\n";
	std::cout << GetByteStreamStr(&p->GetData());

	//echo message
	ByteStream bs;
	bs.Write((BYTE)p->GetPacketId());
	unsigned int len = 0;
	unsigned char* data = p->GetData().GetData(&len);
	bs.Write(data, len);
	SAFE_FREE(data);

	g_Server->Send(&bs, p->GetSenderInfo().first, p->GetSenderInfo().second);
}

int main() {
	int cli_srv_flag = 0;
	std::cout << "enter 0 to create client, enter 1 to create server.\n> ";
	std::cin >> cli_srv_flag;
	if (cli_srv_flag) {
		std::cout << "Started server\n";
		g_Server = new UdpClient(7772);
		g_Server->Initialize(NULL, 7772, 100);
		g_Server->RegisterRecvCallback(OnServerPacketRecieve);
	}
	else {
		std::cout << "Started client\n";
		g_Server = new UdpClient(7772);
		if (!g_Server->Connect("192.168.0.107", 100))
		{
			std::cout << "Failed to connect\n";
			delete g_Server;
			system("pause");
			return 0;
		}
		g_Server->RegisterRecvCallback(OnClientPacketRecieve);
		ByteStream bs;
		const char* str = "Houston, we've had a problem!";
		bs.Write((BYTE)0).Write(str, strlen(str));
		g_Server->Send(&bs);
	}
	
	system("pause");
	delete g_Server;
	return 0;
}