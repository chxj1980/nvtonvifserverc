#include "runDeviceService.h"
#include <pthread.h>
#include <string.h>

#include "appCommon.h"
#include "appTools.h"
#include "onvifHandle.h"
#include "logInfo.h"

#define BACKLOG 100

RunServiceInfo deviceServiceServiceInfo = { .m_Active = FALSE };

bool checkDeviceServiceSoapAcceptTimeOut() {
	return (!deviceServiceServiceInfo.m_Soap.errnum);
}

void * runDeviceServiceThreadMethod() {
	bool stopSoapF = false;
	while (!deviceServiceServiceInfo.m_Terminate) {
		usleep(10000);
		stopSoapF = false;
		if (!soap_valid_socket(soap_accept(&deviceServiceServiceInfo.m_Soap))) {
			if (checkDeviceServiceSoapAcceptTimeOut()) {
				stopSoap(&deviceServiceServiceInfo.m_Soap);
				stopSoapF = true;
				continue;
			}
			soap_print_fault(&deviceServiceServiceInfo.m_Soap, stderr);
			stopSoap(&deviceServiceServiceInfo.m_Soap);
			stopSoapF = true;
			return (void*) RET_CODE_ERROR_SOAP_ACCEPT;
		}
		if (soap_serve(&deviceServiceServiceInfo.m_Soap)) {
			soap_print_fault(&deviceServiceServiceInfo.m_Soap, stderr);
		}

		stopSoap(&deviceServiceServiceInfo.m_Soap);
		stopSoapF = true;
	}
	if (!stopSoapF) {
		stopSoap(&deviceServiceServiceInfo.m_Soap);
	}
	return (void*) RET_CODE_SUCCESS;
}

int startDeviceService() {
	stopDeviceService();
	soap_init(&deviceServiceServiceInfo.m_Soap);
	soap_set_namespaces(&deviceServiceServiceInfo.m_Soap, namespaces);
	deviceServiceServiceInfo.m_Soap.accept_timeout = SOAP_ACCEPT_TIMEOUT;
	debugInfo("startDeviceService bind");
	if (!soap_valid_socket(soap_bind(&deviceServiceServiceInfo.m_Soap, NULL, onvifRunParam.servicePort, BACKLOG))) {
		logInfo("device service bind error");
		return RET_CODE_ERROR_SOAP_BIND;
	}
	deviceServiceServiceInfo.m_Terminate = false;
	int err = pthread_create(&deviceServiceServiceInfo.m_RunThread, NULL,
			runDeviceServiceThreadMethod, NULL);
	if (0 != err) {
		stopSoap(&deviceServiceServiceInfo.m_Soap);
		soap_done(&deviceServiceServiceInfo.m_Soap);
		return RET_CODE_ERROR_CREATE_THREAD;
	}
	deviceServiceServiceInfo.m_Active = true;
	return RET_CODE_SUCCESS;
}

void stopDeviceService() {
	if (!deviceServiceServiceInfo.m_Active)
		return;
	deviceServiceServiceInfo.m_Terminate = true;
	void* status;
	pthread_join(deviceServiceServiceInfo.m_RunThread, &status);
	soap_done(&deviceServiceServiceInfo.m_Soap);
	logInfo("stopDeviceService success");
	deviceServiceServiceInfo.m_Active = false;
}
