#pragma once
#include "main.h"

#define SAFE_FREE(x) if(x != NULL) free(x)
#define SAFE_DELETE(x) if(x != NULL) delete x
#define SAFE_MEMCPY(dest, src, size) if(dest && src) memcpy(dest, src, size)

void* safe_realloc(void* obj, int obj_size, int dest_size);

#define SAFE_REALLOC(obj, obj_size, dest_size) safe_realloc(obj, obj_size, dest_size);

class SafeAppender {
public:
	template <typename T>
	unsigned int Append(unsigned char** buff, unsigned int buff_len, unsigned int reserved, const T& data, unsigned int data_size = sizeof(T)) {
		unsigned int new_length = buff_len + data_size;
		if (!*buff) {
			*buff = (unsigned char*)malloc(sizeof(data));
			for (int i = 0, end = data_size, p = 0; i < end; i++) {
				(*buff)[i] = ((unsigned char*)&data)[p++];
			}
			return new_length;
		}
		if (reserved - buff_len < data_size) {
			*buff = (unsigned char*)SAFE_REALLOC(*buff, buff_len, new_length);	
		}	
		for (int i = buff_len, end = buff_len + data_size, p = 0; i < end; i++) {
			(*buff)[i] = ((unsigned char*)&data)[p++];
		}
		return new_length;
	}
};

class SafeReader {
public:
	template <typename T>
	T Read(unsigned char* buff, unsigned int buff_len, unsigned int data_size = sizeof(T)) {

	}
};

template<class Appender, class Reader>
class Buffer : public Appender, public Reader {
public:
	Buffer() : m_Buffer(nullptr), m_uiLength(0), m_uiReserved(0) {}
	template<typename T>
	void Append(T data, unsigned int data_size = sizeof(data)) {
		unsigned int out_length = Append<T>(&m_Buffer, m_uiLength, m_uiReserved, data, data_size);
		if (out_length > m_uiReserved) {
			m_uiReserved = out_length;		
		}
		m_uiLength = out_length;
	}
	~Buffer();
	unsigned char* GetData(unsigned int *length);
private:
	using Appender::Append;
	using Reader::Read;
	unsigned char* m_Buffer;
	unsigned int m_uiLength;
	unsigned int m_uiReserved;
};

template<class Appender, class Reader>
inline Buffer<Appender, Reader>::~Buffer()
{
	SAFE_FREE(m_Buffer);
}

template<class Appender, class Reader>
inline unsigned char* Buffer<Appender, Reader>::GetData(unsigned int *length)
{
	*length = m_uiLength;
	return m_Buffer;
}
