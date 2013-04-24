/*
 * map.h
 *
 *  Created on: Apr 24, 2013
 *      Author: SW-ITS-HHE
 */

#ifndef MAP_H_
#define MAP_H_

#include "list.h"

#if defined(__cplusplus)
extern "C" {
#endif

typedef struct map *Map;
typedef ListElement MapElement;

typedef struct mapnode{
	char key[100];
	MapElement element;
}*MapNode;

//无序列表
struct map{
    List dataList;
    void (*put)(Map this, const char*, MapElement);
    MapNode (*get)(Map this, const char*);
    void (*forEach)(Map this,void(*)(MapNode));          //遍历节点
    int (*size)(Map this);
    void (*removeNode)(MapNode);            //节点资源释放函数
};

Map newMap(void (*removeNode)(MapNode));      //new一个列表对象
void delMap(Map this);                 //删除一个列表对象

#if defined(__cplusplus)
}
#endif


#endif /* MAP_H_ */
