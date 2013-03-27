#include "soapH.h"
#include "appTools.h"
#include "appCommon.h"
#include "onvifHandle.h"
#include "threads.h"

#define METADATA_VERSION    1
#define SOAP_WSA_ID "SOAP-WSA-1.3"
const char soap_wsa_id[] = SOAP_WSA_ID;

static MUTEX_TYPE soap_wsdd_state = MUTEX_INITIALIZER;

static unsigned int soap_wsdd_InstanceId = 0;
static const char *soap_wsdd_SequenceId = NULL;
static unsigned int soap_wsdd_MessageNumber = 1;

#if defined(SOAP_WSA_2003)
/** Anonymous Reply/To endpoint address */
const char *soap_wsa_anonymousURI = "http://schemas.xmlsoap.org/ws/2003/03/addressing/role/anonymous";
/** Specifies no Reply endpoint address (no reply) */
const char *soap_wsa_noneURI = "addressing/none not supported";
const char *soap_wsa_faultAction = "http://schemas.xmlsoap.org/ws/2003/03/addressing/fault";
#elif defined(SOAP_WSA_2004)
/** Anonymous Reply/To endpoint address */
const char *soap_wsa_anonymousURI = "http://schemas.xmlsoap.org/ws/2004/03/addressing/role/anonymous";
/** Specifies no Reply endpoint address (no reply) */
const char *soap_wsa_noneURI = "addressing/none not supported";
const char *soap_wsa_faultAction = "http://schemas.xmlsoap.org/ws/2004/03/addressing/fault";
#elif defined(SOAP_WSA_2005)
/** Anonymous Reply/To endpoint address */
const char *soap_wsa_anonymousURI =
		"http://www.w3.org/2005/08/addressing/anonymous";
/** Specifies no Reply endpoint address (no reply) */
const char *soap_wsa_noneURI = "http://www.w3.org/2005/08/addressing/none";
const char *soap_wsa_faultAction =
		"http://www.w3.org/2005/08/addressing/soap/fault";
#else
/** Anonymous Reply/To endpoint address */
const char *soap_wsa_anonymousURI = "http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous";
/** Specifies no Reply endpoint address (no reply) */
const char *soap_wsa_noneURI = "addressing/none not supported";
const char *soap_wsa_faultAction = "http://schemas.xmlsoap.org/ws/2004/08/addressing/fault";
#endif

/** anonymous URI of 2004 and 2005 schemas */
const char *soap_wsa_allAnonymousURI =
		"http://schemas.xmlsoap.org/ws/2004/03/addressing/role/anonymous http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous http://www.w3.org/2005/08/addressing/anonymous";

#ifdef SOAP_WSA_2005
/* WS-Discovery 1.0 */
const char *to_ts_URL = "urn:schemas-xmlsoap-org:ws:2005:04:discovery";
#else
/* WS-Discovery 1.1 */
const char *to_ts_URL = "urn:docs-oasis-open-org:ws-dd:ns:discovery:2009:01";
#endif
#if defined(SOAP_WSA_2003)
# define SOAP_WSA(member) wsa3__##member
# define SOAP_WSA_(prefix,member) prefix##_wsa3__##member
# define SOAP_WSA__(prefix,member) prefix##wsa3__##member
#elif defined(SOAP_WSA_2004)
# define SOAP_WSA(member) wsa4__##member
# define SOAP_WSA_(prefix,member) prefix##_wsa4__##member
# define SOAP_WSA__(prefix,member) prefix##wsa4__##member
#elif defined(SOAP_WSA_2005)
# define SOAP_WSA(member) wsa5__##member
# define SOAP_WSA_(prefix,member) prefix##_wsa5__##member
# define SOAP_WSA__(prefix,member) prefix##wsa5__##member
#else
# define SOAP_WSA(member) wsa__##member
# define SOAP_WSA_(prefix,member) prefix##_wsa__##member
# define SOAP_WSA__(prefix,member) prefix##wsa__##member
#endif

typedef enum soap_wsdd_mode {
	SOAP_WSDD_MANAGED, SOAP_WSDD_ADHOC
} soap_wsdd_mode;

struct soap_wsa_data { /** fheader callback is invoked immediately after parsing a SOAP Header */
	int (*fheader)(struct soap*);
	/** fseterror callback is used to inspect and change gSOAP error codes */
	void (*fseterror)(struct soap*, const char**, const char**);
	/** fresponse callback is used to change a HTTP response into a HTTP POST */
	int (*fresponse)(struct soap*, int, size_t);
};

SOAP_FMAC5 int SOAP_FMAC6 soap_wsa_sender_fault(struct soap *soap,
		const char *faultstring, const char *faultdetail) {
	return soap_wsa_fault_subcode(soap, 1, NULL, faultstring, faultdetail);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_wsdd_http(struct soap *soap,
		const char *endpoint, const char *host, int port, const char *path,
		const char *action, size_t count) {
	return soap->fresponse(soap, SOAP_OK, count);
}

char* getUUId() {
	return URN_HARDWARE_ID + 4;
}

char* soap_wsa_rand_uuid(struct soap *soap) {
	char *uuid = (char*) soap_malloc(soap, 48);
	int r1, r2, r3, r4;
#ifdef WITH_OPENSSL
	r1 = soap_random;
	r2 = soap_random;
#else
	static int k = 0xFACEB00B;
	int lo = k % 127773;
	int hi = k / 127773;
# ifdef HAVE_GETTIMEOFDAY
	struct timeval tv;
	gettimeofday(&tv, NULL);
	r1 = 10000000 * tv.tv_sec + tv.tv_usec;
#else
	r1 = (int) time(NULL);
# endif
	k = 16807 * lo - 2836 * hi;
	if (k <= 0)
		k += 0x7FFFFFFF;
	r2 = k;
	k &= 0x8FFFFFFF;
	r2 += *(int*) soap->buf;
#endif
	r3 = soap_random;
	r4 = soap_random;
	sprintf(
			uuid,
			"urn:uuid:%8.8x-%4.4hx-4%3.3hx-%4.4hx-%4.4hx%8.8x",
			r1,
			(short) (r2 >> 16), ((short)r2 >> 4) & 0x0FFF, ((short)(r3 >> 16) & 0x3FFF) | 0x8000, (short)r3, r4);
	DBGFUN1("soap_wsa_rand_uuid", "%s", uuid);
	return uuid;
}

SOAP_FMAC5 int SOAP_FMAC6 soap_wsa_receiver_fault_subcode(struct soap *soap,
		const char *faultsubcode, const char *faultstring,
		const char *faultdetail) {
	return soap_wsa_fault_subcode(soap, 0, faultsubcode, faultstring,
			faultdetail);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_wsa_sender_fault_subcode(struct soap *soap,
		const char *faultsubcode, const char *faultstring,
		const char *faultdetail) {
	return soap_wsa_fault_subcode(soap, 1, faultsubcode, faultstring,
			faultdetail);
}

#if defined(SOAP_WSA_2005)
/**
 @fn SOAP_FMAC5 int SOAP_FMAC6 soap_wsa_error(struct soap *soap, SOAP_WSA(FaultCodesType) fault, const char *info)
 @brief Sets SOAP Fault (sub)code for server WS-Addressing fault response.
 @param soap context
 @param[in] fault is one of wsa:FaultCodesType enumeration values
 @param[in] info is the value of the element in the Fault detail field
 @return SOAP_FAULT
 */
SOAP_FMAC5 int SOAP_FMAC6 soap_wsa_error(struct soap *soap,
		SOAP_WSA(FaultCodesType) fault, const char *info) {
	const char *code = SOAP_WSA_(soap,FaultCodesType2s)(soap, fault);
	/* populate the SOAP Fault as per WS-Addressing spec */
	switch (fault) {
	case SOAP_WSA(InvalidAddressingHeader):
		soap_faultdetail(soap);
		if (soap->version == 1) {
			soap->fault->detail->__type =
					SOAP_WSA_(SOAP_TYPE_,ProblemHeaderQName);
			soap->fault->detail->fault = (void*) info;
		} else {
			soap->fault->SOAP_ENV__Detail->__type =
					SOAP_WSA_(SOAP_TYPE_,ProblemHeaderQName);
			soap->fault->SOAP_ENV__Detail->fault = (void*) info;
		}
		soap_wsa_sender_fault_subcode(
				soap,
				code,
				"A header representing a Message Addressing Property is not valid and the message cannot be processed.",
				NULL);
		break;
	case SOAP_WSA(InvalidAddress):
		soap_wsa_sender_fault_subcode(soap, code, "Invalid address.", NULL);
		break;
	case SOAP_WSA(InvalidEPR):
		soap_wsa_sender_fault_subcode(soap, code, "Invalid EPR.", NULL);
		break;
	case SOAP_WSA(InvalidCardinality):
		soap_wsa_sender_fault_subcode(soap, code,
				"Invalid cardinality of headers.", NULL);
		break;
	case SOAP_WSA(MissingAddressInEPR):
		soap_wsa_sender_fault_subcode(soap, code, "Missing EPR address.", NULL);
		break;
	case SOAP_WSA(DuplicateMessageID):
		soap_wsa_sender_fault_subcode(
				soap,
				code,
				"Message contains the message ID of a message already received.",
				NULL);
		break;
	case SOAP_WSA(ActionMismatch):
		soap_wsa_sender_fault_subcode(soap, code,
				"Action and SOAP action of the message do not match.", NULL);
		break;
	case SOAP_WSA(MessageAddressingHeaderRequired):
		soap_faultdetail(soap);
		if (soap->version == 1) {
			soap->fault->detail->__type =
					SOAP_WSA_(SOAP_TYPE_,ProblemHeaderQName);
			soap->fault->detail->fault = (void*) info;
		} else {
			soap->fault->SOAP_ENV__Detail->__type =
					SOAP_WSA_(SOAP_TYPE_,ProblemHeaderQName);
			soap->fault->SOAP_ENV__Detail->fault = (void*) info;
		}
		soap_wsa_sender_fault_subcode(
				soap,
				code,
				"A required header representing a Message Addressing Property is not present.",
				NULL);
		break;
	case SOAP_WSA(DestinationUnreachable):
		soap_faultdetail(soap);
		if (soap->version == 1) {
			soap->fault->detail->__type = SOAP_WSA_(SOAP_TYPE_,ProblemIRI);
			soap->fault->detail->fault = (void*) info;
		} else {
			soap->fault->SOAP_ENV__Detail->__type =
					SOAP_WSA_(SOAP_TYPE_,ProblemIRI);
			soap->fault->SOAP_ENV__Detail->fault = (void*) info;
		}
		soap_wsa_sender_fault_subcode(soap, code,
				"No route can be determined to reach [destination]", NULL);
		break;
	case SOAP_WSA(ActionNotSupported):
		soap_faultdetail(soap);
		if (soap->version == 1) {
			soap->fault->detail->__type = SOAP_WSA_(SOAP_TYPE_,ProblemAction);
			soap->fault->detail->fault = (void*) soap_malloc(soap,
					sizeof(SOAP_WSA_(,ProblemAction)));
			SOAP_WSA_(soap_default_, ProblemAction)
			(soap, (SOAP_WSA_(,ProblemAction)*)soap->fault->detail->fault);
			((SOAP_WSA_(,ProblemAction)*) soap->fault->detail->fault)->Action =
					(char*) info;
		} else {
			soap->fault->SOAP_ENV__Detail->__type =
					SOAP_WSA_(SOAP_TYPE_,ProblemAction);
			soap->fault->SOAP_ENV__Detail->fault = (void*) soap_malloc(soap,
					sizeof(SOAP_WSA_(,ProblemAction)));
			SOAP_WSA_(soap_default_, ProblemAction)
			(soap, (SOAP_WSA_(,ProblemAction)*)soap->fault->SOAP_ENV__Detail->fault);
			((SOAP_WSA_(,ProblemAction)*) soap->fault->SOAP_ENV__Detail->fault)->Action =
					(char*) info;
		}
		soap_wsa_sender_fault_subcode(soap, code,
				"The [action] cannot be processed at the receiver.", NULL);
		break;
	case SOAP_WSA(EndpointUnavailable):
		soap_faultdetail(soap);
		if (soap->version == 1) {
			soap->fault->detail->__type = SOAP_WSA_(SOAP_TYPE_,ProblemIRI);
			soap->fault->detail->fault = (void*) info;
		} else {
			soap->fault->SOAP_ENV__Detail->__type =
					SOAP_WSA_(SOAP_TYPE_,ProblemIRI);
			soap->fault->SOAP_ENV__Detail->fault = (void*) info;
		}
		soap_wsa_receiver_fault_subcode(soap, code,
				"The endpoint is unable to process the message at this time.",
				NULL);
		break;
	default:
		break;
	}
	return SOAP_FAULT;
}
#elif defined(SOAP_WSA_2003)
SOAP_FMAC5 int SOAP_FMAC6 soap_wsa_error(struct soap *soap, const char *fault)
{	return soap_wsa_sender_fault_subcode(soap, NULL, fault, NULL);
}
#else
/**
 @fn SOAP_FMAC5 int SOAP_FMAC6 soap_wsa_error(struct soap *soap, SOAP_WSA(FaultSubcodeValues) fault)
 @brief Sets SOAP Fault (sub)code for server WS-Addressing fault response.
 @param soap context
 @param[in] fault is one of wsa:FaultSubcodeValues
 @return SOAP_FAULT
 */
SOAP_FMAC5 int SOAP_FMAC6 soap_wsa_error(struct soap *soap, SOAP_WSA(FaultSubcodeValues) fault)
{	const char *code = SOAP_WSA_(soap,FaultSubcodeValues2s)(soap, fault);
	/* populate the SOAP Fault as per WS-Addressing spec */
	switch (fault)
	{	case SOAP_WSA(InvalidMessageInformationHeader):
		return soap_wsa_sender_fault_subcode(soap, code, "A message information header is not valid and the message cannot be processed. The validity failure can be either structural or semantic, e.g. a [destination] that is not a URI or a [relationship] to a [message id] that was never issued.", "Invalid header");
		case SOAP_WSA(MessageInformationHeaderRequired):
		return soap_wsa_sender_fault_subcode(soap, code, "A required message information header, To, MessageID, or Action, is not present.", "Missing Header QName");
		case SOAP_WSA(DestinationUnreachable):
		return soap_wsa_sender_fault_subcode(soap, code, "No route can be determined to reach the destination role defined by the WS-Addressing To.", NULL);
		case SOAP_WSA(ActionNotSupported):
		return soap_wsa_sender_fault_subcode(soap, code, "The [action] cannot be processed at the receiver.", soap->action);
		case SOAP_WSA(EndpointUnavailable):
		return soap_wsa_receiver_fault_subcode(soap, code, "The endpoint is unable to process the message at this time.", NULL);
		default:
		break;
	}
	return SOAP_FAULT;
}
#endif

SOAP_FMAC5 int SOAP_FMAC6 soap_wsa_check(struct soap *soap) {
	DBGFUN("soap_wsa_check");
	if (!soap->header || !soap->header->SOAP_WSA(Action)
	)
#if defined(SOAP_WSA_2005)
		return soap_wsa_error(soap, wsa5__MessageAddressingHeaderRequired, NULL);
#elif defined(SOAP_WSA_2003)
	return soap_wsa_error(soap, "WS-Addressing header missing");
#else
	return soap_wsa_error(soap, SOAP_WSA(MessageInformationHeaderRequired));
#endif
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 soap_wsa_request(struct soap *soap, const char *id,
		const char *to, const char *action) {
	DBGFUN3("soap_wsa_request", "id=%s", id?id:"(null)", "to=%s", to?to:"(null)", "action=%s", action?action:"(null)");
	if (soap_wsa_alloc_header(soap))
		return soap->error;
	soap->header->SOAP_WSA(MessageID) = soap_strdup(soap, id);
	if (to)
		soap->header->SOAP_WSA(To) = soap_strdup(soap, to);
	else
		/* this is optional */
		soap->header->SOAP_WSA(To) = (char*) soap_wsa_anonymousURI;
	soap->header->SOAP_WSA(Action) = soap_strdup(soap, action);
	soap->header->SOAP_WSA(RelatesTo) = NULL;
	soap->header->SOAP_WSA(From) = NULL;
	soap->header->SOAP_WSA(FaultTo) = NULL;
	soap_wsa_add_ReplyTo(soap, NULL);
	return soap_wsa_check(soap);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_wsa_fault_subcode(struct soap *soap, int flag,
		const char *faultsubcode, const char *faultstring,
		const char *faultdetail) {
	return soap_wsa_fault_subcode_action(soap, flag, faultsubcode, faultstring,
			faultdetail, NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_wsa_response(struct soap *soap, int status,
		size_t count) {
	struct soap_wsa_data *data = (struct soap_wsa_data*) soap_lookup_plugin(
			soap, soap_wsa_id);
	DBGFUN2("soap_wsa_response", "status=%d", status, "count=%lu", (unsigned long)count);
	if (!data)
		return SOAP_PLUGIN_ERROR;
	soap->fresponse = data->fresponse; /* reset (HTTP response) */
	return soap->fpost(soap, soap_strdup(soap, soap->endpoint), soap->host,
			soap->port, soap->path, soap->action, count);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_wsa_add_ReplyTo(struct soap *soap,
		const char *replyTo) {
	if (!soap->header)
		return SOAP_ERR;
#ifndef SOAP_WSA_2005
	if (!replyTo)
	replyTo = soap_wsa_anonymousURI;
#endif
	if (replyTo) {
		soap->header->SOAP_WSA(ReplyTo) = (SOAP_WSA_(,ReplyTo)*) soap_malloc(
				soap, sizeof(SOAP_WSA_(,ReplyTo)));
		SOAP_WSA_(soap_default,EndpointReferenceType)(soap,
				soap->header->SOAP_WSA(ReplyTo));
		soap->header->SOAP_WSA(ReplyTo)->Address = soap_strdup(soap, replyTo);
	} else
		soap->header->SOAP_WSA(ReplyTo) = NULL;
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 soap_wsa_add_RelatesTo(struct soap *soap,
		const char *relatesTo) {
	if (!soap->header)
		return SOAP_ERR;
	if (relatesTo) {
		soap->header->SOAP_WSA(RelatesTo) =
				(SOAP_WSA_(,RelatesTo)*) soap_malloc(soap,
						sizeof(SOAP_WSA_(,RelatesTo)));
		SOAP_WSA_(soap_default_, RelatesTo)
		(soap, soap->header->SOAP_WSA(RelatesTo));
		soap->header->SOAP_WSA(RelatesTo)->__item = soap_strdup(soap,
				relatesTo);
	}
	return SOAP_OK;
}

SOAP_FMAC5 void SOAP_FMAC6 soap_wsdd_set_AppSequence(struct soap *soap) {
	soap_header(soap);
	if (!soap->header->wsdd__AppSequence) {
		soap->header->wsdd__AppSequence = (wsdd__AppSequenceType*) soap_malloc(
				soap, sizeof(wsdd__AppSequenceType));
		soap_default_wsdd__AppSequenceType(soap,
				soap->header->wsdd__AppSequence);
	}

	MUTEX_LOCK(soap_wsdd_state);
	soap->header->wsdd__AppSequence->InstanceId = soap_wsdd_InstanceId;
	soap->header->wsdd__AppSequence->SequenceId = (char*) soap_wsdd_SequenceId;
	soap->header->wsdd__AppSequence->MessageNumber = soap_wsdd_MessageNumber++;
	MUTEX_UNLOCK(soap_wsdd_state);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_wsa_reply(struct soap *soap, const char *id,
		const char *action) {
	struct soap_wsa_data *data = (struct soap_wsa_data*) soap_lookup_plugin(
			soap, soap_wsa_id);
	struct SOAP_ENV__Header *oldheader, *newheader;
	DBGFUN1("soap_wsa_reply", "action=%s", action?action:"(null)");
	if (!data)
		return soap->error = SOAP_PLUGIN_ERROR;
	oldheader = soap->header;
	soap->header = NULL;
	/* if endpoint address for reply is 'none' return immediately */
	if (oldheader && oldheader->SOAP_WSA(ReplyTo)
			&& oldheader->SOAP_WSA(ReplyTo)->Address
			&& !strcmp(oldheader->SOAP_WSA(ReplyTo)->Address, soap_wsa_noneURI))
		return soap_send_empty_response(soap, SOAP_OK);
	/* allocate a new header */
	if (soap_wsa_alloc_header(soap))
		return soap->error;
	newheader = soap->header;
	/* copy members to new header, except WSA data */
	if (oldheader)
		*newheader = *oldheader;
	newheader->SOAP_WSA(MessageID) = soap_strdup(soap, id);
	newheader->SOAP_WSA(Action) = soap_strdup(soap, action);
	newheader->SOAP_WSA(RelatesTo) = NULL;
	newheader->SOAP_WSA(From) = NULL;
	newheader->SOAP_WSA(To) = NULL;
	newheader->SOAP_WSA(ReplyTo) = NULL;
	newheader->SOAP_WSA(FaultTo) = NULL;
	/* check current header content */
	if (oldheader && oldheader->SOAP_WSA(MessageID))
	{
		newheader->SOAP_WSA(RelatesTo) = (SOAP_WSA_(,RelatesTo)*) soap_malloc(
				soap, sizeof(SOAP_WSA_(,RelatesTo)));
		SOAP_WSA_(soap_default_, RelatesTo)
		(soap, newheader->SOAP_WSA(RelatesTo));
		newheader->SOAP_WSA(RelatesTo)->__item = oldheader->SOAP_WSA(MessageID);
	}
#ifdef SOAP_WSA_2005
	/* WCF Interoperability:
	 ChannelInstance is required when the WCF Application hosts multiple
	 Callback Channels within the same application. The
	 ReferenceParameters->ChannelInstance element serves as a hint to the WCF
	 Client dispatcher, as to which WCF callback instance a received SOAP
	 Envelope belongs to ChannelInstance is declared as a pointer, so it is
	 essentially an optional element. Tests with Applications not requiring
	 ChannelInstance have also been done for the following fix.
	 */
	if (oldheader && oldheader->SOAP_WSA(ReplyTo)
			&& oldheader->SOAP_WSA(ReplyTo)->ReferenceParameters
			&& oldheader->SOAP_WSA(ReplyTo)->ReferenceParameters->chan__ChannelInstance) {
		if (newheader) {
			if (!newheader->chan__ChannelInstance) {
				newheader->chan__ChannelInstance =
						(struct chan__ChannelInstanceType*) soap_malloc(soap,
								sizeof(struct chan__ChannelInstanceType));
				if (newheader->chan__ChannelInstance) {
					soap_default_chan__ChannelInstanceType(soap,
							newheader->chan__ChannelInstance);
					newheader->chan__ChannelInstance->__item =
							*(oldheader->SOAP_WSA(ReplyTo)->ReferenceParameters->chan__ChannelInstance);
					newheader->chan__ChannelInstance->wsa5__IsReferenceParameter =
							_wsa5__IsReferenceParameter__true;
				}
			} else {
				newheader->chan__ChannelInstance->__item =
						*(oldheader->SOAP_WSA(ReplyTo)->ReferenceParameters->chan__ChannelInstance);
				newheader->chan__ChannelInstance->wsa5__IsReferenceParameter =
						_wsa5__IsReferenceParameter__true;
			}
		}
	}
#endif
	if (oldheader && oldheader->SOAP_WSA(ReplyTo)
			&& oldheader->SOAP_WSA(ReplyTo)->Address
			&& !soap_tagsearch(soap_wsa_allAnonymousURI,
					oldheader->SOAP_WSA(ReplyTo)->Address)) {
		newheader->SOAP_WSA(To) = oldheader->SOAP_WSA(ReplyTo)->Address;
		/* (re)connect to ReplyTo endpoint if From != ReplyTo */
		if (!oldheader->SOAP_WSA(From) || !oldheader->SOAP_WSA(From)->Address
				|| strcmp(oldheader->SOAP_WSA(From)->Address,
						oldheader->SOAP_WSA(ReplyTo)->Address)) {
			struct soap *reply_soap = soap_copy(soap);
			if (reply_soap) {
				soap_copy_stream(reply_soap, soap);
				soap_free_stream(soap); /* prevents close in soap_connect() below */
				if (soap_connect(soap, newheader->SOAP_WSA(To),
						newheader->SOAP_WSA(Action))) {
					int err;
					soap_copy_stream(soap, reply_soap);
					soap_free_stream(reply_soap);
					soap_end(reply_soap);
					soap_free(reply_soap);
					soap->header = oldheader;
#if defined(SOAP_WSA_2005)
					err = soap_wsa_error(soap, SOAP_WSA(DestinationUnreachable),
							newheader->SOAP_WSA(To));
#elif defined(SOAP_WSA_2003)
					err = soap_wsa_error(soap, "WS-Addessing destination unreachable");
#else
					err = soap_wsa_error(soap, SOAP_WSA(DestinationUnreachable));
#endif
					soap->header = NULL;
					return err;
				}
				if (soap_valid_socket(reply_soap->socket))
					soap_send_empty_response(reply_soap, SOAP_OK); /* HTTP ACCEPTED */
				soap->header = newheader;
				soap_end(reply_soap);
				soap_free(reply_soap);
				data->fresponse = soap->fresponse;
				soap->fresponse = soap_wsa_response; /* response will be a POST */
			}
		}
	} else if (oldheader && oldheader->SOAP_WSA(From)
		)
		newheader->SOAP_WSA(To) = oldheader->SOAP_WSA(From)->Address;
	else
		newheader->SOAP_WSA(To) = (char*) soap_wsa_anonymousURI;
	soap->header = newheader;
	soap->action = newheader->SOAP_WSA(Action);
	return SOAP_OK;
}
SOAP_FMAC5 int SOAP_FMAC6 soap_wsa_fault_subcode_action(struct soap *soap,
		int flag, const char *faultsubcode, const char *faultstring,
		const char *faultdetail, const char *action) {
	struct soap_wsa_data *data = (struct soap_wsa_data*) soap_lookup_plugin(
			soap, soap_wsa_id);
	struct SOAP_ENV__Header *oldheader, *newheader;
	DBGFUN2("soap_wsa_fault_subcode", "faultsubcode=%s", faultsubcode?faultsubcode:"(null)", "faultstring=%s", faultstring?faultstring:"(null)");
	if (!data)
		return soap->error = SOAP_PLUGIN_ERROR;
	oldheader = soap->header;
	/* no FaultTo: use ReplyTo */
	if (oldheader && oldheader->SOAP_WSA(ReplyTo)
			&& (!oldheader->SOAP_WSA(FaultTo)
					|| soap_tagsearch(soap_wsa_allAnonymousURI,
							oldheader->SOAP_WSA(FaultTo)->Address))) {
		if (!oldheader->SOAP_WSA(FaultTo)) {
			oldheader->SOAP_WSA(FaultTo) = (SOAP_WSA_(,FaultTo)*) soap_malloc(
					soap, sizeof(SOAP_WSA_(,FaultTo)));
			SOAP_WSA_(soap_default,EndpointReferenceType)(soap,
					soap->header->SOAP_WSA(FaultTo));
		}
		oldheader->SOAP_WSA(FaultTo)->Address =
				oldheader->SOAP_WSA(ReplyTo)->Address;
	}
	if (oldheader && oldheader->SOAP_WSA(FaultTo))
	{
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "WSA FaultTo='%s'\n", oldheader->SOAP_WSA(FaultTo)->Address));
	}
	if (oldheader && oldheader->SOAP_WSA(FaultTo)
			&& !strcmp(oldheader->SOAP_WSA(FaultTo)->Address, soap_wsa_noneURI))
		return soap_send_empty_response(soap, SOAP_OK); /* HTTP ACCEPTED */
	soap->header = NULL;
	/* allocate a new header */
	if (soap_wsa_alloc_header(soap))
		return soap->error;
	newheader = soap->header;
	soap_default_SOAP_ENV__Header(soap, newheader); /* remove/clear SOAP Header */
	/* check header */
	if (oldheader && oldheader->SOAP_WSA(MessageID))
	{
		newheader->SOAP_WSA(RelatesTo) = (SOAP_WSA_(,RelatesTo)*) soap_malloc(
				soap, sizeof(SOAP_WSA_(,RelatesTo)));
		SOAP_WSA_(soap_default_, RelatesTo)
		(soap, newheader->SOAP_WSA(RelatesTo));
		newheader->SOAP_WSA(RelatesTo)->__item = oldheader->SOAP_WSA(MessageID);
	}
	/* header->wsa__MessageID = "..."; */
	newheader->SOAP_WSA(Action) = (char*) soap_wsa_faultAction;
	if (oldheader && oldheader->SOAP_WSA(FaultTo)
			&& oldheader->SOAP_WSA(FaultTo)->Address
			&& !soap_tagsearch(soap_wsa_allAnonymousURI,
					oldheader->SOAP_WSA(FaultTo)->Address)) {
		newheader->SOAP_WSA(To) = oldheader->SOAP_WSA(FaultTo)->Address;
		/* (re)connect to FaultTo endpoint if From != FaultTo */
		if (!oldheader->SOAP_WSA(From) || !oldheader->SOAP_WSA(From)->Address
				|| strcmp(oldheader->SOAP_WSA(From)->Address,
						oldheader->SOAP_WSA(FaultTo)->Address)) {
			soap->keep_alive = 0;
			soap_send_empty_response(soap, SOAP_OK); /* HTTP ACCEPTED */
			if (soap_connect(soap, newheader->SOAP_WSA(To),
					newheader->SOAP_WSA(Action)))
				return soap->error = SOAP_STOP; /* nowhere to go */
			soap_set_endpoint(soap, newheader->SOAP_WSA(To));
			if (action)
				soap->action = (char*) action;
			else
				soap->action = newheader->SOAP_WSA(Action);
			data->fresponse = soap->fresponse;
			soap->fresponse = soap_wsa_response; /* response will be a POST */
		}
	} else if (oldheader && oldheader->SOAP_WSA(From)
	)
		newheader->SOAP_WSA(To) = oldheader->SOAP_WSA(From)->Address;
	else
		newheader->SOAP_WSA(To) = (char*) soap_wsa_anonymousURI;
	soap->header = newheader;
	if (flag)
		return soap_sender_fault_subcode(soap, faultsubcode, faultstring,
				faultdetail);
	return soap_receiver_fault_subcode(soap, faultsubcode, faultstring,
			faultdetail);
}

SOAP_FMAC5 int SOAP_FMAC6 soap_wsa_alloc_header(struct soap *soap) {
	soap_header(soap);
	if (soap->header)
		return SOAP_OK;
	return soap->error = SOAP_EOM;
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
	char _xmaddr[256] = { 0 };
	if (RET_CODE_SUCCESS != getServiceURL(_xmaddr))
		return GSOAP_RET_CODE_NOT_IMPLEMENT;
	int interface_num = 1;
	wsdd__ProbeMatchesType ProbeMatches;
	ProbeMatches.__sizeProbeMatch = interface_num;
	ProbeMatches.ProbeMatch = (struct wsdd__ProbeMatchType *) soap_malloc(soap,
			sizeof(struct wsdd__ProbeMatchType) * interface_num);
	int i = 0;
	for (i = 0; i < interface_num; i++) {
		ProbeMatches.ProbeMatch->MetadataVersion = METADATA_VERSION;
		// should be onvif device mgmt address
		ProbeMatches.ProbeMatch->XAddrs = (char *) soap_malloc(soap,
				sizeof(char) * INFO_LENGTH);
		sprintf(ProbeMatches.ProbeMatch->XAddrs, "%s", _xmaddr);
		// probe type
		ProbeMatches.ProbeMatch->Types = (char *) soap_malloc(soap,
				sizeof(char) * INFO_LENGTH);
		strcpy(ProbeMatches.ProbeMatch->Types, DEVICE_TYPE);
		// Scope
		ProbeMatches.ProbeMatch->Scopes =
				(struct wsdd__ScopesType*) soap_malloc(soap,
						sizeof(struct wsdd__ScopesType));
		ProbeMatches.ProbeMatch->Scopes->__item = (char *) soap_malloc(soap,
				1024);
		memset(ProbeMatches.ProbeMatch->Scopes->__item, 0,
				sizeof(ProbeMatches.ProbeMatch->Scopes->__item));
		strcat(ProbeMatches.ProbeMatch->Scopes->__item, ONVIF_SCOPE_NAME);
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
		strcpy(ProbeMatches.ProbeMatch->wsa__EndpointReference.Address,
				URN_HARDWARE_ID);
	}

	if (soap->header == NULL) {
		soap->header = (struct SOAP_ENV__Header*) soap_malloc(soap,
				sizeof(struct SOAP_ENV__Header));
		soap->header->wsa__RelatesTo = (struct wsa__Relationship*) soap_malloc(
				soap, sizeof(struct wsa__Relationship));
		//it's here
		soap->header->wsa__MessageID = (char *) soap_malloc(soap,
				sizeof(char) * INFO_LENGTH);
		strcpy(soap->header->wsa__MessageID, getUUId());
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
		strcpy(soap->header->wsa__MessageID, getUUId());
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

SOAP_FMAC5 int SOAP_FMAC6 soap_send___wsdd__Hello(struct soap *soap,
		const char *soap_endpoint, const char *soap_action,
		struct wsdd__HelloType *wsdd__Hello) {
	struct __wsdd__Hello soap_tmp___wsdd__Hello;
	if (!soap_action)
		soap_action =
				"http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01/Hello";
	soap->encodingStyle = NULL;
	soap_tmp___wsdd__Hello.wsdd__Hello = wsdd__Hello;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___wsdd__Hello(soap, &soap_tmp___wsdd__Hello);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{
		if (soap_envelope_begin_out(soap) || soap_putheader(soap)
				|| soap_body_begin_out(soap)
				|| soap_put___wsdd__Hello(soap, &soap_tmp___wsdd__Hello,
						"-wsdd:Hello", NULL) || soap_body_end_out(soap)
				|| soap_envelope_end_out(soap))
			return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)) {
		return soap_closesock(soap);
	}
	soap_set_endpoint(soap, soap_endpoint);
	soap->error = SOAP_OK;
	struct sockaddr_in peer;
	peer.sin_family = AF_INET;
	peer.sin_port = htons(soap->port);
	peer.sin_addr.s_addr = inet_addr(soap->host);
	memcpy(&soap->peer, &peer, sizeof(struct sockaddr_in));
	soap->peerlen = sizeof(peer);
	if (soap_envelope_begin_out(soap) || soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___wsdd__Hello(soap, &soap_tmp___wsdd__Hello,
					"-wsdd:Hello", NULL) || soap_body_end_out(soap)
			|| soap_envelope_end_out(soap) || soap_end_send(soap))
		return soap_closesock(soap);
	return SOAP_OK;
}

int soap_wsdd_Hello(struct soap *soap, soap_wsdd_mode mode,
		const char *endpoint, const char *MessageID, const char *RelatesTo,
		const char *EndpointReference, const char *Types, const char *Scopes,
		const char *MatchBy, const char *XAddrs, unsigned int MetadataVersion) {
	struct wsdd__HelloType req;
	struct wsdd__ScopesType scopes;
	const char *Action =
			"http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01/Hello";
	const char *To = endpoint;

	/* SOAP Header */
	if (mode == SOAP_WSDD_ADHOC)
		To = to_ts_URL;
	soap_wsa_request(soap, MessageID, To, Action);
	soap_wsa_add_RelatesTo(soap, RelatesTo);
#ifdef SOAP_WSA_2005
	soap_wsdd_set_AppSequence(soap);
#else
	if (mode == SOAP_WSDD_ADHOC)
	soap_wsdd_set_AppSequence(soap);
	else
	soap_wsdd_reset_AppSequence(soap);
#endif

	/* Hello */
	soap_default_wsdd__HelloType(soap, &req);
#ifdef SOAP_WSA_2005
	req.wsa__EndpointReference.Address = (char*) EndpointReference;
#else
	req.wsa__EndpointReference.Address = (char*)EndpointReference;
#endif
	req.Types = (char*) Types;
	if (Scopes) {
		soap_default_wsdd__ScopesType(soap, &scopes);
		scopes.__item = (char*) Scopes;
		scopes.MatchBy = (char*) MatchBy;
		req.Scopes = &scopes;
	}
	req.XAddrs = (char*) XAddrs;
	req.MetadataVersion = MetadataVersion;
	if (soap_send___wsdd__Hello(soap, endpoint, Action, &req))
		return soap->error;

	/* HTTP(S) POST expects an HTTP OK response */
	if (endpoint && !strncmp(endpoint, "http", 4))
		return soap_recv_empty_response(soap);
	return SOAP_OK;
}

void wsdd_event_Hello(struct soap *soap, unsigned int InstanceId,
		const char *SequenceId, unsigned int MessageNumber,
		const char *MessageID, const char *RelatesTo,
		const char *EndpointReference, const char *Types, const char *Scopes,
		const char *MatchBy, const char *XAddrs, unsigned int MetadataVersion) {

}

SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Hello(struct soap* soap,
		struct wsdd__HelloType *wsdd__Hello) {
	unsigned int InstanceId = 0;
	const char *SequenceId = NULL;
	unsigned int MessageNumber = 0;

	LOG("__wsdd__Hello");

	/* check for WSA */
	if (soap_wsa_check(soap))
		return soap->error;

	/* check WSDD */
	if (!wsdd__Hello
#ifdef SOAP_WSA_2005
			|| !wsdd__Hello->wsa__EndpointReference.Address
#else
			|| !wsdd__Hello->wsa__EndpointReference.Address
#endif
			)
		return soap_wsa_sender_fault(soap, "WSDD Hello incomplete", NULL);

#ifdef SOAP_WSA_2005
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "EndpointReference=%s\n", wsdd__Hello->wsa__EndpointReference.Address));
#else
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "EndpointReference=%s\n", wsdd__Hello->wsa__EndpointReference.Address));
#endif

	if (soap->header->wsdd__AppSequence) {
		wsdd__AppSequenceType *seq = soap->header->wsdd__AppSequence;
		InstanceId = seq->InstanceId;
		SequenceId = seq->SequenceId;
		MessageNumber = seq->MessageNumber;
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "AppSeq id=%u seq=%s msg=%u\n", InstanceId, SequenceId ? SequenceId : "(null)", MessageNumber));
	}

	/* pass on to user-defined event handler */
	wsdd_event_Hello(
			soap,
			InstanceId,
			SequenceId,
			MessageNumber,
			soap->header->SOAP_WSA(MessageID),
			soap->header->SOAP_WSA(RelatesTo) ?
					soap->header->SOAP_WSA(RelatesTo)->__item : NULL,
			wsdd__Hello->wsa__EndpointReference.Address,
			wsdd__Hello->Types,
			wsdd__Hello->Scopes ? wsdd__Hello->Scopes->__item : NULL,
			wsdd__Hello->Scopes ? wsdd__Hello->Scopes->MatchBy : NULL,
			wsdd__Hello->XAddrs,
			wsdd__Hello->MetadataVersion);

	/* one-way HTTP(S) POST message requires an HTTP OK response message */
	if (!strncmp(soap->endpoint, "http", 4))
		return soap_send_empty_response(soap, SOAP_OK);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tdn__Hello(struct soap* soap,
		struct wsdd__HelloType tdn__Hello,
		struct wsdd__ResolveType* tdn__HelloResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

SOAP_FMAC5 int SOAP_FMAC6 soap_send___wsdd__Bye(struct soap *soap,
		const char *soap_endpoint, const char *soap_action,
		struct wsdd__ByeType *wsdd__Bye) {
	struct __wsdd__Bye soap_tmp___wsdd__Bye;
	if (soap_action == NULL
	)
		soap_action =
				"http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01/Bye";
	soap->encodingStyle = NULL;
	soap_tmp___wsdd__Bye.wsdd__Bye = wsdd__Bye;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___wsdd__Bye(soap, &soap_tmp___wsdd__Bye);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{
		if (soap_envelope_begin_out(soap) || soap_putheader(soap)
				|| soap_body_begin_out(soap)
				|| soap_put___wsdd__Bye(soap, &soap_tmp___wsdd__Bye,
						"-wsdd:Bye", NULL) || soap_body_end_out(soap)
				|| soap_envelope_end_out(soap))
			return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
			|| soap_envelope_begin_out(soap) || soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___wsdd__Bye(soap, &soap_tmp___wsdd__Bye, "-wsdd:Bye",
					NULL) || soap_body_end_out(soap)
			|| soap_envelope_end_out(soap) || soap_end_send(soap))
		return soap_closesock(soap);
	return SOAP_OK;
}

int soap_wsdd_Bye(struct soap *soap, soap_wsdd_mode mode, const char *endpoint,
		const char *MessageID, const char *EndpointReference, const char *Types,
		const char *Scopes, const char *MatchBy, const char *XAddrs,
		unsigned int MetadataVersion) {
	struct wsdd__ByeType req;
	struct wsdd__ScopesType scopes;
	const char *Action =
			"http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01/Bye";
	const char *To = endpoint;

	/* SOAP Header */
	if (mode == SOAP_WSDD_ADHOC)
		To = to_ts_URL;
	soap_wsa_request(soap, MessageID, To, Action);
#ifdef SOAP_WSA_2005
	soap_wsdd_set_AppSequence(soap);
#else
	if (mode == SOAP_WSDD_ADHOC)
	soap_wsdd_set_AppSequence(soap);
	else
	soap_wsdd_reset_AppSequence(soap);
#endif

	/* Bye */
	soap_default_wsdd__ByeType(soap, &req);
#ifdef SOAP_WSA_2005
	req.wsa__EndpointReference.Address = (char*) EndpointReference;
#else
	req.wsa__EndpointReference.Address = (char*)EndpointReference;
#endif
	req.Types = (char*) Types;
	if (Scopes) {
		soap_default_wsdd__ScopesType(soap, &scopes);
		scopes.__item = (char*) Scopes;
		scopes.MatchBy = (char*) MatchBy;
		req.Scopes = &scopes;
	}
	req.XAddrs = (char*) XAddrs;
	req.MetadataVersion = &MetadataVersion;
	if (soap_send___wsdd__Bye(soap, endpoint, Action, &req))
		return soap->error;

	/* HTTP(S) POST expects an HTTP OK response */
	if (endpoint && !strncmp(endpoint, "http", 4))
		return soap_recv_empty_response(soap);
	return SOAP_OK;
}

void wsdd_event_Bye(struct soap *soap, unsigned int InstanceId,
		const char *SequenceId, unsigned int MessageNumber,
		const char *MessageID, const char *RelatesTo,
		const char *EndpointReference, const char *Types, const char *Scopes,
		const char *MatchBy, const char *XAddrs, unsigned int *MetadataVersion) {

}

SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Bye(struct soap* soap,
		struct wsdd__ByeType *wsdd__Bye) {
	unsigned int InstanceId = 0;
	const char *SequenceId = NULL;
	unsigned int MessageNumber = 0;

	DBG("__wsdd__Bye");

	/* check for WSA */
	if (soap_wsa_check(soap))
		return soap->error;

	/* check WSDD */
	if (!wsdd__Bye
#ifdef SOAP_WSA_2005
			|| !wsdd__Bye->wsa__EndpointReference.Address
#else
			|| !wsdd__Bye->wsa__EndpointReference.Address
#endif
			)
		return soap_wsa_sender_fault(soap, "WSDD Bye incomplete", NULL);

#ifdef SOAP_WSA_2005
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "EndpointReference=%s\n", wsdd__Bye->wsa__EndpointReference.Address));
#else
	DBGLOG(TEST, SOAP_MESSAGE(fdebug, "EndpointReference=%s\n", wsdd__Bye->wsa__EndpointReference.Address));
#endif

	if (soap->header->wsdd__AppSequence) {
		wsdd__AppSequenceType *seq = soap->header->wsdd__AppSequence;
		InstanceId = seq->InstanceId;
		SequenceId = seq->SequenceId;
		MessageNumber = seq->MessageNumber;
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "AppSeq id=%u seq=%s msg=%u\n", InstanceId, SequenceId ? SequenceId : "(null)", MessageNumber));
	}

	/* pass on to user-defined event handler */
	wsdd_event_Bye(
			soap,
			InstanceId,
			SequenceId,
			MessageNumber,
			soap->header->SOAP_WSA(MessageID),
			soap->header->SOAP_WSA(RelatesTo) ?
					soap->header->SOAP_WSA(RelatesTo)->__item : NULL,
			wsdd__Bye->wsa__EndpointReference.Address,
			wsdd__Bye->Types,
			wsdd__Bye->Scopes ? wsdd__Bye->Scopes->__item : NULL,
			wsdd__Bye->Scopes ? wsdd__Bye->Scopes->MatchBy : NULL,
			wsdd__Bye->XAddrs,
			wsdd__Bye->MetadataVersion);

	/* one-way HTTP(S) POST message requires an HTTP OK response message */
	if (!strncmp(soap->endpoint, "http", 4))
		return soap_send_empty_response(soap, SOAP_OK);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tdn__Bye(struct soap* soap,
		struct wsdd__ByeType tdn__Bye,
		struct wsdd__ResolveType* tdn__ByeResponse) {
	return GSOAP_RET_CODE_NOT_IMPLEMENT;
}

void wsdd_event_ProbeMatches(struct soap *soap, unsigned int InstanceId,
		const char *SequenceId, unsigned int MessageNumber,
		const char *MessageID, const char *RelatesTo,
		struct wsdd__ProbeMatchesType *matches) {

}

SOAP_FMAC5 int SOAP_FMAC6 __wsdd__ProbeMatches(struct soap* soap,
		struct wsdd__ProbeMatchesType *wsdd__ProbeMatches) {
	unsigned int InstanceId = 0;
	const char *SequenceId = NULL;
	unsigned int MessageNumber = 0;

	DBG("__wsdd__ProbeMatches");

	/* check for WSA */
	if (soap_wsa_check(soap))
		return soap->error;

	/* check WSDD */
	if (!wsdd__ProbeMatches)
		return soap_wsa_sender_fault(soap, "WSDD ProbeMatches incomplete", NULL);

	if (soap->header->wsdd__AppSequence) {
		wsdd__AppSequenceType *seq = soap->header->wsdd__AppSequence;
		InstanceId = seq->InstanceId;
		SequenceId = seq->SequenceId;
		MessageNumber = seq->MessageNumber;
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "AppSeq id=%u seq=%s msg=%u\n", InstanceId, SequenceId ? SequenceId : "(null)", MessageNumber));
	}

	/* pass probe matches on to user-defined event handler */
	wsdd_event_ProbeMatches(
			soap,
			InstanceId,
			SequenceId,
			MessageNumber,
			soap->header->SOAP_WSA(MessageID),
			soap->header->SOAP_WSA(RelatesTo) ?
					soap->header->SOAP_WSA(RelatesTo)->__item : NULL,
			wsdd__ProbeMatches);

	/* one-way HTTP(S) POST message requires an HTTP OK response message */
	if (!strncmp(soap->endpoint, "http", 4))
		return soap_send_empty_response(soap, SOAP_OK);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 soap_send___wsdd__ResolveMatches(struct soap *soap,
		const char *soap_endpoint, const char *soap_action,
		struct wsdd__ResolveMatchesType *wsdd__ResolveMatches) {
	struct __wsdd__ResolveMatches soap_tmp___wsdd__ResolveMatches;
	if (soap_action == NULL
	)
		soap_action =
				"http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01/ResolveMatches";
	soap->encodingStyle = NULL;
	soap_tmp___wsdd__ResolveMatches.wsdd__ResolveMatches = wsdd__ResolveMatches;
	soap_begin(soap);
	soap_serializeheader(soap);
	soap_serialize___wsdd__ResolveMatches(soap,
			&soap_tmp___wsdd__ResolveMatches);
	if (soap_begin_count(soap))
		return soap->error;
	if (soap->mode & SOAP_IO_LENGTH)
	{
		if (soap_envelope_begin_out(soap) || soap_putheader(soap)
				|| soap_body_begin_out(soap)
				|| soap_put___wsdd__ResolveMatches(soap,
						&soap_tmp___wsdd__ResolveMatches,
						"-wsdd:ResolveMatches", NULL) || soap_body_end_out(soap)
				|| soap_envelope_end_out(soap))
			return soap->error;
	}
	if (soap_end_count(soap))
		return soap->error;
	if (soap_connect(soap, soap_endpoint, soap_action)
			|| soap_envelope_begin_out(soap) || soap_putheader(soap)
			|| soap_body_begin_out(soap)
			|| soap_put___wsdd__ResolveMatches(soap,
					&soap_tmp___wsdd__ResolveMatches, "-wsdd:ResolveMatches",
					NULL) || soap_body_end_out(soap)
			|| soap_envelope_end_out(soap) || soap_end_send(soap))
		return soap_closesock(soap);
	return SOAP_OK;
}

soap_wsdd_mode wsdd_event_Resolve(struct soap *soap, const char *MessageID,
		const char *ReplyTo, const char *EndpointReference,
		struct wsdd__ResolveMatchType *match) {
	match->wsa__EndpointReference.ReferenceProperties =
			(struct wsa__ReferencePropertiesType*) soap_malloc(soap,
					sizeof(struct wsa__ReferencePropertiesType));
	match->wsa__EndpointReference.ReferenceProperties->__size = 0;
	match->wsa__EndpointReference.ReferenceProperties->__any = NULL;
	match->wsa__EndpointReference.ReferenceParameters =
			(struct wsa__ReferenceParametersType*) soap_malloc(soap,
					sizeof(struct wsa__ReferenceParametersType));
	match->wsa__EndpointReference.ReferenceParameters->__size = 0;
	match->wsa__EndpointReference.ReferenceParameters->__any = NULL;
	match->wsa__EndpointReference.PortType = (char **) soap_malloc(soap,
			sizeof(char*) * SMALL_INFO_LENGTH);
	match->wsa__EndpointReference.PortType[0] = (char *) soap_malloc(soap,
			sizeof(char) * SMALL_INFO_LENGTH);
	strcpy(match->wsa__EndpointReference.PortType[0], "ttl");
	match->wsa__EndpointReference.ServiceName =
			(struct wsa__ServiceNameType*) soap_malloc(soap,
					sizeof(struct wsa__ServiceNameType));
	match->wsa__EndpointReference.ServiceName->__item = NULL;
	match->wsa__EndpointReference.ServiceName->PortName = NULL;
	match->wsa__EndpointReference.ServiceName->__anyAttribute = NULL;
	match->wsa__EndpointReference.__any = (char **) soap_malloc(soap,
			sizeof(char*) * SMALL_INFO_LENGTH);
	match->wsa__EndpointReference.__any[0] = (char *) soap_malloc(soap,
			sizeof(char) * SMALL_INFO_LENGTH);
	strcpy(match->wsa__EndpointReference.__any[0], "Any");
	match->wsa__EndpointReference.__anyAttribute = (char *) soap_malloc(soap,
			sizeof(char) * SMALL_INFO_LENGTH);
	strcpy(match->wsa__EndpointReference.__anyAttribute, "Attribute");
	match->wsa__EndpointReference.__size = 0;
	match->wsa__EndpointReference.Address = (char *) soap_malloc(soap,
			sizeof(char) * INFO_LENGTH);
	strcpy(match->wsa__EndpointReference.Address, URN_HARDWARE_ID);
	return SOAP_WSDD_MANAGED;
}

SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Resolve(struct soap* soap,
		struct wsdd__ResolveType *wsdd__Resolve) {
	struct wsdd__ResolveMatchesType resolveMatches;
	struct wsdd__ResolveMatchType match;
	soap_wsdd_mode mode;

	DBGFUN("__wsdd__Resolve");

	/* check for WSA */
	if (soap_wsa_check(soap))
		return soap->error;

	/* check WSDD */
	if (!wsdd__Resolve
#ifdef SOAP_WSA_2005
			|| !wsdd__Resolve->wsa__EndpointReference.Address
#else
			|| !Resolve->wsa__EndpointReference.Address
#endif
			)
		return soap_wsa_sender_fault(soap, "WSDD Resolve incomplete", NULL);

	soap_default_wsdd__ResolveMatchesType(soap, &resolveMatches);
	soap_default_wsdd__ResolveMatchType(soap, &match);
	resolveMatches.ResolveMatch = &match;

	/* pass resolve request on to user-defined event handler */
	mode =
			wsdd_event_Resolve(
					soap,
#ifdef SOAP_WSA_2005
					soap->header->wsa5__MessageID,
					soap->header->wsa5__ReplyTo ?
							soap->header->wsa5__ReplyTo->Address : NULL
							,
					wsdd__Resolve->wsa__EndpointReference.Address,
#else
					soap->header->wsa__MessageID,
					soap->header->wsa__ReplyTo ? soap->header->wsa__ReplyTo->Address : NULL,
					Resolve->wsa__EndpointReference.Address,
#endif
					&match);

	if (mode == SOAP_WSDD_ADHOC) {
		/* event handler or main application MUST return match independently */

		/* one-way HTTP(S) POST message requires an HTTP OK response message */
		if (!strncmp(soap->endpoint, "http", 4))
			return soap_send_empty_response(soap, SOAP_OK);
	} else {
		int err;
		int (*fpost)(struct soap*, const char*, const char*, int, const char*,
				const char*, size_t);
		const char *MessageID = getUUId();
		const char *Action =
				"http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01/ResolveMatches";

		/* SOAP Header */
		soap_wsdd_set_AppSequence(soap);
		soap_wsa_reply(soap, MessageID, Action);

		/* send over current socket as HTTP OK response: */
		fpost = soap->fpost;
		soap->fpost = soap_wsdd_http;
		err = soap_send___wsdd__ResolveMatches(soap, NULL, Action,
				&resolveMatches);
		soap->fpost = fpost;
		return err;
	}
	return SOAP_OK;
}

void wsdd_event_ResolveMatches(struct soap *soap, unsigned int InstanceId,
		const char *SequenceId, unsigned int MessageNumber,
		const char *MessageID, const char *RelatesTo,
		struct wsdd__ResolveMatchType *match) {
}

SOAP_FMAC5 int SOAP_FMAC6 __wsdd__ResolveMatches(struct soap* soap,
		struct wsdd__ResolveMatchesType *wsdd__ResolveMatches) {
	unsigned int InstanceId = 0;
	const char *SequenceId = NULL;
	unsigned int MessageNumber = 0;

	DBG("__wsdd__ResolveMatches");

	/* check for WSA */
	if (soap_wsa_check(soap))
		return soap->error;

	/* check WSDD */
	if (!wsdd__ResolveMatches || !wsdd__ResolveMatches->ResolveMatch)
		return soap_wsa_sender_fault(soap, "WSDD ResolveMatch incomplete", NULL);

	if (soap->header->wsdd__AppSequence) {
		wsdd__AppSequenceType *seq = soap->header->wsdd__AppSequence;
		InstanceId = seq->InstanceId;
		SequenceId = seq->SequenceId;
		MessageNumber = seq->MessageNumber;
		DBGLOG(TEST, SOAP_MESSAGE(fdebug, "AppSeq id=%u seq=%s msg=%u\n", InstanceId, SequenceId ? SequenceId : "(null)", MessageNumber));
	}

	/* pass resolve matches on to user-defined event handler */
	wsdd_event_ResolveMatches(
			soap,
			InstanceId,
			SequenceId,
			MessageNumber,
			soap->header->SOAP_WSA(MessageID),
			soap->header->SOAP_WSA(RelatesTo) ?
					soap->header->SOAP_WSA(RelatesTo)->__item : NULL,
			wsdd__ResolveMatches->ResolveMatch);

	/* one-way HTTP(S) POST message requires an HTTP OK response message */
	if (!strncmp(soap->endpoint, "http", 4))
		return soap_send_empty_response(soap, SOAP_OK);
	return SOAP_OK;
}

