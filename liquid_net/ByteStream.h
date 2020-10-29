#pragma once
#include "main.h"
#include "Buffer.h"

class ByteStream
{
public:
	ByteStream();
	template <typename T>
	ByteStream& Write(T data, unsigned int data_size = sizeof(T));
	virtual ~ByteStream();
	unsigned char* GetData(unsigned int* len);
private:
	Buffer<SafeAppender, SafeReader>* m_Buffer;
};

template<typename T>
inline ByteStream& ByteStream::Write(T data, unsigned int data_size)
{
	m_Buffer->Append<T>(data, data_size);
	return *this;
}
