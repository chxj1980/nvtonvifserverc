/*
 * strListTest.c
 *
 *  Created on: 2013-9-24
 *      Author: PC01
 */

#include <unity.h>

#include "../strList.h"

PStrList list1 = NULL;


void setUp(void) {
	list1 = newStrList();
}

void tearDown(void) {
	if (NULL != list1) {
		delStrList(list1);
		list1 = NULL;
	}
}

void test_NewStrList() {
	TEST_ASSERT_NOT_EQUAL(NULL, list1);
}

void createObjs(int count) {
	int i;
	char v[10] = {0};
	for (i = 0; i < count; i++) {
		sprintf(v, "%d", i);
		list1->addLast(list1, v);
	}
}

void test_Size() {
	TEST_ASSERT_EQUAL(0, list1->size(list1));
	createObjs(1);
	TEST_ASSERT_EQUAL(1, list1->size(list1));
	createObjs(10);
	TEST_ASSERT_EQUAL(11, list1->size(list1));
}

void test_AddFirst() {
	createObjs(10);
	list1->addFirst(list1, "1100");
	char* node = list1->get(list1, 9);
	TEST_ASSERT_NOT_EQUAL(NULL, node);
	TEST_ASSERT_EQUAL(0, strcmp(node, "8"));
}

void test_Get() {
	createObjs(10);
	char* node = list1->get(list1, 1);
	TEST_ASSERT_NOT_EQUAL(NULL, node);
	TEST_ASSERT_EQUAL(0, strcmp(node, "1"));
}

void for_each_p(char* node, void* arg) {
	TEST_ASSERT_NOT_EQUAL(NULL, node);
}

void test_ForEach() {
	createObjs(10);
	list1->forEach(list1, for_each_p, NULL);
}

void test_RemoveFirst() {
	createObjs(10);
	list1->removeFirst(list1);
	ListNode node = list1->get(list1, 0);
	TEST_ASSERT_EQUAL(0, strcmp(node, "1"));
}

void test_First() {
	createObjs(10);
	char* node = list1->first(list1);
	TEST_ASSERT_NOT_EQUAL(NULL, node);
	TEST_ASSERT_EQUAL(0, strcmp(node, "0"));
}

void test_Last() {
	createObjs(10);
	char* node = list1->last(list1);
	TEST_ASSERT_NOT_EQUAL(NULL, node);
	TEST_ASSERT_EQUAL(0, strcmp(node, "9"));
}

void test_RemoveLast() {
	createObjs(10);
	char* node = list1->last(list1);
	TEST_ASSERT_EQUAL(0, strcmp(node, "9"));
	list1->removeLast(list1);
	node = list1->last(list1);
	TEST_ASSERT_EQUAL(0, strcmp(node, "8"));
}

void test_DeleteIndex() {
	createObjs(10);
	char* node = list1->get(list1, 2);
	TEST_ASSERT_EQUAL(0, strcmp(node, "2"));
	list1->deleteIndex(list1, 2);
	node = list1->get(list1, 2);
	TEST_ASSERT_EQUAL(0, strcmp(node, "3"));
}

void test_ClearAll() {
	createObjs(10);
	TEST_ASSERT_EQUAL(10, list1->size(list1));
	list1->clearAll(list1);
	TEST_ASSERT_EQUAL(0, list1->size(list1));
}
