#ifndef ONVIFHANDLE_H_
#define ONVIFHANDLE_H_

#include "appCommon.h"
#include "stdsoap2.h"

#define GSOAP_RET_CODE_NOT_IMPLEMENT -1

#define ONVIF_SCOPE_NAME "onvif://www.onvif.org/type/video_encoder onvif://www.onvif.org/type/audio_encoder onvif://www.onvif.org/type/ptz onvif://www.onvif.org/location/country/china onvif://www.onvif.org/name/NVTCYH onvif://www.onvif.org/hardware/NVT20130806-01"
#define DEFAULT_HARDWARE_ID "3707D96D27A4"
#define DEFAULT_URN_HARDWARE_ID_PREFIX "urn:uuid:D149F919-4013-437E-B480-"
#define DEVICE_TYPE "dn:NetworkVideoTransmitter"
#define VIDEO_SOURCE_TOKEN "CYH_Video_Source_token"
#define AUDIO_SOURCE_TOKEN "CYH_Audio_Source_token"
#define ALL 1
#define HOST_NAME "NVTCYH"
#define ONVIF_VERSION_MAJOR 2
#define ONVIF_VERSION_MINOR 20

#ifdef __cplusplus
extern "C" {
#endif

typedef struct St_OnvifRunParam {
	bool runFlag; // 启动onvif标志
	int servicePort; // 对应onvif服务端口
	char ip[IPV4_LEN];
	char hardwareId[SMALL_INFO_LENGTH];
	char urnHardwareId[INFO_LENGTH];
	char address[IPV4_LEN]; // 设置的地址.
	int urnIndex;
} OnvifRunParam;

typedef struct St_OnvifDeviceInfo {
	char hardwareId[SMALL_INFO_LENGTH];
	char firmwareVersion[SMALL_INFO_LENGTH];
	char manufacturer[SMALL_INFO_LENGTH];
	char model[SMALL_INFO_LENGTH];
	char serialNumber[INFO_LENGTH];
	char macAddr[SMALL_INFO_LENGTH];
} OnvifDeviceInfo;

typedef struct St_OnvifNTPInfo {
	int enable;
	char address[INFO_LENGTH];
} OnvifNTPInfo;

typedef struct St_OnvifNetCardInfo {
	int size;
	NetCardInfo netCardInfos[5];
} OnvifNetCardInfo;

typedef struct St_OnvifSystemDateTime {
	bool ntpSet;
	time_t utcTime;
	int timeZone;
} OnvifSystemDateTime;

typedef struct St_OnvifVideoChannelInfo {
	int channelNo; // channel No, start from 0
	int stream_enable; ///< 0:disable, 1:enable
	int enc_type; ///< 0:ENC_TYPE_H264, 1:ENC_TYPE_MPEG, 2:ENC_TYPE_MJPEG
	int frame_rate; ///< frame rate per second
	int bit_rate; ///< Bitrate per second
	int width; ///< width
	int height; ///< hieght
	int bright;
	int saturation;
	int contrast;
	int sharpness;
	int wbMode; // whitebalance mode 0 is auto, other
	int wbCrGain;
	int wbCbGain;
	int backLightCompMode; // BacklightCompensation mode
	int backLightCompLevel; // BacklightCompensation level
	int wideDynamicMode; // wide Dynamic range Mode
	int wideDynamicLevel; // wide Dynamic range Mode
	int govLength; //
	int videoEncodeProfile;
	int quality;
	int rtspPort;
	char videoAddr[LARGE_INFO_LENGTH];
} OnvifVideoChannelInfo;

typedef struct St_OnvifAudioChannelInfo {
	int channelNo; // channel No, start from 0
	int stream_enable; ///< 0:disable, 1:enable
	int enc_type; ///< 0:ENC_TYPE_G711, 1:ENC_TYPE_G726, 2:ENC_TYPE_AAC
	int bit_rate; ///< Bitrate per second
	int sample_size;
	int sample_rate;
	int rtspPort;
	char audioAddr[LARGE_INFO_LENGTH];
} OnvifAudioChannelInfo;

typedef struct St_OnvifPTZConfigurationInfo {
	int defaultTimeout;  // continous move default time out
} OnvifPTZConfigurationInfo;

typedef struct St_OnvifPTZContinousMoveInfo {
	bool setTimeOut;	// set timeout flag
	int timeOut;	// timeout value
	bool setZoom;	// set zoom flag
	float zoom;		// zoom value
	bool setPt;		// set pan /tilt
	float x;		// pan value
	float y;		// tilt value
} OnvifPTZContinousMoveInfo;

typedef struct St_OnvifPTZStopInfo {
	int stopPt;		// stop pan tilt
	int stopZoom;	// stop zoom
} OnvifPTZStopInfo;

typedef struct St_OnvifPTZPreset {
	int index;  // 索引号
	char name[INFO_LENGTH]; // 名称
	int error; //错误码
} OnvifPTZPreset;

typedef struct St_OnvifPTZAllPresets {
	int size;  // 长度
	OnvifPTZPreset presets[1024];  // 预制位
} OnvifPTZAllPresets;

typedef struct St_OnvifVideoResolution {
	int width;
	int height;
} OnvifVideoResolution;

typedef struct St_OnvifRange {
	int min;
	int max;
} OnvifRange;

typedef enum St_OnvifVideoH264Profile {
	H264_Baseline, H264_Main, H264_High
} OnvifVideoH264Profile;

typedef struct St_OnvifVideoEncoderConfigurationOptionInfo {
	int channelNo; // channel No, start from 0
	OnvifRange quality;
	int resolutionCount;
	OnvifVideoResolution resolutions[5];
	OnvifRange govLength;
	OnvifRange frameRate;
	OnvifRange encodingInterval;
	int profileCount;
	OnvifVideoH264Profile profiles[3];
} OnvifVideoEncoderConfigurationOptionInfo;

extern OnvifRunParam onvifRunParam;
void* my_soap_malloc(struct soap* soap, size_t n);
int startOnvifApp();
void stopOnvifApp();

int getOnvifSoapActionNotSupportCode(struct soap *soap, const char *faultInfo,
		const char* faultDetail);
int getOnvifSoapActionNotSupportSubCode1(struct soap *soap, const char* subCode1, const char *faultInfo,
		const char* faultDetail);
int getOnvifSoapActionFailedCode(struct soap *soap, const char *faultInfo,
		const char* faultDetail);
int getOnvifSoapSendInvalidArgFailedCode(struct soap *soap,
		const char *faultInfo, const char* faultDetail);
int getOnvifSoapSendInvalidArgSubCodeFailedCode(struct soap *soap, const char *faultsubcodeQName1,
		const char *faultInfo, const char* faultDetail);
int setNTPInfo(OnvifNTPInfo* info);
int getNTPInfo(OnvifNTPInfo* info);

int getDeviceInfo(OnvifDeviceInfo* info);
int getNetCardInfo(OnvifNetCardInfo* info);
int deviceReboot();
int setSystemFactoryDefault();
int getVideoCount(int* count);
int getAudioCount(int* count);
int getDeviceTime(OnvifSystemDateTime* info);
int setDeviceTime(OnvifSystemDateTime* info);
int getVideoChannelInfo(OnvifVideoChannelInfo* info);
int getVideoChannelStreamInfo(OnvifVideoChannelInfo* info);
int getAudioChannelInfo(OnvifAudioChannelInfo* info);
int getAudioChannelStreamInfo(OnvifAudioChannelInfo* info);
int setPTZStopInfo(OnvifPTZStopInfo* info);
int setPTZContinousMoveInfo(OnvifPTZContinousMoveInfo* info);
char* getPTZConfigurationToken(struct soap* soap, int index);
int getPTZConfigurationInfo(OnvifPTZConfigurationInfo* info);
int getPTZPresetsCapacity(int* info);
int getPTZAllPresets(OnvifPTZAllPresets* info);
int gotoPTZPreset(OnvifPTZPreset* info);
int removePTZPreset(OnvifPTZPreset* info);
int setPTZPreset(OnvifPTZPreset* info);
int getPTZPreset(OnvifPTZPreset* info);
int setVideoSynchronizationPoint(int index);

char* getIndexTokeName(struct soap *soap, const char* prefix, const int index);
int getIndexFromTokenName(const char* tokenName, const char* prefix);
int getVideoEncoderConfigurationOptionInfo(OnvifVideoEncoderConfigurationOptionInfo* info);

void setRunParamUrnHardwareId(char* info);
SOAP_FMAC1 int SOAP_FMAC2 getOnvifSoapSenderSubCode2Fault(struct soap *soap,
		const char *faultsubcodeQName, const char *faultsubcodeQName1, const char *faultstring,
		const char *faultdetailXML);
SOAP_FMAC1 int SOAP_FMAC2 getOnvifSoapReceiverSubCode2Fault(struct soap *soap,
		const char *faultsubcodeQName, const char *faultsubcodeQName1, const char *faultstring,
		const char *faultdetailXML);
struct tt__PTZConfiguration* getPTZConfiguration(struct soap* soap);
enum xsd__boolean * getxsdBoolean(struct soap* soap, bool value);
struct tt__IntRange* getIntRangeByOnvifRange(struct soap* soap, OnvifRange* range);

#ifdef __cplusplus
}
#endif
#endif /* ONVIFHANDLE_H_ */
