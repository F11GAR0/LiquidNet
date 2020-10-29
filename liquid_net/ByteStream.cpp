#include "main.h"
#include "ByteStream.h"

ByteStream::ByteStream()
{
	m_Buffer = new Buffer<SafeAppender, SafeReader>();
}

ByteStream::~ByteStream()
{
	SAFE_DELETE(m_Buffer);
}

unsigned char* ByteStream::GetData(unsigned int* len)
{
	return m_Buffer->GetData(len);
}
