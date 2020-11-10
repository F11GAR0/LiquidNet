#pragma once

enum eSystemClientPacket {
	ID_REQUEST_CONNECT = 100,
	ID_DISCONNECT,
	ID_RSA_PUBLIC_NUMBERS = 8, //choise simple number (and own secret number) | send->public_num
	ID_RSA_END_TRANSMITTION // take public key and create private key | send->public_key(s % secnum)
};

enum eSystemServerPacket {
	ID_ACCEPT_CONNECT = 4,
	ID_DENY_CONNECT, //something went wrong..
	ID_RSA_PUBLIC_KEY = 104, //take simple nimber and choise own secret number | send->public_key(s % secnum)
};

enum eSystemLocalPacket {
	ID_CONNECTED = 40,
	ID_DISCONNECTED
};

enum ePacketEnumerations {
	ID_MESSAGE = 200
};

struct stMessage {
	unsigned int len;
	char* data;
};

struct stConnectedUserInfo {
	unsigned long ip;
	unsigned short remote_port;
};