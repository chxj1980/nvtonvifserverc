#include "map.h"
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

MapNode map_get(Map this, const char* key) {
	if (NULL == this)
		return NULL;
	MapNode result = NULL;
	ListNode node = this->dataList->dataNodes;
	while(NULL != node) {
		MapNode mapNode = node->element;
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

void map_put(Map this, const char* key, MapElement element) {
	if (NULL == this) {
		return;
	}
	MapNode mapNode = map_get(this, key);
	if (NULL == mapNode) {
		mapNode = malloc(sizeof(struct mapnode));
		if (NULL == mapNode)
			return;
		mapNode->map = this;
		strcpy(mapNode->key, key);
		mapNode->element = element;
		this->dataList->addLast(this->dataList, mapNode);
	}
	else {
		this->clearElement(mapNode->element);
		mapNode->element = element;
	}
}

void map_forEach(Map this,void (*eachFunc)(MapNode)) {
	if (NULL == this)
		return;
	ListNode node = this->dataList->dataNodes;
	while(NULL != node) {
		MapNode mapNode = node->element;
		if (NULL != mapNode) {
			eachFunc(mapNode);
		}
		node = node->nextNode;
	}
}

void map_removeNode(MapNode mapNode) {
	if (NULL == mapNode) {
		return;
	}
	if (NULL != mapNode->element) {
		if (NULL !=mapNode->map->clearElement)
			mapNode->map->clearElement(mapNode->element);
	}
	free(mapNode);
}

void map_Remove(Map this, const char* key) {
	if (NULL == this)
		return;
	int pos = 0;
	ListNode node = this->dataList->dataNodes;
	while(NULL != node) {
		MapNode mapNode = node->element;
		node = node->nextNode;
		if (NULL == mapNode) {
			pos++;
			continue;
		}
		if (0 == strcmp(mapNode->key, key)) {
			this->dataList->deleteIndex(this->dataList, pos);
			break;
		}
		pos++;
	}
}

Map newMap(void (*_clearElement)(MapElement)) {
	Map map;
	map = malloc(sizeof(struct map));
	if (NULL == map)
		return NULL;
	map->dataList = newList(map_removeNode);
	if (NULL == map->dataList) {
		free(map);
		return NULL;
	}
	map->forEach = map_forEach;
	map->size = map_size;
	map->get = map_get;
	map->put = map_put;
	map->clearElement = _clearElement;
	return map;
}



void delMap(Map this) {
	if (NULL == this)
		return;
	delList(this->dataList);
	this->dataList = NULL;
	free(this);
}
