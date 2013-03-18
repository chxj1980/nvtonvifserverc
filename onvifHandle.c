#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

#include <sys/socket.h>
#include <net/if.h>
#include <net/route.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "soapH.h"
#include "soapStub.h"
#include "appTools.h"
#include "appCommon.h"

#define METADATA_VERSION    1
#define INFO_LENGTH 100
#define SMALL_INFO_LENGTH 20
#define ONVIF_SERVER_CALL()    printf("onvifs: call %s, path=%s\r\n", __FUNCTION__, soap->path)
#define VIDEO_SOURCE_TOKEN "CaoYonghuaSource_token"
#define MAX_PROF_TOKEN 20
#define LARGE_INFO_LENGTH 200

#define GSOAP_RET_CODE_NOT_IMPLEMENT -1

#define ONVIF_RETURN_OK(soap, namespaces)   \
	ONVIF_SERVER_CALL();    \
	if(namespaces!=NULL) soap_set_namespaces(soap, namespaces);  \
	return SOAP_OK;

int soap_False = 0;
int soap_True = 1;

static inline int onvif_receiver_fault_subcode_oom(struct soap *soap) {
	return soap_receiver_fault_subcode(
			soap,
			"ter:OutofMemory",
			"Out of Memory",
			"The device does not have sufficient memory to complete the action.");
}

SOAP_FMAC5 int SOAP_FMAC6 soap_send___wsdd__ProbeMatches(struct soap *soap,
		const char *soap_endpoint, const char *soap_action,
		struct wsdd__ProbeMatchesType *wsdd__ProbeMatches) {
	struct __wsdd__ProbeMatches soap_tmp___wsdd__ProbeMatches;

	if (!soap_action)
		soap_action =
				"http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01/ProbeMatches";

	soap->encodingStyle = NULL;
	soap_tmp___wsdd__ProbeMatches.wsdd__ProbeMatches = wsdd__ProbeMatches;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___wsdd__ProbeMatches(soap, &soap_tmp___wsdd__ProbeMatches);

	if (soap_begin_count(soap))
		return soap->error;

	if (soap->mode & SOAP_IO_LENGTH) {
		if (soap_envelope_begin_out(soap) || soap_putheader(soap)
				|| soap_body_begin_out(soap)
				|| soap_put___wsdd__ProbeMatches(soap,
						&soap_tmp___wsdd__ProbeMatches, "-wsdd:ProbeMatches",
						NULL) || soap_body_end_out(soap)
				|| soap_envelope_end_out(soap))
			return soap->error;
	}

	if (soap_end_count(soap))
		return soap->error;

	if (soap_connect(soap, soap_endpoint, soap_action)
			|| soap_envelope_begin_out(soap) || soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___wsdd__ProbeMatches(soap,
					&soap_tmp___wsdd__ProbeMatches, "-wsdd:ProbeMatches", NULL)
			|| soap_body_end_out(soap) || soap_envelope_end_out(soap)
			|| soap_end_send(soap))
		return soap_closesock(soap);

	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Probe(struct soap* soap,
		struct wsdd__ProbeType *wsdd__Probe) {
	char _HwId[64] = "urn:uuid:D149F919-4013-437E-B480-3707D96D27A4";

	int interface_num = 1;
	char ip_list[512] = { 0 };
	int result = getLocalIp(ip_list);
	if (RET_CODE_SUCCESS != result)
		return result;
	wsdd__ProbeMatchesType ProbeMatches;
	ProbeMatches.__sizeProbeMatch = interface_num;
	ProbeMatches.ProbeMatch = (struct wsdd__ProbeMatchType *) soap_malloc(soap,
			sizeof(struct wsdd__ProbeMatchType) * interface_num);
	int i = 0;
	for (i = 0; i < interface_num; i++) {
		ProbeMatches.ProbeMatch->MetadataVersion = 1;
		// should be onvif device mgmt address
		ProbeMatches.ProbeMatch->XAddrs = (char *) soap_malloc(soap,
				sizeof(char) * INFO_LENGTH);
		sprintf(ProbeMatches.ProbeMatch->XAddrs, "http://%s:%d/",
				ip_list + i * 20, DEVICE_WEBSERVICE_PORT);
		// probe type
		ProbeMatches.ProbeMatch->Types = (char *) soap_malloc(soap,
				sizeof(char) * INFO_LENGTH);
		strcpy(ProbeMatches.ProbeMatch->Types, "tdn:NetworkVideoTransmitter");
		// Scope
		ProbeMatches.ProbeMatch->Scopes =
				(struct wsdd__ScopesType*) soap_malloc(soap,
						sizeof(struct wsdd__ScopesType));
		ProbeMatches.ProbeMatch->Scopes->__item = (char *) soap_malloc(soap,
				1024);
		memset(ProbeMatches.ProbeMatch->Scopes->__item, 0,
				sizeof(ProbeMatches.ProbeMatch->Scopes->__item));
		strcat(ProbeMatches.ProbeMatch->Scopes->__item,
				"onvif://www.onvif.org/type/NetworkVideoTransmitter");
		ProbeMatches.ProbeMatch->Scopes->MatchBy = NULL;

		//ws-discovery鹿忙露篓 脦陋驴脡脩隆脧卯 , ReferenceProperties
		ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceProperties =
				(struct wsa__ReferencePropertiesType*) soap_malloc(soap,
						sizeof(struct wsa__ReferencePropertiesType));
		ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceProperties->__size =
				0;
		ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceProperties->__any =
				NULL;
		//ws-discovery鹿忙露篓 脦陋驴脡脩隆脧卯 , ReferenceParameters
		ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceParameters =
				(struct wsa__ReferenceParametersType*) soap_malloc(soap,
						sizeof(struct wsa__ReferenceParametersType));
		ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceParameters->__size =
				0;
		ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceParameters->__any =
				NULL;
		//ws-discovery鹿忙露篓 脦陋驴脡脩隆脧卯 , PortType
		ProbeMatches.ProbeMatch->wsa__EndpointReference.PortType =
				(char **) soap_malloc(soap, sizeof(char*) * SMALL_INFO_LENGTH);
		ProbeMatches.ProbeMatch->wsa__EndpointReference.PortType[0] =
				(char *) soap_malloc(soap, sizeof(char) * SMALL_INFO_LENGTH);
		strcpy(ProbeMatches.ProbeMatch->wsa__EndpointReference.PortType[0],
				"ttl");
		//ws-discovery鹿忙露篓 脦陋驴脡脩隆脧卯 , ServiceName
		ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName =
				(struct wsa__ServiceNameType*) soap_malloc(soap,
						sizeof(struct wsa__ServiceNameType));
		ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName->__item =
				NULL;
		ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName->PortName =
				NULL;
		ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName->__anyAttribute =
				NULL;
		//ws-discovery鹿忙露篓 脦陋驴脡脩隆脧卯 , __any
		ProbeMatches.ProbeMatch->wsa__EndpointReference.__any =
				(char **) soap_malloc(soap, sizeof(char*) * SMALL_INFO_LENGTH);
		ProbeMatches.ProbeMatch->wsa__EndpointReference.__any[0] =
				(char *) soap_malloc(soap, sizeof(char) * SMALL_INFO_LENGTH);
		strcpy(ProbeMatches.ProbeMatch->wsa__EndpointReference.__any[0], "Any");
		//ws-discovery鹿忙露篓 脦陋驴脡脩隆脧卯 , __anyAttribute
		ProbeMatches.ProbeMatch->wsa__EndpointReference.__anyAttribute =
				(char *) soap_malloc(soap, sizeof(char) * SMALL_INFO_LENGTH);
		strcpy(ProbeMatches.ProbeMatch->wsa__EndpointReference.__anyAttribute,
				"Attribute");
		ProbeMatches.ProbeMatch->wsa__EndpointReference.__size = 0;
		//ws-discovery鹿忙露篓 脦陋驴脡脩隆脧卯 , Address
		ProbeMatches.ProbeMatch->wsa__EndpointReference.Address =
				(char *) soap_malloc(soap, sizeof(char) * INFO_LENGTH);
		strcpy(ProbeMatches.ProbeMatch->wsa__EndpointReference.Address, _HwId);
	}

	if (soap->header == 0) {
		soap->header = (struct SOAP_ENV__Header*) soap_malloc(soap,
				sizeof(struct SOAP_ENV__Header));
		soap->header->wsa__RelatesTo = (struct wsa__Relationship*) soap_malloc(
				soap, sizeof(struct wsa__Relationship));
		//it's here
		soap->header->wsa__MessageID = (char *) soap_malloc(soap,
				sizeof(char) * INFO_LENGTH);
		strcpy(soap->header->wsa__MessageID, _HwId + 4);
		soap->header->wsa__RelatesTo->__item = soap->header->wsa__MessageID;
		soap->header->wsa__RelatesTo->RelationshipType = NULL;
		soap->header->wsa__RelatesTo->__anyAttribute = NULL;
	} else {
		soap->header->wsa__RelatesTo = (struct wsa__Relationship*) soap_malloc(
				soap, sizeof(struct wsa__Relationship));
		//it's here
		soap->header->wsa__RelatesTo->__item = soap->header->wsa__MessageID;
		soap->header->wsa__RelatesTo->RelationshipType = NULL;
		soap->header->wsa__RelatesTo->__anyAttribute = NULL;

		soap->header->wsa__MessageID = (char *) soap_malloc(soap,
				sizeof(char) * INFO_LENGTH);
		strcpy(soap->header->wsa__MessageID, _HwId + 4);
	}

	soap->header->wsa__From = 0;
	soap->header->wsa__ReplyTo = 0;
	soap->header->wsa__FaultTo = 0;
	soap->header->wsdd__AppSequence = 0;
	soap->header->wsa__To = (char*) soap_malloc(soap, 128);
	strcpy(soap->header->wsa__To,
			"http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous");
	soap->header->wsa__Action = (char*) soap_malloc(soap, 128);
	strcpy(soap->header->wsa__Action,
			"http://schemas.xmlsoap.org/ws/2005/04/discovery/ProbeMatches");

	/* send over current socket as HTTP OK response: */
	soap_send___wsdd__ProbeMatches(soap, "http://", NULL, &ProbeMatches);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tdn__Probe(struct soap* soap,
		struct wsdd__ProbeType tdn__Probe,
		struct wsdd__ProbeMatchesType* tdn__ProbeResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Hello(struct soap* soap,
		struct wsdd__HelloType *wsdd__Hello) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tdn__Hello(struct soap* soap,
		struct wsdd__HelloType tdn__Hello,
		struct wsdd__ResolveType* tdn__HelloResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Bye(struct soap* soap,
		struct wsdd__ByeType *wsdd__Bye) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tdn__Bye(struct soap* soap,
		struct wsdd__ByeType tdn__Bye,
		struct wsdd__ResolveType* tdn__ByeResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __wsdd__ProbeMatches(struct soap* soap,
		struct wsdd__ProbeMatchesType *wsdd__ProbeMatches) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Resolve(struct soap* soap,
		struct wsdd__ResolveType *wsdd__Resolve) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __wsdd__ResolveMatches(struct soap* soap,
		struct wsdd__ResolveMatchesType *wsdd__ResolveMatches) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetServices(struct soap* soap,
		struct _tds__GetServices *tds__GetServices,
		struct _tds__GetServicesResponse *tds__GetServicesResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetServiceCapabilities(
		struct soap* soap,
		struct _tds__GetServiceCapabilities *tds__GetServiceCapabilities,
		struct _tds__GetServiceCapabilitiesResponse *tds__GetServiceCapabilitiesResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDeviceInformation(
		struct soap* soap,
		struct _tds__GetDeviceInformation *tds__GetDeviceInformation,
		struct _tds__GetDeviceInformationResponse *tds__GetDeviceInformationResponse) {

	tds__GetDeviceInformationResponse->HardwareId = (char *) soap_malloc(soap,
			100);
	strcpy(tds__GetDeviceInformationResponse->HardwareId, "xchd01");
	tds__GetDeviceInformationResponse->FirmwareVersion = (char *) soap_malloc(
			soap, 100);
	strcpy(tds__GetDeviceInformationResponse->FirmwareVersion, "v1.0");
	tds__GetDeviceInformationResponse->Manufacturer = (char *) soap_malloc(soap,
			100);
	strcpy(tds__GetDeviceInformationResponse->Manufacturer, "caoyonghua");
	tds__GetDeviceInformationResponse->Model = (char *) soap_malloc(soap, 100);
	strcpy(tds__GetDeviceInformationResponse->Model, "ipc01");
	tds__GetDeviceInformationResponse->SerialNumber = (char *) soap_malloc(soap,
			100);
	strcpy(tds__GetDeviceInformationResponse->SerialNumber, "132423423");
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetSystemDateAndTime(
		struct soap* soap,
		struct _tds__SetSystemDateAndTime *tds__SetSystemDateAndTime,
		struct _tds__SetSystemDateAndTimeResponse *tds__SetSystemDateAndTimeResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemDateAndTime(
		struct soap* soap,
		struct _tds__GetSystemDateAndTime *tds__GetSystemDateAndTime,
		struct _tds__GetSystemDateAndTimeResponse *tds__GetSystemDateAndTimeResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetSystemFactoryDefault(
		struct soap* soap,
		struct _tds__SetSystemFactoryDefault *tds__SetSystemFactoryDefault,
		struct _tds__SetSystemFactoryDefaultResponse *tds__SetSystemFactoryDefaultResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__UpgradeSystemFirmware(
		struct soap* soap,
		struct _tds__UpgradeSystemFirmware *tds__UpgradeSystemFirmware,
		struct _tds__UpgradeSystemFirmwareResponse *tds__UpgradeSystemFirmwareResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SystemReboot(struct soap* soap,
		struct _tds__SystemReboot *tds__SystemReboot,
		struct _tds__SystemRebootResponse *tds__SystemRebootResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__RestoreSystem(struct soap* soap,
		struct _tds__RestoreSystem *tds__RestoreSystem,
		struct _tds__RestoreSystemResponse *tds__RestoreSystemResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemBackup(struct soap* soap,
		struct _tds__GetSystemBackup *tds__GetSystemBackup,
		struct _tds__GetSystemBackupResponse *tds__GetSystemBackupResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemLog(struct soap* soap,
		struct _tds__GetSystemLog *tds__GetSystemLog,
		struct _tds__GetSystemLogResponse *tds__GetSystemLogResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemSupportInformation(
		struct soap* soap,
		struct _tds__GetSystemSupportInformation *tds__GetSystemSupportInformation,
		struct _tds__GetSystemSupportInformationResponse *tds__GetSystemSupportInformationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetScopes(struct soap* soap,
		struct _tds__GetScopes *tds__GetScopes,
		struct _tds__GetScopesResponse *tds__GetScopesResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetScopes(struct soap* soap,
		struct _tds__SetScopes *tds__SetScopes,
		struct _tds__SetScopesResponse *tds__SetScopesResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__AddScopes(struct soap* soap,
		struct _tds__AddScopes *tds__AddScopes,
		struct _tds__AddScopesResponse *tds__AddScopesResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__RemoveScopes(struct soap* soap,
		struct _tds__RemoveScopes *tds__RemoveScopes,
		struct _tds__RemoveScopesResponse *tds__RemoveScopesResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDiscoveryMode(struct soap* soap,
		struct _tds__GetDiscoveryMode *tds__GetDiscoveryMode,
		struct _tds__GetDiscoveryModeResponse *tds__GetDiscoveryModeResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDiscoveryMode(struct soap* soap,
		struct _tds__SetDiscoveryMode *tds__SetDiscoveryMode,
		struct _tds__SetDiscoveryModeResponse *tds__SetDiscoveryModeResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRemoteDiscoveryMode(
		struct soap* soap,
		struct _tds__GetRemoteDiscoveryMode *tds__GetRemoteDiscoveryMode,
		struct _tds__GetRemoteDiscoveryModeResponse *tds__GetRemoteDiscoveryModeResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRemoteDiscoveryMode(
		struct soap* soap,
		struct _tds__SetRemoteDiscoveryMode *tds__SetRemoteDiscoveryMode,
		struct _tds__SetRemoteDiscoveryModeResponse *tds__SetRemoteDiscoveryModeResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDPAddresses(struct soap* soap,
		struct _tds__GetDPAddresses *tds__GetDPAddresses,
		struct _tds__GetDPAddressesResponse *tds__GetDPAddressesResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetEndpointReference(
		struct soap* soap,
		struct _tds__GetEndpointReference *tds__GetEndpointReference,
		struct _tds__GetEndpointReferenceResponse *tds__GetEndpointReferenceResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRemoteUser(struct soap* soap,
		struct _tds__GetRemoteUser *tds__GetRemoteUser,
		struct _tds__GetRemoteUserResponse *tds__GetRemoteUserResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRemoteUser(struct soap* soap,
		struct _tds__SetRemoteUser *tds__SetRemoteUser,
		struct _tds__SetRemoteUserResponse *tds__SetRemoteUserResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetUsers(struct soap* soap,
		struct _tds__GetUsers *tds__GetUsers,
		struct _tds__GetUsersResponse *tds__GetUsersResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateUsers(struct soap* soap,
		struct _tds__CreateUsers *tds__CreateUsers,
		struct _tds__CreateUsersResponse *tds__CreateUsersResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteUsers(struct soap* soap,
		struct _tds__DeleteUsers *tds__DeleteUsers,
		struct _tds__DeleteUsersResponse *tds__DeleteUsersResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetUser(struct soap* soap,
		struct _tds__SetUser *tds__SetUser,
		struct _tds__SetUserResponse *tds__SetUserResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetWsdlUrl(struct soap* soap,
		struct _tds__GetWsdlUrl *tds__GetWsdlUrl,
		struct _tds__GetWsdlUrlResponse *tds__GetWsdlUrlResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCapabilities(struct soap* soap,
		struct _tds__GetCapabilities *tds__GetCapabilities,
		struct _tds__GetCapabilitiesResponse *tds__GetCapabilitiesResponse) {
	char ip_list[512] = { 0 };
	getLocalIp(ip_list);
	tds__GetCapabilitiesResponse->Capabilities->Media = (struct tt__MediaCapabilities*)soap_malloc(soap, sizeof(struct tt__MediaCapabilities));
	tds__GetCapabilitiesResponse->Capabilities->Media->XAddr = (char *) soap_malloc(soap, sizeof(char) * LARGE_INFO_LENGTH);
	strcpy(tds__GetCapabilitiesResponse->Capabilities->Media->XAddr, ip_list);
	tds__GetCapabilitiesResponse->Capabilities->Media->StreamingCapabilities = (struct tt__RealTimeStreamingCapabilities*)soap_malloc(soap, sizeof(struct tt__RealTimeStreamingCapabilities));
	tds__GetCapabilitiesResponse->Capabilities->Media->StreamingCapabilities->RTPMulticast = (int *)soap_malloc(soap, sizeof(int));
	*tds__GetCapabilitiesResponse->Capabilities->Media->StreamingCapabilities->RTPMulticast = soap_False;
	tds__GetCapabilitiesResponse->Capabilities->Media->StreamingCapabilities->RTP_USCORETCP = (int *)soap_malloc(soap, sizeof(int));
	*tds__GetCapabilitiesResponse->Capabilities->Media->StreamingCapabilities->RTP_USCORETCP = soap_True;
	tds__GetCapabilitiesResponse->Capabilities->Media->StreamingCapabilities->RTP_USCORERTSP_USCORETCP = (int *)soap_malloc(soap, sizeof(int));
	*tds__GetCapabilitiesResponse->Capabilities->Media->StreamingCapabilities->RTP_USCORERTSP_USCORETCP = soap_True;
	tds__GetCapabilitiesResponse->Capabilities->Media->StreamingCapabilities->Extension = NULL;
	tds__GetCapabilitiesResponse->Capabilities->Media->Extension = NULL;
	tds__GetCapabilitiesResponse->Capabilities->Media->__size = 0;
	tds__GetCapabilitiesResponse->Capabilities->Media->__any = 0;

	tds__GetCapabilitiesResponse->Capabilities->Extension->DeviceIO->VideoSources = TRUE;
	tds__GetCapabilitiesResponse->Capabilities->Extension->DeviceIO->VideoOutputs = FALSE;
	tds__GetCapabilitiesResponse->Capabilities->Extension->DeviceIO->AudioSources = FALSE;
	tds__GetCapabilitiesResponse->Capabilities->Extension->DeviceIO->AudioOutputs = FALSE;
	tds__GetCapabilitiesResponse->Capabilities->Extension->DeviceIO->RelayOutputs = FALSE;
	tds__GetCapabilitiesResponse->Capabilities->Extension->DeviceIO->__size = 0;
	tds__GetCapabilitiesResponse->Capabilities->Extension->DeviceIO->__any = NULL;

	tds__GetCapabilitiesResponse->Capabilities->Extension->Display = NULL;
	tds__GetCapabilitiesResponse->Capabilities->Extension->Recording = NULL;
	tds__GetCapabilitiesResponse->Capabilities->Extension->Search = NULL;
	tds__GetCapabilitiesResponse->Capabilities->Extension->Replay = NULL;
	tds__GetCapabilitiesResponse->Capabilities->Extension->Receiver = NULL;
	tds__GetCapabilitiesResponse->Capabilities->Extension->AnalyticsDevice = NULL;
	tds__GetCapabilitiesResponse->Capabilities->Extension->Extensions = NULL;
	tds__GetCapabilitiesResponse->Capabilities->Extension->__size = 0;
	tds__GetCapabilitiesResponse->Capabilities->Extension->__any = NULL;
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDPAddresses(struct soap* soap,
		struct _tds__SetDPAddresses *tds__SetDPAddresses,
		struct _tds__SetDPAddressesResponse *tds__SetDPAddressesResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetHostname(struct soap* soap,
		struct _tds__GetHostname *tds__GetHostname,
		struct _tds__GetHostnameResponse *tds__GetHostnameResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetHostname(struct soap* soap,
		struct _tds__SetHostname *tds__SetHostname,
		struct _tds__SetHostnameResponse *tds__SetHostnameResponse) {
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetHostnameFromDHCP(
		struct soap* soap,
		struct _tds__SetHostnameFromDHCP *tds__SetHostnameFromDHCP,
		struct _tds__SetHostnameFromDHCPResponse *tds__SetHostnameFromDHCPResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDNS(struct soap* soap,
		struct _tds__GetDNS *tds__GetDNS,
		struct _tds__GetDNSResponse *tds__GetDNSResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDNS(struct soap* soap,
		struct _tds__SetDNS *tds__SetDNS,
		struct _tds__SetDNSResponse *tds__SetDNSResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNTP(struct soap* soap,
		struct _tds__GetNTP *tds__GetNTP,
		struct _tds__GetNTPResponse *tds__GetNTPResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNTP(struct soap* soap,
		struct _tds__SetNTP *tds__SetNTP,
		struct _tds__SetNTPResponse *tds__SetNTPResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDynamicDNS(struct soap* soap,
		struct _tds__GetDynamicDNS *tds__GetDynamicDNS,
		struct _tds__GetDynamicDNSResponse *tds__GetDynamicDNSResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDynamicDNS(struct soap* soap,
		struct _tds__SetDynamicDNS *tds__SetDynamicDNS,
		struct _tds__SetDynamicDNSResponse *tds__SetDynamicDNSResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkInterfaces(
		struct soap* soap,
		struct _tds__GetNetworkInterfaces *tds__GetNetworkInterfaces,
		struct _tds__GetNetworkInterfacesResponse *tds__GetNetworkInterfacesResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkInterfaces(
		struct soap* soap,
		struct _tds__SetNetworkInterfaces *tds__SetNetworkInterfaces,
		struct _tds__SetNetworkInterfacesResponse *tds__SetNetworkInterfacesResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkProtocols(
		struct soap* soap,
		struct _tds__GetNetworkProtocols *tds__GetNetworkProtocols,
		struct _tds__GetNetworkProtocolsResponse *tds__GetNetworkProtocolsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkProtocols(
		struct soap* soap,
		struct _tds__SetNetworkProtocols *tds__SetNetworkProtocols,
		struct _tds__SetNetworkProtocolsResponse *tds__SetNetworkProtocolsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkDefaultGateway(
		struct soap* soap,
		struct _tds__GetNetworkDefaultGateway *tds__GetNetworkDefaultGateway,
		struct _tds__GetNetworkDefaultGatewayResponse *tds__GetNetworkDefaultGatewayResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkDefaultGateway(
		struct soap* soap,
		struct _tds__SetNetworkDefaultGateway *tds__SetNetworkDefaultGateway,
		struct _tds__SetNetworkDefaultGatewayResponse *tds__SetNetworkDefaultGatewayResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetZeroConfiguration(
		struct soap* soap,
		struct _tds__GetZeroConfiguration *tds__GetZeroConfiguration,
		struct _tds__GetZeroConfigurationResponse *tds__GetZeroConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetZeroConfiguration(
		struct soap* soap,
		struct _tds__SetZeroConfiguration *tds__SetZeroConfiguration,
		struct _tds__SetZeroConfigurationResponse *tds__SetZeroConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetIPAddressFilter(
		struct soap* soap,
		struct _tds__GetIPAddressFilter *tds__GetIPAddressFilter,
		struct _tds__GetIPAddressFilterResponse *tds__GetIPAddressFilterResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetIPAddressFilter(
		struct soap* soap,
		struct _tds__SetIPAddressFilter *tds__SetIPAddressFilter,
		struct _tds__SetIPAddressFilterResponse *tds__SetIPAddressFilterResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__AddIPAddressFilter(
		struct soap* soap,
		struct _tds__AddIPAddressFilter *tds__AddIPAddressFilter,
		struct _tds__AddIPAddressFilterResponse *tds__AddIPAddressFilterResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__RemoveIPAddressFilter(
		struct soap* soap,
		struct _tds__RemoveIPAddressFilter *tds__RemoveIPAddressFilter,
		struct _tds__RemoveIPAddressFilterResponse *tds__RemoveIPAddressFilterResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetAccessPolicy(struct soap* soap,
		struct _tds__GetAccessPolicy *tds__GetAccessPolicy,
		struct _tds__GetAccessPolicyResponse *tds__GetAccessPolicyResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetAccessPolicy(struct soap* soap,
		struct _tds__SetAccessPolicy *tds__SetAccessPolicy,
		struct _tds__SetAccessPolicyResponse *tds__SetAccessPolicyResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateCertificate(struct soap* soap,
		struct _tds__CreateCertificate *tds__CreateCertificate,
		struct _tds__CreateCertificateResponse *tds__CreateCertificateResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificates(struct soap* soap,
		struct _tds__GetCertificates *tds__GetCertificates,
		struct _tds__GetCertificatesResponse *tds__GetCertificatesResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificatesStatus(
		struct soap* soap,
		struct _tds__GetCertificatesStatus *tds__GetCertificatesStatus,
		struct _tds__GetCertificatesStatusResponse *tds__GetCertificatesStatusResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetCertificatesStatus(
		struct soap* soap,
		struct _tds__SetCertificatesStatus *tds__SetCertificatesStatus,
		struct _tds__SetCertificatesStatusResponse *tds__SetCertificatesStatusResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteCertificates(
		struct soap* soap,
		struct _tds__DeleteCertificates *tds__DeleteCertificates,
		struct _tds__DeleteCertificatesResponse *tds__DeleteCertificatesResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetPkcs10Request(struct soap* soap,
		struct _tds__GetPkcs10Request *tds__GetPkcs10Request,
		struct _tds__GetPkcs10RequestResponse *tds__GetPkcs10RequestResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCertificates(struct soap* soap,
		struct _tds__LoadCertificates *tds__LoadCertificates,
		struct _tds__LoadCertificatesResponse *tds__LoadCertificatesResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetClientCertificateMode(
		struct soap* soap,
		struct _tds__GetClientCertificateMode *tds__GetClientCertificateMode,
		struct _tds__GetClientCertificateModeResponse *tds__GetClientCertificateModeResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetClientCertificateMode(
		struct soap* soap,
		struct _tds__SetClientCertificateMode *tds__SetClientCertificateMode,
		struct _tds__SetClientCertificateModeResponse *tds__SetClientCertificateModeResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRelayOutputs(struct soap* soap,
		struct _tds__GetRelayOutputs *tds__GetRelayOutputs,
		struct _tds__GetRelayOutputsResponse *tds__GetRelayOutputsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRelayOutputSettings(
		struct soap* soap,
		struct _tds__SetRelayOutputSettings *tds__SetRelayOutputSettings,
		struct _tds__SetRelayOutputSettingsResponse *tds__SetRelayOutputSettingsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRelayOutputState(
		struct soap* soap,
		struct _tds__SetRelayOutputState *tds__SetRelayOutputState,
		struct _tds__SetRelayOutputStateResponse *tds__SetRelayOutputStateResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SendAuxiliaryCommand(
		struct soap* soap,
		struct _tds__SendAuxiliaryCommand *tds__SendAuxiliaryCommand,
		struct _tds__SendAuxiliaryCommandResponse *tds__SendAuxiliaryCommandResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCACertificates(struct soap* soap,
		struct _tds__GetCACertificates *tds__GetCACertificates,
		struct _tds__GetCACertificatesResponse *tds__GetCACertificatesResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCertificateWithPrivateKey(
		struct soap* soap,
		struct _tds__LoadCertificateWithPrivateKey *tds__LoadCertificateWithPrivateKey,
		struct _tds__LoadCertificateWithPrivateKeyResponse *tds__LoadCertificateWithPrivateKeyResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificateInformation(
		struct soap* soap,
		struct _tds__GetCertificateInformation *tds__GetCertificateInformation,
		struct _tds__GetCertificateInformationResponse *tds__GetCertificateInformationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCACertificates(
		struct soap* soap,
		struct _tds__LoadCACertificates *tds__LoadCACertificates,
		struct _tds__LoadCACertificatesResponse *tds__LoadCACertificatesResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateDot1XConfiguration(
		struct soap* soap,
		struct _tds__CreateDot1XConfiguration *tds__CreateDot1XConfiguration,
		struct _tds__CreateDot1XConfigurationResponse *tds__CreateDot1XConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDot1XConfiguration(
		struct soap* soap,
		struct _tds__SetDot1XConfiguration *tds__SetDot1XConfiguration,
		struct _tds__SetDot1XConfigurationResponse *tds__SetDot1XConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot1XConfiguration(
		struct soap* soap,
		struct _tds__GetDot1XConfiguration *tds__GetDot1XConfiguration,
		struct _tds__GetDot1XConfigurationResponse *tds__GetDot1XConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot1XConfigurations(
		struct soap* soap,
		struct _tds__GetDot1XConfigurations *tds__GetDot1XConfigurations,
		struct _tds__GetDot1XConfigurationsResponse *tds__GetDot1XConfigurationsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteDot1XConfiguration(
		struct soap* soap,
		struct _tds__DeleteDot1XConfiguration *tds__DeleteDot1XConfiguration,
		struct _tds__DeleteDot1XConfigurationResponse *tds__DeleteDot1XConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot11Capabilities(
		struct soap* soap,
		struct _tds__GetDot11Capabilities *tds__GetDot11Capabilities,
		struct _tds__GetDot11CapabilitiesResponse *tds__GetDot11CapabilitiesResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot11Status(struct soap* soap,
		struct _tds__GetDot11Status *tds__GetDot11Status,
		struct _tds__GetDot11StatusResponse *tds__GetDot11StatusResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__ScanAvailableDot11Networks(
		struct soap* soap,
		struct _tds__ScanAvailableDot11Networks *tds__ScanAvailableDot11Networks,
		struct _tds__ScanAvailableDot11NetworksResponse *tds__ScanAvailableDot11NetworksResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemUris(struct soap* soap,
		struct _tds__GetSystemUris *tds__GetSystemUris,
		struct _tds__GetSystemUrisResponse *tds__GetSystemUrisResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__StartFirmwareUpgrade(
		struct soap* soap,
		struct _tds__StartFirmwareUpgrade *tds__StartFirmwareUpgrade,
		struct _tds__StartFirmwareUpgradeResponse *tds__StartFirmwareUpgradeResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

int  __trt__GetServiceCapabilities(struct soap* soap, struct _trt__GetServiceCapabilities *trt__GetServiceCapabilities, struct _trt__GetServiceCapabilitiesResponse *trt__GetServiceCapabilitiesResponse)
{
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__StartSystemRestore(
		struct soap* soap,
		struct _tds__StartSystemRestore *tds__StartSystemRestore,
		struct _tds__StartSystemRestoreResponse *tds__StartSystemRestoreResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSources(struct soap* soap,
		struct _trt__GetVideoSources *trt__GetVideoSources,
		struct _trt__GetVideoSourcesResponse *trt__GetVideoSourcesResponse) {
	int size1 = 1;
	trt__GetVideoSourcesResponse->__sizeVideoSources = size1;
	trt__GetVideoSourcesResponse->VideoSources =
			(struct tt__VideoSource *) soap_malloc(soap,
					sizeof(struct tt__VideoSource) * size1);
	trt__GetVideoSourcesResponse->VideoSources[0].Framerate = 30;
	trt__GetVideoSourcesResponse->VideoSources[0].Resolution =
			(struct tt__VideoResolution *) soap_malloc(soap,
					sizeof(struct tt__VideoResolution));
	trt__GetVideoSourcesResponse->VideoSources[0].Resolution->Height = 720;
	trt__GetVideoSourcesResponse->VideoSources[0].Resolution->Width = 1280;
	trt__GetVideoSourcesResponse->VideoSources[0].token = (char *) soap_malloc(
			soap, sizeof(char) * INFO_LENGTH);
	strcpy(trt__GetVideoSourcesResponse->VideoSources[0].token,
			VIDEO_SOURCE_TOKEN); //注意这里需要和GetProfile中的sourcetoken相同

	trt__GetVideoSourcesResponse->VideoSources[0].Imaging =
			(struct tt__ImagingSettings*) soap_malloc(soap,
					sizeof(struct tt__ImagingSettings));
	trt__GetVideoSourcesResponse->VideoSources[0].Imaging->Brightness =
			(float*) soap_malloc(soap, sizeof(float));
	trt__GetVideoSourcesResponse->VideoSources[0].Imaging->Brightness[0] = 128;
	trt__GetVideoSourcesResponse->VideoSources[0].Imaging->ColorSaturation =
			(float*) soap_malloc(soap, sizeof(float));
	trt__GetVideoSourcesResponse->VideoSources[0].Imaging->ColorSaturation[0] =
			128;
	trt__GetVideoSourcesResponse->VideoSources[0].Imaging->Contrast =
			(float*) soap_malloc(soap, sizeof(float));
	trt__GetVideoSourcesResponse->VideoSources[0].Imaging->Contrast[0] = 128;
	trt__GetVideoSourcesResponse->VideoSources[0].Imaging->IrCutFilter =
			(int *) soap_malloc(soap, sizeof(int));
	*trt__GetVideoSourcesResponse->VideoSources[0].Imaging->IrCutFilter = 0;
	trt__GetVideoSourcesResponse->VideoSources[0].Imaging->Sharpness =
			(float*) soap_malloc(soap, sizeof(float));
	trt__GetVideoSourcesResponse->VideoSources[0].Imaging->Sharpness[0] = 128;
	trt__GetVideoSourcesResponse->VideoSources[0].Imaging->BacklightCompensation =
			(struct tt__BacklightCompensation*) soap_malloc(soap,
					sizeof(struct tt__BacklightCompensation));
	trt__GetVideoSourcesResponse->VideoSources[0].Imaging->BacklightCompensation->Mode =
			0;
	trt__GetVideoSourcesResponse->VideoSources[0].Imaging->BacklightCompensation->Level =
			20;
	trt__GetVideoSourcesResponse->VideoSources[0].Imaging->Exposure = NULL;
	trt__GetVideoSourcesResponse->VideoSources[0].Imaging->Focus = NULL;
	trt__GetVideoSourcesResponse->VideoSources[0].Imaging->WideDynamicRange =
			(struct tt__WideDynamicRange*) soap_malloc(soap,
					sizeof(struct tt__WideDynamicRange));
	trt__GetVideoSourcesResponse->VideoSources[0].Imaging->WideDynamicRange->Mode =
			0;
	trt__GetVideoSourcesResponse->VideoSources[0].Imaging->WideDynamicRange->Level =
			20;
	trt__GetVideoSourcesResponse->VideoSources[0].Imaging->WhiteBalance =
			(struct tt__WhiteBalance*) soap_malloc(soap,
					sizeof(struct tt__WhiteBalance));
	trt__GetVideoSourcesResponse->VideoSources[0].Imaging->WhiteBalance->Mode =
			0;
	trt__GetVideoSourcesResponse->VideoSources[0].Imaging->WhiteBalance->CrGain =
			0;
	trt__GetVideoSourcesResponse->VideoSources[0].Imaging->WhiteBalance->CbGain =
			0;
	trt__GetVideoSourcesResponse->VideoSources[0].Imaging->Extension = NULL;
	trt__GetVideoSourcesResponse->VideoSources[0].Extension = NULL;
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSources(struct soap* soap,
		struct _trt__GetAudioSources *trt__GetAudioSources,
		struct _trt__GetAudioSourcesResponse *trt__GetAudioSourcesResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputs(struct soap* soap,
		struct _trt__GetAudioOutputs *trt__GetAudioOutputs,
		struct _trt__GetAudioOutputsResponse *trt__GetAudioOutputsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__CreateProfile(struct soap* soap,
		struct _trt__CreateProfile *trt__CreateProfile,
		struct _trt__CreateProfileResponse *trt__CreateProfileResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetProfile(struct soap* soap,
		struct _trt__GetProfile *trt__GetProfile,
		struct _trt__GetProfileResponse *trt__GetProfileResponse) {
	DBG("__trt__GetProfile\n");
	if(trt__GetProfile){
		DBG("trt__GetProfile=%s\n",trt__GetProfile->ProfileToken );
	}
	/*这里的ProfileToken是选定的，得到特定的profile描述*/
	//但odm单击一个profile时，需要获取，不然不会出现live video和video streaming
	trt__GetProfileResponse->Profile = (struct tt__Profile *)soap_malloc(soap,sizeof(struct tt__Profile));
	trt__GetProfileResponse->Profile->Name = (char *)soap_malloc(soap,sizeof(char)*20);
	trt__GetProfileResponse->Profile->token = (char *)soap_malloc(soap,sizeof(char)*20);
	strcpy(trt__GetProfileResponse->Profile->Name,"my_profile");
	strcpy(trt__GetProfileResponse->Profile->token,"token_profile");
	trt__GetProfileResponse->Profile->fixed = &soap_False;
	trt__GetProfileResponse->Profile->__anyAttribute = NULL;

	trt__GetProfileResponse->Profile->VideoSourceConfiguration = NULL;
	trt__GetProfileResponse->Profile->AudioSourceConfiguration = NULL;

	/*VideoEncoderConfiguration*/
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration = (struct tt__VideoEncoderConfiguration *)soap_malloc(soap,sizeof(struct tt__VideoEncoderConfiguration));
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Name = (char *)soap_malloc(soap,sizeof(char)*MAX_PROF_TOKEN);
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->token= (char *)soap_malloc(soap,sizeof(char)*MAX_PROF_TOKEN);
	strcpy(trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Name,"VE_Name");
	strcpy(trt__GetProfileResponse->Profile->VideoEncoderConfiguration->token,"VE_token");
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->UseCount = 1;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Quality = 10;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Encoding = 1;//JPEG = 0, MPEG4 = 1, H264 = 2;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Resolution = (struct tt__VideoResolution *)soap_malloc(soap, sizeof(struct tt__VideoResolution));
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Resolution->Height = 720;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Resolution->Width = 1280;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->RateControl = (struct tt__VideoRateControl *)soap_malloc(soap, sizeof(struct tt__VideoRateControl));
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->RateControl->FrameRateLimit = 30;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->RateControl->EncodingInterval = 1;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->RateControl->BitrateLimit = 500;

	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->MPEG4 = NULL;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->H264 = NULL;
#if 0
	/*可选，可以不设置*/
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->MPEG4 = (struct tt__Mpeg4Configuration *)soap_malloc(soap, sizeof(struct tt__Mpeg4Configuration));
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->MPEG4->GovLength = 1;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->MPEG4->Mpeg4Profile = 1;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->H264 = (struct tt__H264Configuration *)soap_malloc(soap, sizeof(struct tt__H264Configuration));
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->H264->GovLength = 1;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->H264->H264Profile = 1;
#endif

	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Multicast = (struct tt__MulticastConfiguration *)soap_malloc(soap, sizeof(struct tt__MulticastConfiguration));
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Multicast->Address = (struct tt__IPAddress *)soap_malloc(soap, sizeof(struct tt__IPAddress));
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Multicast->Address->IPv4Address = (char **)soap_malloc(soap, sizeof(char *));
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Multicast->Address->IPv4Address[0] = (char *)soap_malloc(soap, sizeof(char) * INFO_LENGTH);
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Multicast->Address->IPv6Address = NULL;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Multicast->Address->Type = 0;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Multicast->Port = 554;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Multicast->TTL = 60;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Multicast->AutoStart = 1;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Multicast->__size = 0;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Multicast->__any = NULL;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Multicast->__anyAttribute = NULL;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->SessionTimeout = 720000;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->__size = 0;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->__any = NULL;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->__anyAttribute = NULL;

	trt__GetProfileResponse->Profile->AudioEncoderConfiguration = NULL;
	trt__GetProfileResponse->Profile->VideoAnalyticsConfiguration = NULL;
	trt__GetProfileResponse->Profile->PTZConfiguration = NULL;
	trt__GetProfileResponse->Profile->MetadataConfiguration = NULL;
	trt__GetProfileResponse->Profile->Extension = NULL;

	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetProfiles(struct soap* soap,
		struct _trt__GetProfiles *trt__GetProfiles,
		struct _trt__GetProfilesResponse *trt__GetProfilesResponse) {
	DBG("__trt__GetProfiles\n");
		int i = 0;
		int size = 1;
		trt__GetProfilesResponse->Profiles =(struct tt__Profile *)soap_malloc(soap, sizeof(struct tt__Profile) * size);
		trt__GetProfilesResponse->__sizeProfiles = size;

		trt__GetProfilesResponse->Profiles[i].Name = (char *)soap_malloc(soap,sizeof(char)*MAX_PROF_TOKEN);
		strcpy(trt__GetProfilesResponse->Profiles[i].Name,"my_profile");
		trt__GetProfilesResponse->Profiles[i].token= (char *)soap_malloc(soap,sizeof(char)*MAX_PROF_TOKEN);
		strcpy(trt__GetProfilesResponse->Profiles[i].token,"token_profile");
		trt__GetProfilesResponse->Profiles[i].fixed = soap_False;
		trt__GetProfilesResponse->Profiles[i].__anyAttribute = NULL;
		trt__GetProfilesResponse->Profiles[i].VideoAnalyticsConfiguration = NULL;

		/*必须包含VideoEncoderConfiguration的配置
		不然不会出现live video 和 video streaming*/
		/*VideoEncoderConfiguration*/
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration = NULL;
	#if 1
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration = (struct tt__VideoEncoderConfiguration *)soap_malloc(soap,sizeof(struct tt__VideoEncoderConfiguration));
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Name = (char *)soap_malloc(soap,sizeof(char)*MAX_PROF_TOKEN);
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->token= (char *)soap_malloc(soap,sizeof(char)*MAX_PROF_TOKEN);
		strcpy(trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Name,"VE_Name1");
		strcpy(trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->token,"VE_token1");
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->UseCount = 1;
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Quality = 10;
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Encoding = 1;//JPEG = 0, MPEG4 = 1, H264 = 2;
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Resolution = (struct tt__VideoResolution *)soap_malloc(soap, sizeof(struct tt__VideoResolution));
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Resolution->Height = 720;
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Resolution->Width = 1280;
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->RateControl = (struct tt__VideoRateControl *)soap_malloc(soap, sizeof(struct tt__VideoRateControl));
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->RateControl->FrameRateLimit = 30;
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->RateControl->EncodingInterval = 1;
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->RateControl->BitrateLimit = 500;

		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->MPEG4 = NULL;
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->H264 = NULL;
	#if 1
		/*可选项，可以不配置*/
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->MPEG4 = (struct tt__Mpeg4Configuration *)soap_malloc(soap, sizeof(struct tt__Mpeg4Configuration));
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->MPEG4->GovLength = 30;
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->MPEG4->Mpeg4Profile = 1;
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->H264 = (struct tt__H264Configuration *)soap_malloc(soap, sizeof(struct tt__H264Configuration));
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->H264->GovLength = 30;
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->H264->H264Profile = 1;
	#endif
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast = (struct tt__MulticastConfiguration *)soap_malloc(soap, sizeof(struct tt__MulticastConfiguration));
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->Address = (struct tt__IPAddress *)soap_malloc(soap, sizeof(struct tt__IPAddress));
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->Address->IPv4Address = (char **)soap_malloc(soap, sizeof(char *));
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->Address->IPv4Address[0] = (char *)soap_malloc(soap, sizeof(char) * INFO_LENGTH);
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->Address->IPv6Address = NULL;
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->Address->Type = 0;
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->Port = 554;
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->TTL = 60;
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->AutoStart = 1;
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->__size = 0;
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->__any = NULL;
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->__anyAttribute = NULL;
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->SessionTimeout = 720000;
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->__size = 0;
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->__any = NULL;
		trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->__anyAttribute = NULL;
	#endif

		/* VideoSourceConfiguration */
		//trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration = NULL;
		trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration = (struct tt__VideoSourceConfiguration *)soap_malloc(soap,sizeof(struct tt__VideoSourceConfiguration ));
		trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Name = (char *)soap_malloc(soap,sizeof(char)*MAX_PROF_TOKEN);
		trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->token = (char *)soap_malloc(soap,sizeof(char)*MAX_PROF_TOKEN);
		trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->SourceToken = (char *)soap_malloc(soap,sizeof(char)*MAX_PROF_TOKEN);
		trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Bounds = (struct tt__IntRectangle *)soap_malloc(soap,sizeof(struct tt__IntRectangle));
		trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Extension = NULL;
		trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->__any = NULL;
		trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->__anyAttribute = NULL;
		trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->__size = 0;
		/*注意SourceToken*/
		strcpy(trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Name,"VS_Name");
		strcpy(trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->token,"VS_Token");
		strcpy(trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->SourceToken,"GhostyuSource_token"); /*必须与__tmd__GetVideoSources中的token相同*/
		trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->UseCount = 1;
		trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Bounds->x = 1;
		trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Bounds->y = 1;
		trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Bounds->height = 720;
		trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Bounds->width = 1280;


		trt__GetProfilesResponse->Profiles[i].AudioEncoderConfiguration = NULL;
		trt__GetProfilesResponse->Profiles[i].AudioSourceConfiguration= NULL;
		trt__GetProfilesResponse->Profiles[i].PTZConfiguration = NULL;
		trt__GetProfilesResponse->Profiles[i].MetadataConfiguration = NULL;
		trt__GetProfilesResponse->Profiles[i].Extension = NULL;

		return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoEncoderConfiguration(
		struct soap* soap,
		struct _trt__AddVideoEncoderConfiguration *trt__AddVideoEncoderConfiguration,
		struct _trt__AddVideoEncoderConfigurationResponse *trt__AddVideoEncoderConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoSourceConfiguration(
		struct soap* soap,
		struct _trt__AddVideoSourceConfiguration *trt__AddVideoSourceConfiguration,
		struct _trt__AddVideoSourceConfigurationResponse *trt__AddVideoSourceConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioEncoderConfiguration(
		struct soap* soap,
		struct _trt__AddAudioEncoderConfiguration *trt__AddAudioEncoderConfiguration,
		struct _trt__AddAudioEncoderConfigurationResponse *trt__AddAudioEncoderConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioSourceConfiguration(
		struct soap* soap,
		struct _trt__AddAudioSourceConfiguration *trt__AddAudioSourceConfiguration,
		struct _trt__AddAudioSourceConfigurationResponse *trt__AddAudioSourceConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__AddPTZConfiguration(
		struct soap* soap,
		struct _trt__AddPTZConfiguration *trt__AddPTZConfiguration,
		struct _trt__AddPTZConfigurationResponse *trt__AddPTZConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoAnalyticsConfiguration(
		struct soap* soap,
		struct _trt__AddVideoAnalyticsConfiguration *trt__AddVideoAnalyticsConfiguration,
		struct _trt__AddVideoAnalyticsConfigurationResponse *trt__AddVideoAnalyticsConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__AddMetadataConfiguration(
		struct soap* soap,
		struct _trt__AddMetadataConfiguration *trt__AddMetadataConfiguration,
		struct _trt__AddMetadataConfigurationResponse *trt__AddMetadataConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioOutputConfiguration(
		struct soap* soap,
		struct _trt__AddAudioOutputConfiguration *trt__AddAudioOutputConfiguration,
		struct _trt__AddAudioOutputConfigurationResponse *trt__AddAudioOutputConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioDecoderConfiguration(
		struct soap* soap,
		struct _trt__AddAudioDecoderConfiguration *trt__AddAudioDecoderConfiguration,
		struct _trt__AddAudioDecoderConfigurationResponse *trt__AddAudioDecoderConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoEncoderConfiguration(
		struct soap* soap,
		struct _trt__RemoveVideoEncoderConfiguration *trt__RemoveVideoEncoderConfiguration,
		struct _trt__RemoveVideoEncoderConfigurationResponse *trt__RemoveVideoEncoderConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoSourceConfiguration(
		struct soap* soap,
		struct _trt__RemoveVideoSourceConfiguration *trt__RemoveVideoSourceConfiguration,
		struct _trt__RemoveVideoSourceConfigurationResponse *trt__RemoveVideoSourceConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioEncoderConfiguration(
		struct soap* soap,
		struct _trt__RemoveAudioEncoderConfiguration *trt__RemoveAudioEncoderConfiguration,
		struct _trt__RemoveAudioEncoderConfigurationResponse *trt__RemoveAudioEncoderConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioSourceConfiguration(
		struct soap* soap,
		struct _trt__RemoveAudioSourceConfiguration *trt__RemoveAudioSourceConfiguration,
		struct _trt__RemoveAudioSourceConfigurationResponse *trt__RemoveAudioSourceConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__RemovePTZConfiguration(
		struct soap* soap,
		struct _trt__RemovePTZConfiguration *trt__RemovePTZConfiguration,
		struct _trt__RemovePTZConfigurationResponse *trt__RemovePTZConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoAnalyticsConfiguration(
		struct soap* soap,
		struct _trt__RemoveVideoAnalyticsConfiguration *trt__RemoveVideoAnalyticsConfiguration,
		struct _trt__RemoveVideoAnalyticsConfigurationResponse *trt__RemoveVideoAnalyticsConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveMetadataConfiguration(
		struct soap* soap,
		struct _trt__RemoveMetadataConfiguration *trt__RemoveMetadataConfiguration,
		struct _trt__RemoveMetadataConfigurationResponse *trt__RemoveMetadataConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioOutputConfiguration(
		struct soap* soap,
		struct _trt__RemoveAudioOutputConfiguration *trt__RemoveAudioOutputConfiguration,
		struct _trt__RemoveAudioOutputConfigurationResponse *trt__RemoveAudioOutputConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioDecoderConfiguration(
		struct soap* soap,
		struct _trt__RemoveAudioDecoderConfiguration *trt__RemoveAudioDecoderConfiguration,
		struct _trt__RemoveAudioDecoderConfigurationResponse *trt__RemoveAudioDecoderConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__DeleteProfile(struct soap* soap,
		struct _trt__DeleteProfile *trt__DeleteProfile,
		struct _trt__DeleteProfileResponse *trt__DeleteProfileResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfigurations(
		struct soap* soap,
		struct _trt__GetVideoSourceConfigurations *trt__GetVideoSourceConfigurations,
		struct _trt__GetVideoSourceConfigurationsResponse *trt__GetVideoSourceConfigurationsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfigurations(
		struct soap* soap,
		struct _trt__GetVideoEncoderConfigurations *trt__GetVideoEncoderConfigurations,
		struct _trt__GetVideoEncoderConfigurationsResponse *trt__GetVideoEncoderConfigurationsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfigurations(
		struct soap* soap,
		struct _trt__GetAudioSourceConfigurations *trt__GetAudioSourceConfigurations,
		struct _trt__GetAudioSourceConfigurationsResponse *trt__GetAudioSourceConfigurationsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfigurations(
		struct soap* soap,
		struct _trt__GetAudioEncoderConfigurations *trt__GetAudioEncoderConfigurations,
		struct _trt__GetAudioEncoderConfigurationsResponse *trt__GetAudioEncoderConfigurationsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoAnalyticsConfigurations(
		struct soap* soap,
		struct _trt__GetVideoAnalyticsConfigurations *trt__GetVideoAnalyticsConfigurations,
		struct _trt__GetVideoAnalyticsConfigurationsResponse *trt__GetVideoAnalyticsConfigurationsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfigurations(
		struct soap* soap,
		struct _trt__GetMetadataConfigurations *trt__GetMetadataConfigurations,
		struct _trt__GetMetadataConfigurationsResponse *trt__GetMetadataConfigurationsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfigurations(
		struct soap* soap,
		struct _trt__GetAudioOutputConfigurations *trt__GetAudioOutputConfigurations,
		struct _trt__GetAudioOutputConfigurationsResponse *trt__GetAudioOutputConfigurationsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfigurations(
		struct soap* soap,
		struct _trt__GetAudioDecoderConfigurations *trt__GetAudioDecoderConfigurations,
		struct _trt__GetAudioDecoderConfigurationsResponse *trt__GetAudioDecoderConfigurationsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfiguration(
		struct soap* soap,
		struct _trt__GetVideoSourceConfiguration *trt__GetVideoSourceConfiguration,
		struct _trt__GetVideoSourceConfigurationResponse *trt__GetVideoSourceConfigurationResponse) {
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfiguration(
		struct soap* soap,
		struct _trt__GetVideoEncoderConfiguration *trt__GetVideoEncoderConfiguration,
		struct _trt__GetVideoEncoderConfigurationResponse *trt__GetVideoEncoderConfigurationResponse) {
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfiguration(
		struct soap* soap,
		struct _trt__GetAudioSourceConfiguration *trt__GetAudioSourceConfiguration,
		struct _trt__GetAudioSourceConfigurationResponse *trt__GetAudioSourceConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfiguration(
		struct soap* soap,
		struct _trt__GetAudioEncoderConfiguration *trt__GetAudioEncoderConfiguration,
		struct _trt__GetAudioEncoderConfigurationResponse *trt__GetAudioEncoderConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoAnalyticsConfiguration(
		struct soap* soap,
		struct _trt__GetVideoAnalyticsConfiguration *trt__GetVideoAnalyticsConfiguration,
		struct _trt__GetVideoAnalyticsConfigurationResponse *trt__GetVideoAnalyticsConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfiguration(
		struct soap* soap,
		struct _trt__GetMetadataConfiguration *trt__GetMetadataConfiguration,
		struct _trt__GetMetadataConfigurationResponse *trt__GetMetadataConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfiguration(
		struct soap* soap,
		struct _trt__GetAudioOutputConfiguration *trt__GetAudioOutputConfiguration,
		struct _trt__GetAudioOutputConfigurationResponse *trt__GetAudioOutputConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfiguration(
		struct soap* soap,
		struct _trt__GetAudioDecoderConfiguration *trt__GetAudioDecoderConfiguration,
		struct _trt__GetAudioDecoderConfigurationResponse *trt__GetAudioDecoderConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoEncoderConfigurations(
		struct soap* soap,
		struct _trt__GetCompatibleVideoEncoderConfigurations *trt__GetCompatibleVideoEncoderConfigurations,
		struct _trt__GetCompatibleVideoEncoderConfigurationsResponse *trt__GetCompatibleVideoEncoderConfigurationsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoSourceConfigurations(
		struct soap* soap,
		struct _trt__GetCompatibleVideoSourceConfigurations *trt__GetCompatibleVideoSourceConfigurations,
		struct _trt__GetCompatibleVideoSourceConfigurationsResponse *trt__GetCompatibleVideoSourceConfigurationsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioEncoderConfigurations(
		struct soap* soap,
		struct _trt__GetCompatibleAudioEncoderConfigurations *trt__GetCompatibleAudioEncoderConfigurations,
		struct _trt__GetCompatibleAudioEncoderConfigurationsResponse *trt__GetCompatibleAudioEncoderConfigurationsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioSourceConfigurations(
		struct soap* soap,
		struct _trt__GetCompatibleAudioSourceConfigurations *trt__GetCompatibleAudioSourceConfigurations,
		struct _trt__GetCompatibleAudioSourceConfigurationsResponse *trt__GetCompatibleAudioSourceConfigurationsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoAnalyticsConfigurations(
		struct soap* soap,
		struct _trt__GetCompatibleVideoAnalyticsConfigurations *trt__GetCompatibleVideoAnalyticsConfigurations,
		struct _trt__GetCompatibleVideoAnalyticsConfigurationsResponse *trt__GetCompatibleVideoAnalyticsConfigurationsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleMetadataConfigurations(
		struct soap* soap,
		struct _trt__GetCompatibleMetadataConfigurations *trt__GetCompatibleMetadataConfigurations,
		struct _trt__GetCompatibleMetadataConfigurationsResponse *trt__GetCompatibleMetadataConfigurationsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioOutputConfigurations(
		struct soap* soap,
		struct _trt__GetCompatibleAudioOutputConfigurations *trt__GetCompatibleAudioOutputConfigurations,
		struct _trt__GetCompatibleAudioOutputConfigurationsResponse *trt__GetCompatibleAudioOutputConfigurationsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioDecoderConfigurations(
		struct soap* soap,
		struct _trt__GetCompatibleAudioDecoderConfigurations *trt__GetCompatibleAudioDecoderConfigurations,
		struct _trt__GetCompatibleAudioDecoderConfigurationsResponse *trt__GetCompatibleAudioDecoderConfigurationsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoSourceConfiguration(
		struct soap* soap,
		struct _trt__SetVideoSourceConfiguration *trt__SetVideoSourceConfiguration,
		struct _trt__SetVideoSourceConfigurationResponse *trt__SetVideoSourceConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoEncoderConfiguration(
		struct soap* soap,
		struct _trt__SetVideoEncoderConfiguration *trt__SetVideoEncoderConfiguration,
		struct _trt__SetVideoEncoderConfigurationResponse *trt__SetVideoEncoderConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioSourceConfiguration(
		struct soap* soap,
		struct _trt__SetAudioSourceConfiguration *trt__SetAudioSourceConfiguration,
		struct _trt__SetAudioSourceConfigurationResponse *trt__SetAudioSourceConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioEncoderConfiguration(
		struct soap* soap,
		struct _trt__SetAudioEncoderConfiguration *trt__SetAudioEncoderConfiguration,
		struct _trt__SetAudioEncoderConfigurationResponse *trt__SetAudioEncoderConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoAnalyticsConfiguration(
		struct soap* soap,
		struct _trt__SetVideoAnalyticsConfiguration *trt__SetVideoAnalyticsConfiguration,
		struct _trt__SetVideoAnalyticsConfigurationResponse *trt__SetVideoAnalyticsConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__SetMetadataConfiguration(
		struct soap* soap,
		struct _trt__SetMetadataConfiguration *trt__SetMetadataConfiguration,
		struct _trt__SetMetadataConfigurationResponse *trt__SetMetadataConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioOutputConfiguration(
		struct soap* soap,
		struct _trt__SetAudioOutputConfiguration *trt__SetAudioOutputConfiguration,
		struct _trt__SetAudioOutputConfigurationResponse *trt__SetAudioOutputConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioDecoderConfiguration(
		struct soap* soap,
		struct _trt__SetAudioDecoderConfiguration *trt__SetAudioDecoderConfiguration,
		struct _trt__SetAudioDecoderConfigurationResponse *trt__SetAudioDecoderConfigurationResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfigurationOptions(
		struct soap* soap,
		struct _trt__GetVideoSourceConfigurationOptions *trt__GetVideoSourceConfigurationOptions,
		struct _trt__GetVideoSourceConfigurationOptionsResponse *trt__GetVideoSourceConfigurationOptionsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfigurationOptions(
		struct soap* soap,
		struct _trt__GetVideoEncoderConfigurationOptions *trt__GetVideoEncoderConfigurationOptions,
		struct _trt__GetVideoEncoderConfigurationOptionsResponse *trt__GetVideoEncoderConfigurationOptionsResponse) {
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfigurationOptions(
		struct soap* soap,
		struct _trt__GetAudioSourceConfigurationOptions *trt__GetAudioSourceConfigurationOptions,
		struct _trt__GetAudioSourceConfigurationOptionsResponse *trt__GetAudioSourceConfigurationOptionsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfigurationOptions(
		struct soap* soap,
		struct _trt__GetAudioEncoderConfigurationOptions *trt__GetAudioEncoderConfigurationOptions,
		struct _trt__GetAudioEncoderConfigurationOptionsResponse *trt__GetAudioEncoderConfigurationOptionsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfigurationOptions(
		struct soap* soap,
		struct _trt__GetMetadataConfigurationOptions *trt__GetMetadataConfigurationOptions,
		struct _trt__GetMetadataConfigurationOptionsResponse *trt__GetMetadataConfigurationOptionsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfigurationOptions(
		struct soap* soap,
		struct _trt__GetAudioOutputConfigurationOptions *trt__GetAudioOutputConfigurationOptions,
		struct _trt__GetAudioOutputConfigurationOptionsResponse *trt__GetAudioOutputConfigurationOptionsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfigurationOptions(
		struct soap* soap,
		struct _trt__GetAudioDecoderConfigurationOptions *trt__GetAudioDecoderConfigurationOptions,
		struct _trt__GetAudioDecoderConfigurationOptionsResponse *trt__GetAudioDecoderConfigurationOptionsResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetGuaranteedNumberOfVideoEncoderInstances(
		struct soap* soap,
		struct _trt__GetGuaranteedNumberOfVideoEncoderInstances *trt__GetGuaranteedNumberOfVideoEncoderInstances,
		struct _trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse *trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetStreamUri(struct soap* soap,
		struct _trt__GetStreamUri *trt__GetStreamUri,
		struct _trt__GetStreamUriResponse *trt__GetStreamUriResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__StartMulticastStreaming(
		struct soap* soap,
		struct _trt__StartMulticastStreaming *trt__StartMulticastStreaming,
		struct _trt__StartMulticastStreamingResponse *trt__StartMulticastStreamingResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__StopMulticastStreaming(
		struct soap* soap,
		struct _trt__StopMulticastStreaming *trt__StopMulticastStreaming,
		struct _trt__StopMulticastStreamingResponse *trt__StopMulticastStreamingResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__SetSynchronizationPoint(
		struct soap* soap,
		struct _trt__SetSynchronizationPoint *trt__SetSynchronizationPoint,
		struct _trt__SetSynchronizationPointResponse *trt__SetSynchronizationPointResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetSnapshotUri(struct soap* soap,
		struct _trt__GetSnapshotUri *trt__GetSnapshotUri,
		struct _trt__GetSnapshotUriResponse *trt__GetSnapshotUriResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}
