#include "main.h"
#include "CriticalSection.h"
#include <assert.h>

CriticalSection::CriticalSection()
{
#ifdef WIN32
	InitializeCriticalSection(&criticalSection);
#else
	int error = pthread_mutex_init(&hMutex, 0);
	assert(error == 0);
#endif
}

CriticalSection::~CriticalSection()
{
#ifdef WIN32
	DeleteCriticalSection(&criticalSection);
#else
	pthread_mutex_destroy(&hMutex);
#endif
}


void CriticalSection::Lock()
{
#ifdef WIN32
	EnterCriticalSection(&criticalSection);
#else
	int error = pthread_mutex_lock(&hMutex);
	assert(error == 0);
#endif
}

void CriticalSection::Unlock()
{
#ifdef WIN32
	LeaveCriticalSection(&criticalSection);
#else
	int error = pthread_mutex_unlock(&hMutex);
	assert(error == 0);
#endif
}