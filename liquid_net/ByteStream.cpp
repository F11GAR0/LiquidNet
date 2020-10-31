#include "main.h"
#include "ByteStream.h"

ByteStream::ByteStream()
{
	m_Buffer = new Buffer<SafeAppender, SafeReader>();
	m_uiReadPointer = 0;
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

unsigned int ByteStream::GetLength()
{
	return m_Buffer->GetLength();
}
