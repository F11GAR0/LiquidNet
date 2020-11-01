#include "main.h"

int main() {
	SocketLayer* slayer = new SocketLayer();
	SOCKET sock = slayer->CreateSocket(7773, nullptr, 0);
	PacketQueue<OrderedQueue, StlBuffer> queue;

	std::cout << "waitin to recieve" << std::endl;
	while (queue.GetCount() < 4) {
		slayer->Recieve(sock, 7773, &queue);
	}
	std::cout << "recieve end";

	closesocket(sock);
	delete slayer;
	return 0;
}