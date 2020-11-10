#include "main.h"
#include "ByteStream.h"

ByteStream::ByteStream()
{
	m_Buffer = new Buffer<SafeAppender, SafeReader>();
	m_uiReadPointer = 0;
}

ByteStream::ByteStream(unsigned char* data, unsigned int data_size)
{
	m_Buffer = new Buffer<SafeAppender, SafeReader>();
	m_Buffer->Append(data, data_size);
	m_uiReadPointer = 0;
}

ByteStream& ByteStream::Write(unsigned char* data, unsigned int data_size)
{
	m_Buffer->Append(data, data_size);
	return *this;
}

ByteStream& ByteStream::Write(const char* data, unsigned int data_size)
{
	return Write((unsigned char*)data, data_size);
}

void ByteStream::Read(unsigned char** var, unsigned int len)
{
	m_Buffer->Read(var, m_uiReadPointer, len);
}

void ByteStream::Read(unsigned char* var, unsigned int len)
{
	m_Buffer->Read(var, m_uiReadPointer, len);
}

ByteStream::~ByteStream()
{
	SAFE_DELETE(m_Buffer);
}

unsigned char* ByteStream::GetData(unsigned int* len)
{
	return m_Buffer->GetData(len);
}

void ByteStream::ResetReadPointer()
{
	m_uiReadPointer = 0;
}

void ByteStream::Clear()
{
	m_Buffer->Clear();
}

ByteStream& ByteStream::Copy()
{
	unsigned int len = 0;
	unsigned char* data = m_Buffer->GetData(&len);
	ByteStream* bs = new ByteStream(data, len);
	return *bs;
}

unsigned int ByteStream::GetLength()
{
	return m_Buffer->GetLength();
}
