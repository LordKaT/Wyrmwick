#include "include.h"

void items_listAdd(itemList *pList, item itemData) {
	if (pList == nullptr) {
		pList = new itemList;
		pList->m_item = itemData;
		pList->next = nullptr;
	} else {
		itemList *tempList = pList;
		while (tempList->next != nullptr) {
			tempList = tempList->next;
		}
		tempList->next = new itemList;
		tempList->next->m_item = itemData;
		tempList->next->next = nullptr;
		tempList = nullptr;
	}
	return;
}
