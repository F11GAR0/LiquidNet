#include "main.h"
#include "SystemPacketEnumerations.h"

UdpClient* g_Server;

void Recv() {
	while (true) {
		Packet* p = g_Server->Recieve();
		if (p) {
			switch (p->GetPacketId()) {
				case eSystemLocalPacket::ID_CONNECTED:
				{
					stConnectedUserInfo inf;
					p->GetData().Read((BYTE*)&inf, sizeof(stConnectedUserInfo));
					std::cout << "Connected new user! IP: " << inf.ip << " Remote port: " << inf.remote_port << std::endl;
					break;
				}
				case eSystemLocalPacket::ID_DISCONNECTED:
				{
					stConnectedUserInfo inf;
					p->GetData().Read((BYTE*)&inf, sizeof(stConnectedUserInfo));
					std::cout << "User disconnected! IP: " << inf.ip << " Remote port: " << inf.remote_port << std::endl;
					break;
				}
				case ePacketEnumerations::ID_MESSAGE: {
					unsigned int len;
					auto bs = p->GetData();
					bs.Read(len);
					char* text = (char*)malloc(len + 1);
					bs.Read((unsigned char*)text, len);
					text[len] = '\0';
					std::cout << "Got text: " << text << std::endl;
					break;
				}
				default:
					std::cout << "Unknown packet: " << (int)p->GetPacketId() << std::endl;
					std::cout << GetByteStreamStr(&p->GetData());
					break;
			}
		}
		SAFE_DELETE(p);
		Sleep(1);
	}
}

int main(int argvc, char **argvs) {
	srand(GetTickCount());
	int cli_srv_flag = 0;
	if (argvc > 1) {
		cli_srv_flag = atoi(argvs[1]);
	}
	else {
		std::cout << "enter 0 to create client, enter 1 to create server.\n> ";
		std::cin >> cli_srv_flag;
	}
	if (cli_srv_flag) {
		std::cout << "Started server\n";
		g_Server = new UdpClient(7772);
		g_Server->Listen(10);
		Recv();
	}
	else {
		std::cout << "Started client\n";
		g_Server = new UdpClient(7772);
		if (argvc > 2) {
			if (!g_Server->Connect("192.168.1.63", 10, atoi(argvs[2]), std::string(argvs[3]), atoi(argvs[4])))
			{
				std::cout << "Failed to connect\n";
				delete g_Server;
				system("pause");
				return 0;
			}
		}
		else {
			if (!g_Server->Connect("192.168.1.63", 10))
			{
				std::cout << "Failed to connect\n";
				delete g_Server;
				system("pause");
				return 0;
			}
		}
		//send test message
		ByteStream bs;
		const char* str = "Houston, we've had a problem!";
		bs.Write((BYTE)ePacketEnumerations::ID_MESSAGE);
		bs.Write(strlen(str));
		bs.Write(str, strlen(str));
		g_Server->Send(&bs);
	}
	
	system("pause");
	delete g_Server;
	return 0;
}