#include "main.h"
#include "UdpClient.h"
#include "SystemPacketEnumerations.h"
#include "MiscFunctions.h"

UdpClient::~UdpClient() {
	if (m_ClientRCAData.initialized)
	{
		ByteStream bs;
		bs.Write((BYTE)eSystemClientPacket::ID_DISCONNECT);
		TransformBs(&bs, m_ClientRCAData.end_key);
		//unsafe!
		this->GetSocketLayerInstance()->Send(this->GetSocket(), &bs, this->GetDestAddr(), this->m_usPort, true);
	}
	auto threads = this->GetThreadStates();
	*threads.first = false;
	*threads.second = false;
}

bool UdpClient::Connect(const char* ip, unsigned int timeout) {
	Initialize(ip, m_usPort, timeout);
	if (this->GetSocketLayerInstance()->Connect(this->GetSocket(), ip, m_usPort)) {
		ByteStream initial;
		initial.Write((BYTE)eSystemClientPacket::ID_REQUEST_CONNECT);
		Send(&initial);
		while (!m_ClientRCAData.initialized) {
			Packet* p;
			int tries = 100;
			while ((p = Recieve()) == nullptr) {
				tries--;
				if (tries < 0) {
					std::cout << "timeout";
					return false;
				}
				Sleep(10);
			}
			switch (p->GetPacketId()) {
			case eSystemServerPacket::ID_ACCEPT_CONNECT:
			{
#ifdef SECURITY
				m_ClientRCAData.p = gen_prime(rand() % 10 + 45, rand() % 30 + 55);
				m_ClientRCAData.g = gen_prime(3, rand() % 30 + 15);
				m_ClientRCAData.private_num = rand() % 10 + 3;
				m_ClientRCAData.public_cli = pow(m_ClientRCAData.g, m_ClientRCAData.private_num) % m_ClientRCAData.p;
				ByteStream bs;
				bs.Write((BYTE)eSystemClientPacket::ID_RSA_PUBLIC_NUMBERS);
				bs.Write(m_ClientRCAData.p);
				bs.Write((int)m_ClientRCAData.g.to_string().length());
				bs.Write(m_ClientRCAData.g);
				bs.Write((int)m_ClientRCAData.public_cli.to_string().length());
				bs.Write(m_ClientRCAData.public_cli);
				//std::cout << GetByteStreamStr(&bs) << std::endl;
				Send(&bs);
				break;
#else
				this->GetRCAMap()->insert(std::pair<size_t, RCAData>(CLIENT_HASH, m_ClientRCAData));
				m_ClientRCAData.endkey = -1;
				m_ClientRCAData.initialized = true;
				delete p;
				return true;
#endif
			}
#ifdef SECURITY
			case eSystemServerPacket::ID_RSA_PUBLIC_KEY: {

				int digits;
				p->GetData().Read(digits);
				byte* data = (byte*)malloc(digits + 1);
				p->GetData().Read(data, digits);
				data[digits] = '\0';

				m_ClientRCAData.public_srv = BigInt(std::string((char*)data));
				m_ClientRCAData.end_key = pow(m_ClientRCAData.public_srv, m_ClientRCAData.private_num) % m_ClientRCAData.p;

				ByteStream bs;
				bs.Write((BYTE)eSystemClientPacket::ID_RSA_END_TRANSMITTION);
				//unsafe!
				this->GetSocketLayerInstance()->Send(this->GetSocket(), &bs, this->GetDestAddr(), this->m_usPort, true);

				m_ClientRCAData.initialized = true;

				this->GetRCAMap()->insert(std::pair<size_t, RCAData>(CLIENT_HASH, m_ClientRCAData));
				delete p;
				return true;
			}
#endif
			case eSystemServerPacket::ID_DENY_CONNECT: // what a shame(((
				delete p;
				return false;
			}
			delete p; //deallocate...
		}
	}
	return false;
}

bool UdpClient::Connect(const char* ip, unsigned int timeout, int p_, BigInt g, int private_num) {
	Initialize(ip, m_usPort, timeout);
	if (this->GetSocketLayerInstance()->Connect(this->GetSocket(), ip, m_usPort)) {
		ByteStream initial;
		initial.Write((BYTE)eSystemClientPacket::ID_REQUEST_CONNECT);
		Send(&initial);
		while (!m_ClientRCAData.initialized) {
			Packet* p;
			int tries = 100;
			while ((p = Recieve()) == nullptr) {
				tries--;
				if (tries < 0) {
					std::cout << "timeout";
					return false;
				}
				Sleep(10);
			}
			switch (p->GetPacketId()) {
			case eSystemServerPacket::ID_ACCEPT_CONNECT:
			{
#ifdef SECURITY
				m_ClientRCAData.p = p_;
				m_ClientRCAData.g = g;
				m_ClientRCAData.private_num = private_num;
				m_ClientRCAData.public_cli = pow(m_ClientRCAData.g, m_ClientRCAData.private_num) % m_ClientRCAData.p;
				ByteStream bs;
				bs.Write((BYTE)eSystemClientPacket::ID_RSA_PUBLIC_NUMBERS);
				bs.Write(m_ClientRCAData.p);
				bs.Write((int)m_ClientRCAData.g.to_string().length());
				bs.Write(m_ClientRCAData.g);
				bs.Write((int)m_ClientRCAData.public_cli.to_string().length());
				bs.Write(m_ClientRCAData.public_cli);
				//std::cout << GetByteStreamStr(&bs) << std::endl;
				Send(&bs);
				break;
#else
				this->GetRCAMap()->insert(std::pair<size_t, RCAData>(CLIENT_HASH, m_ClientRCAData));
				m_ClientRCAData.endkey = -1;
				m_ClientRCAData.initialized = true;
				delete p;
				return true;
#endif
			}
#ifdef SECURITY
			case eSystemServerPacket::ID_RSA_PUBLIC_KEY: {

				int digits;
				p->GetData().Read(digits);
				byte* data = (byte*)malloc(digits + 1);
				p->GetData().Read(data, digits);
				data[digits] = '\0';

				m_ClientRCAData.public_srv = BigInt(std::string((char*)data));
				m_ClientRCAData.end_key = pow(m_ClientRCAData.public_srv, m_ClientRCAData.private_num) % m_ClientRCAData.p;

				ByteStream bs;
				bs.Write((BYTE)eSystemClientPacket::ID_RSA_END_TRANSMITTION);
				//unsafe!
				this->GetSocketLayerInstance()->Send(this->GetSocket(), &bs, this->GetDestAddr(), this->m_usPort, true);

				m_ClientRCAData.initialized = true;

				std::cout << "Security Data: " << std::endl;
				std::cout << "p: " << m_ClientRCAData.p << std::endl;
				std::cout << "g: " << m_ClientRCAData.g << std::endl;
				std::cout << "private_number: " << m_ClientRCAData.private_num << std::endl;
				std::cout << "public client: " << m_ClientRCAData.public_cli << std::endl;
				std::cout << "public server: " << m_ClientRCAData.public_srv << std::endl;
				std::cout << "end key: " << m_ClientRCAData.end_key << std::endl;


				this->GetRCAMap()->insert(std::pair<size_t, RCAData>(CLIENT_HASH, m_ClientRCAData));
				delete p;
				return true;
			}
#endif
			case eSystemServerPacket::ID_DENY_CONNECT: // what a shame(((
				delete p;
				return false;
			}
			delete p; //deallocate...
		}
	}
	return false;
}

void UdpClient::Send(ByteStream* bs, unsigned long addr, unsigned short remote_port) {
	SendTo(addr == (unsigned long)-1 ? this->GetDestAddr() : addr, remote_port == (unsigned short)-1 ? m_usPort : remote_port, remote_port != (unsigned short)-1, bs);
}

Packet* UdpClient::Recieve()
{
	//std::cout << "tryna to recieve" << std::endl;
	this->GetListenListSection().Lock();
	unsigned int count = this->GetRecvQueue()->GetCount();
	if (count > 0) {
		auto pack = this->GetRecvQueue()->Pop().Copy();
		this->GetListenListSection().Unlock();
		return pack;
	}
	this->GetListenListSection().Unlock();
	return nullptr;
}

void RSASrvCallback(UdpTraficGuide* self, Packet** packet) {
	Packet* p = *packet;
	//printf("received packet: %d\n", p->GetPacketId());
	switch (p->GetPacketId()) {

	case eSystemClientPacket::ID_REQUEST_CONNECT:
		if (self->GetRCAMap()->find(p->GetSenderHash()) == self->GetRCAMap()->end()) {
			//does not exist
			self->GetRCAMap()->insert(std::pair<size_t, RCAData>(p->GetSenderHash(), RCAData()));
#ifdef SECURITY
			self->GetRCAMap()->at(p->GetSenderHash()).initialized = false;
			ByteStream bs;
			bs.Write((BYTE)eSystemServerPacket::ID_ACCEPT_CONNECT);
			self->SendTo(p->GetSenderInfo().first, p->GetSenderInfo().second, true, &bs);
#else
			ByteStream bs_accept;
			bs_accept.Write((BYTE)eSystemServerPacket::ID_ACCEPT_CONNECT); //all good, accept connect
			self->SendTo(p->GetSenderInfo().first, p->GetSenderInfo().second, true, &bs_accept);

			self->GetRCAMap()->at(p->GetSenderHash()).initialized = true;
			ByteStream bs;
			bs.Write((BYTE)eSystemLocalPacket::ID_CONNECTED);
			stConnectedUserInfo inf;
			inf.ip = p->GetSenderInfo().first;
			inf.remote_port = p->GetSenderInfo().second;
			bs.Write((BYTE*)&inf, sizeof(stConnectedUserInfo));
			SAFE_DELETE(*packet);
			*packet = new Packet(&bs, p->GetSenderInfo().first, p->GetSenderInfo().second); //local sync
			break;
#endif
		}
		else {
			//not allowed
			ByteStream bs;
			bs.Write((BYTE)eSystemServerPacket::ID_DENY_CONNECT);
			self->SendTo(p->GetSenderInfo().first, p->GetSenderInfo().second, true, &bs);
		}
		SAFE_DELETE(*packet);
		break;
#ifdef SECURITY
	case eSystemClientPacket::ID_RSA_PUBLIC_NUMBERS:
		if (self->GetRCAMap()->find(p->GetSenderHash()) != self->GetRCAMap()->end()) {
			stConnect connect;
			p->GetData().Read(connect.p);

			p->GetData().Read(connect.len_g);
			connect.g = (byte*)malloc(connect.len_g + 1);
			p->GetData().Read(connect.g, connect.len_g);
			connect.g[connect.len_g] = '\0';

			p->GetData().Read(connect.len_public_cli);
			connect.public_cli = (byte*)malloc(connect.len_public_cli);
			p->GetData().Read(connect.public_cli, connect.len_public_cli);
			connect.public_cli[connect.len_public_cli] = '\0';

			self->GetRCAMap()->at(p->GetSenderHash()).p = connect.p;
			self->GetRCAMap()->at(p->GetSenderHash()).g = BigInt(std::string((char*)connect.g));
			self->GetRCAMap()->at(p->GetSenderHash()).public_cli = BigInt(std::string((char*)connect.public_cli));
			self->GetRCAMap()->at(p->GetSenderHash()).private_num = rand() % 46 + 3;

			self->GetRCAMap()->at(p->GetSenderHash()).end_key = pow(self->GetRCAMap()->at(p->GetSenderHash()).public_cli, self->GetRCAMap()->at(p->GetSenderHash()).private_num) % connect.p;
			//self->GetRCAMap()->at(p->GetSenderHash()).initialized = true;

			BigInt public_key = pow(self->GetRCAMap()->at(p->GetSenderHash()).g, self->GetRCAMap()->at(p->GetSenderHash()).private_num) % connect.p;
			ByteStream bs;
			bs.Write((BYTE)eSystemServerPacket::ID_RSA_PUBLIC_KEY);
			bs.Write((int)public_key.to_string().length());
			bs.Write(public_key);
			self->SendTo(p->GetSenderInfo().first, p->GetSenderInfo().second, true, &bs);
		}
		else {
			//stop tryin dumb..
			ByteStream bs;
			bs.Write((BYTE)eSystemServerPacket::ID_DENY_CONNECT);
			self->SendTo(p->GetSenderInfo().first, p->GetSenderInfo().second, true, &bs);
		}
		SAFE_DELETE(*packet);
		break;
	case eSystemClientPacket::ID_RSA_END_TRANSMITTION:
		if (self->GetRCAMap()->find(p->GetSenderHash()) != self->GetRCAMap()->end()) {
			self->GetRCAMap()->at(p->GetSenderHash()).initialized = true;
			ByteStream bs;
			bs.Write((BYTE)eSystemLocalPacket::ID_CONNECTED);
			stConnectedUserInfo inf;
			inf.ip = p->GetSenderInfo().first;
			inf.remote_port = p->GetSenderInfo().second;
			bs.Write((BYTE*)&inf, sizeof(stConnectedUserInfo));
			SAFE_DELETE(*packet);
			*packet = new Packet(&bs, p->GetSenderInfo().first, p->GetSenderInfo().second);
			break;
		}
		else {
			//meh...
			ByteStream bs;
			bs.Write((BYTE)eSystemServerPacket::ID_DENY_CONNECT);
			self->SendTo(p->GetSenderInfo().first, p->GetSenderInfo().second, true, &bs);
			SAFE_DELETE(*packet);
		}
		break;
#endif
	case eSystemClientPacket::ID_DISCONNECT:
		if (self->GetRCAMap()->find(p->GetSenderHash()) != self->GetRCAMap()->end()) {
			ByteStream bs;
			bs.Write((BYTE)eSystemLocalPacket::ID_DISCONNECTED);
			stConnectedUserInfo inf;
			inf.ip = p->GetSenderInfo().first;
			inf.remote_port = p->GetSenderInfo().second;
			bs.Write((BYTE*)&inf, sizeof(stConnectedUserInfo));
			self->GetListenListSection().Lock();
			self->GetRecvQueue()->Push(new Packet(&bs, p->GetSenderInfo().first, p->GetSenderInfo().second));
			self->GetListenListSection().Unlock();
			self->GetRCAMap()->erase(p->GetSenderHash());
		}
		SAFE_DELETE(*packet);
		break;
	default:
		break;
	}
	
	//self->GetRecvQueue()->PopFront();
}

void UdpClient::Listen(unsigned int timeout)
{
	this->Initialize(NULL, m_usPort, timeout);
	this->RegisterRecvCallback(RSASrvCallback);
}
