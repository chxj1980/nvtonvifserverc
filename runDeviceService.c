#include "runDeviceService.h"
#include <pthread.h>

#include "appCommon.h"
#include "appTools.h"

RunServiceInfo deviceServiceServiceInfo;

void initDeviceService() {
	deviceServiceServiceInfo.m_Active = false;
}

void * runDeviceServiceThreadMethod() {
	while (!deviceServiceServiceInfo.m_Terminate) {
		if (!soap_valid_socket(soap_accept(&deviceServiceServiceInfo.m_Soap))) {
			LOG("device service soap accept error\n");
			return (void*) RET_CODE_ERROR_SOAP_ACCEPT;
		}
		if (soap_serve(&deviceServiceServiceInfo.m_Soap)) {
			soap_print_fault(&deviceServiceServiceInfo.m_Soap, stderr);
		}
		stopSoap(&deviceServiceServiceInfo.m_Soap);
	}
	soap_done(&deviceServiceServiceInfo.m_Soap);
	return (void*) RET_CODE_SUCCESS;
}

int startDeviceService() {
	stopDeviceService();
	soap_init(&deviceServiceServiceInfo.m_Soap);
	soap_set_namespaces(&deviceServiceServiceInfo.m_Soap, namespaces);
	DBG("startDeviceService bind\n");
	if (!soap_valid_socket(soap_bind(&deviceServiceServiceInfo.m_Soap, NULL, DEVICE_WEBSERVICE_PORT, 100))) {
		LOG("device service bind error\n");
		return RET_CODE_ERROR_SOAP_BIND;
	}
	DBG("startDeviceService thread\n");
	deviceServiceServiceInfo.m_Terminate = false;
	int err = pthread_create(&deviceServiceServiceInfo.m_RunThread, NULL,
			runDeviceServiceThreadMethod, NULL);
	if (0 != err) {
		stopSoap(&deviceServiceServiceInfo.m_Soap);
		return RET_CODE_ERROR_CREATETHREAD;
	}
	DBG("startDeviceService sfs\n");
	deviceServiceServiceInfo.m_Active = true;
	return RET_CODE_SUCCESS;
}

void stopDeviceService() {
	DBG("stopDeviceService 1\n");
	if (!deviceServiceServiceInfo.m_Active)
		return;
	DBG("stopDeviceService 2\n");
	deviceServiceServiceInfo.m_Terminate = true;
	void* status;
	pthread_join(deviceServiceServiceInfo.m_RunThread, &status);
	deviceServiceServiceInfo.m_Active = false;
}
