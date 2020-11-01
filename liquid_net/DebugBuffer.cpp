#include "main.h"
#include "DebugBuffer.h"

std::string GetByteStreamStr(ByteStream* bs) {
	std::string mess = "[BYTESTREAM]: ";
	unsigned int len = 0;
	unsigned char* data = bs->GetData(&len);
	for (int i = 0; i < len; i++) {
		char* bytepart = (char*)malloc(4);
		sprintf(bytepart, "%.2X ", data[i]);
		mess += bytepart;
		SAFE_FREE(bytepart);
	}
	SAFE_FREE(data);
	return mess;
}

void NullBuffer::Send(const std::string& message) { }

void NullBuffer::SendByteStream(ByteStream* bs) { }

void StlBuffer::Send(const std::string& message)
{
	std::cout << message << std::endl;
}

void StlBuffer::SendByteStream(ByteStream* bs)
{
	this->Send(GetByteStreamStr(bs));
}

void FileBuffer::Send(const std::string& message)
{
	//TODO: Add log file
}

void FileBuffer::SendByteStream(ByteStream* message)
{
	//TODO: Add log file
}
