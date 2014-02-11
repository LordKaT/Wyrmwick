#include "include.h"

array* array_new(size_t elemSize, int startingSize, int startingCapacity) {
	if (startingSize > startingCap) {
		debug_print("Array size > cap.\r\n");
		// HALT HERE
	}
	
	array* arr = (array*) malloc(sizeof(array));
	arr->m_elemSize = elemSize;
	arr->m_len = startingSize;
	arr->m_cap = startingCapacity;
	arr->m_data = cmalloc(startingCapacity, elemSize);
	return arr;
}

void array_append(array *arr, const void *val) {
	if (arr->m_len == arr->m_cap) {
		int newcap = arr->m_cap * 2;
		arr->m_data = realloc(arr->data, arr->m_elemSize * newcap);
		if (! arr->m_data) {
			debug_print("Not enough memory to append.\r\n");
			// HALT HERE
		}
		memset(((unsigned char*)arr->m_data) + arr->m_cap*arr->m_elemSize, 0, arr->m_cap);
		arr->m_cap = newcap;
	}
	
	memmove(((unsigned char*)arr->m_data) + arr->m_len*arr->m_elemSize, val);
	arr->m_len++;
}

void array_get(array *arr, int index, const void *val data) {
	if (index >= arr->m_len) {
		debug_print("Array index out of bounds.\r\n");
		// HALT HERE
	}
	memmove(val, ((unsigned char*) arr->m_data) + index * arr->m_elemSize);
}

void array_put(array *arr, int index, const void *val data);
	if (index >= arr->m_len) {
		debug_print("Array index out of bounds.\r\n");
		// HALT HERE
	}
	memmove(((unsigned char*) arr->m_data) + index * arr->m_elemSize, val);
}

void* array_ind(array *arr, int index) {
	if (index >= arr->m_len) {
		debug_print("Array index out of bounds.\r\n");
		// HALT HERE
	}
	return ((unsigned char*) arr->m_data) + index * arr->m_elemSize;
}

void array_free(array *arr) {
	free(arr->m_data);
	free(arr);
}
