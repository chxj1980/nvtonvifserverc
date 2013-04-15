#include "onvifHandle.h"
#include "soapH.h"
#include "soapStub.h"
#include "runProbeServer.h"
#include "runDeviceService.h"
#include "appCommon.h"
#include "commIPC.h"
#include "appTools.h"
#include "logInfo.h"

#define ONVIF_SERVER_CALL()    printf("onvifs: call %s, path=%s\r", __FUNCTION__, soap->path)

int soap_False = xsd__boolean__false_;
int soap_True = xsd__boolean__true_;
OnvifRunParam onvifRunParam;

#define ONVIF_RETURN_OK(soap, namespaces)   \
	ONVIF_SERVER_CALL();    \
	if(namespaces!=NULL) soap_set_namespaces(soap, namespaces);  \
	return SOAP_OK;

static inline int onvif_receiver_fault_subcode_oom(struct soap *soap) {
	return soap_receiver_fault_subcode(
			soap,
			"ter:OutofMemory",
			"Out of Memory",
			"The device does not have sufficient memory to complete the action.");
}

int startOnvifApp() {
	int result = startIPCComm();
	if (!isRetCodeSuccess(result)) {
		logInfo("Connect IPC Error");
		return result;
	}
	result = startProbeServer();
	if (!isRetCodeSuccess(result)) {
		logInfo("Start Onvif Probe Server Error");
		return result;
	}

	result = startDeviceService();
	if (!isRetCodeSuccess(result)) {
		logInfo("Start Onvif Device Service Error");
		stopProbeServer();
		return result;
	}
	return result;
}

void stopOnvifApp() {
	stopDeviceService();
	stopProbeServer();
	stopIPCComm();
}

int getOnvifSoapActionNotSupportCode(const struct soap *soap, const char *faultInfo, const char* faultDetail) {
	return soap_receiver_fault_subcode(soap, "ter:ActionNotSupported", faultInfo, faultDetail);
}
int getOnvifSoapActionFailedCode(const struct soap *soap, const char *faultInfo, const char* faultDetail) {
	return soap_receiver_fault_subcode(soap, "ter:Action", faultInfo, faultDetail);
}

