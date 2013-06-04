#include <misc.h>
#include "soapH.h"
#include "soapStub.h"
#include "onvifHandle.h"
#include "appCommon.h"
#include "appTools.h"
#include "logInfo.h"

void getDeviceManagementServicesResponseService(struct soap* soap,
		struct tds__Service *service, const char* nameSpace,
		const char* address) {
	service->XAddr = (char *) my_soap_malloc(soap, sizeof(char) * INFO_LENGTH);
	service->Namespace = (char *) my_soap_malloc(soap,
			sizeof(char) * INFO_LENGTH);
	strcpy(service->Namespace, nameSpace);
	strcpy(service->XAddr, address);
	service->Capabilities = NULL;
	service->Version = (struct tt__OnvifVersion *) my_soap_malloc(soap,
			sizeof(struct tt__OnvifVersion));
	service->Version->Major = ONVIF_VERSION_MAJOR;
	service->Version->Minor = ONVIF_VERSION_MINOR;
	service->__size = 0;
	service->__anyAttribute = NULL;
	service->__any = NULL;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetServices(struct soap* soap,
		struct _tds__GetServices *tds__GetServices,
		struct _tds__GetServicesResponse *tds__GetServicesResponse) {
	logInfo("__tds__GetServices");
	char _xmAddr[INFO_LENGTH] = { 0 };
	char* _services[] = { "event", "media", "ptz" };
	char* _namespaces[] = { "http://www.onvif.org/ver10/events/wsdl",
			"http://www.onvif.org/ver10/media/wsdl",
			"http://www.onvif.org/ver20/ptz/wsdl" };
	if (RET_CODE_SUCCESS
			!= getServiceURL(_xmAddr, onvifRunParam.ip,
					onvifRunParam.servicePort)) {
		return getOnvifSoapActionFailedCode(soap, "GetServices",
				"not get service url");
	}
	char serviceAddr[INFO_LENGTH] = { 0 };
	int i;
	if (!tds__GetServices->IncludeCapability) {
		tds__GetServicesResponse->__sizeService = 3;
		tds__GetServicesResponse->Service =
				(struct tds__Service *) my_soap_malloc(
						soap,
						sizeof(struct tds__Service)
								* tds__GetServicesResponse->__sizeService);
		for (i = 0; i < tds__GetServicesResponse->__sizeService; i++) {
			memset(serviceAddr, 0, INFO_LENGTH);
			getAppointServiceURL(serviceAddr, _xmAddr, _services[i]);
			getDeviceManagementServicesResponseService(soap,
					&(tds__GetServicesResponse->Service[i]), _namespaces[i],
					serviceAddr);
		}
		return SOAP_OK;
	} else {
		tds__GetServicesResponse->Service =
				(struct tds__Service *) my_soap_malloc(soap,
						sizeof(struct tds__Service));
		tds__GetServicesResponse->Service->XAddr = (char *) my_soap_malloc(soap,
				sizeof(char) * INFO_LENGTH);
		tds__GetServicesResponse->__sizeService = 1;
		tds__GetServicesResponse->Service->Namespace = NULL;
		strcpy(tds__GetServicesResponse->Service->XAddr, _xmAddr);
		tds__GetServicesResponse->Service->Capabilities = NULL;
		tds__GetServicesResponse->Service->Version =
				(struct tt__OnvifVersion *) my_soap_malloc(soap,
						sizeof(struct tt__OnvifVersion));
		tds__GetServicesResponse->Service->Version->Major = ONVIF_VERSION_MAJOR;
		tds__GetServicesResponse->Service->Version->Minor = ONVIF_VERSION_MINOR;
		tds__GetServicesResponse->Service->__size = 2;
		char** pot = (char **) my_soap_malloc(soap,
				sizeof(char *) * tds__GetServicesResponse->Service->__size);

		pot[0] = (char *) my_soap_malloc(soap, sizeof(char) * INFO_LENGTH);
		strcpy(pot[0], "Any1");
		pot[1] = (char *) my_soap_malloc(soap, sizeof(char) * INFO_LENGTH);
		strcpy(pot[1], "Any2");
		tds__GetServicesResponse->Service->__any = pot;
		tds__GetServicesResponse->Service->__anyAttribute = NULL;
	}
	return SOAP_OK;
}

void getServiceCapabilitiesNetworkSettings(struct soap* soap,
		struct tds__DeviceServiceCapabilities *deviceServiceCapabilities) {
	/* NETWORK */
	deviceServiceCapabilities->Network =
			(struct tds__NetworkCapabilities *) my_soap_malloc(soap,
					sizeof(struct tds__NetworkCapabilities));
	deviceServiceCapabilities->Network->IPFilter =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	deviceServiceCapabilities->Network->ZeroConfiguration =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	deviceServiceCapabilities->Network->IPVersion6 =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	deviceServiceCapabilities->Network->DynDNS =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	deviceServiceCapabilities->Network->Dot11Configuration =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	deviceServiceCapabilities->Network->HostnameFromDHCP =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*deviceServiceCapabilities->Network->IPFilter = xsd__boolean__false_;

	*deviceServiceCapabilities->Network->ZeroConfiguration =
			xsd__boolean__false_;
	*deviceServiceCapabilities->Network->IPVersion6 = xsd__boolean__false_;
	*deviceServiceCapabilities->Network->DynDNS = xsd__boolean__false_;
	*deviceServiceCapabilities->Network->Dot11Configuration =
			xsd__boolean__false_;
	*deviceServiceCapabilities->Network->HostnameFromDHCP =
			xsd__boolean__false_;
	*deviceServiceCapabilities->Network->NTP = 1;
}

void getServiceCapabilitiesSystemSettings(struct soap* soap,
		struct tds__DeviceServiceCapabilities *deviceServiceCapabilities) {
	/* SYSTEM */
	deviceServiceCapabilities->System =
			(struct tds__SystemCapabilities *) my_soap_malloc(soap,
					sizeof(struct tds__SystemCapabilities));
	deviceServiceCapabilities->System->DiscoveryResolve =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*deviceServiceCapabilities->System->DiscoveryResolve = xsd__boolean__false_;
	deviceServiceCapabilities->System->DiscoveryBye =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*deviceServiceCapabilities->System->DiscoveryBye = xsd__boolean__false_;
	deviceServiceCapabilities->System->RemoteDiscovery =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*deviceServiceCapabilities->System->RemoteDiscovery = xsd__boolean__false_;
	deviceServiceCapabilities->System->SystemBackup =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*deviceServiceCapabilities->System->SystemBackup = xsd__boolean__false_;
	deviceServiceCapabilities->System->FirmwareUpgrade =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*deviceServiceCapabilities->System->FirmwareUpgrade = xsd__boolean__false_;
	deviceServiceCapabilities->System->SystemLogging =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*deviceServiceCapabilities->System->SystemLogging = xsd__boolean__false_;
	deviceServiceCapabilities->System->HttpSystemBackup =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*deviceServiceCapabilities->System->HttpSystemBackup = xsd__boolean__false_;
	deviceServiceCapabilities->System->HttpFirmwareUpgrade =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*deviceServiceCapabilities->System->HttpFirmwareUpgrade =
			xsd__boolean__false_;
	deviceServiceCapabilities->System->HttpSystemLogging =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*deviceServiceCapabilities->System->HttpSystemLogging =
			xsd__boolean__false_;
	deviceServiceCapabilities->System->HttpSupportInformation =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*deviceServiceCapabilities->System->HttpSupportInformation =
			xsd__boolean__false_;
}

void getServiceCapabilitiesSecuritySettings(struct soap* soap,
		struct tds__DeviceServiceCapabilities *deviceServiceCapabilities) {
	/* SECURITY */
	deviceServiceCapabilities->Security =
			(struct tds__SecurityCapabilities *) my_soap_malloc(soap,
					sizeof(struct tds__SecurityCapabilities));
	deviceServiceCapabilities->Security->TLS1_x002e0 =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*deviceServiceCapabilities->Security->TLS1_x002e0 = xsd__boolean__false_;
	deviceServiceCapabilities->Security->TLS1_x002e1 =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*deviceServiceCapabilities->Security->TLS1_x002e1 = xsd__boolean__false_;
	deviceServiceCapabilities->Security->TLS1_x002e2 =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*deviceServiceCapabilities->Security->TLS1_x002e2 = xsd__boolean__false_;
	deviceServiceCapabilities->Security->OnboardKeyGeneration =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*deviceServiceCapabilities->Security->OnboardKeyGeneration =
			xsd__boolean__false_;
	deviceServiceCapabilities->Security->AccessPolicyConfig =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*deviceServiceCapabilities->Security->AccessPolicyConfig =
			xsd__boolean__false_;
	deviceServiceCapabilities->Security->Dot1X =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*deviceServiceCapabilities->Security->Dot1X = xsd__boolean__false_;
	deviceServiceCapabilities->Security->RemoteUserHandling =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*deviceServiceCapabilities->Security->RemoteUserHandling =
			xsd__boolean__false_;
	deviceServiceCapabilities->Security->X_x002e509Token =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*deviceServiceCapabilities->Security->X_x002e509Token =
			xsd__boolean__false_;
	deviceServiceCapabilities->Security->SAMLToken =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*deviceServiceCapabilities->Security->SAMLToken = xsd__boolean__false_;
	deviceServiceCapabilities->Security->KerberosToken =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*deviceServiceCapabilities->Security->KerberosToken = xsd__boolean__false_;
	deviceServiceCapabilities->Security->UsernameToken =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*deviceServiceCapabilities->Security->UsernameToken = xsd__boolean__false_;
	deviceServiceCapabilities->Security->HttpDigest =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*deviceServiceCapabilities->Security->HttpDigest = xsd__boolean__false_;
	deviceServiceCapabilities->Security->RELToken =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*deviceServiceCapabilities->Security->RELToken = xsd__boolean__false_;
	deviceServiceCapabilities->Security->SupportedEAPMethods = NULL;
	deviceServiceCapabilities->Security->__anyAttribute = NULL;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetServiceCapabilities(
		struct soap* soap,
		struct _tds__GetServiceCapabilities *tds__GetServiceCapabilities,
		struct _tds__GetServiceCapabilitiesResponse *tds__GetServiceCapabilitiesResponse) {
	logInfo("__tds__GetServiceCapabilities");
	tds__GetServiceCapabilitiesResponse->Capabilities =
			(struct tds__DeviceServiceCapabilities *) my_soap_malloc(soap,
					sizeof(struct tds__DeviceServiceCapabilities));

	getServiceCapabilitiesNetworkSettings(soap,
			tds__GetServiceCapabilitiesResponse->Capabilities);
	getServiceCapabilitiesSystemSettings(soap,
			tds__GetServiceCapabilitiesResponse->Capabilities);
	getServiceCapabilitiesSecuritySettings(soap,
			tds__GetServiceCapabilitiesResponse->Capabilities);

	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDeviceInformation(
		struct soap* soap,
		struct _tds__GetDeviceInformation *tds__GetDeviceInformation,
		struct _tds__GetDeviceInformationResponse *tds__GetDeviceInformationResponse) {
	logInfo("__tds__GetDeviceInformation");
	OnvifDeviceInfo info;
	if (!isRetCodeSuccess(getDeviceInfo(&info))) {
		return getOnvifSoapActionFailedCode(soap, "GetDeviceInformation",
				"not get deviceinfo");
	}
	tds__GetDeviceInformationResponse->HardwareId = (char *) my_soap_malloc(
			soap, INFO_LENGTH);
	strcpy(tds__GetDeviceInformationResponse->HardwareId, info.hardwareId);
	tds__GetDeviceInformationResponse->FirmwareVersion =
			(char *) my_soap_malloc(soap, INFO_LENGTH);
	strcpy(tds__GetDeviceInformationResponse->FirmwareVersion,
			info.firmwareVersion);
	tds__GetDeviceInformationResponse->Manufacturer = (char *) my_soap_malloc(
			soap, INFO_LENGTH);
	strcpy(tds__GetDeviceInformationResponse->Manufacturer, info.manufacturer);
	tds__GetDeviceInformationResponse->Model = (char *) my_soap_malloc(soap,
			INFO_LENGTH);
	strcpy(tds__GetDeviceInformationResponse->Model, info.model);
	tds__GetDeviceInformationResponse->SerialNumber = (char *) my_soap_malloc(
			soap, INFO_LENGTH);
	strcpy(tds__GetDeviceInformationResponse->SerialNumber, info.serialNumber);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetSystemDateAndTime(
		struct soap* soap,
		struct _tds__SetSystemDateAndTime *tds__SetSystemDateAndTime,
		struct _tds__SetSystemDateAndTimeResponse *tds__SetSystemDateAndTimeResponse) {
	logInfo("__tds__SetSystemDateAndTime");
	OnvifSystemDateTime info;
	if (tt__SetDateTimeType__NTP == tds__SetSystemDateAndTime->DateTimeType) {
		logInfo("__tds__SetSystemDateAndTime is set ntp");
		info.ntpSet = true;
	} else {
		info.ntpSet = false;
		if (NULL == tds__SetSystemDateAndTime->TimeZone)
			logInfo("__tds__SetSystemDateAndTime is TimeZone NULL");
		else
			logInfo("__tds__SetSystemDateAndTime TimeZone %s",
					tds__SetSystemDateAndTime->TimeZone);
		info.timeZone = 8;
		if (NULL == tds__SetSystemDateAndTime->UTCDateTime) {
			return getOnvifSoapSenderSubCode2Fault(soap, "ter:InvalidArgVal",
					"ter:InvalidDateTime", "Device Manager Set SystemDateTime",
					"UTCDateTime is null");
		} else if (NULL == tds__SetSystemDateAndTime->UTCDateTime->Date) {
			return getOnvifSoapSenderSubCode2Fault(soap, "ter:InvalidArgVal",
					"ter:InvalidDateTime", "Device Manager Set SystemDateTime",
					"UTCDateTime  Date is null");
		} else if (NULL == tds__SetSystemDateAndTime->UTCDateTime->Time) {
			return getOnvifSoapSenderSubCode2Fault(soap, "ter:InvalidArgVal",
					"ter:InvalidDateTime", "Device Manager Set SystemDateTime",
					"UTCDateTime  Time is null");
		}
		struct tm tm1;
		tm1.tm_year = tds__SetSystemDateAndTime->UTCDateTime->Date->Year - 1900;
		tm1.tm_mon = tds__SetSystemDateAndTime->UTCDateTime->Date->Month - 1;
		tm1.tm_mday = tds__SetSystemDateAndTime->UTCDateTime->Date->Day;
		tm1.tm_isdst = 0;
		tm1.tm_hour = tds__SetSystemDateAndTime->UTCDateTime->Time->Hour;
		tm1.tm_min = tds__SetSystemDateAndTime->UTCDateTime->Time->Minute;
		tm1.tm_sec = tds__SetSystemDateAndTime->UTCDateTime->Time->Second;
		info.localTime = mktime(&tm1);
		char dtinfo[INFO_LENGTH] = {0};
		getDateTimeStr(dtinfo, INFO_LENGTH, info.localTime);
		logInfo("__tds__SetSystemDateAndTime set time %s", dtinfo);
	}
	if (!isRetCodeSuccess(setDeviceTime(&info))) {
		return getOnvifSoapSenderSubCode2Fault(soap, "ter:InvalidArgVal",
				"ter:InvalidDateTime", "Device Manager Set SystemDateTime",
				"Set Error");
	}
	return SOAP_OK;
}

struct tt__DateTime * getSystemDateAndTimeDateTimeValue(struct soap* soap,
		struct tm* timeValue) {
	struct tt__DateTime * result = (struct tt__DateTime *) my_soap_malloc(soap,
			sizeof(struct tt__DateTime));
	result->Date = (struct tt__Date *) my_soap_malloc(soap,
			sizeof(struct tt__Date));
	result->Date->Year = timeValue->tm_year + 1900;
	result->Date->Month = timeValue->tm_mon + 1;
	result->Date->Day = timeValue->tm_mday;
	result->Time = (struct tt__Time *) my_soap_malloc(soap,
			sizeof(struct tt__Time));
	result->Time->Hour = timeValue->tm_hour;
	result->Time->Minute = timeValue->tm_min;
	result->Time->Second = timeValue->tm_sec;
	return result;
}

struct tt__SystemDateTime* getSystemDateAndTimeSystemTimeInfo(struct soap* soap,
		OnvifSystemDateTime* info) {
	struct tt__SystemDateTime* result =
			(struct tt__SystemDateTime *) my_soap_malloc(soap,
					sizeof(struct tt__SystemDateTime));
	result->DateTimeType = tt__SetDateTimeType__Manual;
	if (info->ntpSet) {
		result->DateTimeType = tt__SetDateTimeType__NTP;
		return result;
	}
	result->DaylightSavings = xsd__boolean__false_;
	struct tm* tm1;
	tm1 = localtime(&info->localTime);
	result->LocalDateTime = getSystemDateAndTimeDateTimeValue(soap, tm1);
	tm1 = gmtime(&info->localTime);
	result->UTCDateTime = getSystemDateAndTimeDateTimeValue(soap, tm1);
	result->TimeZone = (struct tt__TimeZone *) my_soap_malloc(soap,
			sizeof(struct tt__TimeZone));
	result->TimeZone->TZ = (char*) my_soap_malloc(soap, SMALL_INFO_LENGTH);
	sprintf(result->TimeZone->TZ, "GMT+%d", info->timeZone);
	return result;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemDateAndTime(
		struct soap* soap,
		struct _tds__GetSystemDateAndTime *tds__GetSystemDateAndTime,
		struct _tds__GetSystemDateAndTimeResponse *tds__GetSystemDateAndTimeResponse) {
	logInfo("__tds__GetSystemDateAndTime");
	OnvifSystemDateTime info;
	if (!isRetCodeSuccess(getDeviceTime(&info))) {
		return getOnvifSoapActionNotSupportCode(soap,
				"Device Manager GetSystemDateAndTime", NULL);
	}
	tds__GetSystemDateAndTimeResponse->SystemDateAndTime =
			getSystemDateAndTimeSystemTimeInfo(soap, &info);
	return SOAP_OK;

}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetSystemFactoryDefault(
		struct soap* soap,
		struct _tds__SetSystemFactoryDefault *tds__SetSystemFactoryDefault,
		struct _tds__SetSystemFactoryDefaultResponse *tds__SetSystemFactoryDefaultResponse) {
	logInfo("__tds__SetSystemFactoryDefault");
	if (!isRetCodeSuccess(setSystemFactoryDefault())) {
		return getOnvifSoapActionFailedCode(soap, "RestoreSystem",
				"restore failed");
	}
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__UpgradeSystemFirmware(
		struct soap* soap,
		struct _tds__UpgradeSystemFirmware *tds__UpgradeSystemFirmware,
		struct _tds__UpgradeSystemFirmwareResponse *tds__UpgradeSystemFirmwareResponse) {
	logInfo("__tds__UpgradeSystemFirmware");
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager UpgradeSystemFirmware", NULL);

}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SystemReboot(struct soap* soap,
		struct _tds__SystemReboot *tds__SystemReboot,
		struct _tds__SystemRebootResponse *tds__SystemRebootResponse) {
	logInfo("__tds__SystemReboot");
	if (!isRetCodeSuccess(deviceReboot())) {
		return getOnvifSoapActionFailedCode(soap, "SystemReboot",
				"reboot failed");
	}
	tds__SystemRebootResponse->Message = (char*) my_soap_malloc(soap,
			INFO_LENGTH);
	strcpy(tds__SystemRebootResponse->Message, "Rebooting in 5 seconds.");
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__RestoreSystem(struct soap* soap,
		struct _tds__RestoreSystem *tds__RestoreSystem,
		struct _tds__RestoreSystemResponse *tds__RestoreSystemResponse) {
	logInfo("__tds__RestoreSystem");
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager RestoreSystem", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemBackup(struct soap* soap,
		struct _tds__GetSystemBackup *tds__GetSystemBackup,
		struct _tds__GetSystemBackupResponse *tds__GetSystemBackupResponse) {
	logInfo("__tds__GetSystemBackup");
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager RestoreSystem", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemLog(struct soap* soap,
		struct _tds__GetSystemLog *tds__GetSystemLog,
		struct _tds__GetSystemLogResponse *tds__GetSystemLogResponse) {
	logInfo("__tds__GetSystemLog");
	return getOnvifSoapActionNotSupportCode(soap, "Device Manager GetSystemLog",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemSupportInformation(
		struct soap* soap,
		struct _tds__GetSystemSupportInformation *tds__GetSystemSupportInformation,
		struct _tds__GetSystemSupportInformationResponse *tds__GetSystemSupportInformationResponse) {
	logInfo("__tds__GetSystemSupportInformation");
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager GetSystemSupportInformation", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetScopes(struct soap* soap,
		struct _tds__GetScopes *tds__GetScopes,
		struct _tds__GetScopesResponse *tds__GetScopesResponse) {
	logInfo("__tds__GetScopes");
	int size = 1;
	tds__GetScopesResponse->__sizeScopes = size;
	tds__GetScopesResponse->Scopes = (struct tt__Scope*) my_soap_malloc(soap,
			size * sizeof(struct tt__Scope));
	tds__GetScopesResponse->Scopes->ScopeItem = (char*) my_soap_malloc(soap,
			INFO_LENGTH);
	tds__GetScopesResponse->Scopes->ScopeDef = 0;
	strcpy(tds__GetScopesResponse->Scopes->ScopeItem, ONVIF_SCOPE_NAME);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetScopes(struct soap* soap,
		struct _tds__SetScopes *tds__SetScopes,
		struct _tds__SetScopesResponse *tds__SetScopesResponse) {
	return getOnvifSoapActionNotSupportCode(soap, "Device Manager SetScopes",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__AddScopes(struct soap* soap,
		struct _tds__AddScopes *tds__AddScopes,
		struct _tds__AddScopesResponse *tds__AddScopesResponse) {
	return getOnvifSoapActionNotSupportCode(soap, "Device Manager AddScopes",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__RemoveScopes(struct soap* soap,
		struct _tds__RemoveScopes *tds__RemoveScopes,
		struct _tds__RemoveScopesResponse *tds__RemoveScopesResponse) {
	return getOnvifSoapActionNotSupportCode(soap, "Device Manager RemoveScopes",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDiscoveryMode(struct soap* soap,
		struct _tds__GetDiscoveryMode *tds__GetDiscoveryMode,
		struct _tds__GetDiscoveryModeResponse *tds__GetDiscoveryModeResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager GetDiscoveryMode", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDiscoveryMode(struct soap* soap,
		struct _tds__SetDiscoveryMode *tds__SetDiscoveryMode,
		struct _tds__SetDiscoveryModeResponse *tds__SetDiscoveryModeResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager SetDiscoveryMode", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRemoteDiscoveryMode(
		struct soap* soap,
		struct _tds__GetRemoteDiscoveryMode *tds__GetRemoteDiscoveryMode,
		struct _tds__GetRemoteDiscoveryModeResponse *tds__GetRemoteDiscoveryModeResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager GetRemoteDiscoveryMode", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRemoteDiscoveryMode(
		struct soap* soap,
		struct _tds__SetRemoteDiscoveryMode *tds__SetRemoteDiscoveryMode,
		struct _tds__SetRemoteDiscoveryModeResponse *tds__SetRemoteDiscoveryModeResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager SetRemoteDiscoveryMode", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDPAddresses(struct soap* soap,
		struct _tds__GetDPAddresses *tds__GetDPAddresses,
		struct _tds__GetDPAddressesResponse *tds__GetDPAddressesResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager GetDPAddresses", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetEndpointReference(
		struct soap* soap,
		struct _tds__GetEndpointReference *tds__GetEndpointReference,
		struct _tds__GetEndpointReferenceResponse *tds__GetEndpointReferenceResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager GetEndpointReference", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRemoteUser(struct soap* soap,
		struct _tds__GetRemoteUser *tds__GetRemoteUser,
		struct _tds__GetRemoteUserResponse *tds__GetRemoteUserResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager GetRemoteUser", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRemoteUser(struct soap* soap,
		struct _tds__SetRemoteUser *tds__SetRemoteUser,
		struct _tds__SetRemoteUserResponse *tds__SetRemoteUserResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager SetRemoteUser", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetUsers(struct soap* soap,
		struct _tds__GetUsers *tds__GetUsers,
		struct _tds__GetUsersResponse *tds__GetUsersResponse) {
	return getOnvifSoapActionNotSupportCode(soap, "Device Manager GetUsers",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateUsers(struct soap* soap,
		struct _tds__CreateUsers *tds__CreateUsers,
		struct _tds__CreateUsersResponse *tds__CreateUsersResponse) {
	return getOnvifSoapActionNotSupportCode(soap, "Device Manager CreateUsers",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteUsers(struct soap* soap,
		struct _tds__DeleteUsers *tds__DeleteUsers,
		struct _tds__DeleteUsersResponse *tds__DeleteUsersResponse) {
	return getOnvifSoapActionNotSupportCode(soap, "Device Manager DeleteUsers",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetUser(struct soap* soap,
		struct _tds__SetUser *tds__SetUser,
		struct _tds__SetUserResponse *tds__SetUserResponse) {
	return getOnvifSoapActionNotSupportCode(soap, "Device Manager SetUser",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetWsdlUrl(struct soap* soap,
		struct _tds__GetWsdlUrl *tds__GetWsdlUrl,
		struct _tds__GetWsdlUrlResponse *tds__GetWsdlUrlResponse) {
	return getOnvifSoapActionNotSupportCode(soap, "Device Manager GetWsdlUrl",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 getCapabilitiesResponseExtensionDeviceIO(
		struct soap* soap,
		struct tt__CapabilitiesExtension *capabilitiesExtension, char* address) {
	capabilitiesExtension->DeviceIO =
			(struct tt__DeviceIOCapabilities*) my_soap_malloc(soap,
					sizeof(struct tt__DeviceIOCapabilities));
	capabilitiesExtension->DeviceIO->XAddr = (char *) my_soap_malloc(soap,
			sizeof(char) * INFO_LENGTH);
	strcpy(capabilitiesExtension->DeviceIO->XAddr, address);
	int vSource = 0;
	int ret = getVideoCount(&vSource);
	if (!isRetCodeSuccess(ret)) {
		return getOnvifSoapActionFailedCode(soap, "GetCapabilities",
				"getVideCount failed");
	}
	//下面的重要，这里只实现视频流，需要设置VideoSources
	capabilitiesExtension->DeviceIO->VideoSources = vSource;
	capabilitiesExtension->DeviceIO->VideoOutputs = 0;
	capabilitiesExtension->DeviceIO->AudioSources = 0;
	capabilitiesExtension->DeviceIO->AudioOutputs = 0;
	capabilitiesExtension->DeviceIO->RelayOutputs = 0;
	capabilitiesExtension->DeviceIO->__size = 0;
	capabilitiesExtension->DeviceIO->__any = NULL;
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 getCapabilitiesResponseExtension(struct soap* soap,
		struct _tds__GetCapabilitiesResponse *tds__GetCapabilitiesResponse,
		char* address) {
	tds__GetCapabilitiesResponse->Capabilities->Extension =
			(struct tt__CapabilitiesExtension*) my_soap_malloc(soap,
					sizeof(struct tt__CapabilitiesExtension));
	int result = getCapabilitiesResponseExtensionDeviceIO(soap,
			tds__GetCapabilitiesResponse->Capabilities->Extension, address);
	if (SOAP_OK != result) {
		return result;
	}
	tds__GetCapabilitiesResponse->Capabilities->Extension->Display = NULL;
	tds__GetCapabilitiesResponse->Capabilities->Extension->Recording = NULL;
	tds__GetCapabilitiesResponse->Capabilities->Extension->Search = NULL;
	tds__GetCapabilitiesResponse->Capabilities->Extension->Replay = NULL;
	tds__GetCapabilitiesResponse->Capabilities->Extension->Receiver = NULL;
	tds__GetCapabilitiesResponse->Capabilities->Extension->AnalyticsDevice =
			NULL;
	tds__GetCapabilitiesResponse->Capabilities->Extension->Extensions = NULL;
	tds__GetCapabilitiesResponse->Capabilities->Extension->__size = 0;
	tds__GetCapabilitiesResponse->Capabilities->Extension->__any = NULL;
	return SOAP_OK;
}
void getCapabilitiesResponseDeviceNetwork(struct soap* soap,
		struct tt__DeviceCapabilities *deviceCapabilities) {
	/*必要部分*/
	deviceCapabilities->Network =
			(struct tt__NetworkCapabilities*) my_soap_malloc(soap,
					sizeof(struct tt__NetworkCapabilities));
	deviceCapabilities->Network->IPFilter =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	deviceCapabilities->Network->ZeroConfiguration =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	deviceCapabilities->Network->IPVersion6 =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	deviceCapabilities->Network->DynDNS = (enum xsd__boolean *) my_soap_malloc(
			soap, sizeof(enum xsd__boolean));

	*deviceCapabilities->Network->IPFilter = xsd__boolean__false_;
	*deviceCapabilities->Network->ZeroConfiguration = xsd__boolean__false_;
	*deviceCapabilities->Network->IPVersion6 = xsd__boolean__false_;
	*deviceCapabilities->Network->DynDNS = xsd__boolean__false_;
	deviceCapabilities->Network->Extension =
			(struct tt__NetworkCapabilitiesExtension*) my_soap_malloc(soap,
					sizeof(struct tt__NetworkCapabilitiesExtension));
	deviceCapabilities->Network->Extension->Dot11Configuration =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*deviceCapabilities->Network->Extension->Dot11Configuration =
			xsd__boolean__false_;
	deviceCapabilities->Network->Extension->Extension = NULL;
	/*ghostyu  MUST BE*/
	deviceCapabilities->Network->Extension->__size = 0;
	deviceCapabilities->Network->Extension->__any = NULL;

}

void getCapabilitiesResponseDeviceSystem(struct soap* soap,
		struct tt__DeviceCapabilities *deviceCapabilities) {
#if 0
	deviceCapabilities->System = (struct tt__SystemCapabilities*)my_soap_malloc(soap, sizeof(struct tt__SystemCapabilities));
	deviceCapabilities->System->DiscoveryResolve =
	(enum xsd__boolean *) my_soap_malloc(soap, sizeof(enum xsd__boolean));
	*deviceCapabilities->System->DiscoveryResolve = xsd__boolean__false_;
	deviceCapabilities->System->DiscoveryBye =
	(enum xsd__boolean *) my_soap_malloc(soap, sizeof(enum xsd__boolean));
	*deviceCapabilities->System->DiscoveryBye = xsd__boolean__false_;
	deviceCapabilities->System->RemoteDiscovery =
	(enum xsd__boolean *) my_soap_malloc(soap, sizeof(enum xsd__boolean));
	*deviceCapabilities->System->RemoteDiscovery = xsd__boolean__false_;
	deviceCapabilities->System->SystemBackup =
	(enum xsd__boolean *) my_soap_malloc(soap, sizeof(enum xsd__boolean));
	*deviceCapabilities->System->SystemBackup = xsd__boolean__false_;
	deviceCapabilities->System->SystemLogging =
	(enum xsd__boolean *) my_soap_malloc(soap, sizeof(enum xsd__boolean));
	*deviceCapabilities->System->SystemLogging = xsd__boolean__false_;
	deviceCapabilities->System->FirmwareUpgrade =
	(enum xsd__boolean *) my_soap_malloc(soap, sizeof(enum xsd__boolean));
	*deviceCapabilities->System->FirmwareUpgrade = xsd__boolean__false_;
	deviceCapabilities->System->__sizeSupportedVersions =
	(enum xsd__boolean *) my_soap_malloc(soap, sizeof(enum xsd__boolean));
	*deviceCapabilities->System->__sizeSupportedVersions = xsd__boolean__true_;
	deviceCapabilities->System->SupportedVersions = (struct tt__OnvifVersion*)my_soap_malloc(soap, sizeof(struct tt__OnvifVersion));
	deviceCapabilities->System->SupportedVersions->Major = MAJOR;
	deviceCapabilities->System->SupportedVersions->Minor = MINOR;

	deviceCapabilities->System->Extension = (struct tt__SystemCapabilitiesExtension*)my_soap_malloc(soap, sizeof(struct tt__SystemCapabilitiesExtension));
	deviceCapabilities->System->Extension->HttpSystemBackup = (enum xsd__boolean *) my_soap_malloc(soap, sizeof(enum xsd__boolean));
	*deviceCapabilities->System->Extension->HttpSystemBackup = xsd__boolean__false_;
	deviceCapabilities->System->Extension->HttpFirmwareUpgrade = (enum xsd__boolean *) my_soap_malloc(soap, sizeof(enum xsd__boolean));
	*deviceCapabilities->System->Extension->HttpFirmwareUpgrade = xsd__boolean__true_;
	deviceCapabilities->System->Extension->HttpSystemLogging = (enum xsd__boolean *) my_soap_malloc(soap, sizeof(enum xsd__boolean));
	*deviceCapabilities->System->Extension->HttpSystemLogging = xsd__boolean__true_;
	deviceCapabilities->System->Extension->HttpSupportInformation = (enum xsd__boolean *) my_soap_malloc(soap, sizeof(enum xsd__boolean));
	*deviceCapabilities->System->Extension->HttpSupportInformation = xsd__boolean__true_;
	deviceCapabilities->System->Extension->Extension = NULL;
	deviceCapabilities->System->Extension->__size = 0;
	deviceCapabilities->System->Extension->__any = NULL;
#endif
}

void getCapabilitiesResponseDeviceIO(struct soap* soap,
		struct tt__DeviceCapabilities *deviceCapabilities) {
#if 0

	deviceCapabilities->IO = (struct tt__IOCapabilities*)my_soap_malloc(soap, sizeof(struct tt__IOCapabilities));
	deviceCapabilities->IO->InputConnectors = (enum xsd__boolean *) my_soap_malloc(soap, sizeof(enum xsd__boolean));
	*deviceCapabilities->IO->InputConnectors = xsd__boolean__false_;
	deviceCapabilities->IO->RelayOutputs = (enum xsd__boolean *) my_soap_malloc(soap, sizeof(enum xsd__boolean));
	*deviceCapabilities->IO->RelayOutputs = xsd__boolean__false_;

	deviceCapabilities->IO->Extension = (struct tt__IOCapabilitiesExtension *)my_soap_malloc(soap, sizeof(struct tt__IOCapabilitiesExtension));
	deviceCapabilities->IO->Extension->__size =0;
	deviceCapabilities->IO->Extension->__any = NULL;
	deviceCapabilities->IO->Extension->Auxiliary = (enum xsd__boolean *) my_soap_malloc(soap, sizeof(enum xsd__boolean));
	*deviceCapabilities->IO->Extension->Auxiliary = xsd__boolean__false_;
	deviceCapabilities->IO->Extension->__anyAttribute = NULL;
	deviceCapabilities->IO->Extension->__sizeAuxiliaryCommands = 0;
	deviceCapabilities->IO->Extension->AuxiliaryCommands= NULL;
	deviceCapabilities->IO->Extension->Extension = NULL;
#endif
}

void getCapabilitiesResponseDeviceSecurity(struct soap* soap,
		struct tt__DeviceCapabilities *deviceCapabilities) {
#if 0
	deviceCapabilities->Security = (struct tt__SecurityCapabilities*)my_soap_malloc(soap, sizeof(struct tt__SecurityCapabilities));
	deviceCapabilities->Security->TLS1_x002e1 = (enum xsd__boolean *) my_soap_malloc(soap, sizeof(enum xsd__boolean));
	*deviceCapabilities->Security->TLS1_x002e1 = xsd__boolean__false_;
	deviceCapabilities->Security->TLS1_x002e2 = (enum xsd__boolean *) my_soap_malloc(soap, sizeof(enum xsd__boolean));
	*deviceCapabilities->Security->TLS1_x002e2 = xsd__boolean__false_;
	deviceCapabilities->Security->OnboardKeyGeneration = (enum xsd__boolean *) my_soap_malloc(soap, sizeof(enum xsd__boolean));
	*deviceCapabilities->Security->OnboardKeyGeneration = xsd__boolean__false_;
	deviceCapabilities->Security->AccessPolicyConfig = (enum xsd__boolean *) my_soap_malloc(soap, sizeof(enum xsd__boolean));
	*deviceCapabilities->Security->AccessPolicyConfig = xsd__boolean__false_;
	deviceCapabilities->Security->X_x002e509Token = (enum xsd__boolean *) my_soap_malloc(soap, sizeof(enum xsd__boolean));
	*deviceCapabilities->Security->X_x002e509Token = xsd__boolean__false_;
	deviceCapabilities->Security->SAMLToken = (enum xsd__boolean *) my_soap_malloc(soap, sizeof(enum xsd__boolean));
	*deviceCapabilities->Security->SAMLToken = xsd__boolean__false_;
	deviceCapabilities->Security->KerberosToken = (enum xsd__boolean *) my_soap_malloc(soap, sizeof(enum xsd__boolean));
	*deviceCapabilities->Security->KerberosToken = xsd__boolean__false_;
	deviceCapabilities->Security->RELToken = (enum xsd__boolean *) my_soap_malloc(soap, sizeof(enum xsd__boolean));
	*deviceCapabilities->Security->RELToken = xsd__boolean__false_;
	deviceCapabilities->Security->Extension = NULL;
	deviceCapabilities->Security->__size = 0;
	deviceCapabilities->Security->__any = NULL;
	deviceCapabilities->Security->__anyAttribute = NULL;
#endif
}

void getCapabilitiesResponseDevice(struct soap* soap,
		struct tt__Capabilities *capabilities, char* address) {
	/*基础设置，很必须*/
	capabilities->Device = (struct tt__DeviceCapabilities*) my_soap_malloc(soap,
			sizeof(struct tt__DeviceCapabilities));
	capabilities->Device->XAddr = (char *) my_soap_malloc(soap,
			sizeof(char) * LARGE_INFO_LENGTH);
	strcpy(capabilities->Device->XAddr, address);
	capabilities->Device->Extension = NULL;

	getCapabilitiesResponseDeviceNetwork(soap, capabilities->Device);

	capabilities->Device->System = NULL;
	capabilities->Device->IO = NULL;
	capabilities->Device->Security = NULL;
	getCapabilitiesResponseDeviceSystem(soap, capabilities->Device);
	getCapabilitiesResponseDeviceIO(soap, capabilities->Device);
	getCapabilitiesResponseDeviceSecurity(soap, capabilities->Device);
}

void getCapabilitiesResponseMedia(struct soap* soap,
		struct tt__Capabilities *capabilities, char* address) {
	//想要对接RTSP视频，必须设置Media
	capabilities->Media = (struct tt__MediaCapabilities*) my_soap_malloc(soap,
			sizeof(struct tt__MediaCapabilities));
	capabilities->Media->XAddr = (char *) my_soap_malloc(soap,
			sizeof(char) * LARGE_INFO_LENGTH);
	getAppointServiceURL(capabilities->Media->XAddr, address, "media");
	capabilities->Media->StreamingCapabilities =
			(struct tt__RealTimeStreamingCapabilities*) my_soap_malloc(soap,
					sizeof(struct tt__RealTimeStreamingCapabilities));
	capabilities->Media->StreamingCapabilities->RTPMulticast =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*capabilities->Media->StreamingCapabilities->RTPMulticast =
			xsd__boolean__false_;
	capabilities->Media->StreamingCapabilities->RTP_USCORETCP =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*capabilities->Media->StreamingCapabilities->RTP_USCORETCP =
			xsd__boolean__true_;
	capabilities->Media->StreamingCapabilities->RTP_USCORERTSP_USCORETCP =
			(enum xsd__boolean *) my_soap_malloc(soap,
					sizeof(enum xsd__boolean));
	*capabilities->Media->StreamingCapabilities->RTP_USCORERTSP_USCORETCP =
			xsd__boolean__true_;
	capabilities->Media->StreamingCapabilities->Extension = NULL;
	capabilities->Media->Extension = NULL;
	capabilities->Media->__size = 0;
	capabilities->Media->__any = 0;
}

void getCapabilitiesResponsePTZ(struct soap* soap,
		struct tt__Capabilities *capabilities, char* address) {
	capabilities->PTZ = (struct tt__PTZCapabilities*) my_soap_malloc(soap,
			sizeof(struct tt__PTZCapabilities));
	capabilities->PTZ->XAddr = (char *) my_soap_malloc(soap,
			sizeof(char) * LARGE_INFO_LENGTH);
	getAppointServiceURL(capabilities->PTZ->XAddr, address, "ptz");
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCapabilities(struct soap* soap,
		struct _tds__GetCapabilities *tds__GetCapabilities,
		struct _tds__GetCapabilitiesResponse *tds__GetCapabilitiesResponse) {
	logInfo("__tds__GetCapabilities");
	char _IPv4Address[LARGE_INFO_LENGTH];
	if (RET_CODE_SUCCESS
			!= getServiceURL(_IPv4Address, onvifRunParam.ip,
					onvifRunParam.servicePort))
		return getOnvifSoapActionFailedCode(soap, "GetCapabilities",
				"not get service url");;
	if (tds__GetCapabilities->Category == NULL) {
		tds__GetCapabilities->Category =
				(enum tt__CapabilityCategory *) my_soap_malloc(soap,
						sizeof(enum tt__CapabilityCategory));
		*tds__GetCapabilities->Category = ALL;
	}

	tds__GetCapabilitiesResponse->Capabilities =
			(struct tt__Capabilities*) my_soap_malloc(soap,
					sizeof(struct tt__Capabilities));
	tds__GetCapabilitiesResponse->Capabilities->Analytics = NULL;
	tds__GetCapabilitiesResponse->Capabilities->Device = NULL;
	tds__GetCapabilitiesResponse->Capabilities->Events = NULL;
	tds__GetCapabilitiesResponse->Capabilities->Imaging = NULL;
	tds__GetCapabilitiesResponse->Capabilities->Media = NULL;
	tds__GetCapabilitiesResponse->Capabilities->PTZ = NULL;
	int result = getCapabilitiesResponseExtension(soap,
			tds__GetCapabilitiesResponse, _IPv4Address);
	if (SOAP_OK != result) {
		return result;
	}
	getCapabilitiesResponseDevice(soap,
			tds__GetCapabilitiesResponse->Capabilities, _IPv4Address);
	getCapabilitiesResponseMedia(soap,
			tds__GetCapabilitiesResponse->Capabilities, _IPv4Address);
	getCapabilitiesResponsePTZ(soap, tds__GetCapabilitiesResponse->Capabilities,
			_IPv4Address);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDPAddresses(struct soap* soap,
		struct _tds__SetDPAddresses *tds__SetDPAddresses,
		struct _tds__SetDPAddressesResponse *tds__SetDPAddressesResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager SetDPAddresses", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetHostname(struct soap* soap,
		struct _tds__GetHostname *tds__GetHostname,
		struct _tds__GetHostnameResponse *tds__GetHostnameResponse) {
	logInfo("__tds__GetHostname");
	tds__GetHostnameResponse->HostnameInformation =
			(struct tt__HostnameInformation*) my_soap_malloc(soap,
					sizeof(struct tt__HostnameInformation));
	tds__GetHostnameResponse->HostnameInformation->Name =
			(char*) my_soap_malloc(soap, INFO_LENGTH);
	tds__GetHostnameResponse->HostnameInformation->Extension =
			(struct tt__HostnameInformationExtension*) my_soap_malloc(soap,
					sizeof(struct tt__HostnameInformationExtension));

	tds__GetHostnameResponse->HostnameInformation->Extension->__size = 0;
	tds__GetHostnameResponse->HostnameInformation->Extension->__any = NULL;
	tds__GetHostnameResponse->HostnameInformation->FromDHCP =
			xsd__boolean__false_; //not from DHCP
	strcpy(tds__GetHostnameResponse->HostnameInformation->Name, HOST_NAME);
	return SOAP_OK;
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
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager SetHostnameFromDHCP", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDNS(struct soap* soap,
		struct _tds__GetDNS *tds__GetDNS,
		struct _tds__GetDNSResponse *tds__GetDNSResponse) {
	return getOnvifSoapActionNotSupportCode(soap, "Device Manager GetDNS", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDNS(struct soap* soap,
		struct _tds__SetDNS *tds__SetDNS,
		struct _tds__SetDNSResponse *tds__SetDNSResponse) {
	return getOnvifSoapActionNotSupportCode(soap, "Device Manager SetDNS", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNTP(struct soap* soap,
		struct _tds__GetNTP *tds__GetNTP,
		struct _tds__GetNTPResponse *tds__GetNTPResponse) {
	logInfo("__tds__GetNTP");
	OnvifNTPInfo info;
	if (RET_CODE_SUCCESS != getNTPInfo(&info)) {
		return getOnvifSoapActionNotSupportCode(soap, "Device Manager GetNTP",
				"get ntp info error");
	}
	struct tt__NTPInformation *pNTPInformation =
			(struct tt__NTPInformation*) my_soap_malloc(soap,
					sizeof(struct tt__NTPInformation));
	// No support DHCP
	pNTPInformation->FromDHCP = xsd__boolean__false_;
	pNTPInformation->__sizeNTPFromDHCP = 0;
	pNTPInformation->__sizeNTPManual = 1;
	// Set NTP Manual parameter
	struct tt__NetworkHost *pNTPManual =
			(struct tt__NetworkHost*) my_soap_malloc(soap,
					sizeof(struct tt__NetworkHost));
	pNTPManual->Type = tt__NetworkHostType__IPv4;
	char pAddr[16] = { 0 };
	strcpy(pAddr, info.address);
	pNTPManual->IPv4Address = pAddr;
	pNTPInformation->NTPManual = pNTPManual;
	tds__GetNTPResponse->NTPInformation = pNTPInformation;
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNTP(struct soap* soap,
		struct _tds__SetNTP *tds__SetNTP,
		struct _tds__SetNTPResponse *tds__SetNTPResponse) {
	logInfo("__tds__SetNTP");
	if (xsd__boolean__true_ == tds__SetNTP->FromDHCP) {
		return getOnvifSoapActionNotSupportCode(soap, "Device Manager SetNTP",
				"DHCPNotSupported");
	}
	if (tt__NetworkHostType__IPv4 == tds__SetNTP->NTPManual->Type) {
		char *pszIP = tds__SetNTP->NTPManual->IPv4Address;
		if (0 == ipOk(pszIP)) {
			OnvifNTPInfo ntpInfo;
			strcpy(ntpInfo.address, pszIP);
			int ret = setNTPInfo(&ntpInfo);
			if (RET_CODE_SUCCESS != ret) {
				return getOnvifSoapActionFailedCode(soap, "SetNTP",
						"set ntp failed");
			}
		} else {
			return getOnvifSoapActionFailedCode(soap, "SetNTP",
					"invalidIPv4Address IPv4");
		}
	} else if (tt__NetworkHostType__IPv6 == tds__SetNTP->NTPManual->Type) {
		return getOnvifSoapActionNotSupportCode(soap, "Device Manager SetNTP",
				"No Support IPv6");
	} else if (tt__NetworkHostType__DNS == tds__SetNTP->NTPManual->Type) {
		return getOnvifSoapActionNotSupportCode(soap, "Device Manager SetNTP",
				"No Support DNS");
	}
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDynamicDNS(struct soap* soap,
		struct _tds__GetDynamicDNS *tds__GetDynamicDNS,
		struct _tds__GetDynamicDNSResponse *tds__GetDynamicDNSResponse) {
	logInfo("__tds__GetDynamicDNS");
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager GetDynamicDNS", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDynamicDNS(struct soap* soap,
		struct _tds__SetDynamicDNS *tds__SetDynamicDNS,
		struct _tds__SetDynamicDNSResponse *tds__SetDynamicDNSResponse) {
	logInfo("__tds__SetDynamicDNS");
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager SetDynamicDNS", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkInterfaces(
		struct soap* soap,
		struct _tds__GetNetworkInterfaces *tds__GetNetworkInterfaces,
		struct _tds__GetNetworkInterfacesResponse *tds__GetNetworkInterfacesResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager GetNetworkInterfaces", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkInterfaces(
		struct soap* soap,
		struct _tds__SetNetworkInterfaces *tds__SetNetworkInterfaces,
		struct _tds__SetNetworkInterfacesResponse *tds__SetNetworkInterfacesResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager SetNetworkInterfaces", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkProtocols(
		struct soap* soap,
		struct _tds__GetNetworkProtocols *tds__GetNetworkProtocols,
		struct _tds__GetNetworkProtocolsResponse *tds__GetNetworkProtocolsResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager GetNetworkProtocols", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkProtocols(
		struct soap* soap,
		struct _tds__SetNetworkProtocols *tds__SetNetworkProtocols,
		struct _tds__SetNetworkProtocolsResponse *tds__SetNetworkProtocolsResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager SetNetworkProtocols", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetNetworkDefaultGateway(
		struct soap* soap,
		struct _tds__GetNetworkDefaultGateway *tds__GetNetworkDefaultGateway,
		struct _tds__GetNetworkDefaultGatewayResponse *tds__GetNetworkDefaultGatewayResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager GetNetworkDefaultGateway", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetNetworkDefaultGateway(
		struct soap* soap,
		struct _tds__SetNetworkDefaultGateway *tds__SetNetworkDefaultGateway,
		struct _tds__SetNetworkDefaultGatewayResponse *tds__SetNetworkDefaultGatewayResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager SetNetworkDefaultGateway", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetZeroConfiguration(
		struct soap* soap,
		struct _tds__GetZeroConfiguration *tds__GetZeroConfiguration,
		struct _tds__GetZeroConfigurationResponse *tds__GetZeroConfigurationResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager GetZeroConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetZeroConfiguration(
		struct soap* soap,
		struct _tds__SetZeroConfiguration *tds__SetZeroConfiguration,
		struct _tds__SetZeroConfigurationResponse *tds__SetZeroConfigurationResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager SetZeroConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetIPAddressFilter(
		struct soap* soap,
		struct _tds__GetIPAddressFilter *tds__GetIPAddressFilter,
		struct _tds__GetIPAddressFilterResponse *tds__GetIPAddressFilterResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager GetIPAddressFilter", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetIPAddressFilter(
		struct soap* soap,
		struct _tds__SetIPAddressFilter *tds__SetIPAddressFilter,
		struct _tds__SetIPAddressFilterResponse *tds__SetIPAddressFilterResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager SetIPAddressFilter", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__AddIPAddressFilter(
		struct soap* soap,
		struct _tds__AddIPAddressFilter *tds__AddIPAddressFilter,
		struct _tds__AddIPAddressFilterResponse *tds__AddIPAddressFilterResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager AddIPAddressFilter", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__RemoveIPAddressFilter(
		struct soap* soap,
		struct _tds__RemoveIPAddressFilter *tds__RemoveIPAddressFilter,
		struct _tds__RemoveIPAddressFilterResponse *tds__RemoveIPAddressFilterResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager RemoveIPAddressFilter", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetAccessPolicy(struct soap* soap,
		struct _tds__GetAccessPolicy *tds__GetAccessPolicy,
		struct _tds__GetAccessPolicyResponse *tds__GetAccessPolicyResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager GetAccessPolicy", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetAccessPolicy(struct soap* soap,
		struct _tds__SetAccessPolicy *tds__SetAccessPolicy,
		struct _tds__SetAccessPolicyResponse *tds__SetAccessPolicyResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager SetAccessPolicy", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateCertificate(struct soap* soap,
		struct _tds__CreateCertificate *tds__CreateCertificate,
		struct _tds__CreateCertificateResponse *tds__CreateCertificateResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager CreateCertificate", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificates(struct soap* soap,
		struct _tds__GetCertificates *tds__GetCertificates,
		struct _tds__GetCertificatesResponse *tds__GetCertificatesResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager GetCertificates", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificatesStatus(
		struct soap* soap,
		struct _tds__GetCertificatesStatus *tds__GetCertificatesStatus,
		struct _tds__GetCertificatesStatusResponse *tds__GetCertificatesStatusResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager GetCertificatesStatus", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetCertificatesStatus(
		struct soap* soap,
		struct _tds__SetCertificatesStatus *tds__SetCertificatesStatus,
		struct _tds__SetCertificatesStatusResponse *tds__SetCertificatesStatusResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager SetCertificatesStatus", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteCertificates(
		struct soap* soap,
		struct _tds__DeleteCertificates *tds__DeleteCertificates,
		struct _tds__DeleteCertificatesResponse *tds__DeleteCertificatesResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager DeleteCertificates", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetPkcs10Request(struct soap* soap,
		struct _tds__GetPkcs10Request *tds__GetPkcs10Request,
		struct _tds__GetPkcs10RequestResponse *tds__GetPkcs10RequestResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager GetPkcs10Request", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCertificates(struct soap* soap,
		struct _tds__LoadCertificates *tds__LoadCertificates,
		struct _tds__LoadCertificatesResponse *tds__LoadCertificatesResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager LoadCertificates", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetClientCertificateMode(
		struct soap* soap,
		struct _tds__GetClientCertificateMode *tds__GetClientCertificateMode,
		struct _tds__GetClientCertificateModeResponse *tds__GetClientCertificateModeResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager GetClientCertificateMode", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetClientCertificateMode(
		struct soap* soap,
		struct _tds__SetClientCertificateMode *tds__SetClientCertificateMode,
		struct _tds__SetClientCertificateModeResponse *tds__SetClientCertificateModeResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager SetClientCertificateMode", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetRelayOutputs(struct soap* soap,
		struct _tds__GetRelayOutputs *tds__GetRelayOutputs,
		struct _tds__GetRelayOutputsResponse *tds__GetRelayOutputsResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager GetRelayOutputs", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRelayOutputSettings(
		struct soap* soap,
		struct _tds__SetRelayOutputSettings *tds__SetRelayOutputSettings,
		struct _tds__SetRelayOutputSettingsResponse *tds__SetRelayOutputSettingsResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager SetRelayOutputSettings", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetRelayOutputState(
		struct soap* soap,
		struct _tds__SetRelayOutputState *tds__SetRelayOutputState,
		struct _tds__SetRelayOutputStateResponse *tds__SetRelayOutputStateResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager SetRelayOutputState", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SendAuxiliaryCommand(
		struct soap* soap,
		struct _tds__SendAuxiliaryCommand *tds__SendAuxiliaryCommand,
		struct _tds__SendAuxiliaryCommandResponse *tds__SendAuxiliaryCommandResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager SendAuxiliaryCommand", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCACertificates(struct soap* soap,
		struct _tds__GetCACertificates *tds__GetCACertificates,
		struct _tds__GetCACertificatesResponse *tds__GetCACertificatesResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager GetCACertificates", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCertificateWithPrivateKey(
		struct soap* soap,
		struct _tds__LoadCertificateWithPrivateKey *tds__LoadCertificateWithPrivateKey,
		struct _tds__LoadCertificateWithPrivateKeyResponse *tds__LoadCertificateWithPrivateKeyResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager LoadCertificateWithPrivateKey", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetCertificateInformation(
		struct soap* soap,
		struct _tds__GetCertificateInformation *tds__GetCertificateInformation,
		struct _tds__GetCertificateInformationResponse *tds__GetCertificateInformationResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager GetCertificateInformation", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__LoadCACertificates(
		struct soap* soap,
		struct _tds__LoadCACertificates *tds__LoadCACertificates,
		struct _tds__LoadCACertificatesResponse *tds__LoadCACertificatesResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager LoadCACertificates", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__CreateDot1XConfiguration(
		struct soap* soap,
		struct _tds__CreateDot1XConfiguration *tds__CreateDot1XConfiguration,
		struct _tds__CreateDot1XConfigurationResponse *tds__CreateDot1XConfigurationResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager CreateDot1XConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__SetDot1XConfiguration(
		struct soap* soap,
		struct _tds__SetDot1XConfiguration *tds__SetDot1XConfiguration,
		struct _tds__SetDot1XConfigurationResponse *tds__SetDot1XConfigurationResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager SetDot1XConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot1XConfiguration(
		struct soap* soap,
		struct _tds__GetDot1XConfiguration *tds__GetDot1XConfiguration,
		struct _tds__GetDot1XConfigurationResponse *tds__GetDot1XConfigurationResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager GetDot1XConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot1XConfigurations(
		struct soap* soap,
		struct _tds__GetDot1XConfigurations *tds__GetDot1XConfigurations,
		struct _tds__GetDot1XConfigurationsResponse *tds__GetDot1XConfigurationsResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager GetDot1XConfigurations", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__DeleteDot1XConfiguration(
		struct soap* soap,
		struct _tds__DeleteDot1XConfiguration *tds__DeleteDot1XConfiguration,
		struct _tds__DeleteDot1XConfigurationResponse *tds__DeleteDot1XConfigurationResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager DeleteDot1XConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot11Capabilities(
		struct soap* soap,
		struct _tds__GetDot11Capabilities *tds__GetDot11Capabilities,
		struct _tds__GetDot11CapabilitiesResponse *tds__GetDot11CapabilitiesResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager GetDot11Capabilities", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetDot11Status(struct soap* soap,
		struct _tds__GetDot11Status *tds__GetDot11Status,
		struct _tds__GetDot11StatusResponse *tds__GetDot11StatusResponse) {
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager GetDot11Status", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__ScanAvailableDot11Networks(
		struct soap* soap,
		struct _tds__ScanAvailableDot11Networks *tds__ScanAvailableDot11Networks,
		struct _tds__ScanAvailableDot11NetworksResponse *tds__ScanAvailableDot11NetworksResponse) {
	logInfo("__tds__ScanAvailableDot11Networks");
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager ScanAvailableDot11Networks", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__GetSystemUris(struct soap* soap,
		struct _tds__GetSystemUris *tds__GetSystemUris,
		struct _tds__GetSystemUrisResponse *tds__GetSystemUrisResponse) {
	logInfo("__tds__GetSystemUris");
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager GetSystemUris", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__StartFirmwareUpgrade(
		struct soap* soap,
		struct _tds__StartFirmwareUpgrade *tds__StartFirmwareUpgrade,
		struct _tds__StartFirmwareUpgradeResponse *tds__StartFirmwareUpgradeResponse) {
	logInfo("__tds__StartFirmwareUpgrade");
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager StartFirmwareUpgrade", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tds__StartSystemRestore(
		struct soap* soap,
		struct _tds__StartSystemRestore *tds__StartSystemRestore,
		struct _tds__StartSystemRestoreResponse *tds__StartSystemRestoreResponse) {
	logInfo("__tds__StartSystemRestore");
	return getOnvifSoapActionNotSupportCode(soap,
			"Device Manager StartSystemRestore", NULL);
}

