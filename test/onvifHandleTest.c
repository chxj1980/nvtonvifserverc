#include <cmd_type.h>
#include <unity.h>

#include "../onvifHandle.h"
#include "../commIPC.h"
#include "../logInfo.h"

void setUp(void) {

}

void tearDown(void) {

}

void test_StartOnvifApp() {
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, startOnvifApp());
	sleep(1);
	stopOnvifApp();
}




