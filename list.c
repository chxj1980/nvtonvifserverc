#include "list.h"
#include <malloc.h>
#include <stdlib.h>

#define NULL 0

//添加一个元素到列表
void list_add(List this, ListElement e) {
	if (NULL == this)
		return;
	ListNode node = NULL;
	node = malloc(sizeof(*node));
	if (NULL == node)
		return;
	node->element = e;
	node->nextNode = this->dataNodes;
	this->dataNodes = node;
}

//添加一个元素到列表尾
void list_addLast(List this, ListElement e) {
	if (NULL == this)
		return;
	ListNode node;
	ListNode p = this->dataNodes;
	node = malloc(sizeof(*node));
	if (NULL == node)
		exit(1);
	node->element = e;
	node->nextNode = NULL;
	if (NULL == this->dataNodes)
		this->dataNodes = node;
	else {
		while (NULL != p->nextNode)
			p = p->nextNode;
		p->nextNode = node;
	}
}

//删除列表头部元素
void list_removefirst(List this) {
	ListNode node;
	if (NULL == this)
		return;
	if (NULL != this->dataNodes) {
		node = this->dataNodes;
		this->dataNodes = node->nextNode;
		this->removeNode(node);
	}
}

//删除列表尾部元素
void list_removeLast(List this) {
	ListNode node, p;
	if (NULL == this)
		exit(1);
	p = this->dataNodes;
	node = this->dataNodes;
	if (NULL != p) {
		while (NULL != node->nextNode) {
			p = node;
			node = node->nextNode;
		}
		p->nextNode = NULL;
		this->removeNode(node);
	}

}

//获取列表第一个节点
ListNode list_first(List this) {
	if (NULL == this)
		return NULL;
	return this->dataNodes;
}

//获取列表最后一个节点
ListNode list_last(List this) {
	if (NULL == this)
		return NULL;
	ListNode node;
	ListNode result = this->dataNodes;
	for (node = this->dataNodes; NULL != node; node = node->nextNode) {
		result = node;
	}
	return result;
}

//获取列表最后一个节点
int list_size(List this) {
	if (NULL == this)
		return 0;
	ListNode node;
	int result = 0;
	for (node = this->dataNodes; NULL != node; node = node->nextNode) {
		result++;
	}
	return result;
}

//遍历节点
void list_forEach(List this, void (*backfunc)(ListNode)) {
	if (NULL == this)
		return;
	ListNode node = this->dataNodes;
	while (NULL != node) {
		backfunc(node);
		node = node->nextNode;
	}
}

ListNode list_get(List this, int index) {
	if (NULL == this)
		return NULL;
	if (index < 0)
		return NULL;
	ListNode result = NULL;
	int pos = 0;
	ListNode node = this->dataNodes;
	while (NULL != node) {
		if (pos == index) {
			result = node;
			break;
		}
		node = node->nextNode;
		pos++;
	}
	return result;
}

void list_deleteIndex(List this, int index) {
	if (NULL == this)
		return;
	if (index < 0)
		return;
	if (0 == index) {
		remove(this);
		return;
	}
	ListNode result = NULL;
	int pos = 0;
	ListNode node = this->dataNodes;
	ListNode pNode = this->dataNodes;
	while (NULL != node) {
		if (pos == index) {
			pNode->nextNode = node->nextNode;
			this->removeNode(node);
			break;
		}
		pNode = node;
		node = node->nextNode;
		pos++;
	}
}

//new一个List对象
List newList(void (*_removeNode)(ListNode node)) {
	List L;
	L = malloc(sizeof(*L));
	if (NULL == L)
		return NULL;
	L->dataNodes = NULL;
	L->addFirst = list_add;
	L->addLast = list_addLast;
	L->removeFirst = list_removefirst;
	L->removeLast = list_removeLast;
	L->last = list_last;
	L->first = list_first;
	L->removeNode = _removeNode;
	L->forEach = list_forEach;
	L->size = list_size;
	L->get = list_get;
	L->deleteIndex = list_deleteIndex;
	return L;
}

//删除对象
void deList(List this) {
	if (NULL == this)
		return;
	ListNode node;
	ListNode p = this->dataNodes;
	while (p != NULL) {
		node = p->nextNode;
		this->removeNode(p);
		p = node;
	}
	free(this);

}

