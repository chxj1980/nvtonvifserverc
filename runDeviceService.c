#include "runDeviceService.h"
#include <pthread.h>
#include <string.h>

#include "appCommon.h"
#include "appTools.h"
#include "onvifHandle.h"
#include "logInfo.h"

RunServiceInfo deviceServiceServiceInfo = { .m_Active = FALSE };

bool checkDeviceServiceSoapAcceptTimeOut() {
	bool result = false;
	const char **s;
	if (NULL != deviceServiceServiceInfo.m_Soap.fault) {
		s = soap_faultstring(&deviceServiceServiceInfo.m_Soap);
		if (0 == strcmp("Timeout", *s)) {
			result = true;
		}
	}
	return result;
}

void * runDeviceServiceThreadMethod() {
	while (!deviceServiceServiceInfo.m_Terminate) {
		if (!soap_valid_socket(soap_accept(&deviceServiceServiceInfo.m_Soap))) {
			if (checkDeviceServiceSoapAcceptTimeOut()) {
				stopSoap(&deviceServiceServiceInfo.m_Soap);
				continue;
			}
			soap_print_fault(&deviceServiceServiceInfo.m_Soap, stderr);
			stopSoap(&deviceServiceServiceInfo.m_Soap);
			return (void*) RET_CODE_ERROR_SOAP_ACCEPT;
		}
		if (soap_serve(&deviceServiceServiceInfo.m_Soap)) {
			soap_print_fault(&deviceServiceServiceInfo.m_Soap, stderr);
		}
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
	if (!soap_valid_socket(soap_bind(&deviceServiceServiceInfo.m_Soap, NULL, onvifRunParam.servicePort, 100))) {
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
