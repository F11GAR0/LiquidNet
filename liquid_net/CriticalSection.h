#pragma once
#include "main.h"
class CriticalSection
{
public:
	CriticalSection();
	~CriticalSection();
	void Lock();
	void Unlock();
private:
#ifdef WIN32
	CRITICAL_SECTION criticalSection;
#else
	pthread_mutex_t hMutex;
#endif
};