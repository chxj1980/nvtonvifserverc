#include "soapH.h"
#include "soapStub.h"
#include "onvifHandle.h"
#include "appTools.h"

#define MAX_PROF_TOKEN 20

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetServiceCapabilities(
		struct soap* soap,
		struct _trt__GetServiceCapabilities *trt__GetServiceCapabilities,
		struct _trt__GetServiceCapabilitiesResponse *trt__GetServiceCapabilitiesResponse) {
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
	DBG("__tmd__GetVideoSources\n");

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
	if (trt__GetProfile) {
		DBG("trt__GetProfile=%s\n", trt__GetProfile->ProfileToken);
	}
	/*这里的ProfileToken是选定的，得到特定的profile描述*/
	//但odm单击一个profile时，需要获取，不然不会出现live video和video streaming
	trt__GetProfileResponse->Profile = (struct tt__Profile *) soap_malloc(soap,
			sizeof(struct tt__Profile));
	trt__GetProfileResponse->Profile->Name = (char *) soap_malloc(soap,
			sizeof(char) * 20);
	trt__GetProfileResponse->Profile->token = (char *) soap_malloc(soap,
			sizeof(char) * 20);
	strcpy(trt__GetProfileResponse->Profile->Name, "my_profile");
	strcpy(trt__GetProfileResponse->Profile->token, "token_profile");
	trt__GetProfileResponse->Profile->fixed = &soap_False;
	trt__GetProfileResponse->Profile->__anyAttribute = NULL;

	trt__GetProfileResponse->Profile->VideoSourceConfiguration = NULL;
	trt__GetProfileResponse->Profile->AudioSourceConfiguration = NULL;

	/*VideoEncoderConfiguration*/
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration =
			(struct tt__VideoEncoderConfiguration *) soap_malloc(soap,
					sizeof(struct tt__VideoEncoderConfiguration));
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Name =
			(char *) soap_malloc(soap, sizeof(char) * MAX_PROF_TOKEN);
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->token =
			(char *) soap_malloc(soap, sizeof(char) * MAX_PROF_TOKEN);
	strcpy(trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Name,
			"VE_Name");
	strcpy(trt__GetProfileResponse->Profile->VideoEncoderConfiguration->token,
			"VE_token");
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->UseCount = 1;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Quality = 10;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Encoding = 2; //JPEG = 0, MPEG4 = 1, H264 = 2;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Resolution =
			(struct tt__VideoResolution *) soap_malloc(soap,
					sizeof(struct tt__VideoResolution));
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Resolution->Height =
			720;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Resolution->Width =
			1280;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->RateControl =
			(struct tt__VideoRateControl *) soap_malloc(soap,
					sizeof(struct tt__VideoRateControl));
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->RateControl->FrameRateLimit =
			30;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->RateControl->EncodingInterval =
			1;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->RateControl->BitrateLimit =
			500;

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

	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Multicast =
			(struct tt__MulticastConfiguration *) soap_malloc(soap,
					sizeof(struct tt__MulticastConfiguration));
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Multicast->Address =
			(struct tt__IPAddress *) soap_malloc(soap,
					sizeof(struct tt__IPAddress));
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Multicast->Address->IPv4Address =
			(char **) soap_malloc(soap, sizeof(char *));
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Multicast->Address->IPv4Address[0] =
			(char *) soap_malloc(soap, sizeof(char) * INFO_LENGTH);
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Multicast->Address->IPv6Address =
			NULL;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Multicast->Address->Type =
			0;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Multicast->Port =
			554;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Multicast->TTL =
			60;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Multicast->AutoStart =
			1;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Multicast->__size =
			0;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Multicast->__any =
			NULL;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->Multicast->__anyAttribute =
			NULL;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->SessionTimeout =
			720000;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->__size = 0;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->__any = NULL;
	trt__GetProfileResponse->Profile->VideoEncoderConfiguration->__anyAttribute =
			NULL;

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
	int i;
	char _IPAddr[LARGE_INFO_LENGTH];
	if (RET_CODE_SUCCESS != getLocalIp(_IPAddr))
		return GSOAP_RET_CODE_NOT_IMPLEMENT;

	int size = 1;
	trt__GetProfilesResponse->Profiles = (struct tt__Profile *) soap_malloc(
			soap, sizeof(struct tt__Profile) * size);
	trt__GetProfilesResponse->__sizeProfiles = size;

	i = 0;
	trt__GetProfilesResponse->Profiles[i].Name = (char *) soap_malloc(soap,
			sizeof(char) * MAX_PROF_TOKEN);
	strcpy(trt__GetProfilesResponse->Profiles[i].Name, "my_profile");
	trt__GetProfilesResponse->Profiles[i].token = (char *) soap_malloc(soap,
			sizeof(char) * MAX_PROF_TOKEN);
	strcpy(trt__GetProfilesResponse->Profiles[i].token, "token_profile");
	trt__GetProfilesResponse->Profiles[i].fixed = soap_False;
	trt__GetProfilesResponse->Profiles[i].__anyAttribute = NULL;
	trt__GetProfilesResponse->Profiles[i].VideoAnalyticsConfiguration = NULL;

	/*必须包含VideoEncoderConfiguration的配置
	 不然不会出现live video 和 video streaming*/
	/*VideoEncoderConfiguration*/
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration = NULL;
#if 1
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration =
			(struct tt__VideoEncoderConfiguration *) soap_malloc(soap,
					sizeof(struct tt__VideoEncoderConfiguration));
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Name =
			(char *) soap_malloc(soap, sizeof(char) * MAX_PROF_TOKEN);
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->token =
			(char *) soap_malloc(soap, sizeof(char) * MAX_PROF_TOKEN);
	strcpy(
			trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Name,
			"VE_Name1");
	strcpy(
			trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->token,
			"VE_token1");
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->UseCount =
			1;
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Quality =
			10;
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Encoding =
			2; //JPEG = 0, MPEG4 = 1, H264 = 2;
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Resolution =
			(struct tt__VideoResolution *) soap_malloc(soap,
					sizeof(struct tt__VideoResolution));
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Resolution->Height =
			720;
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Resolution->Width =
			1280;
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->RateControl =
			(struct tt__VideoRateControl *) soap_malloc(soap,
					sizeof(struct tt__VideoRateControl));
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->RateControl->FrameRateLimit =
			30;
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->RateControl->EncodingInterval =
			1;
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->RateControl->BitrateLimit =
			500;

	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->MPEG4 =
			NULL;
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->H264 = NULL;
#if 1
	/*可选项，可以不配置*/
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->MPEG4 =
			(struct tt__Mpeg4Configuration *) soap_malloc(soap,
					sizeof(struct tt__Mpeg4Configuration));
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->MPEG4->GovLength =
			30;
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->MPEG4->Mpeg4Profile =
			1;
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->H264 =
			(struct tt__H264Configuration *) soap_malloc(soap,
					sizeof(struct tt__H264Configuration));
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->H264->GovLength =
			30;
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->H264->H264Profile =
			1;
#endif
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast =
			(struct tt__MulticastConfiguration *) soap_malloc(soap,
					sizeof(struct tt__MulticastConfiguration));
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->Address =
			(struct tt__IPAddress *) soap_malloc(soap,
					sizeof(struct tt__IPAddress));
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->Address->IPv4Address =
			(char **) soap_malloc(soap, sizeof(char *));
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->Address->IPv4Address[0] =
			(char *) soap_malloc(soap, sizeof(char) * INFO_LENGTH);
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->Address->IPv6Address =
			NULL;
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->Address->Type =
			0;
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->Port =
			554;
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->TTL =
			60;
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->AutoStart =
			1;
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->__size =
			0;
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->__any =
			NULL;
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->Multicast->__anyAttribute =
			NULL;
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->SessionTimeout =
			720000;
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->__size = 0;
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->__any =
			NULL;
	trt__GetProfilesResponse->Profiles[i].VideoEncoderConfiguration->__anyAttribute =
			NULL;
#endif

	/* VideoSourceConfiguration */
	//trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration = NULL;
	trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration =
			(struct tt__VideoSourceConfiguration *) soap_malloc(soap,
					sizeof(struct tt__VideoSourceConfiguration));
	trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Name =
			(char *) soap_malloc(soap, sizeof(char) * MAX_PROF_TOKEN);
	trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->token =
			(char *) soap_malloc(soap, sizeof(char) * MAX_PROF_TOKEN);
	trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->SourceToken =
			(char *) soap_malloc(soap, sizeof(char) * MAX_PROF_TOKEN);
	trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Bounds =
			(struct tt__IntRectangle *) soap_malloc(soap,
					sizeof(struct tt__IntRectangle));
	trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Extension =
			NULL;
	trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->__any = NULL;
	trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->__anyAttribute =
			NULL;
	trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->__size = 0;
	/*注意SourceToken*/
	strcpy(trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Name,
			"VS_Name");
	strcpy(
			trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->token,
			"VS_Token");
	strcpy(
			trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->SourceToken,
			"GhostyuSource_token"); /*必须与__tmd__GetVideoSources中的token相同*/
	trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->UseCount =
			1;
	trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Bounds->x =
			1;
	trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Bounds->y =
			1;
	trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Bounds->height =
			720;
	trt__GetProfilesResponse->Profiles[i].VideoSourceConfiguration->Bounds->width =
			1280;

	trt__GetProfilesResponse->Profiles[i].AudioEncoderConfiguration = NULL;
	trt__GetProfilesResponse->Profiles[i].AudioSourceConfiguration = NULL;
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
	DBG("__trt__GetVideoSourceConfigurations\n");
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfigurations(
		struct soap* soap,
		struct _trt__GetVideoEncoderConfigurations *trt__GetVideoEncoderConfigurations,
		struct _trt__GetVideoEncoderConfigurationsResponse *trt__GetVideoEncoderConfigurationsResponse) {
	DBG("__trt__GetVideoEncoderConfigurations\n");
	//return SOAP_OK;
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
	DBG("__tmd__GetVideoSourceConfiguration\n");
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
	DBG("__trt__GetVideoEncoderConfigurationOptions\n");
	//该函数必要，video streaming需要
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
	DBG("__trt__GetStreamUri\n");
	char _IPAddr[INFO_LENGTH];
	char _IPAddr1[INFO_LENGTH] = { 0 };
	if (RET_CODE_SUCCESS != getLocalIp(_IPAddr1))
		return GSOAP_RET_CODE_NOT_IMPLEMENT;
	sprintf(_IPAddr, "rtsp://%s:8554/day.264", _IPAddr1);
	/* Response */
	trt__GetStreamUriResponse->MediaUri = (struct tt__MediaUri *) soap_malloc(
			soap, sizeof(struct tt__MediaUri));
	trt__GetStreamUriResponse->MediaUri->Uri = (char *) soap_malloc(soap,
			sizeof(char) * LARGE_INFO_LENGTH);
	strcpy(trt__GetStreamUriResponse->MediaUri->Uri, _IPAddr);
	trt__GetStreamUriResponse->MediaUri->InvalidAfterConnect = 0;
	trt__GetStreamUriResponse->MediaUri->InvalidAfterReboot = 0;
	trt__GetStreamUriResponse->MediaUri->Timeout = 200;
	return SOAP_OK;
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
	DBG("__trt__GetSnapshotUri\n");
	trt__GetSnapshotUriResponse->MediaUri = (struct tt__MediaUri *) soap_malloc(
			soap, sizeof(struct tt__MediaUri));
	trt__GetSnapshotUriResponse->MediaUri->Uri = (char *) soap_malloc(soap,
			sizeof(char) * 200);
	strcpy(trt__GetSnapshotUriResponse->MediaUri->Uri,
			"http://avatar.csdn.net/7/E/1/1_ghostyu.jpg");
	trt__GetSnapshotUriResponse->MediaUri->InvalidAfterConnect = 0;
	trt__GetSnapshotUriResponse->MediaUri->InvalidAfterReboot = 0;
	trt__GetSnapshotUriResponse->MediaUri->Timeout = 2; //seconds
	trt__GetSnapshotUriResponse->MediaUri->__size = 0;
	trt__GetSnapshotUriResponse->MediaUri->__any = NULL;
	trt__GetSnapshotUriResponse->MediaUri->__anyAttribute = NULL;
	return SOAP_OK;
}

