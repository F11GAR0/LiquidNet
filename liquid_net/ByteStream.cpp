#include "main.h"
#include "ByteStream.h"

ByteStream::ByteStream()
{
	m_Buffer = nullptr;
	m_iLength = 0;
	m_iWritePointer = 0;
	m_iReadPointer = 0;
}

ByteStream::ByteStream(unsigned char* data, unsigned int data_size)
{
	m_Buffer = (byte*)malloc(data_size);
	m_iLength = data_size;
	m_iWritePointer = 0;
	m_iReadPointer = 0;
	Write(data, data_size);
}

void ByteStream::Read(char* data, int len)
{
	for (int i = 0; i < len; i++) {
		data[i] = m_Buffer[m_iReadPointer++];
	}
}

void ByteStream::Read(byte* data, int len)
{
	for (int i = 0; i < len; i++) {
		data[i] = m_Buffer[m_iReadPointer++];
	}
}

void ByteStream::Write(byte* data, int len)
{
	if (!m_Buffer) {
		m_Buffer = (byte*)malloc(len);
		m_iLength = len;
	}
	else {
		unsigned int free_space = m_iLength - m_iWritePointer;
		if (free_space < len) {
			m_Buffer = (byte*)realloc(m_Buffer, m_iLength + (len - free_space));
		}
		m_iLength = m_iLength + (len - free_space);
	}
	for (int end = m_iWritePointer + len, k = 0; m_iWritePointer < end; m_iWritePointer++) {
		m_Buffer[m_iWritePointer] = data[k++];
	}
}

void ByteStream::Write(const char* text, int len)
{
	if (!m_Buffer) {
		m_Buffer = (byte*)malloc(len);
		m_iLength = len;
	}
	else {
		unsigned int free_space = m_iLength - m_iWritePointer;
		if (free_space < len) {
			m_Buffer = (byte*)realloc(m_Buffer, m_iLength + (len - free_space));
		}
		m_iLength = m_iLength + (len - free_space);
	}
	for (int end = m_iWritePointer + len, k = 0; m_iWritePointer < end; m_iWritePointer++) {
		m_Buffer[m_iWritePointer] = text[k++];
	}
}

void ByteStream::Write(BigInt num)
{
	std::string s = num.to_string();
	Write(s.c_str(), s.length());
}

void ByteStream::Clear()
{
	free(m_Buffer);
	m_Buffer = nullptr;
	m_iLength = 0;
	m_iReadPointer = 0;
	m_iWritePointer = 0;
}

unsigned int ByteStream::GetLength()
{
	return m_iLength;
}

byte* ByteStream::GetData(unsigned int OUT* len)
{
	*len = m_iLength;
	unsigned char* ret = (unsigned char*)malloc(m_iLength);
	memcpy(ret, m_Buffer, m_iLength);
	return m_Buffer;
}

ByteStream& ByteStream::Copy()
{
	unsigned int len = 0;
	unsigned char* data = GetData(&len);
	ByteStream* bs = new ByteStream(data, len);
	return *bs;
}

void ByteStream::ResetWritePointer()
{
	m_iWritePointer = 0;
}

void ByteStream::ResetReadPointer()
{
	m_iReadPointer = 0;
}
