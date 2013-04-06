#include "runDeviceService.h"
#include <pthread.h>

#include "appCommon.h"
#include "appTools.h"
#include "onvifHandle.h"
#include "logInfo.h"

RunServiceInfo deviceServiceServiceInfo = { .m_Active = FALSE };

void * runDeviceServiceThreadMethod() {
	while (!deviceServiceServiceInfo.m_Terminate) {
		if (!soap_valid_socket(soap_accept(&deviceServiceServiceInfo.m_Soap))) {
			logInfo("device service soap accept error");
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
	debugInfo("startDeviceService bind");
	if (!soap_valid_socket(soap_bind(&deviceServiceServiceInfo.m_Soap, NULL, onvifRunParam.servicePort, 100))) {
		logInfo("device service bind error");
		return RET_CODE_ERROR_SOAP_BIND;
	}
	debugInfo("startDeviceService thread");
	deviceServiceServiceInfo.m_Terminate = false;
	int err = pthread_create(&deviceServiceServiceInfo.m_RunThread, NULL,
			runDeviceServiceThreadMethod, NULL);
	if (0 != err) {
		stopSoap(&deviceServiceServiceInfo.m_Soap);
		return RET_CODE_ERROR_CREATE_THREAD;
	}
	debugInfo("startDeviceService sfs");
	deviceServiceServiceInfo.m_Active = true;
	return RET_CODE_SUCCESS;
}

void stopDeviceService() {
	debugInfo("stopDeviceService 1");
	if (!deviceServiceServiceInfo.m_Active)
		return;
	debugInfo("stopDeviceService 2");
	deviceServiceServiceInfo.m_Terminate = true;

	void* status;
	pthread_join(deviceServiceServiceInfo.m_RunThread, &status);
	debugInfo("stopDeviceService 3");
	soap_done(&deviceServiceServiceInfo.m_Soap);
	deviceServiceServiceInfo.m_Active = false;
}
