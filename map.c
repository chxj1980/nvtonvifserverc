#include "map.h"
#include <malloc.h>
#include <stdlib.h>


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
	map->forEach = list_forEach;
	map->size = list_size;
	map->get = list_get;
	map->put = _put;
	return map;
}



void delMap(Map this) {
	if (NULL == this)
		return;
	delList(this->dataList);
	free(this);
}
