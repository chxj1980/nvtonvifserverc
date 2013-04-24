#include "list.h"
#include <malloc.h>
#include <stdlib.h>

#define NULL 0

//���һ��Ԫ�ص��б�
void _add(List this, Element e) {
	if (NULL == this)
		return;
	Node node = NULL;
	node = malloc(sizeof(*node));
	if (NULL == node)
		return;
	node->element = e;
	node->nextNode = this->dataNodes;
	this->dataNodes = node;
}

//���һ��Ԫ�ص��б�β
void _addLast(List this, Element e) {
	if (NULL == this)
		return;
	Node node;
	Node p = this->dataNodes;
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

//ɾ���б�ͷ��Ԫ��
void _remove(List this) {
	Node node;
	if (NULL == this)
		return;
	if (NULL != this->dataNodes) {
		node = this->dataNodes;
		this->dataNodes = node->nextNode;
		this->removeNode(node);
	}
}

//ɾ���б�β��Ԫ��
void _removeLast(List this) {
	Node node, p;
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

//��ȡ�б��һ���ڵ�
Node _first(List this) {
	if (NULL == this)
		return NULL;
	return this->dataNodes;
}

//��ȡ�б����һ���ڵ�
Node _last(List this) {
	if (NULL == this)
		return NULL;
	Node node;
	Node result = this->dataNodes;
	for (node = this->dataNodes; NULL != node; node = node->nextNode) {
		result = node;
	}
	return result;
}

//��ȡ�б����һ���ڵ�
int _size(List this) {
	if (NULL == this)
		return 0;
	Node node;
	int result = 0;
	for (node = this->dataNodes; NULL != node; node = node->nextNode) {
		result++;
	}
	return result;
}

//�����ڵ�
void _forEach(List this, void (*backfunc)(Node)) {
	if (NULL == this)
		return;
	Node node = this->dataNodes;
	while (NULL != node) {
		backfunc(node);
		node = node->nextNode;
	}
}

Node _get(List this, int index) {
	if (NULL == this)
		return NULL;
	if (index < 0)
		return NULL;
	Node result = NULL;
	int pos = 0;
	Node node = this->dataNodes;
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

void _deleteIndex(List this, int index) {
	if (NULL == this)
		return;
	if (index < 0)
		return;
	if (0 == index) {
		remove(this);
		return;
	}
	Node result = NULL;
	int pos = 0;
	Node node = this->dataNodes;
	Node pNode = this->dataNodes;
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

//newһ��List����
List newList(void (*_removeNode)(Node node)) {
	List L;
	L = malloc(sizeof(*L));
	if (NULL == L)
		return NULL;
	L->dataNodes = NULL;
	L->addFirst = _add;
	L->addLast = _addLast;
	L->removeFirst = _remove;
	L->removeLast = _removeLast;
	L->last = _last;
	L->first = _first;
	L->removeNode = _removeNode;
	L->forEach = _forEach;
	L->size = _size;
	L->get = _get;
	L->deleteIndex = _deleteIndex;
	return L;
}

//ɾ������
void deList(List this) {
	if (NULL == this)
		return;
	Node node;
	Node p = this->dataNodes;
	while (p != NULL) {
		node = p->nextNode;
		this->removeNode(p);
		p = node;
	}
	free(this);

}

