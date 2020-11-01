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
	SOCKET sock = slayer->CreateSocket(7773, nullptr, 0);
	PacketQueue<OrderedQueue, StlBuffer> queue;
	std::cout << "waitin to recieve";
	while (queue.GetCount() < 4) {
		slayer->Recieve(sock, 7773, &queue);
	}
	std::cout << "recieve end";
	closesocket(sock);
	delete slayer;
	
	return 0;
}