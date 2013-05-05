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

OnvifRunParam onvifRunParam = { .ip = { 0 }, .servicePort =
		DEVICE_WEBSERVICE_PORT, .hardwareId = DEFAULT_HARDWARE_ID,
		.urnHardwareId = { 0 } };

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

int getLocalIPInfo() {
	OnvifNetCardInfo onvifNetCardInfo;
	int result = getNetCardInfo(&onvifNetCardInfo);
	if (!isRetCodeSuccess(result)) {
		logInfo("Get Net Card Info Error");
		return result;
	}
	if (onvifNetCardInfo.size < 1) {
		result = RET_CODE_ERROR_NULL_VALUE;
		logInfo("Get Net Card Info Error");
		return result;
	}
	if (strlen(onvifRunParam.ip) < 1) {
		result = RET_CODE_ERROR_INVALID_IP;
		logInfo("Get Local IP Error");
		return result;
	}
	return RET_CODE_SUCCESS;
}

int getHardwareIdInfo() {
	sprintf(onvifRunParam.urnHardwareId, "%s%s", DEFAULT_URN_HARDWARE_ID_PREFIX,
			onvifRunParam.hardwareId);
	OnvifDeviceInfo onvifDeviceInfo;
	int result = getDeviceInfo(&onvifDeviceInfo);
	if (!isRetCodeSuccess(result)) {
		logInfo("Get Device Info Error");
		return result;
	}
	return RET_CODE_SUCCESS;
}

void* my_soap_malloc(struct soap* soap, size_t n) {
	void* result = (void*) soap_malloc(soap, n);
	memset(result, 0, n);
	return result;
}

int startOnvifApp() {
	int result = startIPCComm();
	if (!isRetCodeSuccess(result)) {
		logInfo("Connect IPC Error");
		return result;
	}
	if (!isRetCodeSuccess(getLocalIPInfo())) {
		return result;
	}
	if (!isRetCodeSuccess(getHardwareIdInfo())) {
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

int getOnvifSoapActionNotSupportCode(struct soap *soap, const char *faultInfo,
		const char* faultDetail) {
	return soap_receiver_fault_subcode(soap, "ter:ActionNotSupported",
			faultInfo, faultDetail);
}

int getOnvifSoapActionFailedCode(struct soap *soap, const char *faultInfo,
		const char* faultDetail) {
	return soap_receiver_fault_subcode(soap, "ter:Action", faultInfo,
			faultDetail);
}

int getOnvifSoapSendInvalidArgFailedCode(struct soap *soap,
		const char *faultInfo, const char* faultDetail) {
	return soap_sender_fault_subcode(soap, "ter:InvalidArgs", faultInfo,
			faultDetail);
}
