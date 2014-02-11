#ifndef __ARRAY_H__
#define __ARRAY_H__

array* array_new(size_t elemSize, int startingSize, int startingCapacity);

// data pointer must be of *exactly* the same type as the array, i.e. can't append a char to an int array.
void array_append(array *arr, const void *val);
void array_get(array *arr, int index, const void *val);
void array_put(array *arr, int index, const void *val);
void* array_ind(array *arr, int index);

void array_free(array *arr);

#endif
