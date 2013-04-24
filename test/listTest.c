#include <unity.h>

#include "../list.h"

List map1 = NULL;

void removeListElement(ListElement element) {
	int* data = element;
	free(data);
}

void setUp(void) {
	map1 = newList(removeListElement);
}

void tearDown(void) {
	if (NULL != map1)
		delList(map1);
}

void* createTestObj(int value) {
	int* t = malloc(sizeof(int));
	*t = value;
	return t;
}

void test_NewList() {
	TEST_ASSERT_NOT_EQUAL(NULL, map1);
}

void test_Size() {
	TEST_ASSERT_EQUAL(0, map1->size(map1));
	map1->addFirst(map1, createTestObj(1));
	TEST_ASSERT_EQUAL(1, map1->size(map1));
	int i;
	for (i = 1; i < 10; i++) {
		map1->addFirst(map1, createTestObj(i));
	}
	TEST_ASSERT_EQUAL(10, map1->size(map1));
}

void test_AddFirst() {
	int i;
	for (i = 0; i < 10; i++) {
		map1->addFirst(map1, createTestObj(i * 1));
	}
	ListNode node = map1->get(map1, 9);
	TEST_ASSERT_NOT_EQUAL(NULL, node);
	TEST_ASSERT_EQUAL(0, *((int*)(node->element)));
}

void test_Get() {
	int i;
	for (i = 0; i < 10; i++) {
		map1->addLast(map1, createTestObj(i * 10));
	}
	ListNode node = map1->get(map1, 1);
	TEST_ASSERT_NOT_EQUAL(NULL, node);
	TEST_ASSERT_EQUAL(10, *((int*)(node->element)));
}

void for_each_p(ListNode node) {
	TEST_ASSERT_NOT_EQUAL(NULL, node->element);
}

void test_ForEach() {
	int i;
	for (i = 0; i < 10; i++) {
		map1->addLast(map1, createTestObj(i * 10));
	}
	map1->forEach(map1, for_each_p);
}

void test_RemoveFirst() {
	int i;
	for (i = 0; i < 10; i++) {
		map1->addLast(map1, createTestObj(i * 10));
	}
	map1->removeFirst(map1);
	ListNode node = map1->get(map1, 0);
	TEST_ASSERT_EQUAL(10, *((int*)(node->element)));
}

void test_First() {
	int i;
	for (i = 0; i < 10; i++) {
		map1->addLast(map1, createTestObj(i * 10));
	}
	ListNode node = map1->first(map1);
	TEST_ASSERT_NOT_EQUAL(NULL, node);
	TEST_ASSERT_EQUAL(0, *((int*)(node->element)));
}

void test_Last() {
	int i;
	for (i = 0; i < 10; i++) {
		map1->addLast(map1, createTestObj(i * 10));
	}
	ListNode node = map1->last(map1);
	TEST_ASSERT_NOT_EQUAL(NULL, node);
	TEST_ASSERT_EQUAL(90, *((int*)(node->element)));
}

void test_RemoveLast() {
	int i;
	for (i = 0; i < 10; i++) {
		map1->addLast(map1, createTestObj(i * 10));
	}
	ListNode node = map1->last(map1);
	TEST_ASSERT_EQUAL(90, *((int*)(node->element)));
	map1->removeLast(map1);
	node = map1->last(map1);
	TEST_ASSERT_EQUAL(80, *((int*)(node->element)));
}

void test_DeleteIndex() {
	int i;
	for (i = 0; i < 10; i++) {
		map1->addLast(map1, createTestObj(i * 10));
	}
	ListNode node = map1->get(map1, 2);
	TEST_ASSERT_EQUAL(20, *((int*)(node->element)));
	map1->deleteIndex(map1, 2);
	node = map1->get(map1, 2);
	TEST_ASSERT_EQUAL(30, *((int*)(node->element)));
}
