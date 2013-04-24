#include "map.h"
#include <malloc.h>
#include <stdlib.h>

MapNode map_get(Map this, const char* key) {
	if (NULL == this)
		return NULL;
	MapNode result = NULL;
	ListNode node = this->dataList->dataNodes;
	while(NULL != node) {
		MapNode* mapNode = node->element;
		node = node->nextNode;
		if (NULL == mapNode) {
			continue;
		}
		if (0 == strcmp(mapNode->key, key)) {
			result = mapNode;
			break;
		}
	}
	return result;
}

int map_size(Map this) {
	if (NULL == this)
		return 0;
	return this->dataList->size(this->dataList);
}

void map_put(Map this, const char*, MapElement) {
	if (NULL == this) {
		return;
	}
	MapNode mapNode = map_getNode()
}

Map newMap(void (*removeNode)(MapNode)) {
	Map map;
	map = malloc(sizeof(*map));
	if (NULL == map)
		return NULL;
	map->dataList = newList(removeNode);
	if (NULL == map->dataList) {
		free(map);
		return NULL;
	}
	map->forEach = map_forEach;
	map->size = map_size;
	map->get = map_get;
	map->put = map_put;
	return map;
}



void delMap(Map this) {
	if (NULL == this)
		return;
	delList(this->dataList);
	free(this);
}
