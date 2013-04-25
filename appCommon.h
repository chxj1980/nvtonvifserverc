#ifndef APPCOMMON_H
#define	APPCOMMON_H
#include <pthread.h>
#include "stdsoap2.h"

#define __DEBUG
//#ifdef __DEBUG
//#define DBG(fmt,args...) fprintf(stdout,  fmt,  ##args)
//#else
//#define DBG(fmt,args...)
//#endif
//#define ERRLOG(fmt,args...) fprintf(stderr,  fmt,  ##args)
//
//#define LOG(fmt,args...) fprintf(stdout,  fmt,  ##args)
#define RET_CODE_SUCCESS 0
#define RET_CODE_ERROR_UNKNOWN -100
#define RET_CODE_ERROR_NULL_VALUE -1
#define RET_CODE_ERROR_SETSOCKOPT -2
#define RET_CODE_ERROR_SOAP_BIND -3
#define RET_CODE_ERROR_CREATE_THREAD -4
#define RET_CODE_ERROR_SOAP_ACCEPT -5
#define RET_CODE_ERROR_CREATE_SOCKET -6
#define RET_CODE_ERROR_SOCKETIOCTL -7
#define RET_CODE_ERROR_NULL_OBJECT -8
#define RET_CODE_ERROR_CONNECT -9
#define RET_CODE_ERROR_NOT_RUN -10
#define RET_CODE_ERROR_SEND -11
#define RET_CODE_ERROR_RECV -12
#define RET_CODE_ERROR_NOT_SUPPORT -13
#define RET_CODE_ERROR_INVALID_VALUE -14
#define RET_CODE_ERROR_INVALID_IP -15
#define RET_CODE_ERROR_NOT_RUN -10
#define MULTI_CAST_IP "239.255.255.250"
#define MULTI_CAST_PORT 3702
#define DEVICE_WEBSERVICE_PORT 9650

#define INVALID_HANDLE -1
#define SMALL_INFO_LENGTH 20
#define INFO_LENGTH 100
#define LARGE_INFO_LENGTH 200
#define NET_CARD_NAME "eth0"
#define IPV4_LEN 20
#define TRUE 1
#define FALSE 0

#define true 1
#define false 0

#ifdef __cplusplus
extern "C" {
#endif

typedef int bool;

typedef struct St_RunInfo {
	bool active; // 激活标志
	void* param; // 附加参数
} RunInfo;

typedef struct St_RunServiceInfo {
	bool m_Active; // 激活标志
	bool m_Terminate; //
	pthread_t m_RunThread;
	struct soap m_Soap;
} RunServiceInfo;

typedef struct St_CmdParam {
	bool help;
	bool debug;
	int port;
}CmdParam;

typedef struct St_NetCardInfo {
	char ip[IPV4_LEN];
	char mac[INFO_LENGTH];
	char name[INFO_LENGTH];
	char mask[IPV4_LEN];
	char gateway[IPV4_LEN];
}NetCardInfo;

extern CmdParam cmdParam;

#ifdef __cplusplus
}
#endif
#endif
