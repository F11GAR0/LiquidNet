#include "main.h"
#include "DebugBuffer.h"

void NullBuffer::Send(std::string message) { }

void NullBuffer::SendByteStream(ByteStream* bs) { }

void StlBuffer::Send(std::string message)
{
	std::cout << message << std::endl;
}

void StlBuffer::SendByteStream(ByteStream* bs)
{
	std::string mess = "[BYTESTREAM]: ";
	unsigned int len = 0;
	unsigned char* data = bs->GetData(&len);
	for (int i = 0; i < len; i++){
		char* bytepart = (char*)malloc(4);
		sprintf(bytepart, "%.2X ", data[i]);
		mess += bytepart;
		SAFE_FREE(bytepart);
	}
	this->Send(mess);
	SAFE_FREE(data);
}

void FileBuffer::Send(std::string message)
{
	//TODO: Add log file
}

void FileBuffer::SendByteStream(ByteStream* message)
{
	//TODO: Add log file
}
