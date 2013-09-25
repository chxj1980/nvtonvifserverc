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
//���һ��Ԫ�ص��б�
void strList_add(PStrList this, const char* e) {
	if (NULL == this)
		return;
	if (NULL == e)
		return;
	char* s = strdup(e);
	this->list->addFirst(this->list, s);
}

//���һ��Ԫ�ص��б�β
void strList_addLast(PStrList this, const char* e) {
	if (NULL == this)
		return;
	if (NULL == e)
		return;
	char* s = strdup(e);
	this->list->addLast(this->list, s);
}

//ɾ���б�ͷ��Ԫ��
void strList_removefirst(PStrList this) {
	if (NULL == this)
		return;
	this->list->removeFirst(this->list);
}

//ɾ���б�β��Ԫ��
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
//��ȡ�б��һ���ڵ�
char* strList_first(PStrList this) {
	if (NULL == this)
		return NULL;
	return getValueByListNode(this->list->first(this->list));
}

//��ȡ�б����һ���ڵ�
char* strList_last(PStrList this) {
	if (NULL == this)
		return NULL;
	return getValueByListNode(this->list->last(this->list));
}

//��ȡ�б����һ���ڵ�
int strList_Size(PStrList this) {
	if (NULL == this)
		return 0;
	return this->list->size(this->list);
}


//�����ڵ�
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

//newһ��List����
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



//ɾ������
void delStrList(PStrList this) {
	if (NULL == this)
		return;
	delList(this->list);
	this->list = NULL;
	free(this);
}

