#ifndef APPCOMMON_H
#define	APPCOMMON_H
#include <pthread.h>
#include "stdsoap2.h"

#define RET_CODE_SUCCESS 0

#define RET_CODE_ERROR_NULLVALUE -1
#define RET_CODE_ERROR_SETSOCKOPT -2
#define RET_CODE_ERROR_SOAP_BIND -3
#define RET_CODE_ERROR_CREATETHREAD -4
#define RET_CODE_ERROR_SOAP_ACCEPT -5
#define RET_CODE_ERROR_CREATESOCKET -6
#define RET_CODE_ERROR_SOCKETIOCTL -7

#define RET_SOAP_ERROR_NOT_SUPPORT -100

#define MULTI_CAST_IP "239.255.255.250"
#define MULTI_CAST_PORT 3702
#define DEVICE_WEBSERVICE_PORT 8000

#define NET_CARD_NAME "eth0"

#define TRUE 1
#define FALSE 0

#define true 1
#define false 0


typedef int bool;

struct RunServiceInfo {
	bool m_Active;
	bool m_Terminate;
	pthread_t m_RunThread;
	struct soap m_Soap;
};

typedef struct RunServiceInfo RunServiceInfo;

#endif
