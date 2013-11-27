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
	unsigned char mac[INFO_LENGTH] = {0};
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

void test_ConvertDecToHexStr() {
	char dtStr[INFO_LENGTH] = {0};
	convertDecToHexStr(7, dtStr);
	TEST_ASSERT_EQUAL_STRING("7", dtStr);
}

void test_ConvertHexStrToDec() {
	char strv[] ="9";
	int iv = 0;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, convertHexStrToDec(strv, &iv));
	TEST_ASSERT_EQUAL(9, iv);
}

void test_ParseListByDiv() {
	char strV[] = "sfsdf/sdfs1";
	PStrList list = newStrList();
	parseListByDiv(list, strV, "/");
	TEST_ASSERT_EQUAL(2, list->size(list));
	delStrList(list);
}

void test_ParseListByInvalidDiv() {
	char strV[] = "sfsdf/sdfs1";
	PStrList list = newStrList();
	parseListByDiv(list, strV, ",");
	TEST_ASSERT_EQUAL(1, list->size(list));
	delStrList(list);
}

void test_ParseListByOneDiv() {
	char strV[] = "sfsdf/";
	PStrList list = newStrList();
	parseListByDiv(list, strV, "/");
	TEST_ASSERT_EQUAL(1, list->size(list));
	delStrList(list);
}

void test_ParsePosixTimeZone() {
	char strV[] = "EST+8";
	int v = 0;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, parsePosixTimeZone(strV, &v));
	TEST_ASSERT_EQUAL(8, v);
}

void test_ParsePosixTimeZoneNeg() {
	char strV[] = "EST-24";
	int v = 0;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, parsePosixTimeZone(strV, &v));
	TEST_ASSERT_EQUAL(-24, v);
}

void test_ParseTimeStr() {
	char strV[] = "2013-11-23 02:11:33";
	time_t t;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, parseTimeStr(strV, &t));
}
