#ifndef ONVIFHANDLE_H_
#define ONVIFHANDLE_H_

#include "appCommon.h"


#define SMALL_INFO_LENGTH 20

#define LARGE_INFO_LENGTH 200
#define GSOAP_RET_CODE_NOT_IMPLEMENT -1

#define ONVIF_SCOPE_NAME "onvif://www.onvif.org/type/NetworkVideoTransmitter"

#define URN_HARDWARE_ID "urn:uuid:D149F919-4013-437E-B480-3707D96D27A4"
#define HARDWARE_ID "3707D96D27A4"
#define DEVICE_TYPE "tdn:NetworkVideoTransmitter"
#define VIDEO_SOURCE_TOKEN "CaoYonghuaSource_token"
#define ALL 1
#define HOST_NAME "NVTCYH"

#ifdef __cplusplus
	extern "C" {
#endif

extern int soap_False;
extern int soap_True;

typedef struct St_OnvifRunParam{
	bool runFlag;  // 启动onvif标志
	int servicePort;  // 对应onvif服务端口
}OnvifRunParam;

extern OnvifRunParam onvifRunParam;

int startOnvifApp();
void stopOnvifApp();

#ifdef __cplusplus
	}
#endif
#endif /* ONVIFHANDLE_H_ */
