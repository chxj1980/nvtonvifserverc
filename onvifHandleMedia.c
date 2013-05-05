#include "soapH.h"
#include "soapStub.h"
#include "onvifHandle.h"
#include "appTools.h"
#include "logInfo.h"

#define DEFAULT_SESSION_TIME_OUT 720000
#define TOKEN_NAME_PREFIX "token_profile"

void getVideoSourceToken(char* dest, int index) {
	sprintf(dest, "%s%d", VIDEO_SOURCE_TOKEN, index);
}

void getVEToken(char* dest, int index) {
	sprintf(dest, "VE_token%d", index);
}

void getVEName(char* dest, int index) {
	sprintf(dest, "VE_Name%d", index);
}

enum tt__VideoEncoding getVideoEncodeType(int encType) {
	enum tt__VideoEncoding result = tt__VideoEncoding__H264;
	switch (encType) {
	case 2:
		result = tt__VideoEncoding__JPEG; //JPEG = 0, MPEG4 = 1, H264 = 2;
		break;
	case 1:
		result = tt__VideoEncoding__MPEG4; //JPEG = 0, MPEG4 = 1, H264 = 2;
		break;
	}
	return result;
}

void getResponseProfileName(char* dest, int index) {
	sprintf(dest, "my_profile%d", index);
}

int getIndexFromProfileToken(char* token) {
	if (NULL == token)
		return RET_CODE_ERROR_NULL_OBJECT;
	if (strlen(token) <= strlen(TOKEN_NAME_PREFIX))
		return RET_CODE_ERROR_INVALID_VALUE;
	if (NULL == strstr(token, TOKEN_NAME_PREFIX))
		return RET_CODE_ERROR_INVALID_VALUE;
	return atoi((char*) (token + strlen(TOKEN_NAME_PREFIX)));
}

void getResponseProfileToken(char* dest, int index) {
	sprintf(dest, "%s%d", TOKEN_NAME_PREFIX, index);
}

enum tt__BacklightCompensationMode getOnvifBacklightCompensationMode(int mode) {
	return (0 == mode) ?
			tt__BacklightCompensationMode__OFF :
			tt__BacklightCompensationMode__ON;
}

enum tt__WhiteBalanceMode getOnvifWhiteBalanceMode(int mode) {
	return (0 == mode) ?
			tt__WhiteBalanceMode__AUTO : tt__WhiteBalanceMode__MANUAL;
}

enum tt__WideDynamicMode getOnvifWideDynamicMode(int mode) {
	return (0 == mode) ? tt__WideDynamicMode__OFF : tt__WideDynamicMode__ON;
}

void getVideoSourcesResponseVideoSource(struct soap* soap,
		struct tt__VideoSource* videoSource,
		OnvifVideoChannelInfo* onvifVideoChannelInfo, int index) {
	videoSource->Framerate = onvifVideoChannelInfo->frame_rate;
	videoSource->Resolution = (struct tt__VideoResolution *) soap_malloc(soap,
			sizeof(struct tt__VideoResolution));
	videoSource->Resolution->Height = onvifVideoChannelInfo->height;
	videoSource->Resolution->Width = onvifVideoChannelInfo->width;
	videoSource->token = (char *) soap_malloc(soap, sizeof(char) * INFO_LENGTH);
	memset(videoSource->token, 0, sizeof(char) * INFO_LENGTH);
	getVideoSourceToken(videoSource->token, index); //注意这里需要和GetProfile中的sourcetoken相同

	videoSource->Imaging = (struct tt__ImagingSettings*) soap_malloc(soap,
			sizeof(struct tt__ImagingSettings));
	videoSource->Imaging->Brightness = (float*) soap_malloc(soap,
			sizeof(float));
	videoSource->Imaging->Brightness[0] = onvifVideoChannelInfo->bright;
	videoSource->Imaging->ColorSaturation = (float*) soap_malloc(soap,
			sizeof(float));
	videoSource->Imaging->ColorSaturation[0] =
			onvifVideoChannelInfo->saturation;
	videoSource->Imaging->Contrast = (float*) soap_malloc(soap, sizeof(float));
	videoSource->Imaging->Contrast[0] = onvifVideoChannelInfo->contrast;
	videoSource->Imaging->IrCutFilter =
			(enum tt__IrCutFilterMode *) soap_malloc(soap,
					sizeof(enum tt__IrCutFilterMode));
	*videoSource->Imaging->IrCutFilter = 0;
	videoSource->Imaging->Sharpness = (float*) soap_malloc(soap, sizeof(float));
	videoSource->Imaging->Sharpness[0] = onvifVideoChannelInfo->sharpness;
	videoSource->Imaging->BacklightCompensation =
			(struct tt__BacklightCompensation*) soap_malloc(soap,
					sizeof(struct tt__BacklightCompensation));
	videoSource->Imaging->BacklightCompensation->Mode =
			getOnvifBacklightCompensationMode(
					onvifVideoChannelInfo->backLightCompMode);
	videoSource->Imaging->BacklightCompensation->Level =
			onvifVideoChannelInfo->backLightCompLevel;
	videoSource->Imaging->Exposure = NULL;
	videoSource->Imaging->Focus = NULL;
	videoSource->Imaging->WideDynamicRange =
			(struct tt__WideDynamicRange*) soap_malloc(soap,
					sizeof(struct tt__WideDynamicRange));
	videoSource->Imaging->WideDynamicRange->Mode = getOnvifWideDynamicMode(
			onvifVideoChannelInfo->wideDynamicMode);
	videoSource->Imaging->WideDynamicRange->Level =
			onvifVideoChannelInfo->wideDynamicLevel;
	videoSource->Imaging->WhiteBalance = (struct tt__WhiteBalance*) soap_malloc(
			soap, sizeof(struct tt__WhiteBalance));
	videoSource->Imaging->WhiteBalance->Mode = getOnvifWhiteBalanceMode(
			onvifVideoChannelInfo->wbMode);
	videoSource->Imaging->WhiteBalance->CrGain =
			onvifVideoChannelInfo->wbCrGain;
	videoSource->Imaging->WhiteBalance->CbGain =
			onvifVideoChannelInfo->wbCbGain;
	videoSource->Imaging->Extension = NULL;
	videoSource->Extension = NULL;
}

void getResponseProfileInfoVideoEncoderConfiguration(struct soap* soap,
		struct tt__VideoEncoderConfiguration * videoEncoderConfiguration,
		OnvifVideoChannelInfo* onvifVideoChannelInfo, int index) {
	memset(videoEncoderConfiguration, 0,
			sizeof(struct tt__VideoEncoderConfiguration));
	videoEncoderConfiguration->Name = (char *) soap_malloc(soap,
			sizeof(char) * SMALL_INFO_LENGTH);
	memset(videoEncoderConfiguration->Name, 0,
			sizeof(char) * SMALL_INFO_LENGTH);
	videoEncoderConfiguration->token = (char *) soap_malloc(soap,
			sizeof(char) * SMALL_INFO_LENGTH);
	memset(videoEncoderConfiguration->token, 0,
			sizeof(char) * SMALL_INFO_LENGTH);
	getVEName(videoEncoderConfiguration->Name, index);
	getVEToken(videoEncoderConfiguration->token, index);
	videoEncoderConfiguration->UseCount = 1;
	videoEncoderConfiguration->Quality = onvifVideoChannelInfo->quality;
	videoEncoderConfiguration->Encoding = getVideoEncodeType(
			onvifVideoChannelInfo->enc_type);
	videoEncoderConfiguration->Resolution =
			(struct tt__VideoResolution *) soap_malloc(soap,
					sizeof(struct tt__VideoResolution));
	memset(videoEncoderConfiguration->Resolution, 0,
			sizeof(struct tt__VideoResolution));
	videoEncoderConfiguration->Resolution->Height =
			onvifVideoChannelInfo->height;
	videoEncoderConfiguration->Resolution->Width = onvifVideoChannelInfo->width;
	videoEncoderConfiguration->RateControl =
			(struct tt__VideoRateControl *) soap_malloc(soap,
					sizeof(struct tt__VideoRateControl));
	memset(videoEncoderConfiguration->RateControl, 0,
			sizeof(struct tt__VideoRateControl));
	videoEncoderConfiguration->RateControl->FrameRateLimit =
			onvifVideoChannelInfo->frame_rate;
	videoEncoderConfiguration->RateControl->EncodingInterval = 1;
	videoEncoderConfiguration->RateControl->BitrateLimit =
			onvifVideoChannelInfo->bit_rate;
	videoEncoderConfiguration->MPEG4 = NULL;
	videoEncoderConfiguration->H264 = NULL;
#if 1
	/*可选项，可以不配置*/
	if (tt__VideoEncoding__MPEG4 == videoEncoderConfiguration->Encoding) {
		videoEncoderConfiguration->MPEG4 =
				(struct tt__Mpeg4Configuration *) soap_malloc(soap,
						sizeof(struct tt__Mpeg4Configuration));
		memset(videoEncoderConfiguration->MPEG4, 0,
				sizeof(struct tt__Mpeg4Configuration));
		videoEncoderConfiguration->MPEG4->GovLength =
				onvifVideoChannelInfo->govLength;
		videoEncoderConfiguration->MPEG4->Mpeg4Profile =
				onvifVideoChannelInfo->videoEncodeProfile;
	} else if (tt__VideoEncoding__H264 == videoEncoderConfiguration->Encoding) {
		videoEncoderConfiguration->H264 =
				(struct tt__H264Configuration *) soap_malloc(soap,
						sizeof(struct tt__H264Configuration));
		memset(videoEncoderConfiguration->H264, 0,
				sizeof(struct tt__H264Configuration));
		videoEncoderConfiguration->H264->GovLength =
				onvifVideoChannelInfo->govLength;
		videoEncoderConfiguration->H264->H264Profile =
				onvifVideoChannelInfo->videoEncodeProfile;
	}

	videoEncoderConfiguration->Multicast =
			(struct tt__MulticastConfiguration *) soap_malloc(soap,
					sizeof(struct tt__MulticastConfiguration));
	memset(videoEncoderConfiguration->Multicast, 0,
			sizeof(struct tt__MulticastConfiguration));
	videoEncoderConfiguration->Multicast->Address =
			(struct tt__IPAddress *) soap_malloc(soap,
					sizeof(struct tt__IPAddress));
	videoEncoderConfiguration->Multicast->Address->IPv4Address =
			(char *) soap_malloc(soap, sizeof(char) * INFO_LENGTH);
	videoEncoderConfiguration->Multicast->Address->IPv6Address = NULL;
	videoEncoderConfiguration->Multicast->Address->Type = tt__IPType__IPv4;
	videoEncoderConfiguration->Multicast->Port =
			onvifVideoChannelInfo->rtspPort;
	videoEncoderConfiguration->Multicast->TTL = 60;
	videoEncoderConfiguration->Multicast->AutoStart = xsd__boolean__true_;
	videoEncoderConfiguration->Multicast->__size = 0;
	videoEncoderConfiguration->Multicast->__any = NULL;
	videoEncoderConfiguration->Multicast->__anyAttribute = NULL;

	videoEncoderConfiguration->SessionTimeout = DEFAULT_SESSION_TIME_OUT;
	videoEncoderConfiguration->__size = 0;
	videoEncoderConfiguration->__any = NULL;
	videoEncoderConfiguration->__anyAttribute = NULL;
#endif
}

void getResponseProfileInfoVideoSourceConfiguration(struct soap* soap,
		struct tt__VideoSourceConfiguration * videoSourceConfiguration,
		OnvifVideoChannelInfo* onvifVideoChannelInfo, int index) {
	memset(videoSourceConfiguration, 0,
			sizeof(struct tt__VideoSourceConfiguration));
	videoSourceConfiguration->Name = (char *) soap_malloc(soap,
			sizeof(char) * SMALL_INFO_LENGTH);
	videoSourceConfiguration->token = (char *) soap_malloc(soap,
			sizeof(char) * SMALL_INFO_LENGTH);
	videoSourceConfiguration->SourceToken = (char *) soap_malloc(soap,
			sizeof(char) * INFO_LENGTH);
	/*注意SourceToken*/
	getVEName(videoSourceConfiguration->Name, index);
	getVEToken(videoSourceConfiguration->token, index);
	getVideoSourceToken(videoSourceConfiguration->SourceToken, index); /*必须与__tmd__GetVideoSources中的token相同*/
	videoSourceConfiguration->Extension = NULL;
	videoSourceConfiguration->__any = NULL;
	videoSourceConfiguration->__anyAttribute = NULL;
	videoSourceConfiguration->__size = 0;
	videoSourceConfiguration->Bounds = (struct tt__IntRectangle *) soap_malloc(
			soap, sizeof(struct tt__IntRectangle));
	videoSourceConfiguration->UseCount = 1;
	videoSourceConfiguration->Bounds->x = 1;
	videoSourceConfiguration->Bounds->y = 1;
	videoSourceConfiguration->Bounds->height = onvifVideoChannelInfo->height;
	videoSourceConfiguration->Bounds->width = onvifVideoChannelInfo->width;
}

void getResponseProfileInfo(struct soap* soap, struct tt__Profile* destProfile,
		OnvifVideoChannelInfo* onvifVideoChannelInfo, int index) {
	memset(destProfile, 0, sizeof(struct tt__Profile));
	destProfile->Name = (char *) soap_malloc(soap,
			sizeof(char) * SMALL_INFO_LENGTH);
	memset(destProfile->Name, 0, sizeof(char) * SMALL_INFO_LENGTH);
	getResponseProfileName(destProfile->Name, index);
	destProfile->token = (char *) soap_malloc(soap,
			sizeof(char) * SMALL_INFO_LENGTH);
	memset(destProfile->token, 0, sizeof(char) * SMALL_INFO_LENGTH);
	getResponseProfileToken(destProfile->token, index);
	destProfile->fixed = (enum xsd__boolean *) soap_malloc(soap,
			sizeof(enum xsd__boolean));
	*destProfile->fixed = xsd__boolean__false_;
	destProfile->__anyAttribute = NULL;
	destProfile->VideoAnalyticsConfiguration = NULL;

	/*必须包含VideoEncoderConfiguration的配置
	 不然不会出现live video 和 video streaming*/
	/*VideoEncoderConfiguration*/
	destProfile->VideoEncoderConfiguration = NULL;
#if 1
	destProfile->VideoEncoderConfiguration =
			(struct tt__VideoEncoderConfiguration *) soap_malloc(soap,
					sizeof(struct tt__VideoEncoderConfiguration));
	getResponseProfileInfoVideoEncoderConfiguration(soap,
			destProfile->VideoEncoderConfiguration, onvifVideoChannelInfo,
			index);
#endif

	/* VideoSourceConfiguration */
	//getResponseProfileInfo->VideoSourceConfiguration = NULL;
	destProfile->VideoSourceConfiguration =
			(struct tt__VideoSourceConfiguration *) my_soap_malloc(soap,
					sizeof(struct tt__VideoSourceConfiguration));
	getResponseProfileInfoVideoSourceConfiguration(soap,
			destProfile->VideoSourceConfiguration, onvifVideoChannelInfo,
			index);

	destProfile->AudioEncoderConfiguration = NULL;
	destProfile->AudioSourceConfiguration = NULL;
	destProfile->PTZConfiguration = NULL;
	destProfile->MetadataConfiguration = NULL;
	destProfile->Extension = NULL;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetServiceCapabilities(
		struct soap* soap,
		struct _trt__GetServiceCapabilities *trt__GetServiceCapabilities,
		struct _trt__GetServiceCapabilitiesResponse *trt__GetServiceCapabilitiesResponse) {
	logInfo("__trt__GetServiceCapabilities");
	return getOnvifSoapActionNotSupportCode(soap, "GetServiceCapabilities",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSources(struct soap* soap,
		struct _trt__GetVideoSources *trt__GetVideoSources,
		struct _trt__GetVideoSourcesResponse *trt__GetVideoSourcesResponse) {
	logInfo("__trt__GetVideoSources");

	int size1 = 0;
	int ret = getVideoCount(&size1);
	if (!isRetCodeSuccess(ret)) {
		return getOnvifSoapActionFailedCode(soap, "GetVideoSources",
				"getVideCount failed");
	}
	trt__GetVideoSourcesResponse->__sizeVideoSources = size1;
	trt__GetVideoSourcesResponse->VideoSources =
			(struct tt__VideoSource *) soap_malloc(soap,
					sizeof(struct tt__VideoSource) * size1);
	memset(trt__GetVideoSourcesResponse->VideoSources, 0,
			sizeof(struct tt__VideoSource) * size1);
	int i = 0;
	for (i = 0; i < size1; i++) {
		OnvifVideoChannelInfo onvifVideoChannelInfo;
		onvifVideoChannelInfo.channelNo = i;
		ret = getVideoChannelInfo(&onvifVideoChannelInfo);
		if (!isRetCodeSuccess(ret)) {
			return getOnvifSoapActionFailedCode(soap, "GetVideoSources",
					"getVideoChannelInfo failed");
		}
		getVideoSourcesResponseVideoSource(soap,
				&(trt__GetVideoSourcesResponse->VideoSources[i]),
				&onvifVideoChannelInfo, i);
	}
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSources(struct soap* soap,
		struct _trt__GetAudioSources *trt__GetAudioSources,
		struct _trt__GetAudioSourcesResponse *trt__GetAudioSourcesResponse) {
	logInfo("__trt__GetAudioSources");
	return getOnvifSoapActionNotSupportCode(soap, "GetAudioSources", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputs(struct soap* soap,
		struct _trt__GetAudioOutputs *trt__GetAudioOutputs,
		struct _trt__GetAudioOutputsResponse *trt__GetAudioOutputsResponse) {
	logInfo("__trt__GetAudioOutputs");
	return getOnvifSoapActionNotSupportCode(soap, "GetAudioOutputs", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__CreateProfile(struct soap* soap,
		struct _trt__CreateProfile *trt__CreateProfile,
		struct _trt__CreateProfileResponse *trt__CreateProfileResponse) {
	logInfo("__trt__CreateProfile");
	return getOnvifSoapActionNotSupportCode(soap, "CreateProfile", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetProfile(struct soap* soap,
		struct _trt__GetProfile *trt__GetProfile,
		struct _trt__GetProfileResponse *trt__GetProfileResponse) {
	logInfo("__trt__GetProfile");
	int index = getIndexFromProfileToken(trt__GetProfile->ProfileToken);
	if (index < 0) {
		return getOnvifSoapSendInvalidArgFailedCode(soap, "GetProfile",
				"profile token is invalid");
	}
	OnvifVideoChannelInfo onvifVideoChannelInfo;
	onvifVideoChannelInfo.channelNo = index;
	if (!isRetCodeSuccess(getVideoChannelInfo(&onvifVideoChannelInfo))) {
		return getOnvifSoapActionFailedCode(soap, "GetProfile",
				"getVideoChannelInfo failed");
	}

	/*这里的ProfileToken是选定的，得到特定的profile描述*/
	//但odm单击一个profile时，需要获取，不然不会出现live video和video streaming
	trt__GetProfileResponse->Profile = (struct tt__Profile *) my_soap_malloc(soap,
			sizeof(struct tt__Profile));
	getResponseProfileInfo(soap, trt__GetProfileResponse->Profile,
			&onvifVideoChannelInfo, index);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetProfiles(struct soap* soap,
		struct _trt__GetProfiles *trt__GetProfiles,
		struct _trt__GetProfilesResponse *trt__GetProfilesResponse) {
	logInfo("__trt__GetProfiles");
	int size = 0;
	int ret = getVideoCount(&size);
	if (!isRetCodeSuccess(ret)) {
		return getOnvifSoapActionFailedCode(soap, "GetProfiles",
				"getVideCount failed");
	}
	trt__GetProfilesResponse->Profiles = (struct tt__Profile *) soap_malloc(
			soap, sizeof(struct tt__Profile) * size);
	trt__GetProfilesResponse->__sizeProfiles = size;
	int i;
	for (i = 0; i < size; i++) {
		OnvifVideoChannelInfo onvifVideoChannelInfo;
		onvifVideoChannelInfo.channelNo = i;
		ret = getVideoChannelInfo(&onvifVideoChannelInfo);
		if (!isRetCodeSuccess(ret)) {
			return getOnvifSoapActionFailedCode(soap, "GetVideoSources",
					"getVideoChannelInfo failed");
		}
		getResponseProfileInfo(soap, &(trt__GetProfilesResponse->Profiles[i]),
				&onvifVideoChannelInfo, i);
	}
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoEncoderConfiguration(
		struct soap* soap,
		struct _trt__AddVideoEncoderConfiguration *trt__AddVideoEncoderConfiguration,
		struct _trt__AddVideoEncoderConfigurationResponse *trt__AddVideoEncoderConfigurationResponse) {
	logInfo("__trt__AddVideoEncoderConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"AddVideoEncoderConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoSourceConfiguration(
		struct soap* soap,
		struct _trt__AddVideoSourceConfiguration *trt__AddVideoSourceConfiguration,
		struct _trt__AddVideoSourceConfigurationResponse *trt__AddVideoSourceConfigurationResponse) {
	logInfo("__trt__AddVideoSourceConfiguration");
	return getOnvifSoapActionNotSupportCode(soap, "AddVideoSourceConfiguration",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioEncoderConfiguration(
		struct soap* soap,
		struct _trt__AddAudioEncoderConfiguration *trt__AddAudioEncoderConfiguration,
		struct _trt__AddAudioEncoderConfigurationResponse *trt__AddAudioEncoderConfigurationResponse) {
	logInfo("__trt__AddAudioEncoderConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"AddAudioEncoderConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioSourceConfiguration(
		struct soap* soap,
		struct _trt__AddAudioSourceConfiguration *trt__AddAudioSourceConfiguration,
		struct _trt__AddAudioSourceConfigurationResponse *trt__AddAudioSourceConfigurationResponse) {
	logInfo("__trt__AddAudioSourceConfiguration");
	return getOnvifSoapActionNotSupportCode(soap, "AddAudioSourceConfiguration",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__AddPTZConfiguration(
		struct soap* soap,
		struct _trt__AddPTZConfiguration *trt__AddPTZConfiguration,
		struct _trt__AddPTZConfigurationResponse *trt__AddPTZConfigurationResponse) {
	logInfo("__trt__AddPTZConfiguration");
	return getOnvifSoapActionNotSupportCode(soap, "AddPTZConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoAnalyticsConfiguration(
		struct soap* soap,
		struct _trt__AddVideoAnalyticsConfiguration *trt__AddVideoAnalyticsConfiguration,
		struct _trt__AddVideoAnalyticsConfigurationResponse *trt__AddVideoAnalyticsConfigurationResponse) {
	logInfo("__trt__AddVideoAnalyticsConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"AddVideoAnalyticsConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__AddMetadataConfiguration(
		struct soap* soap,
		struct _trt__AddMetadataConfiguration *trt__AddMetadataConfiguration,
		struct _trt__AddMetadataConfigurationResponse *trt__AddMetadataConfigurationResponse) {
	logInfo("__trt__AddMetadataConfiguration");
	return getOnvifSoapActionNotSupportCode(soap, "AddMetadataConfiguration",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioOutputConfiguration(
		struct soap* soap,
		struct _trt__AddAudioOutputConfiguration *trt__AddAudioOutputConfiguration,
		struct _trt__AddAudioOutputConfigurationResponse *trt__AddAudioOutputConfigurationResponse) {
	logInfo("__trt__AddAudioOutputConfiguration");
	return getOnvifSoapActionNotSupportCode(soap, "AddAudioOutputConfiguration",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioDecoderConfiguration(
		struct soap* soap,
		struct _trt__AddAudioDecoderConfiguration *trt__AddAudioDecoderConfiguration,
		struct _trt__AddAudioDecoderConfigurationResponse *trt__AddAudioDecoderConfigurationResponse) {
	logInfo("__trt__AddAudioDecoderConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"AddAudioDecoderConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoEncoderConfiguration(
		struct soap* soap,
		struct _trt__RemoveVideoEncoderConfiguration *trt__RemoveVideoEncoderConfiguration,
		struct _trt__RemoveVideoEncoderConfigurationResponse *trt__RemoveVideoEncoderConfigurationResponse) {
	logInfo("__trt__RemoveVideoEncoderConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"RemoveVideoEncoderConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoSourceConfiguration(
		struct soap* soap,
		struct _trt__RemoveVideoSourceConfiguration *trt__RemoveVideoSourceConfiguration,
		struct _trt__RemoveVideoSourceConfigurationResponse *trt__RemoveVideoSourceConfigurationResponse) {
	logInfo("__trt__RemoveVideoSourceConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"RemoveVideoSourceConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioEncoderConfiguration(
		struct soap* soap,
		struct _trt__RemoveAudioEncoderConfiguration *trt__RemoveAudioEncoderConfiguration,
		struct _trt__RemoveAudioEncoderConfigurationResponse *trt__RemoveAudioEncoderConfigurationResponse) {
	logInfo("__trt__RemoveAudioEncoderConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"RemoveAudioEncoderConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioSourceConfiguration(
		struct soap* soap,
		struct _trt__RemoveAudioSourceConfiguration *trt__RemoveAudioSourceConfiguration,
		struct _trt__RemoveAudioSourceConfigurationResponse *trt__RemoveAudioSourceConfigurationResponse) {
	logInfo("__trt__RemoveAudioSourceConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"RemoveAudioSourceConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__RemovePTZConfiguration(
		struct soap* soap,
		struct _trt__RemovePTZConfiguration *trt__RemovePTZConfiguration,
		struct _trt__RemovePTZConfigurationResponse *trt__RemovePTZConfigurationResponse) {
	logInfo("__trt__RemovePTZConfiguration");
	return getOnvifSoapActionNotSupportCode(soap, "RemovePTZConfiguration",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoAnalyticsConfiguration(
		struct soap* soap,
		struct _trt__RemoveVideoAnalyticsConfiguration *trt__RemoveVideoAnalyticsConfiguration,
		struct _trt__RemoveVideoAnalyticsConfigurationResponse *trt__RemoveVideoAnalyticsConfigurationResponse) {
	logInfo("__trt__RemoveVideoAnalyticsConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"RemoveVideoAnalyticsConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveMetadataConfiguration(
		struct soap* soap,
		struct _trt__RemoveMetadataConfiguration *trt__RemoveMetadataConfiguration,
		struct _trt__RemoveMetadataConfigurationResponse *trt__RemoveMetadataConfigurationResponse) {
	logInfo("__trt__RemoveMetadataConfiguration");
	return getOnvifSoapActionNotSupportCode(soap, "RemoveMetadataConfiguration",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioOutputConfiguration(
		struct soap* soap,
		struct _trt__RemoveAudioOutputConfiguration *trt__RemoveAudioOutputConfiguration,
		struct _trt__RemoveAudioOutputConfigurationResponse *trt__RemoveAudioOutputConfigurationResponse) {
	logInfo("__trt__RemoveAudioOutputConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"RemoveAudioOutputConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioDecoderConfiguration(
		struct soap* soap,
		struct _trt__RemoveAudioDecoderConfiguration *trt__RemoveAudioDecoderConfiguration,
		struct _trt__RemoveAudioDecoderConfigurationResponse *trt__RemoveAudioDecoderConfigurationResponse) {
	logInfo("__trt__RemoveAudioDecoderConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"RemoveAudioDecoderConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__DeleteProfile(struct soap* soap,
		struct _trt__DeleteProfile *trt__DeleteProfile,
		struct _trt__DeleteProfileResponse *trt__DeleteProfileResponse) {
	logInfo("__trt__DeleteProfile");
	return getOnvifSoapActionNotSupportCode(soap, "DeleteProfile", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfigurations(
		struct soap* soap,
		struct _trt__GetVideoSourceConfigurations *trt__GetVideoSourceConfigurations,
		struct _trt__GetVideoSourceConfigurationsResponse *trt__GetVideoSourceConfigurationsResponse) {
	logInfo("__trt__GetVideoSourceConfigurations");
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfigurations(
		struct soap* soap,
		struct _trt__GetVideoEncoderConfigurations *trt__GetVideoEncoderConfigurations,
		struct _trt__GetVideoEncoderConfigurationsResponse *trt__GetVideoEncoderConfigurationsResponse) {
	logInfo("__trt__GetVideoEncoderConfigurations");
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfigurations(
		struct soap* soap,
		struct _trt__GetAudioSourceConfigurations *trt__GetAudioSourceConfigurations,
		struct _trt__GetAudioSourceConfigurationsResponse *trt__GetAudioSourceConfigurationsResponse) {
	logInfo("__trt__GetAudioSourceConfigurations");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetAudioSourceConfigurations", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfigurations(
		struct soap* soap,
		struct _trt__GetAudioEncoderConfigurations *trt__GetAudioEncoderConfigurations,
		struct _trt__GetAudioEncoderConfigurationsResponse *trt__GetAudioEncoderConfigurationsResponse) {
	logInfo("__trt__GetAudioEncoderConfigurations");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetAudioEncoderConfigurations", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoAnalyticsConfigurations(
		struct soap* soap,
		struct _trt__GetVideoAnalyticsConfigurations *trt__GetVideoAnalyticsConfigurations,
		struct _trt__GetVideoAnalyticsConfigurationsResponse *trt__GetVideoAnalyticsConfigurationsResponse) {
	logInfo("__trt__GetVideoAnalyticsConfigurations");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetVideoAnalyticsConfigurations", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfigurations(
		struct soap* soap,
		struct _trt__GetMetadataConfigurations *trt__GetMetadataConfigurations,
		struct _trt__GetMetadataConfigurationsResponse *trt__GetMetadataConfigurationsResponse) {
	logInfo("__trt__GetMetadataConfigurations");
	return getOnvifSoapActionNotSupportCode(soap, "GetMetadataConfigurations",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfigurations(
		struct soap* soap,
		struct _trt__GetAudioOutputConfigurations *trt__GetAudioOutputConfigurations,
		struct _trt__GetAudioOutputConfigurationsResponse *trt__GetAudioOutputConfigurationsResponse) {
	logInfo("__trt__GetAudioOutputConfigurations");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetAudioOutputConfigurations", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfigurations(
		struct soap* soap,
		struct _trt__GetAudioDecoderConfigurations *trt__GetAudioDecoderConfigurations,
		struct _trt__GetAudioDecoderConfigurationsResponse *trt__GetAudioDecoderConfigurationsResponse) {
	logInfo("__trt__GetAudioDecoderConfigurations");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetAudioDecoderConfigurations", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfiguration(
		struct soap* soap,
		struct _trt__GetVideoSourceConfiguration *trt__GetVideoSourceConfiguration,
		struct _trt__GetVideoSourceConfigurationResponse *trt__GetVideoSourceConfigurationResponse) {
	logInfo("__trt__GetVideoSourceConfiguration");
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfiguration(
		struct soap* soap,
		struct _trt__GetVideoEncoderConfiguration *trt__GetVideoEncoderConfiguration,
		struct _trt__GetVideoEncoderConfigurationResponse *trt__GetVideoEncoderConfigurationResponse) {
	logInfo("__trt__GetVideoEncoderConfiguration");
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfiguration(
		struct soap* soap,
		struct _trt__GetAudioSourceConfiguration *trt__GetAudioSourceConfiguration,
		struct _trt__GetAudioSourceConfigurationResponse *trt__GetAudioSourceConfigurationResponse) {
	logInfo("__trt__GetAudioSourceConfiguration");
	return getOnvifSoapActionNotSupportCode(soap, "GetAudioSourceConfiguration",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfiguration(
		struct soap* soap,
		struct _trt__GetAudioEncoderConfiguration *trt__GetAudioEncoderConfiguration,
		struct _trt__GetAudioEncoderConfigurationResponse *trt__GetAudioEncoderConfigurationResponse) {
	logInfo("__trt__GetAudioEncoderConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetAudioEncoderConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoAnalyticsConfiguration(
		struct soap* soap,
		struct _trt__GetVideoAnalyticsConfiguration *trt__GetVideoAnalyticsConfiguration,
		struct _trt__GetVideoAnalyticsConfigurationResponse *trt__GetVideoAnalyticsConfigurationResponse) {
	logInfo("__trt__GetVideoAnalyticsConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetVideoAnalyticsConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfiguration(
		struct soap* soap,
		struct _trt__GetMetadataConfiguration *trt__GetMetadataConfiguration,
		struct _trt__GetMetadataConfigurationResponse *trt__GetMetadataConfigurationResponse) {
	logInfo("__trt__GetMetadataConfiguration");
	return getOnvifSoapActionNotSupportCode(soap, "GetMetadataConfiguration",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfiguration(
		struct soap* soap,
		struct _trt__GetAudioOutputConfiguration *trt__GetAudioOutputConfiguration,
		struct _trt__GetAudioOutputConfigurationResponse *trt__GetAudioOutputConfigurationResponse) {
	logInfo("__trt__GetAudioOutputConfiguration");
	return getOnvifSoapActionNotSupportCode(soap, "GetAudioOutputConfiguration",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfiguration(
		struct soap* soap,
		struct _trt__GetAudioDecoderConfiguration *trt__GetAudioDecoderConfiguration,
		struct _trt__GetAudioDecoderConfigurationResponse *trt__GetAudioDecoderConfigurationResponse) {
	logInfo("__trt__GetAudioDecoderConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetAudioDecoderConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoEncoderConfigurations(
		struct soap* soap,
		struct _trt__GetCompatibleVideoEncoderConfigurations *trt__GetCompatibleVideoEncoderConfigurations,
		struct _trt__GetCompatibleVideoEncoderConfigurationsResponse *trt__GetCompatibleVideoEncoderConfigurationsResponse) {
	logInfo("__trt__GetCompatibleVideoEncoderConfigurations");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetCompatibleVideoEncoderConfigurations", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoSourceConfigurations(
		struct soap* soap,
		struct _trt__GetCompatibleVideoSourceConfigurations *trt__GetCompatibleVideoSourceConfigurations,
		struct _trt__GetCompatibleVideoSourceConfigurationsResponse *trt__GetCompatibleVideoSourceConfigurationsResponse) {
	logInfo("__trt__GetCompatibleVideoSourceConfigurations");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetCompatibleVideoSourceConfigurations", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioEncoderConfigurations(
		struct soap* soap,
		struct _trt__GetCompatibleAudioEncoderConfigurations *trt__GetCompatibleAudioEncoderConfigurations,
		struct _trt__GetCompatibleAudioEncoderConfigurationsResponse *trt__GetCompatibleAudioEncoderConfigurationsResponse) {
	logInfo("__trt__GetCompatibleAudioEncoderConfigurations");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetCompatibleAudioEncoderConfigurations", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioSourceConfigurations(
		struct soap* soap,
		struct _trt__GetCompatibleAudioSourceConfigurations *trt__GetCompatibleAudioSourceConfigurations,
		struct _trt__GetCompatibleAudioSourceConfigurationsResponse *trt__GetCompatibleAudioSourceConfigurationsResponse) {
	logInfo("__trt__GetCompatibleAudioSourceConfigurations");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetCompatibleAudioSourceConfigurations", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoAnalyticsConfigurations(
		struct soap* soap,
		struct _trt__GetCompatibleVideoAnalyticsConfigurations *trt__GetCompatibleVideoAnalyticsConfigurations,
		struct _trt__GetCompatibleVideoAnalyticsConfigurationsResponse *trt__GetCompatibleVideoAnalyticsConfigurationsResponse) {
	logInfo("__trt__GetCompatibleVideoAnalyticsConfigurations");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetCompatibleVideoAnalyticsConfigurations", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleMetadataConfigurations(
		struct soap* soap,
		struct _trt__GetCompatibleMetadataConfigurations *trt__GetCompatibleMetadataConfigurations,
		struct _trt__GetCompatibleMetadataConfigurationsResponse *trt__GetCompatibleMetadataConfigurationsResponse) {
	logInfo("__trt__GetCompatibleMetadataConfigurations");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetCompatibleMetadataConfigurations", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioOutputConfigurations(
		struct soap* soap,
		struct _trt__GetCompatibleAudioOutputConfigurations *trt__GetCompatibleAudioOutputConfigurations,
		struct _trt__GetCompatibleAudioOutputConfigurationsResponse *trt__GetCompatibleAudioOutputConfigurationsResponse) {
	logInfo("__trt__GetCompatibleAudioOutputConfigurations");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetCompatibleAudioOutputConfigurations", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioDecoderConfigurations(
		struct soap* soap,
		struct _trt__GetCompatibleAudioDecoderConfigurations *trt__GetCompatibleAudioDecoderConfigurations,
		struct _trt__GetCompatibleAudioDecoderConfigurationsResponse *trt__GetCompatibleAudioDecoderConfigurationsResponse) {
	logInfo("__trt__GetCompatibleAudioDecoderConfigurations");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetCompatibleAudioDecoderConfigurations", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoSourceConfiguration(
		struct soap* soap,
		struct _trt__SetVideoSourceConfiguration *trt__SetVideoSourceConfiguration,
		struct _trt__SetVideoSourceConfigurationResponse *trt__SetVideoSourceConfigurationResponse) {
	logInfo("__trt__SetVideoSourceConfiguration");
	return getOnvifSoapActionNotSupportCode(soap, "SetVideoSourceConfiguration",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoEncoderConfiguration(
		struct soap* soap,
		struct _trt__SetVideoEncoderConfiguration *trt__SetVideoEncoderConfiguration,
		struct _trt__SetVideoEncoderConfigurationResponse *trt__SetVideoEncoderConfigurationResponse) {
	logInfo("__trt__SetVideoEncoderConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"SetVideoEncoderConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioSourceConfiguration(
		struct soap* soap,
		struct _trt__SetAudioSourceConfiguration *trt__SetAudioSourceConfiguration,
		struct _trt__SetAudioSourceConfigurationResponse *trt__SetAudioSourceConfigurationResponse) {
	logInfo("__trt__SetAudioSourceConfiguration");
	return getOnvifSoapActionNotSupportCode(soap, "SetAudioSourceConfiguration",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioEncoderConfiguration(
		struct soap* soap,
		struct _trt__SetAudioEncoderConfiguration *trt__SetAudioEncoderConfiguration,
		struct _trt__SetAudioEncoderConfigurationResponse *trt__SetAudioEncoderConfigurationResponse) {
	logInfo("__trt__SetAudioEncoderConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"SetAudioEncoderConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoAnalyticsConfiguration(
		struct soap* soap,
		struct _trt__SetVideoAnalyticsConfiguration *trt__SetVideoAnalyticsConfiguration,
		struct _trt__SetVideoAnalyticsConfigurationResponse *trt__SetVideoAnalyticsConfigurationResponse) {
	logInfo("__trt__SetVideoAnalyticsConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"SetVideoAnalyticsConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__SetMetadataConfiguration(
		struct soap* soap,
		struct _trt__SetMetadataConfiguration *trt__SetMetadataConfiguration,
		struct _trt__SetMetadataConfigurationResponse *trt__SetMetadataConfigurationResponse) {
	logInfo("__trt__SetMetadataConfiguration");
	return getOnvifSoapActionNotSupportCode(soap, "SetMetadataConfiguration",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioOutputConfiguration(
		struct soap* soap,
		struct _trt__SetAudioOutputConfiguration *trt__SetAudioOutputConfiguration,
		struct _trt__SetAudioOutputConfigurationResponse *trt__SetAudioOutputConfigurationResponse) {
	logInfo("__trt__SetAudioOutputConfiguration");
	return getOnvifSoapActionNotSupportCode(soap, "SetAudioOutputConfiguration",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioDecoderConfiguration(
		struct soap* soap,
		struct _trt__SetAudioDecoderConfiguration *trt__SetAudioDecoderConfiguration,
		struct _trt__SetAudioDecoderConfigurationResponse *trt__SetAudioDecoderConfigurationResponse) {
	logInfo("__trt__SetAudioDecoderConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"SetAudioDecoderConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfigurationOptions(
		struct soap* soap,
		struct _trt__GetVideoSourceConfigurationOptions *trt__GetVideoSourceConfigurationOptions,
		struct _trt__GetVideoSourceConfigurationOptionsResponse *trt__GetVideoSourceConfigurationOptionsResponse) {
	logInfo("__trt__GetVideoSourceConfigurationOptions");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetVideoSourceConfigurationOptions", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfigurationOptions(
		struct soap* soap,
		struct _trt__GetVideoEncoderConfigurationOptions *trt__GetVideoEncoderConfigurationOptions,
		struct _trt__GetVideoEncoderConfigurationOptionsResponse *trt__GetVideoEncoderConfigurationOptionsResponse) {
	logInfo("__trt__GetVideoEncoderConfigurationOptions");
	//该函数必要，video streaming需要
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfigurationOptions(
		struct soap* soap,
		struct _trt__GetAudioSourceConfigurationOptions *trt__GetAudioSourceConfigurationOptions,
		struct _trt__GetAudioSourceConfigurationOptionsResponse *trt__GetAudioSourceConfigurationOptionsResponse) {
	logInfo("__trt__GetAudioSourceConfigurationOptions");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetAudioSourceConfigurationOptions", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfigurationOptions(
		struct soap* soap,
		struct _trt__GetAudioEncoderConfigurationOptions *trt__GetAudioEncoderConfigurationOptions,
		struct _trt__GetAudioEncoderConfigurationOptionsResponse *trt__GetAudioEncoderConfigurationOptionsResponse) {
	logInfo("__trt__GetAudioEncoderConfigurationOptions");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetAudioEncoderConfigurationOptions", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfigurationOptions(
		struct soap* soap,
		struct _trt__GetMetadataConfigurationOptions *trt__GetMetadataConfigurationOptions,
		struct _trt__GetMetadataConfigurationOptionsResponse *trt__GetMetadataConfigurationOptionsResponse) {
	logInfo("__trt__GetMetadataConfigurationOptions");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetMetadataConfigurationOptions", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfigurationOptions(
		struct soap* soap,
		struct _trt__GetAudioOutputConfigurationOptions *trt__GetAudioOutputConfigurationOptions,
		struct _trt__GetAudioOutputConfigurationOptionsResponse *trt__GetAudioOutputConfigurationOptionsResponse) {
	logInfo("__trt__GetAudioOutputConfigurationOptions");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetAudioOutputConfigurationOptions", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfigurationOptions(
		struct soap* soap,
		struct _trt__GetAudioDecoderConfigurationOptions *trt__GetAudioDecoderConfigurationOptions,
		struct _trt__GetAudioDecoderConfigurationOptionsResponse *trt__GetAudioDecoderConfigurationOptionsResponse) {
	logInfo("__trt__GetAudioDecoderConfigurationOptions");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetAudioDecoderConfigurationOptions", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetGuaranteedNumberOfVideoEncoderInstances(
		struct soap* soap,
		struct _trt__GetGuaranteedNumberOfVideoEncoderInstances *trt__GetGuaranteedNumberOfVideoEncoderInstances,
		struct _trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse *trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse) {
	logInfo("__trt__GetGuaranteedNumberOfVideoEncoderInstances");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetGuaranteedNumberOfVideoEncoderInstances", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetStreamUri(struct soap* soap,
		struct _trt__GetStreamUri *trt__GetStreamUri,
		struct _trt__GetStreamUriResponse *trt__GetStreamUriResponse) {
	logInfo("__trt__GetStreamUri");
	if (NULL == trt__GetStreamUri->ProfileToken) {
		return getOnvifSoapSendInvalidArgFailedCode(soap, "GetStreamUri",
				"profile token is null");
	}
	int index = getIndexFromProfileToken(trt__GetStreamUri->ProfileToken);
	if (index < 0) {
		return getOnvifSoapSendInvalidArgFailedCode(soap, "GetStreamUri",
				"profile token is invalid");
	}
	OnvifVideoChannelInfo onvifVideoChannelInfo;
	onvifVideoChannelInfo.channelNo = index;
	if (!isRetCodeSuccess(getVideoChannelStreamInfo(&onvifVideoChannelInfo))) {
		return getOnvifSoapActionFailedCode(soap, "GetStreamUri",
				"getVideoChannelStreamInfo failed");
	}
	/* Response */
	trt__GetStreamUriResponse->MediaUri = (struct tt__MediaUri *) soap_malloc(
			soap, sizeof(struct tt__MediaUri));
	trt__GetStreamUriResponse->MediaUri->Uri = (char *) soap_malloc(soap,
			sizeof(char) * LARGE_INFO_LENGTH);
	strcpy(trt__GetStreamUriResponse->MediaUri->Uri,
			onvifVideoChannelInfo.videoAddr);
	trt__GetStreamUriResponse->MediaUri->InvalidAfterConnect = 0;
	trt__GetStreamUriResponse->MediaUri->InvalidAfterReboot = 0;
	trt__GetStreamUriResponse->MediaUri->Timeout = 200;
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__StartMulticastStreaming(
		struct soap* soap,
		struct _trt__StartMulticastStreaming *trt__StartMulticastStreaming,
		struct _trt__StartMulticastStreamingResponse *trt__StartMulticastStreamingResponse) {
	logInfo("__trt__StartMulticastStreaming");
	return getOnvifSoapActionNotSupportCode(soap, "StartMulticastStreaming",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__StopMulticastStreaming(
		struct soap* soap,
		struct _trt__StopMulticastStreaming *trt__StopMulticastStreaming,
		struct _trt__StopMulticastStreamingResponse *trt__StopMulticastStreamingResponse) {
	logInfo("__trt__StopMulticastStreaming");
	return getOnvifSoapActionNotSupportCode(soap, "StopMulticastStreaming",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__SetSynchronizationPoint(
		struct soap* soap,
		struct _trt__SetSynchronizationPoint *trt__SetSynchronizationPoint,
		struct _trt__SetSynchronizationPointResponse *trt__SetSynchronizationPointResponse) {
	logInfo("__trt__SetSynchronizationPoint");
	return getOnvifSoapActionNotSupportCode(soap, "SetSynchronizationPoint",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetSnapshotUri(struct soap* soap,
		struct _trt__GetSnapshotUri *trt__GetSnapshotUri,
		struct _trt__GetSnapshotUriResponse *trt__GetSnapshotUriResponse) {
	logInfo("__trt__GetSnapshotUri");
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

