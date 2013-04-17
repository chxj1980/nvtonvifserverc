#ifndef ONVIFHANDLE_H_
#define ONVIFHANDLE_H_

#include "appCommon.h"
#include "stdsoap2.h"

#define SMALL_INFO_LENGTH 20

#define LARGE_INFO_LENGTH 200
#define GSOAP_RET_CODE_NOT_IMPLEMENT -1

#define ONVIF_SCOPE_NAME "onvif://www.onvif.org/type/NetworkVideoTransmitter"
#define DEFAULT_HARDWARE_ID "3707D96D27A4"
#define DEFAULT_URN_HARDWARE_ID_PREFIX "urn:uuid:D149F919-4013-437E-B480-"
#define DEVICE_TYPE "tdn:NetworkVideoTransmitter"
#define VIDEO_SOURCE_TOKEN "CaoYonghuaSource_token"
#define ALL 1
#define HOST_NAME "NVTCYH"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct St_OnvifRunParam {
	bool runFlag; // 启动onvif标志
	int servicePort; // 对应onvif服务端口
	char ip[IPV4_LEN];
	char hardwareId[SMALL_INFO_LENGTH];
	char urnHardwareId[INFO_LENGTH];
} OnvifRunParam;

typedef struct St_OnvifDeviceInfo {
	char hardwareId[SMALL_INFO_LENGTH];
	char firmwareVersion[SMALL_INFO_LENGTH];
	char manufacturer[SMALL_INFO_LENGTH];
	char model[SMALL_INFO_LENGTH];
	char serialNumber[INFO_LENGTH];
} OnvifDeviceInfo;

typedef struct St_OnvifNTPInfo {
	int enable;
	char address[INFO_LENGTH];
} OnvifNTPInfo;

typedef struct St_OnvifNetCardInfo {
	int size;
	NetCardInfo netCardInfos[5];
} OnvifNetCardInfo;

extern OnvifRunParam onvifRunParam;

int startOnvifApp();
void stopOnvifApp();

int getOnvifSoapActionNotSupportCode(struct soap *soap, const char *faultInfo,
		const char* faultDetail);
int getOnvifSoapActionFailedCode(struct soap *soap, const char *faultInfo,
		const char* faultDetail);

int setNTPInfo(OnvifNTPInfo* info);
int getNTPInfo(OnvifNTPInfo* info);

int getDeviceInfo(OnvifDeviceInfo* info);
int getNetCardInfo(OnvifNetCardInfo* info);
int deviceReboot();
int restoreSystem();

#ifdef __cplusplus
}
#endif
#endif /* ONVIFHANDLE_H_ */
