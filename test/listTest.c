#include <unity.h>

#include "../list.h"

List list1 = NULL;

void removeListElement(ListElement element) {
	int* data = element;
	free(data);
}

void setUp(void) {
	list1 = newList(removeListElement);
}

void tearDown(void) {
	if (NULL != list1)
		delList(list1);
}

void* createTestObj(int value) {
	int* t = malloc(sizeof(int));
	*t = value;
	return t;
}

void test_NewList() {
	TEST_ASSERT_NOT_EQUAL(NULL, list1);
}

void test_Size() {
	TEST_ASSERT_EQUAL(0, list1->size(list1));
	list1->addFirst(list1, createTestObj(1));
	TEST_ASSERT_EQUAL(1, list1->size(list1));
	int i;
	for (i = 1; i < 10; i++) {
		list1->addFirst(list1, createTestObj(i));
	}
	TEST_ASSERT_EQUAL(10, list1->size(list1));
}

void test_AddFirst() {
	int i;
	for (i = 0; i < 10; i++) {
		list1->addFirst(list1, createTestObj(i * 1));
	}
	ListNode node = list1->get(list1, 9);
	TEST_ASSERT_NOT_EQUAL(NULL, node);
	TEST_ASSERT_EQUAL(0, *((int*)(node->element)));
}

void test_Get() {
	int i;
	for (i = 0; i < 10; i++) {
		list1->addLast(list1, createTestObj(i * 10));
	}
	ListNode node = list1->get(list1, 1);
	TEST_ASSERT_NOT_EQUAL(NULL, node);
	TEST_ASSERT_EQUAL(10, *((int*)(node->element)));
}

void for_each_p(ListNode node, void* arg) {
	TEST_ASSERT_NOT_EQUAL(NULL, node->element);
}

void test_ForEach() {
	int i;
	for (i = 0; i < 10; i++) {
		list1->addLast(list1, createTestObj(i * 10));
	}
	list1->forEach(list1, for_each_p, NULL);
}

void test_RemoveFirst() {
	int i;
	for (i = 0; i < 10; i++) {
		list1->addLast(list1, createTestObj(i * 10));
	}
	list1->removeFirst(list1);
	ListNode node = list1->get(list1, 0);
	TEST_ASSERT_EQUAL(10, *((int*)(node->element)));
}

void test_First() {
	int i;
	for (i = 0; i < 10; i++) {
		list1->addLast(list1, createTestObj(i * 10));
	}
	ListNode node = list1->first(list1);
	TEST_ASSERT_NOT_EQUAL(NULL, node);
	TEST_ASSERT_EQUAL(0, *((int*)(node->element)));
}

void test_Last() {
	int i;
	for (i = 0; i < 10; i++) {
		list1->addLast(list1, createTestObj(i * 10));
	}
	ListNode node = list1->last(list1);
	TEST_ASSERT_NOT_EQUAL(NULL, node);
	TEST_ASSERT_EQUAL(90, *((int*)(node->element)));
}

void test_RemoveLast() {
	int i;
	for (i = 0; i < 10; i++) {
		list1->addLast(list1, createTestObj(i * 10));
	}
	ListNode node = list1->last(list1);
	TEST_ASSERT_EQUAL(90, *((int*)(node->element)));
	list1->removeLast(list1);
	node = list1->last(list1);
	TEST_ASSERT_EQUAL(80, *((int*)(node->element)));
}

void test_DeleteIndex() {
	int i;
	for (i = 0; i < 10; i++) {
		list1->addLast(list1, createTestObj(i * 10));
	}
	ListNode node = list1->get(list1, 2);
	TEST_ASSERT_EQUAL(20, *((int*)(node->element)));
	list1->deleteIndex(list1, 2);
	node = list1->get(list1, 2);
	TEST_ASSERT_EQUAL(30, *((int*)(node->element)));
}
