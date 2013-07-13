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

void test_GetDeviceInfo() {
	OnvifDeviceInfo onvifDeviceInfo;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, getDeviceInfo(&onvifDeviceInfo));
}

void test_DeviceReboot() {
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, deviceReboot());
}

void test_SetSystemFactoryDefault() {
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, setSystemFactoryDefault());
}

void test_GetVideoCount() {
	int videoCount;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, getVideoCount(&videoCount));
}

void test_GetVideoChannelInfo() {
	OnvifVideoChannelInfo onvifVideoChannelInfo;
	onvifVideoChannelInfo.channelNo = 1;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, getVideoChannelInfo(&onvifVideoChannelInfo));
}

void test_GetVideoChannelStreamInfo() {
	OnvifVideoChannelInfo onvifVideoChannelInfo;
	onvifVideoChannelInfo.channelNo = 1;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, getVideoChannelStreamInfo(&onvifVideoChannelInfo));

}

void test_SetPTZContinousMoveInfo() {
	OnvifPTZContinousMoveInfo onvifPTZContinousMoveInfo;
	memset(&onvifPTZContinousMoveInfo, 0, sizeof(OnvifPTZContinousMoveInfo));
	onvifPTZContinousMoveInfo.setPt = true;
	onvifPTZContinousMoveInfo.x = 0.1;
	onvifPTZContinousMoveInfo.y = 0;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, setPTZContinousMoveInfo(&onvifPTZContinousMoveInfo));
}

void test_SetPTZCreatePreset() {
	OnvifPTZPreset onvifPTZPreset;
	memset(&onvifPTZPreset, 0, sizeof(OnvifPTZPreset));
	onvifPTZPreset.index = 0;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, setPTZPreset(&onvifPTZPreset));
	TEST_ASSERT_EQUAL(23, onvifPTZPreset.index);
}

void test_SetPTZOverritePreset() {
	OnvifPTZPreset onvifPTZPreset;
	memset(&onvifPTZPreset, 0, sizeof(OnvifPTZPreset));
	onvifPTZPreset.index = 44;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, setPTZPreset(&onvifPTZPreset));
	TEST_ASSERT_EQUAL(44, onvifPTZPreset.index);
}

void test_SetPTZInvalidPreset() {
	OnvifPTZPreset onvifPTZPreset;
	memset(&onvifPTZPreset, 0, sizeof(OnvifPTZPreset));
	onvifPTZPreset.index = 96;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, setPTZPreset(&onvifPTZPreset));
	TEST_ASSERT_EQUAL(0, onvifPTZPreset.index);
}
