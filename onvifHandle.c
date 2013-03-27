#include "soapH.h"
#include "soapStub.h"
#include "onvifHandle.h"

#define ONVIF_SERVER_CALL()    printf("onvifs: call %s, path=%s\r\n", __FUNCTION__, soap->path)

int soap_False = 0;
int soap_True = 1;

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

