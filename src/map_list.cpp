#include "include.h"

mapList *map_list_create(mapData data) {
	mapList *node;
	node = (mapList *)malloc(sizeof(mapList));
	if (node == nullptr) {
		debug_print("map_list_create(): out of memory!\r\n");
		return nullptr;
	}
	node->m_mapData = data;
	node->next = nullptr;
	return node;
}

mapList *map_list_add(mapList *list, mapData data) {
	mapList *head = list;
	while (head->next != nullptr) {
		head = head->next;
	}
	head->next = (mapList *)malloc(sizeof(mapList));
	if (head->next == nullptr) {
		debug_print("map_list_add(): out of memory!\r\n");
		return nullptr;
	}
	head->next->next = nullptr;
	head->next->m_mapData = data;
	return head->next;
}

mapList *map_list_insert(mapList *list, mapData data) {
	mapList *node;
	node = map_list_create(data);
	node->next = list->next;
	list->next = node;
	return node;
}

mapList *map_list_push(mapList *list, mapData data) {
	mapList *node;
	node = map_list_create(data);
	node->next = list;
	return node;
}

bool map_list_del(mapList *list, mapList *node) {
	if (list == nullptr || node == nullptr) {
		// WHY U DO DIS
		return false;
	}
	while (list->next != nullptr && list->next != node) {
		list = list->next;
	}
	if (list->next != nullptr) {
		list->next = node->next;
		free(node);
		return true;
	}
	return false;
}

mapList *map_list_find(mapList *node, int iX, int iY) {
	if (node == nullptr) {
		//STAHP
		return nullptr;
	}
	while (node != nullptr) {
		if (node->iX == iX && node->iY == iY) {
			return node;
		}
		node = node->next;
	}
	return nullptr;
}

mapList *map_list_tail(mapList *node) {
	if (node == nullptr) {
		return nullptr;
	}
	while (node->next != nullptr) {
		node = node->next;
	}
	return node;
}

mapList *map_list_partition(mapList *head, mapList *end, mapList **newHead, mapList **newEnd) {
	mapList *pivot = end;
	mapList *prev = nullptr, *curr = head, *tail = pivot;

	while (curr != pivot) {
		if (curr->iX < pivot->iX) {
		// Really pulling my hair out on figuring out how to sort this thing.
			if ((*newHead) == nullptr) {
				(*newHead) = curr;
			}
			prev = curr;
			curr = curr->next;
		}
		else {
			if (prev != nullptr) {
				prev->next = curr->next;
			}
			mapList *temp = curr->next;
			curr->next = nullptr;
			tail->next = curr;
			tail = curr;
			curr = temp;
		}
	}

	if ((*newHead) == nullptr) {
		(*newHead) = pivot;
	}

	(*newEnd) = tail;

	return pivot;
}

mapList *map_list_quicksort(mapList *head, mapList *tail) {
	if (head == nullptr || head == tail) {
		return head;
	}

	mapList *newHead = nullptr, *newTail = nullptr;

	mapList *pivot = map_list_partition(head, tail, &newHead, &newTail);

	if (newHead != pivot) {
		mapList *temp = newHead;
		while (temp->next != pivot) {
			temp = temp->next;
		}
		temp->next = nullptr;
		newHead = map_list_quicksort(newHead, temp);
		temp = map_list_tail(newHead);
		temp->next = pivot;
	}
	pivot->next = map_list_quicksort(pivot->next, newTail);
	return newHead;
}

// For fun because my brain stopped working after quicksort. --lk
mapData **map_list_array_sort(mapList *head) {
	if (head == nullptr) {
		return nullptr;
	}
	mapData **mapArray;
	int iLargestX = 0;
	int iLargestY = 0;
	for (mapList *i = head; i != nullptr; i = i->next) {
		if (i->iX > iLargestX) {
			iLargestX = i->iX;
		}
		if (i->iY > iLargestY) {
			iLargestY = i->iY;
		}
	}
	mapArray = (mapData **)malloc(iLargestX * sizeof(mapData *));
	for (int x = 0; x < iLargestX; x++) {
		mapArray[x] = (mapData *)malloc(iLargestY * sizeof(mapData));
	}

	for (mapList *i = head; i != nullptr; i = i->next) {
		mapArray[i->iX][i->iY].m_iTileID = i->m_mapData.m_iTileID;
		mapArray[i->iX][i->iY].m_cWalk = i->m_mapData.m_cWalk;
		free(head);
		head = i;
	}

	mapData tempData;

	for (int x = 0; x < iLargestX; x++) {
		for (int y = 0; y < iLargestY; y++) {
			tempData.m_iTileID = mapArray[x][y].m_iTileID;
			tempData.m_cWalk = mapArray[x][y].m_cWalk;
			if (head == nullptr) {
				head = map_list_create(tempData);
			} else {
				map_list_add(head, tempData);
			}
		}
	}

	return mapArray;
}
