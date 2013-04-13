#include <cmd_type.h>
#include <unity.h>

#include "../onvifHandle.h"
#include "../commIPC.h"
#include "../logInfo.h"

void setUp(void) {
	startIPCComm();
}

void tearDown(void) {
	stopIPCComm();
}

void test_SetNTPInfo() {
	OnvifNTPInfo ntpInfo;
	strcpy(ntpInfo.address, "192.168.0.1");
	ntpInfo.enable = ENABLE_YES;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, setNTPInfo(&ntpInfo));
}

void test_GetNTPInfo() {
	OnvifNTPInfo ntpInfo;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, getNTPInfo(&ntpInfo));
}

void test_GetNetCardInfo() {
	OnvifNetCardInfo onvifNetCardInfo;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, getNetCardInfo(&onvifNetCardInfo));
}
