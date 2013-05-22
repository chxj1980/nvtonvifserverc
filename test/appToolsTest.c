#include <unity.h>

#include "../appTools.h"
#include "../onvifHandle.h"

void setUp(void) {

}

void tearDown(void) {

}

void test_GetLocalIp() {
	char ip[SMALL_INFO_LENGTH] = {0};
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, getLocalIp(ip));
}

void test_GetNetCardMac() {
	char mac[INFO_LENGTH] = {0};
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, getNetCardMac(mac));
}

void test_GetServiceURL() {
	char value[INFO_LENGTH] = {0};
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, getServiceURL(value, "192.168.128.234", 2203));
}

void test_IsValidHandle() {
	TEST_ASSERT_EQUAL(true, isValidHandle(100));
}

void test_ParseTimeZoneTimeStr() {
	time_t t1;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, parseTimeZoneTimeStr("2013-12-25 13:58:40", 7, 3, &t1));

}
void test_IsRetCodeSuccess() {
	TEST_ASSERT_EQUAL(true, isRetCodeSuccess(RET_CODE_SUCCESS));
}

void test_GetCurrentDateTimeStr() {
	char dtStr[INFO_LENGTH] = {0};
	getCurrentDateTimeStr(dtStr, INFO_LENGTH);
	TEST_ASSERT_TRUE(strlen(dtStr) > 0);
}

void test_GetDateTimeStr() {
	char dtStr[INFO_LENGTH] = {0};
	getDateTimeStr(dtStr, INFO_LENGTH, time(NULL));
	TEST_ASSERT_TRUE(strlen(dtStr) > 0);
}
