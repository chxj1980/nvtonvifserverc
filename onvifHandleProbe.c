#include "soapH.h"
#include "soapStub.h"
#include "appTools.h"
#include "appCommon.h"
#include "onvifHandle.h"

#define METADATA_VERSION    1

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

		//ws-discovery¹æ¶¨ Îª¿ÉÑ¡Ïî , ReferenceProperties
		ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceProperties =
				(struct wsa__ReferencePropertiesType*) soap_malloc(soap,
						sizeof(struct wsa__ReferencePropertiesType));
		ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceProperties->__size =
				0;
		ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceProperties->__any =
				NULL;
		//ws-discovery¹æ¶¨ Îª¿ÉÑ¡Ïî , ReferenceParameters
		ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceParameters =
				(struct wsa__ReferenceParametersType*) soap_malloc(soap,
						sizeof(struct wsa__ReferenceParametersType));
		ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceParameters->__size =
				0;
		ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceParameters->__any =
				NULL;
		//ws-discovery¹æ¶¨ Îª¿ÉÑ¡Ïî , PortType
		ProbeMatches.ProbeMatch->wsa__EndpointReference.PortType =
				(char **) soap_malloc(soap, sizeof(char*) * SMALL_INFO_LENGTH);
		ProbeMatches.ProbeMatch->wsa__EndpointReference.PortType[0] =
				(char *) soap_malloc(soap, sizeof(char) * SMALL_INFO_LENGTH);
		strcpy(ProbeMatches.ProbeMatch->wsa__EndpointReference.PortType[0],
				"ttl");
		//ws-discovery¹æ¶¨ Îª¿ÉÑ¡Ïî , ServiceName
		ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName =
				(struct wsa__ServiceNameType*) soap_malloc(soap,
						sizeof(struct wsa__ServiceNameType));
		ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName->__item =
				NULL;
		ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName->PortName =
				NULL;
		ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName->__anyAttribute =
				NULL;
		//ws-discovery¹æ¶¨ Îª¿ÉÑ¡Ïî , __any
		ProbeMatches.ProbeMatch->wsa__EndpointReference.__any =
				(char **) soap_malloc(soap, sizeof(char*) * SMALL_INFO_LENGTH);
		ProbeMatches.ProbeMatch->wsa__EndpointReference.__any[0] =
				(char *) soap_malloc(soap, sizeof(char) * SMALL_INFO_LENGTH);
		strcpy(ProbeMatches.ProbeMatch->wsa__EndpointReference.__any[0], "Any");
		//ws-discovery¹æ¶¨ Îª¿ÉÑ¡Ïî , __anyAttribute
		ProbeMatches.ProbeMatch->wsa__EndpointReference.__anyAttribute =
				(char *) soap_malloc(soap, sizeof(char) * SMALL_INFO_LENGTH);
		strcpy(ProbeMatches.ProbeMatch->wsa__EndpointReference.__anyAttribute,
				"Attribute");
		ProbeMatches.ProbeMatch->wsa__EndpointReference.__size = 0;
		//ws-discovery¹æ¶¨ Îª¿ÉÑ¡Ïî , Address
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

