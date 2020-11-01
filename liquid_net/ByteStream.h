#pragma once
#include "main.h"
#include "Buffer.h"

class ByteStream
{
public:
	ByteStream();
	ByteStream(unsigned char* data, unsigned int data_size);
	template <typename T>
	ByteStream& Write(T data, unsigned int data_size = sizeof(T));
	ByteStream& Write(unsigned char* data, unsigned int data_size);
	template <typename T>
	void Read(T& var, unsigned int var_size = sizeof(T));
	void Read(unsigned char** var, unsigned int len);
	virtual ~ByteStream();
	unsigned char* GetData(unsigned int* len);
	void ResetReadPointer();
	void Clear();
	ByteStream& Copy();
	unsigned int GetLength();
private:
	Buffer<SafeAppender, SafeReader>* m_Buffer;
	unsigned int m_uiReadPointer;
};

template<typename T>
inline ByteStream& ByteStream::Write(T data, unsigned int data_size)
{
	m_Buffer->Append<T>(data, -1, data_size);
	return *this;
}

template<typename T>
inline void ByteStream::Read(T& var, unsigned int var_size)
{
	m_Buffer->Read(var, m_uiReadPointer, var_size);
	m_uiReadPointer += var_size;
}
