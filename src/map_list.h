#ifndef __MAP_LIST_H__
#define __MAP_LIST_H__

mapList *map_list_create(mapData data);
mapList *map_list_add(mapList *list, mapData data);
mapList *map_list_insert(mapList *list, mapData data);
mapList *map_list_push(mapList *list, mapData data);
bool map_list_del(mapList *list, mapList *node);
mapList *map_list_find(mapList *node, mapData data);
mapList *map_list_tail(mapList *node);
mapList *map_list_partition(mapList *head, mapList *end, mapList **newHead, mapList **newEnd);
mapList *map_list_quicksort(mapList *head, mapList *tail);
mapData **map_list_array_sort(mapList *head);

#endif
