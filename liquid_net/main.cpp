#include "main.h"

struct stTestStruct {
	int m_iVar;
	float m_fVar;
	double m_dVar;
	int m_iszLength;
	char* m_szVar;
};

int main() {
	SocketLayer* slayer = new SocketLayer();
	SOCKET sock = slayer->CreateSocket(77772, NULL, 0);
	PacketQueue<OrderedQueue> queue;
	while (queue.GetCount() < 4) {
		slayer->Recieve(sock, &queue);
		Sleep(100);
	}
	closesocket(sock);
	delete slayer;
	return 0;
}