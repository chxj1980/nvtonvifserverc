#include <unity.h>

#include "../map.h"

Map map1 = NULL;

void clearMapElement(MapElement element) {
	int* data = element;
	free(data);
}

void setUp(void) {
	map1 = newMap(clearMapElement);
}

void tearDown(void) {
	if (NULL != map1)
		delMap(map1);
}

void createTestObj(const char* key, int value) {
	int* t = malloc(sizeof(int));
	*t = value;
	map1->put(map1, key, t);
}

void test_NewMap() {
	TEST_ASSERT_NOT_EQUAL(NULL, map1);
}

void test_Size() {
	TEST_ASSERT_EQUAL(0, map1->size(map1));
	createTestObj("0", 0);
	TEST_ASSERT_EQUAL(1, map1->size(map1));
	char key[3];
	int i;
	for (i = 1; i < 10; i++) {
		sprintf(key, "%d", i);
		createTestObj(key, i);
	}
	TEST_ASSERT_EQUAL(10, map1->size(map1));
	createTestObj("2", 100);
	TEST_ASSERT_EQUAL(10, map1->size(map1));
}

void putAndTest(char* key, int value) {
	createTestObj(key, value);
	MapNode mapNode = map1->get(map1, key);
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
	map1->forEach(map1, map_for_eachp, NULL);
}

