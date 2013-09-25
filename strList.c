/*
 * strList.c
 *
 *  Created on: 2013-9-24
 *      Author: PC01
 */

#include "strList.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
//添加一个元素到列表
void strList_add(PStrList this, const char* e) {
	if (NULL == this)
		return;
	if (NULL == e)
		return;
	char* s = strdup(e);
	this->list->addFirst(this->list, s);
}

//添加一个元素到列表尾
void strList_addLast(PStrList this, const char* e) {
	if (NULL == this)
		return;
	if (NULL == e)
		return;
	char* s = strdup(e);
	this->list->addLast(this->list, s);
}

//删除列表头部元素
void strList_removefirst(PStrList this) {
	if (NULL == this)
		return;
	this->list->removeFirst(this->list);
}

//删除列表尾部元素
void strList_removeLast(PStrList this) {
	if (NULL == this)
		return;
	this->list->removeLast(this->list);

}

char* getValueByListNode(ListNode node) {
	if (NULL == node)
		return NULL;
	return (char*) node->element;
}
//获取列表第一个节点
char* strList_first(PStrList this) {
	if (NULL == this)
		return NULL;
	return getValueByListNode(this->list->first(this->list));
}

//获取列表最后一个节点
char* strList_last(PStrList this) {
	if (NULL == this)
		return NULL;
	return getValueByListNode(this->list->last(this->list));
}

//获取列表最后一个节点
int strList_Size(PStrList this) {
	if (NULL == this)
		return 0;
	return this->list->size(this->list);
}


//遍历节点
void strList_forEach(PStrList this, void (*backfunc)(char* node, void* arg), void* arg) {
	if (NULL == this)
		return;
	ListNode node = this->list->dataNodes;
	while(NULL != node) {
		char* mapNode = node->element;
		if (NULL != mapNode) {
			backfunc(mapNode, arg);
		}
		node = node->nextNode;
	}
}

char* strList_get(PStrList this, int index) {
	if (NULL == this)
		return NULL;
	return getValueByListNode(this->list->get(this->list, index));
}

void strList_clearAll(PStrList this) {
	if (NULL == this)
		return;
	this->list->clearAll(this->list);
}
void strList_deleteIndex(PStrList this, int index) {
	if (NULL == this)
		return;
	this->list->deleteIndex(this->list, index);
}

void strList_removeNode(void* node) {
	char* mapNode = (char*) node;
	if (NULL == mapNode) {
		return;
	}
	free(mapNode);
}

//new一个List对象
PStrList newStrList() {
	PStrList L;
	L = malloc(sizeof(*L));
	if (NULL == L)
		return NULL;
	L->list = newList(strList_removeNode);
	if (NULL == L->list) {
		free(L);
		return NULL;
	}
	L->addFirst = strList_add;
	L->addLast = strList_addLast;
	L->removeFirst = strList_removefirst;
	L->removeLast = strList_removeLast;
	L->last = strList_last;
	L->first = strList_first;
	L->forEach = strList_forEach;
	L->size = strList_Size;
	L->get = strList_get;
	L->deleteIndex = strList_deleteIndex;
	L->clearAll = strList_clearAll;
	return L;
}



//删除对象
void delStrList(PStrList this) {
	if (NULL == this)
		return;
	delList(this->list);
	this->list = NULL;
	free(this);
}

