#include <unity.h>

#include "../map.h"

Map list1 = NULL;

void clearMapElement(MapElement element) {
	int* data = element;
	free(data);
}

void setUp(void) {
	list1 = newMap(clearMapElement);
}

void tearDown(void) {
	if (NULL != list1)
		delMap(list1);
}

void createTestObj(const char* key, int value) {
	int* t = malloc(sizeof(int));
	*t = value;
	list1->put(list1, key, t);
}

void test_NewMap() {
	TEST_ASSERT_NOT_EQUAL(NULL, list1);
}

void test_Size() {
	TEST_ASSERT_EQUAL(0, list1->size(list1));
	createTestObj("0", 0);
	TEST_ASSERT_EQUAL(1, list1->size(list1));
	char key[3];
	int i;
	for (i = 1; i < 10; i++) {
		sprintf(key, "%d", i);
		createTestObj(key, i);
	}
	TEST_ASSERT_EQUAL(10, list1->size(list1));
	createTestObj("2", 100);
	TEST_ASSERT_EQUAL(10, list1->size(list1));
}

void putAndTest(char* key, int value) {
	createTestObj(key, value);
	MapNode mapNode = list1->get(list1, key);
	TEST_ASSERT_NOT_EQUAL(NULL, mapNode);
	int* value1;
	if (NULL != mapNode) {
		value1 = mapNode->element;
		TEST_ASSERT_NOT_EQUAL(NULL, value1);
		if (NULL != value1)
			TEST_ASSERT_EQUAL(value, *value1);
	}
}

void test_PutGet() {
	putAndTest("1", 10);
	putAndTest("2", 20);
	putAndTest("1", 30);
}

void map_for_eachp(MapNode mapNode, void* arg) {
	TEST_ASSERT_NOT_EQUAL(NULL, mapNode->element);
}

void test_ForEach() {
	createTestObj("2", 100);
	createTestObj("3", 100);
	list1->forEach(list1, map_for_eachp, NULL);
}

