#include <cmd_type.h>
#include <unity.h>

#include "../onvifHandle.h"
#include "../commIPC.h"


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
