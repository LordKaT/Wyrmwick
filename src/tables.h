#ifndef __ARRAY_H__
#define __ARRAY_H__

table* table_new(size_t elemSize, int startingSize, int startingCapacity);

// data pointer must be of *exactly* the same type as the table, i.e. can't append a char to an int table.
void table_append(table *arr, const void *val);
void table_get(table *arr, int index, void *val);
void table_put(table *arr, int index, const void *val);
void* table_ind(table *arr, int index);

void table_shrink(table *arr, int num);
void table_free(table *arr);

#endif
