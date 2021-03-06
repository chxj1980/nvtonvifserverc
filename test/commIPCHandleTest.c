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
	int videoCount = 0;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, getVideoCount(&videoCount));
	TEST_ASSERT_TRUE(videoCount  > 0);
}

void test_GetAudioCount() {
	int audioCount;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, getAudioCount(&audioCount));
	TEST_ASSERT_TRUE(audioCount  > 0);
}

void test_GetVideoChannelInfo_1_Success() {
	OnvifVideoChannelInfo onvifVideoChannelInfo;
	onvifVideoChannelInfo.channelNo = 1;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, getVideoChannelInfo(&onvifVideoChannelInfo));
}

void test_GetVideoChannelInfo_0_Success() {
	OnvifVideoChannelInfo onvifVideoChannelInfo;
	onvifVideoChannelInfo.channelNo = 0;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, getVideoChannelInfo(&onvifVideoChannelInfo));
}

void test_GetVideoChannelStreamInfo_1_Success() {
	OnvifVideoChannelInfo onvifVideoChannelInfo;
	onvifVideoChannelInfo.channelNo = 1;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, getVideoChannelStreamInfo(&onvifVideoChannelInfo));
}

void test_GetVideoChannelStreamInfo_0_Success() {
	OnvifVideoChannelInfo onvifVideoChannelInfo;
	onvifVideoChannelInfo.channelNo = 0;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, getVideoChannelStreamInfo(&onvifVideoChannelInfo));
}

void test_GetAudioChannelInfo() {
	OnvifAudioChannelInfo onvifAudioChannelInfo;
	onvifAudioChannelInfo.channelNo = 1;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, getAudioChannelInfo(&onvifAudioChannelInfo));
}

void test_GetAudioChannelStreamInfo() {
	OnvifAudioChannelInfo onvifAudioChannelInfo;
	onvifAudioChannelInfo.channelNo = 1;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, getAudioChannelStreamInfo(&onvifAudioChannelInfo));

}

void test_SetPTZContinousMoveInfo() {
	OnvifPTZContinousMoveInfo onvifPTZContinousMoveInfo;
	memset(&onvifPTZContinousMoveInfo, 0, sizeof(OnvifPTZContinousMoveInfo));
	onvifPTZContinousMoveInfo.setPt = true;
	onvifPTZContinousMoveInfo.x = 0.1;
	onvifPTZContinousMoveInfo.y = 0;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, setPTZContinousMoveInfo(&onvifPTZContinousMoveInfo));
}

void test_RemovePreset() {
	OnvifPTZPreset onvifPTZPreset;
	memset(&onvifPTZPreset, 0, sizeof(OnvifPTZPreset));
	onvifPTZPreset.index = 1;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, removePTZPreset(&onvifPTZPreset));
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

void test_GetPresets() {
	OnvifPTZAllPresets onvifPTZAllPresets;
	memset(&onvifPTZAllPresets, 0, sizeof(OnvifPTZAllPresets));
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, getPTZAllPresets(&onvifPTZAllPresets));
	TEST_ASSERT_EQUAL(9, onvifPTZAllPresets.size);
}

void test_GetPTZPresetsCapacity() {
	int v = -1;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, getPTZPresetsCapacity(&v));
	TEST_ASSERT_EQUAL(9, v);
}

void test_SetVideoSynchronizationPoint() {
	int v = 5;
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, setVideoSynchronizationPoint(v));
}

void test_GetVideoEncoderConfigurationOptionInfo() {
	OnvifVideoEncoderConfigurationOptionInfo info;
	memset(&info, 0, sizeof(OnvifVideoEncoderConfigurationOptionInfo));
	TEST_ASSERT_EQUAL(RET_CODE_SUCCESS, getVideoEncoderConfigurationOptionInfo(&info));
}
