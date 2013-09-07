#include "onvifHandle.h"
#include "soapH.h"
#include "soapStub.h"
#include "stdsoap2.h"
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

static int my_soap_set_error(struct soap *soap, const char *faultcode,
		const char *faultsubcodeQName, const char *faultstring,
		const char *faultdetailXML, int soaperror) {
	*soap_faultcode(soap) = faultcode;
	if (faultsubcodeQName)
		*soap_faultsubcode(soap) = faultsubcodeQName;
	*soap_faultstring(soap) = faultstring;
	if (faultdetailXML && *faultdetailXML) {
		register const char **s = soap_faultdetail(soap);
		if (s)
			*s = faultdetailXML;
	}
	return soap->error = soaperror;
}

static int my_soap_copy_fault(struct soap *soap, const char *faultcode,
		const char *faultsubcodeQName, const char *faultstring,
		const char *faultdetailXML) {
	char *r = NULL, *s = NULL, *t = NULL;
	if (faultsubcodeQName)
		r = soap_strdup(soap, faultsubcodeQName);
	if (faultstring)
		s = soap_strdup(soap, faultstring);
	if (faultdetailXML)
		t = soap_strdup(soap, faultdetailXML);
	return my_soap_set_error(soap, faultcode, r, s, t, SOAP_FAULT);
}

static int my_soap_subcode2_fault(struct soap *soap, const char *faultcode,
		const char *faultsubcodeQName, const char *faultsubcodeQName1,
		const char *faultstring, const char *faultdetailXML) {
	int result = my_soap_copy_fault(soap, faultcode, faultsubcodeQName,
			faultstring, faultdetailXML);
	soap->fault->SOAP_ENV__Code->SOAP_ENV__Subcode->SOAP_ENV__Subcode =
			(struct SOAP_ENV__Code*) my_soap_malloc(soap,
					(sizeof(struct SOAP_ENV__Code)));
	soap->fault->SOAP_ENV__Code->SOAP_ENV__Subcode->SOAP_ENV__Subcode->SOAP_ENV__Value =
			soap_strdup(soap, faultsubcodeQName1);
	return result;
}

int getOnvifSoapActionNotSupportCode(struct soap *soap, const char *faultInfo,
		const char* faultDetail) {
	if (NULL == faultDetail)
		return soap_receiver_fault_subcode(soap, "ter:ActionNotSupported",
				faultInfo, "not support");
	else
		return soap_receiver_fault_subcode(soap, "ter:ActionNotSupported",
				faultInfo, faultDetail);
}

int getOnvifSoapActionNotSupportSubCode1(struct soap *soap,
		const char* subCode1, const char *faultInfo, const char* faultDetail) {
	if (NULL == faultDetail)
		return getOnvifSoapReceiverSubCode2Fault(soap, "ter:ActionNotSupported",
				subCode1, faultInfo, "not support");
	else
		return getOnvifSoapReceiverSubCode2Fault(soap, "ter:ActionNotSupported",
				subCode1, faultInfo, faultDetail);
}

SOAP_FMAC1 int SOAP_FMAC2 getOnvifSoapReceiverSubCode2Fault(struct soap *soap,
		const char *faultsubcodeQName, const char *faultsubcodeQName1,
		const char *faultstring, const char *faultdetailXML) {
	return my_soap_subcode2_fault(soap,
			soap->version == 2 ? "SOAP-ENV:Receiver" : "SOAP-ENV:Server",
			faultsubcodeQName, faultsubcodeQName1, faultstring, faultdetailXML);
}

SOAP_FMAC1 int SOAP_FMAC2 getOnvifSoapSenderSubCode2Fault(struct soap *soap,
		const char *faultsubcodeQName, const char *faultsubcodeQName1,
		const char *faultstring, const char *faultdetailXML) {
	return my_soap_subcode2_fault(soap,
			soap->version == 2 ? "SOAP-ENV:Sender" : "SOAP-ENV:Client",
			faultsubcodeQName, faultsubcodeQName1, faultstring, faultdetailXML);
}

int getIndexFromTokenName(const char* tokenName, const char* prefix) {
	if (NULL == tokenName)
		return RET_CODE_ERROR_NULL_OBJECT;
	if (strlen(tokenName) <= strlen(prefix))
		return RET_CODE_ERROR_INVALID_VALUE;
	if (NULL == strstr(tokenName, prefix))
		return RET_CODE_ERROR_INVALID_VALUE;
	return atoi((char*) (tokenName + strlen(prefix)));
}

char* getIndexTokeName(struct soap *soap, const char* prefix, const int index) {
	char* result = (char *) my_soap_malloc(soap, sizeof(char) * INFO_LENGTH);
	sprintf(result, "%s%d", prefix, index);
	return result;
}

enum xsd__boolean * getxsdBoolean(struct soap* soap, bool value) {
	enum xsd__boolean * result = (enum xsd__boolean *) my_soap_malloc(soap,
			sizeof(enum xsd__boolean));
	*result = value ? xsd__boolean__true_ : xsd__boolean__false_;
	return result;
}
