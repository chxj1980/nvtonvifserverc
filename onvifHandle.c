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
#include "appCommon.h"

static char szDevXAddr[1024];
static char szEndpointReference[1024];
static char tmpBuf[1024];
#define METADATA_VERSION    1
#define INFO_LENGTH 100
#define SMALL_INFO_LENGTH 20
#define ONVIF_SERVER_CALL()    printf("onvifs: call %s, path=%s\r\n", __FUNCTION__, soap->path)

#define RET_SOAP_ERROR_CODE -1

#define ONVIF_RETURN_OK(soap, namespaces)   \
	ONVIF_SERVER_CALL();    \
	if(namespaces!=NULL) soap_set_namespaces(soap, namespaces);  \
	return SOAP_OK;



static inline int onvif_receiver_fault_subcode_oom( struct soap *soap )
{
    return soap_receiver_fault_subcode( soap, "ter:OutofMemory", "Out of Memory", "The device does not have sufficient memory to complete the action." );
}

int netGetMac( char *pInterface, unsigned char *pMac )
{
    struct ifreq ifreq;
    int sockfd = 0;
    unsigned char mac[6] = {0};

    if( ( sockfd = socket( AF_INET, SOCK_STREAM, 0 ) ) < 0 )
    {
        /*创建套接口，后用于获取mac地址*/
        perror( "netGetMac socket" );
        return RET_SOAP_ERROR_CODE;
    }

    strcpy( ifreq.ifr_name, pInterface );

    if( ioctl( sockfd, SIOCGIFHWADDR, &ifreq ) < 0 )
    {
        /*获取mac地址*/
        perror( "netGetMac ioctl" );
        close( sockfd );
        return -2;
    }

    memcpy( mac, ifreq.ifr_hwaddr.sa_data, 6 );  /*复制mac地址到mac*/
    printf( "MAC:%02x:%02x:%02x:%02x:%02x:%02x\n", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5] );

    if( pMac != NULL )
    {
        memcpy( pMac, mac, 6 );
    }

    close( sockfd );

    return 0;
}

SOAP_FMAC5 int SOAP_FMAC6 soap_send___wsdd__ProbeMatches( struct soap *soap, const char *soap_endpoint, const char *soap_action, struct wsdd__ProbeMatchesType *wsdd__ProbeMatches )
{
    struct __wsdd__ProbeMatches soap_tmp___wsdd__ProbeMatches;

    if( !soap_action )
        soap_action = "http://docs.oasis-open.org/ws-dd/ns/discovery/2009/01/ProbeMatches";

    soap->encodingStyle = NULL;
    soap_tmp___wsdd__ProbeMatches.wsdd__ProbeMatches = wsdd__ProbeMatches;
    soap_begin( soap );
    soap_serializeheader( soap );
    soap_serialize___wsdd__ProbeMatches( soap, &soap_tmp___wsdd__ProbeMatches );

    if( soap_begin_count( soap ) )
        return soap->error;

    if( soap->mode & SOAP_IO_LENGTH )
    {
        if( soap_envelope_begin_out( soap )
                || soap_putheader( soap )
                || soap_body_begin_out( soap )
                || soap_put___wsdd__ProbeMatches( soap, &soap_tmp___wsdd__ProbeMatches, "-wsdd:ProbeMatches", NULL )
                || soap_body_end_out( soap )
                || soap_envelope_end_out( soap ) )
            return soap->error;
    }

    if( soap_end_count( soap ) )
        return soap->error;

    if( soap_connect( soap, soap_endpoint, soap_action )
            || soap_envelope_begin_out( soap )
            || soap_putheader( soap )
            || soap_body_begin_out( soap )
            || soap_put___wsdd__ProbeMatches( soap, &soap_tmp___wsdd__ProbeMatches, "-wsdd:ProbeMatches", NULL )
            || soap_body_end_out( soap )
            || soap_envelope_end_out( soap )
            || soap_end_send( soap ) )
        return soap_closesock( soap );

    return SOAP_OK;
}

int netGetIp(char *pInterface, unsigned int *ip)
{
	int sock = 0;
	struct ifreq ifr;

	if((pInterface == NULL) || (*pInterface == '\0'))
	{
		printf("get ip: pInterface == NULL\r\n");
		return -1;
	}

	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, pInterface, IFNAMSIZ);
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock <= 0)
	{
		printf("get ip: sock error, %s\r\n", strerror(errno));
		return -1;
	}

	((struct sockaddr_in*)&ifr.ifr_addr)->sin_family = PF_INET;
	if(ioctl(sock, SIOCGIFADDR, &ifr) < 0)
	{
		printf("get ip error: %s\r\n", strerror(errno));
		close(sock);
		return -1;
	}
	else
	{
		*ip = ((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr.s_addr;
		printf("get ip(%d:%d:%d:%d) success!\r\n", (*ip)&0xff, (*ip>>8)&0xff, (*ip>>16)&0xff, (*ip>>24)&0xff);
	}
	close(sock);

	return 0;
}

SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Probe( struct soap* soap, struct wsdd__ProbeType *wsdd__Probe )
{
    char _HwId[64] = "urn:uuid:D149F919-4013-437E-B480-3707D96D27A4";

    int  interface_num = 1;
    char ip_list[512] = {0};

	unsigned int localIp = 0;
	netGetIp(NET_CARD_NAME, &localIp);
	strcpy(ip_list, inet_ntoa(*((struct in_addr *)&localIp)));

    wsdd__ProbeMatchesType ProbeMatches;
    ProbeMatches.__sizeProbeMatch = interface_num;
    ProbeMatches.ProbeMatch = ( struct wsdd__ProbeMatchType * ) soap_malloc( soap, sizeof( struct wsdd__ProbeMatchType ) * interface_num );
    int i = 0;
    for(i = 0; i < interface_num; i++ )
    {
        ProbeMatches.ProbeMatch->MetadataVersion = 1;
        // should be onvif device mgmt address
        ProbeMatches.ProbeMatch->XAddrs = ( char * ) soap_malloc( soap, sizeof( char ) * INFO_LENGTH );
        sprintf( ProbeMatches.ProbeMatch->XAddrs, "http://%s:%d/", ip_list + i * 20, DEVICE_WEBSERVICE_PORT );
        // probe type
        ProbeMatches.ProbeMatch->Types = ( char * ) soap_malloc( soap, sizeof( char ) * INFO_LENGTH );
        strcpy( ProbeMatches.ProbeMatch->Types , "tdn:NetworkVideoTransmitter" );
        // Scope
        ProbeMatches.ProbeMatch->Scopes = ( struct wsdd__ScopesType* ) soap_malloc( soap, sizeof( struct wsdd__ScopesType ) );
        ProbeMatches.ProbeMatch->Scopes->__item = ( char * ) soap_malloc( soap, 1024 );
        memset( ProbeMatches.ProbeMatch->Scopes->__item, 0, sizeof( ProbeMatches.ProbeMatch->Scopes->__item ) );
        strcat( ProbeMatches.ProbeMatch->Scopes->__item, "onvif://www.onvif.org/type/NetworkVideoTransmitter" );
        ProbeMatches.ProbeMatch->Scopes->MatchBy = NULL;

        //ws-discovery¹æ¶¨ Îª¿ÉÑ¡Ïî , ReferenceProperties
        ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceProperties = ( struct wsa__ReferencePropertiesType* ) soap_malloc( soap, sizeof( struct wsa__ReferencePropertiesType ) );
        ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceProperties->__size = 0;
        ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceProperties->__any = NULL;
        //ws-discovery¹æ¶¨ Îª¿ÉÑ¡Ïî , ReferenceParameters
        ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceParameters = ( struct wsa__ReferenceParametersType* ) soap_malloc( soap, sizeof( struct wsa__ReferenceParametersType ) );
        ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceParameters->__size = 0;
        ProbeMatches.ProbeMatch->wsa__EndpointReference.ReferenceParameters->__any = NULL;
        //ws-discovery¹æ¶¨ Îª¿ÉÑ¡Ïî , PortType
        ProbeMatches.ProbeMatch->wsa__EndpointReference.PortType = ( char ** ) soap_malloc( soap, sizeof( char* ) * SMALL_INFO_LENGTH );
        ProbeMatches.ProbeMatch->wsa__EndpointReference.PortType[0] = ( char * ) soap_malloc( soap, sizeof( char ) * SMALL_INFO_LENGTH );
        strcpy( ProbeMatches.ProbeMatch->wsa__EndpointReference.PortType[0], "ttl" );
        //ws-discovery¹æ¶¨ Îª¿ÉÑ¡Ïî , ServiceName
        ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName = ( struct wsa__ServiceNameType* ) soap_malloc( soap, sizeof( struct wsa__ServiceNameType ) );
        ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName->__item = NULL;
        ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName->PortName = NULL;
        ProbeMatches.ProbeMatch->wsa__EndpointReference.ServiceName->__anyAttribute = NULL;
        //ws-discovery¹æ¶¨ Îª¿ÉÑ¡Ïî , __any
        ProbeMatches.ProbeMatch->wsa__EndpointReference.__any = ( char ** ) soap_malloc( soap, sizeof( char* ) * SMALL_INFO_LENGTH );
        ProbeMatches.ProbeMatch->wsa__EndpointReference.__any[0] = ( char * ) soap_malloc( soap, sizeof( char ) * SMALL_INFO_LENGTH );
        strcpy( ProbeMatches.ProbeMatch->wsa__EndpointReference.__any[0], "Any" );
        //ws-discovery¹æ¶¨ Îª¿ÉÑ¡Ïî , __anyAttribute
        ProbeMatches.ProbeMatch->wsa__EndpointReference.__anyAttribute = ( char * ) soap_malloc( soap, sizeof( char ) * SMALL_INFO_LENGTH );
        strcpy( ProbeMatches.ProbeMatch->wsa__EndpointReference.__anyAttribute, "Attribute" );
        ProbeMatches.ProbeMatch->wsa__EndpointReference.__size = 0;
        //ws-discovery¹æ¶¨ Îª¿ÉÑ¡Ïî , Address
        ProbeMatches.ProbeMatch->wsa__EndpointReference.Address = ( char * ) soap_malloc( soap, sizeof( char ) * INFO_LENGTH );
        strcpy( ProbeMatches.ProbeMatch->wsa__EndpointReference.Address, _HwId );
    }

    if( soap->header == 0 )
    {
        soap->header = (struct SOAP_ENV__Header* ) soap_malloc( soap, sizeof( struct SOAP_ENV__Header ) );
        soap->header->wsa__RelatesTo = (struct  wsa__Relationship* ) soap_malloc( soap, sizeof( struct wsa__Relationship ) );
        //it's here
        soap->header->wsa__MessageID = ( char * ) soap_malloc( soap, sizeof( char ) * INFO_LENGTH );
        strcpy( soap->header->wsa__MessageID, _HwId + 4 );
        soap->header->wsa__RelatesTo->__item = soap->header->wsa__MessageID;
        soap->header->wsa__RelatesTo->RelationshipType = NULL;
        soap->header->wsa__RelatesTo->__anyAttribute = NULL;
    }
    else
    {
        soap->header->wsa__RelatesTo = (struct wsa__Relationship* ) soap_malloc( soap, sizeof( struct wsa__Relationship ) );
        //it's here
        soap->header->wsa__RelatesTo->__item = soap->header->wsa__MessageID;
        soap->header->wsa__RelatesTo->RelationshipType = NULL;
        soap->header->wsa__RelatesTo->__anyAttribute = NULL;

        soap->header->wsa__MessageID = ( char * ) soap_malloc( soap, sizeof( char ) * INFO_LENGTH );
        strcpy( soap->header->wsa__MessageID, _HwId + 4 );
    }

    soap->header->wsa__From = 0;
    soap->header->wsa__ReplyTo = 0;
    soap->header->wsa__FaultTo = 0;
    soap->header->wsdd__AppSequence = 0;
    soap->header->wsa__To = ( char* ) soap_malloc( soap, 128 );
    strcpy( soap->header->wsa__To , "http://schemas.xmlsoap.org/ws/2004/08/addressing/role/anonymous" );
    soap->header->wsa__Action = ( char* ) soap_malloc( soap, 128 );
    strcpy( soap->header->wsa__Action , "http://schemas.xmlsoap.org/ws/2005/04/discovery/ProbeMatches" );

    /* send over current socket as HTTP OK response: */
    soap_send___wsdd__ProbeMatches( soap, "http://", NULL, &ProbeMatches );
    return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tdn__Probe( struct soap* soap, struct wsdd__ProbeType tdn__Probe, struct wsdd__ProbeMatchesType* tdn__ProbeResponse )
{
    return RET_SOAP_ERROR_CODE;
}


SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Hello( struct soap* soap, struct wsdd__HelloType *wsdd__Hello )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tdn__Hello( struct soap* soap, struct wsdd__HelloType tdn__Hello, struct wsdd__ResolveType* tdn__HelloResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Bye( struct soap* soap, struct wsdd__ByeType *wsdd__Bye )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tdn__Bye( struct soap* soap, struct wsdd__ByeType tdn__Bye, struct wsdd__ResolveType* tdn__ByeResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __wsdd__ProbeMatches( struct soap* soap, struct wsdd__ProbeMatchesType *wsdd__ProbeMatches )
{
    return RET_SOAP_ERROR_CODE;
}


SOAP_FMAC5 int SOAP_FMAC6 __wsdd__Resolve( struct soap* soap, struct wsdd__ResolveType *wsdd__Resolve )
{
    return RET_SOAP_ERROR_CODE;
}


SOAP_FMAC5 int SOAP_FMAC6 __wsdd__ResolveMatches( struct soap* soap, struct wsdd__ResolveMatchesType *wsdd__ResolveMatches )
{
    return RET_SOAP_ERROR_CODE;
}


SOAP_FMAC5 int SOAP_FMAC6 __tds__GetServices( struct soap* soap, struct _tds__GetServices *tds__GetServices, struct _tds__GetServicesResponse *tds__GetServicesResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetServiceCapabilities( struct soap* soap, struct _tds__GetServiceCapabilities *tds__GetServiceCapabilities, struct _tds__GetServiceCapabilitiesResponse *tds__GetServiceCapabilitiesResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDeviceInformation( struct soap* soap, struct _tds__GetDeviceInformation *tds__GetDeviceInformation, struct _tds__GetDeviceInformationResponse *tds__GetDeviceInformationResponse )
{

    tds__GetDeviceInformationResponse->HardwareId = ( char * ) soap_malloc( soap, 100 );
    strcpy( tds__GetDeviceInformationResponse->HardwareId, "xchd01" );
    tds__GetDeviceInformationResponse->FirmwareVersion = ( char * ) soap_malloc( soap, 100 );
    strcpy( tds__GetDeviceInformationResponse->FirmwareVersion, "v1.0" );
    tds__GetDeviceInformationResponse->Manufacturer = ( char * ) soap_malloc( soap, 100 );
    strcpy( tds__GetDeviceInformationResponse->Manufacturer, "caoyonghua" );
    tds__GetDeviceInformationResponse->Model = ( char * ) soap_malloc( soap, 100 );
    strcpy( tds__GetDeviceInformationResponse->Model, "ipc01" );
    tds__GetDeviceInformationResponse->SerialNumber = ( char * ) soap_malloc( soap, 100 );
    strcpy( tds__GetDeviceInformationResponse->SerialNumber, "132423423" );
    return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetSystemDateAndTime( struct soap* soap, struct _tds__SetSystemDateAndTime *tds__SetSystemDateAndTime, struct _tds__SetSystemDateAndTimeResponse *tds__SetSystemDateAndTimeResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemDateAndTime( struct soap* soap, struct _tds__GetSystemDateAndTime *tds__GetSystemDateAndTime, struct _tds__GetSystemDateAndTimeResponse *tds__GetSystemDateAndTimeResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetSystemFactoryDefault( struct soap* soap, struct _tds__SetSystemFactoryDefault *tds__SetSystemFactoryDefault, struct _tds__SetSystemFactoryDefaultResponse *tds__SetSystemFactoryDefaultResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__UpgradeSystemFirmware( struct soap* soap, struct _tds__UpgradeSystemFirmware *tds__UpgradeSystemFirmware, struct _tds__UpgradeSystemFirmwareResponse *tds__UpgradeSystemFirmwareResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SystemReboot( struct soap* soap, struct _tds__SystemReboot *tds__SystemReboot, struct _tds__SystemRebootResponse *tds__SystemRebootResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__RestoreSystem( struct soap* soap, struct _tds__RestoreSystem *tds__RestoreSystem, struct _tds__RestoreSystemResponse *tds__RestoreSystemResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemBackup( struct soap* soap, struct _tds__GetSystemBackup *tds__GetSystemBackup, struct _tds__GetSystemBackupResponse *tds__GetSystemBackupResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemLog( struct soap* soap, struct _tds__GetSystemLog *tds__GetSystemLog, struct _tds__GetSystemLogResponse *tds__GetSystemLogResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemSupportInformation( struct soap* soap, struct _tds__GetSystemSupportInformation *tds__GetSystemSupportInformation, struct _tds__GetSystemSupportInformationResponse *tds__GetSystemSupportInformationResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetScopes( struct soap* soap, struct _tds__GetScopes *tds__GetScopes, struct _tds__GetScopesResponse *tds__GetScopesResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetScopes( struct soap* soap, struct _tds__SetScopes *tds__SetScopes, struct _tds__SetScopesResponse *tds__SetScopesResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__AddScopes( struct soap* soap, struct _tds__AddScopes *tds__AddScopes, struct _tds__AddScopesResponse *tds__AddScopesResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__RemoveScopes( struct soap* soap, struct _tds__RemoveScopes *tds__RemoveScopes, struct _tds__RemoveScopesResponse *tds__RemoveScopesResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDiscoveryMode( struct soap* soap, struct _tds__GetDiscoveryMode *tds__GetDiscoveryMode, struct _tds__GetDiscoveryModeResponse *tds__GetDiscoveryModeResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDiscoveryMode( struct soap* soap, struct _tds__SetDiscoveryMode *tds__SetDiscoveryMode, struct _tds__SetDiscoveryModeResponse *tds__SetDiscoveryModeResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRemoteDiscoveryMode( struct soap* soap, struct _tds__GetRemoteDiscoveryMode *tds__GetRemoteDiscoveryMode, struct _tds__GetRemoteDiscoveryModeResponse *tds__GetRemoteDiscoveryModeResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRemoteDiscoveryMode( struct soap* soap, struct _tds__SetRemoteDiscoveryMode *tds__SetRemoteDiscoveryMode, struct _tds__SetRemoteDiscoveryModeResponse *tds__SetRemoteDiscoveryModeResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDPAddresses( struct soap* soap, struct _tds__GetDPAddresses *tds__GetDPAddresses, struct _tds__GetDPAddressesResponse *tds__GetDPAddressesResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetEndpointReference( struct soap* soap, struct _tds__GetEndpointReference *tds__GetEndpointReference, struct _tds__GetEndpointReferenceResponse *tds__GetEndpointReferenceResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRemoteUser( struct soap* soap, struct _tds__GetRemoteUser *tds__GetRemoteUser, struct _tds__GetRemoteUserResponse *tds__GetRemoteUserResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRemoteUser( struct soap* soap, struct _tds__SetRemoteUser *tds__SetRemoteUser, struct _tds__SetRemoteUserResponse *tds__SetRemoteUserResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetUsers( struct soap* soap, struct _tds__GetUsers *tds__GetUsers, struct _tds__GetUsersResponse *tds__GetUsersResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateUsers( struct soap* soap, struct _tds__CreateUsers *tds__CreateUsers, struct _tds__CreateUsersResponse *tds__CreateUsersResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteUsers( struct soap* soap, struct _tds__DeleteUsers *tds__DeleteUsers, struct _tds__DeleteUsersResponse *tds__DeleteUsersResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetUser( struct soap* soap, struct _tds__SetUser *tds__SetUser, struct _tds__SetUserResponse *tds__SetUserResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetWsdlUrl( struct soap* soap, struct _tds__GetWsdlUrl *tds__GetWsdlUrl, struct _tds__GetWsdlUrlResponse *tds__GetWsdlUrlResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCapabilities( struct soap* soap, struct _tds__GetCapabilities *tds__GetCapabilities, struct _tds__GetCapabilitiesResponse *tds__GetCapabilitiesResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDPAddresses( struct soap* soap, struct _tds__SetDPAddresses *tds__SetDPAddresses, struct _tds__SetDPAddressesResponse *tds__SetDPAddressesResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetHostname( struct soap* soap, struct _tds__GetHostname *tds__GetHostname, struct _tds__GetHostnameResponse *tds__GetHostnameResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetHostname( struct soap* soap, struct _tds__SetHostname *tds__SetHostname, struct _tds__SetHostnameResponse *tds__SetHostnameResponse )
{
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetHostnameFromDHCP( struct soap* soap, struct _tds__SetHostnameFromDHCP *tds__SetHostnameFromDHCP, struct _tds__SetHostnameFromDHCPResponse *tds__SetHostnameFromDHCPResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDNS( struct soap* soap, struct _tds__GetDNS *tds__GetDNS, struct _tds__GetDNSResponse *tds__GetDNSResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDNS( struct soap* soap, struct _tds__SetDNS *tds__SetDNS, struct _tds__SetDNSResponse *tds__SetDNSResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNTP( struct soap* soap, struct _tds__GetNTP *tds__GetNTP, struct _tds__GetNTPResponse *tds__GetNTPResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNTP( struct soap* soap, struct _tds__SetNTP *tds__SetNTP, struct _tds__SetNTPResponse *tds__SetNTPResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDynamicDNS( struct soap* soap, struct _tds__GetDynamicDNS *tds__GetDynamicDNS, struct _tds__GetDynamicDNSResponse *tds__GetDynamicDNSResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDynamicDNS( struct soap* soap, struct _tds__SetDynamicDNS *tds__SetDynamicDNS, struct _tds__SetDynamicDNSResponse *tds__SetDynamicDNSResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkInterfaces( struct soap* soap, struct _tds__GetNetworkInterfaces *tds__GetNetworkInterfaces, struct _tds__GetNetworkInterfacesResponse *tds__GetNetworkInterfacesResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkInterfaces( struct soap* soap, struct _tds__SetNetworkInterfaces *tds__SetNetworkInterfaces, struct _tds__SetNetworkInterfacesResponse *tds__SetNetworkInterfacesResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkProtocols( struct soap* soap, struct _tds__GetNetworkProtocols *tds__GetNetworkProtocols, struct _tds__GetNetworkProtocolsResponse *tds__GetNetworkProtocolsResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkProtocols( struct soap* soap, struct _tds__SetNetworkProtocols *tds__SetNetworkProtocols, struct _tds__SetNetworkProtocolsResponse *tds__SetNetworkProtocolsResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkDefaultGateway( struct soap* soap, struct _tds__GetNetworkDefaultGateway *tds__GetNetworkDefaultGateway, struct _tds__GetNetworkDefaultGatewayResponse *tds__GetNetworkDefaultGatewayResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkDefaultGateway( struct soap* soap, struct _tds__SetNetworkDefaultGateway *tds__SetNetworkDefaultGateway, struct _tds__SetNetworkDefaultGatewayResponse *tds__SetNetworkDefaultGatewayResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetZeroConfiguration( struct soap* soap, struct _tds__GetZeroConfiguration *tds__GetZeroConfiguration, struct _tds__GetZeroConfigurationResponse *tds__GetZeroConfigurationResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetZeroConfiguration( struct soap* soap, struct _tds__SetZeroConfiguration *tds__SetZeroConfiguration, struct _tds__SetZeroConfigurationResponse *tds__SetZeroConfigurationResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetIPAddressFilter( struct soap* soap, struct _tds__GetIPAddressFilter *tds__GetIPAddressFilter, struct _tds__GetIPAddressFilterResponse *tds__GetIPAddressFilterResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetIPAddressFilter( struct soap* soap, struct _tds__SetIPAddressFilter *tds__SetIPAddressFilter, struct _tds__SetIPAddressFilterResponse *tds__SetIPAddressFilterResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__AddIPAddressFilter( struct soap* soap, struct _tds__AddIPAddressFilter *tds__AddIPAddressFilter, struct _tds__AddIPAddressFilterResponse *tds__AddIPAddressFilterResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__RemoveIPAddressFilter( struct soap* soap, struct _tds__RemoveIPAddressFilter *tds__RemoveIPAddressFilter, struct _tds__RemoveIPAddressFilterResponse *tds__RemoveIPAddressFilterResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetAccessPolicy( struct soap* soap, struct _tds__GetAccessPolicy *tds__GetAccessPolicy, struct _tds__GetAccessPolicyResponse *tds__GetAccessPolicyResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetAccessPolicy( struct soap* soap, struct _tds__SetAccessPolicy *tds__SetAccessPolicy, struct _tds__SetAccessPolicyResponse *tds__SetAccessPolicyResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateCertificate( struct soap* soap, struct _tds__CreateCertificate *tds__CreateCertificate, struct _tds__CreateCertificateResponse *tds__CreateCertificateResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificates( struct soap* soap, struct _tds__GetCertificates *tds__GetCertificates, struct _tds__GetCertificatesResponse *tds__GetCertificatesResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificatesStatus( struct soap* soap, struct _tds__GetCertificatesStatus *tds__GetCertificatesStatus, struct _tds__GetCertificatesStatusResponse *tds__GetCertificatesStatusResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetCertificatesStatus( struct soap* soap, struct _tds__SetCertificatesStatus *tds__SetCertificatesStatus, struct _tds__SetCertificatesStatusResponse *tds__SetCertificatesStatusResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteCertificates( struct soap* soap, struct _tds__DeleteCertificates *tds__DeleteCertificates, struct _tds__DeleteCertificatesResponse *tds__DeleteCertificatesResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetPkcs10Request( struct soap* soap, struct _tds__GetPkcs10Request *tds__GetPkcs10Request, struct _tds__GetPkcs10RequestResponse *tds__GetPkcs10RequestResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCertificates( struct soap* soap, struct _tds__LoadCertificates *tds__LoadCertificates, struct _tds__LoadCertificatesResponse *tds__LoadCertificatesResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetClientCertificateMode( struct soap* soap, struct _tds__GetClientCertificateMode *tds__GetClientCertificateMode, struct _tds__GetClientCertificateModeResponse *tds__GetClientCertificateModeResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetClientCertificateMode( struct soap* soap, struct _tds__SetClientCertificateMode *tds__SetClientCertificateMode, struct _tds__SetClientCertificateModeResponse *tds__SetClientCertificateModeResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRelayOutputs( struct soap* soap, struct _tds__GetRelayOutputs *tds__GetRelayOutputs, struct _tds__GetRelayOutputsResponse *tds__GetRelayOutputsResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRelayOutputSettings( struct soap* soap, struct _tds__SetRelayOutputSettings *tds__SetRelayOutputSettings, struct _tds__SetRelayOutputSettingsResponse *tds__SetRelayOutputSettingsResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRelayOutputState( struct soap* soap, struct _tds__SetRelayOutputState *tds__SetRelayOutputState, struct _tds__SetRelayOutputStateResponse *tds__SetRelayOutputStateResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SendAuxiliaryCommand( struct soap* soap, struct _tds__SendAuxiliaryCommand *tds__SendAuxiliaryCommand, struct _tds__SendAuxiliaryCommandResponse *tds__SendAuxiliaryCommandResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCACertificates( struct soap* soap, struct _tds__GetCACertificates *tds__GetCACertificates, struct _tds__GetCACertificatesResponse *tds__GetCACertificatesResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCertificateWithPrivateKey( struct soap* soap, struct _tds__LoadCertificateWithPrivateKey *tds__LoadCertificateWithPrivateKey, struct _tds__LoadCertificateWithPrivateKeyResponse *tds__LoadCertificateWithPrivateKeyResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificateInformation( struct soap* soap, struct _tds__GetCertificateInformation *tds__GetCertificateInformation, struct _tds__GetCertificateInformationResponse *tds__GetCertificateInformationResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCACertificates( struct soap* soap, struct _tds__LoadCACertificates *tds__LoadCACertificates, struct _tds__LoadCACertificatesResponse *tds__LoadCACertificatesResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateDot1XConfiguration( struct soap* soap, struct _tds__CreateDot1XConfiguration *tds__CreateDot1XConfiguration, struct _tds__CreateDot1XConfigurationResponse *tds__CreateDot1XConfigurationResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDot1XConfiguration( struct soap* soap, struct _tds__SetDot1XConfiguration *tds__SetDot1XConfiguration, struct _tds__SetDot1XConfigurationResponse *tds__SetDot1XConfigurationResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot1XConfiguration( struct soap* soap, struct _tds__GetDot1XConfiguration *tds__GetDot1XConfiguration, struct _tds__GetDot1XConfigurationResponse *tds__GetDot1XConfigurationResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot1XConfigurations( struct soap* soap, struct _tds__GetDot1XConfigurations *tds__GetDot1XConfigurations, struct _tds__GetDot1XConfigurationsResponse *tds__GetDot1XConfigurationsResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteDot1XConfiguration( struct soap* soap, struct _tds__DeleteDot1XConfiguration *tds__DeleteDot1XConfiguration, struct _tds__DeleteDot1XConfigurationResponse *tds__DeleteDot1XConfigurationResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot11Capabilities( struct soap* soap, struct _tds__GetDot11Capabilities *tds__GetDot11Capabilities, struct _tds__GetDot11CapabilitiesResponse *tds__GetDot11CapabilitiesResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot11Status( struct soap* soap, struct _tds__GetDot11Status *tds__GetDot11Status, struct _tds__GetDot11StatusResponse *tds__GetDot11StatusResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__ScanAvailableDot11Networks( struct soap* soap, struct _tds__ScanAvailableDot11Networks *tds__ScanAvailableDot11Networks, struct _tds__ScanAvailableDot11NetworksResponse *tds__ScanAvailableDot11NetworksResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemUris( struct soap* soap, struct _tds__GetSystemUris *tds__GetSystemUris, struct _tds__GetSystemUrisResponse *tds__GetSystemUrisResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__StartFirmwareUpgrade( struct soap* soap, struct _tds__StartFirmwareUpgrade *tds__StartFirmwareUpgrade, struct _tds__StartFirmwareUpgradeResponse *tds__StartFirmwareUpgradeResponse )
{
    return RET_SOAP_ERROR_CODE;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__StartSystemRestore( struct soap* soap, struct _tds__StartSystemRestore *tds__StartSystemRestore, struct _tds__StartSystemRestoreResponse *tds__StartSystemRestoreResponse )
{
    return RET_SOAP_ERROR_CODE;
}
