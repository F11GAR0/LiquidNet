#include "main.h"
#include "Buffer.h"

void* safe_realloc(void* obj, int obj_size, int dest_size) {
	void* temp = malloc(obj_size);
	SAFE_MEMCPY(temp, obj, obj_size);
	SAFE_FREE(obj);
	obj = malloc(dest_size);
	SAFE_MEMCPY(obj, temp, obj_size);
	SAFE_FREE(temp);
	return obj;
}