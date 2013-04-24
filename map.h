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

//�����б�
struct map{
    List dataList;
    void (*put)(Map this, const char*, MapElement);
    MapNode (*get)(Map this, const char*);
    void (*forEach)(Map this,void(*)(MapNode));          //�����ڵ�
    int (*size)(Map this);
    void (*removeNode)(MapNode);            //�ڵ���Դ�ͷź���
};

Map newMap(void (*removeNode)(MapNode));      //newһ���б����
void delMap(Map this);                 //ɾ��һ���б����

#if defined(__cplusplus)
}
#endif


#endif /* MAP_H_ */
