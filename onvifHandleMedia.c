#include "soapH.h"
#include "soapStub.h"
#include "onvifHandle.h"
#include "appTools.h"
#include "logInfo.h"

#define DEFAULT_SESSION_TIME_OUT 720000
#define MEDIA_TOKEN_VIDEO_PREFIX "media_token_profile"
#define MEDIA_TOKEN_AUDIO_PREFIX "media_token_profile"
#define MEDIA_TOKENNAME_VIDEO_PREFIX "media_token_name_profile"
#define MEDIA_TOKENNAME_AUDIO_PREFIX "media_token_name_profile"
#define VIDEO_SOURCE_CONFIG_TOKEN_PREFIX "VE_Source_token"
#define AUDIO_SOURCE_CONFIG_TOKEN_PREFIX "AE_Source_token"
#define VIDEO_ENCODE_CONFIG_TOKEN_PREFIX "VE_Encode_token"
#define AUDIO_ENCODE_CONFIG_TOKEN_PREFIX "AE_Encode_token"
#define AUDIO_CHANNEL_MONO 1
#define MULTI_CAST_IP4_ADDRESS "0.0.0.0"

char* getVideoSourceToken(struct soap* soap, int index) {
	return getIndexTokeName(soap, VIDEO_SOURCE_TOKEN, 0);
}

char* getAudioSourceToken(struct soap* soap, int index) {
	return getIndexTokeName(soap, AUDIO_SOURCE_TOKEN, 0);
}


char* getVideoSourceConfigToken(struct soap* soap, int index) {
	return getIndexTokeName(soap, VIDEO_SOURCE_CONFIG_TOKEN_PREFIX, 0);
}

int getIndexFromVideoSourceConfigToken(char* token) {
	return getIndexFromTokenName(token, VIDEO_SOURCE_CONFIG_TOKEN_PREFIX);
}

char* getVideoEncodeToken(struct soap* soap, int index) {
	return getIndexTokeName(soap, VIDEO_ENCODE_CONFIG_TOKEN_PREFIX, index);
}

int getIndexFromVideoEncodeConfigToken(char* token) {
	return getIndexFromTokenName(token, VIDEO_ENCODE_CONFIG_TOKEN_PREFIX);
}

char* getVideoEncodeName(struct soap* soap, int index) {
	return getIndexTokeName(soap, "VE_Name", index);
}

char* getVideoSourceName(struct soap* soap, int index) {
	return getIndexTokeName(soap, "VS_Name", 0);
}

char* getAudioSourceConfigToken(struct soap* soap, int index) {
	return getIndexTokeName(soap, AUDIO_SOURCE_CONFIG_TOKEN_PREFIX, 0);
}

int getIndexFromAudioSourceConfigToken(char* token) {
	return getIndexFromTokenName(token, AUDIO_SOURCE_CONFIG_TOKEN_PREFIX);
}

char* getAudioEncodeToken(struct soap* soap, int index) {
	return getIndexTokeName(soap, AUDIO_ENCODE_CONFIG_TOKEN_PREFIX, index);
}


int getIndexFromAudioEncodeConfigToken(char* token) {
	return getIndexFromTokenName(token, AUDIO_ENCODE_CONFIG_TOKEN_PREFIX);
}

char* getAudioEncodeName(struct soap* soap, int index) {
	return getIndexTokeName(soap, "AE_Name", index);
}

char* getAudioSourceName(struct soap* soap, int index) {
	return getIndexTokeName(soap, "AS_Name", 0);
}

enum tt__VideoEncoding getVideoEncodeType(int encType) {
	//encType JPEG = 2, MPEG4 = 1, H264 = 0;
	enum tt__VideoEncoding result = tt__VideoEncoding__H264;
	switch (encType) {
	case 2:
		result = tt__VideoEncoding__JPEG;
		break;
	case 1:
		result = tt__VideoEncoding__MPEG4;
		break;
	}
	return result;
}

enum tt__AudioEncoding getAudioEncodeType(int encType) {
	//encType G711 = 0, G726 = 1, AAC = 2;
	enum tt__AudioEncoding result = tt__AudioEncoding__G711;
	switch (encType) {
	case 2:
		result = tt__AudioEncoding__AAC;
		break;
	case 1:
		result = tt__AudioEncoding__G726; //G711 = 0, G726 = 1, AAC = 2;
		break;
	}
	return result;
}

char* getVideoMediaProfileName(struct soap* soap, int index) {
	return getIndexTokeName(soap, MEDIA_TOKENNAME_VIDEO_PREFIX, index);
}

char* getAudioMediaProfileName(struct soap* soap, int index) {
	return getIndexTokeName(soap, MEDIA_TOKENNAME_AUDIO_PREFIX, index);
}

int getIndexFromVideoMediaProfileToken(char* token) {
	return getIndexFromTokenName(token, MEDIA_TOKEN_VIDEO_PREFIX);
}

char* getVideoMediaProfileToken(struct soap* soap, int index) {
	return getIndexTokeName(soap, MEDIA_TOKEN_VIDEO_PREFIX, index);
}

int getIndexFromAudioMediaProfileToken(char* token) {
	return getIndexFromTokenName(token, MEDIA_TOKEN_AUDIO_PREFIX);
}

char* getAudioMediaProfileToken(struct soap* soap, int index) {
	return getIndexTokeName(soap, MEDIA_TOKEN_AUDIO_PREFIX, index);
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
	videoSource->token = getVideoSourceToken(soap, index); //注意这里需要和GetProfile中的sourcetoken相同
	videoSource->Framerate = onvifVideoChannelInfo->frame_rate;
	videoSource->Resolution = (struct tt__VideoResolution *) my_soap_malloc(
			soap, sizeof(struct tt__VideoResolution));
	videoSource->Resolution->Height = onvifVideoChannelInfo->height;
	videoSource->Resolution->Width = onvifVideoChannelInfo->width;
	videoSource->Imaging = (struct tt__ImagingSettings*) my_soap_malloc(soap,
			sizeof(struct tt__ImagingSettings));
	videoSource->Imaging->Brightness = (float*) my_soap_malloc(soap,
			sizeof(float));
	videoSource->Imaging->Brightness[0] = onvifVideoChannelInfo->bright;
	videoSource->Imaging->ColorSaturation = (float*) my_soap_malloc(soap,
			sizeof(float));
	videoSource->Imaging->ColorSaturation[0] =
			onvifVideoChannelInfo->saturation;
	videoSource->Imaging->Contrast = (float*) my_soap_malloc(soap,
			sizeof(float));
	videoSource->Imaging->Contrast[0] = onvifVideoChannelInfo->contrast;
	videoSource->Imaging->IrCutFilter =
			(enum tt__IrCutFilterMode *) my_soap_malloc(soap,
					sizeof(enum tt__IrCutFilterMode));
	*videoSource->Imaging->IrCutFilter = 0;
	videoSource->Imaging->Sharpness = (float*) my_soap_malloc(soap,
			sizeof(float));
	videoSource->Imaging->Sharpness[0] = onvifVideoChannelInfo->sharpness;
	videoSource->Imaging->BacklightCompensation =
			(struct tt__BacklightCompensation*) my_soap_malloc(soap,
					sizeof(struct tt__BacklightCompensation));
	videoSource->Imaging->BacklightCompensation->Mode =
			getOnvifBacklightCompensationMode(
					onvifVideoChannelInfo->backLightCompMode);
	videoSource->Imaging->BacklightCompensation->Level =
			onvifVideoChannelInfo->backLightCompLevel;
	videoSource->Imaging->Exposure = NULL;
	videoSource->Imaging->Focus = NULL;
	videoSource->Imaging->WideDynamicRange =
			(struct tt__WideDynamicRange*) my_soap_malloc(soap,
					sizeof(struct tt__WideDynamicRange));
	videoSource->Imaging->WideDynamicRange->Mode = getOnvifWideDynamicMode(
			onvifVideoChannelInfo->wideDynamicMode);
	videoSource->Imaging->WideDynamicRange->Level =
			onvifVideoChannelInfo->wideDynamicLevel;
	videoSource->Imaging->WhiteBalance =
			(struct tt__WhiteBalance*) my_soap_malloc(soap,
					sizeof(struct tt__WhiteBalance));
	videoSource->Imaging->WhiteBalance->Mode = getOnvifWhiteBalanceMode(
			onvifVideoChannelInfo->wbMode);
	videoSource->Imaging->WhiteBalance->CrGain =
			onvifVideoChannelInfo->wbCrGain;
	videoSource->Imaging->WhiteBalance->CbGain =
			onvifVideoChannelInfo->wbCbGain;
	videoSource->Imaging->Extension = NULL;
	videoSource->Extension = NULL;
}

void getAudioSourcesResponseAudioSource(struct soap* soap,
		struct tt__AudioSource* audioSource,
		OnvifAudioChannelInfo* onvifAudioChannelInfo, int index) {
	audioSource->token = getAudioSourceToken(soap, index); //注意这里需要和GetProfile中的sourcetoken相同
	audioSource->Channels = AUDIO_CHANNEL_MONO;
}

void setMultiCastConfiguration(struct soap* soap, struct tt__MulticastConfiguration * multicastConfiguration, int rtspPort) {
	multicastConfiguration->Address =
			(struct tt__IPAddress *) my_soap_malloc(soap,
					sizeof(struct tt__IPAddress));
	multicastConfiguration->Address->IPv4Address =
			(char *) my_soap_malloc(soap, sizeof(char) * INFO_LENGTH);
	sprintf(multicastConfiguration->Address->IPv4Address, "%s", MULTI_CAST_IP4_ADDRESS);
	multicastConfiguration->Address->IPv6Address = NULL;
	multicastConfiguration->Address->Type = tt__IPType__IPv4;
	multicastConfiguration->Port = rtspPort;
	multicastConfiguration->TTL = 60;
	multicastConfiguration->AutoStart = xsd__boolean__true_;
	multicastConfiguration->__size = 0;
	multicastConfiguration->__any = NULL;
	multicastConfiguration->__anyAttribute = NULL;
}

void getResponseProfileInfoAudioEncoderConfiguration(struct soap* soap,
		struct tt__AudioEncoderConfiguration* audioEncoderConfiguration,
		OnvifAudioChannelInfo* onvifAudioChannelInfo, int index) {
	audioEncoderConfiguration->Name = getAudioEncodeName(soap, index);
	audioEncoderConfiguration->token = getAudioEncodeToken(soap, index);
	audioEncoderConfiguration->Bitrate = onvifAudioChannelInfo->bit_rate;
	audioEncoderConfiguration->Encoding = getAudioEncodeType(
			onvifAudioChannelInfo->enc_type);
	audioEncoderConfiguration->SampleRate = onvifAudioChannelInfo->sample_rate;
	audioEncoderConfiguration->UseCount = 1;
	audioEncoderConfiguration->Multicast =
			(struct tt__MulticastConfiguration *) my_soap_malloc(soap,
					sizeof(struct tt__MulticastConfiguration));
	setMultiCastConfiguration(soap, audioEncoderConfiguration->Multicast, onvifAudioChannelInfo->rtspPort);
	audioEncoderConfiguration->SessionTimeout = DEFAULT_SESSION_TIME_OUT;
	audioEncoderConfiguration->__size = 0;
	audioEncoderConfiguration->__any = NULL;
	audioEncoderConfiguration->__anyAttribute = NULL;
}

void getResponseProfileInfoAudioSourceConfiguration(struct soap* soap,
		struct tt__AudioSourceConfiguration * audioSourceConfiguration,
		int index) {
	audioSourceConfiguration->Name = getAudioSourceName(soap, index);
	audioSourceConfiguration->token = getAudioSourceConfigToken(soap, index);
	audioSourceConfiguration->SourceToken = getAudioSourceToken(soap, index);/*必须与__tmd__GetVideoSources中的token相同*/
	/*注意SourceToken*/
	audioSourceConfiguration->UseCount = 2;
}

void getResponseProfileInfoVideoEncoderConfiguration(struct soap* soap,
		struct tt__VideoEncoderConfiguration * videoEncoderConfiguration,
		OnvifVideoChannelInfo* onvifVideoChannelInfo, int index) {
	videoEncoderConfiguration->Name = getVideoEncodeName(soap, index);
	videoEncoderConfiguration->token = getVideoEncodeToken(soap, index);
	videoEncoderConfiguration->UseCount = 1;
	videoEncoderConfiguration->Quality = onvifVideoChannelInfo->quality;
	videoEncoderConfiguration->Encoding = getVideoEncodeType(
			onvifVideoChannelInfo->enc_type);
	videoEncoderConfiguration->Resolution =
			(struct tt__VideoResolution *) my_soap_malloc(soap,
					sizeof(struct tt__VideoResolution));
	videoEncoderConfiguration->Resolution->Height =
			onvifVideoChannelInfo->height;
	videoEncoderConfiguration->Resolution->Width = onvifVideoChannelInfo->width;
	videoEncoderConfiguration->RateControl =
			(struct tt__VideoRateControl *) my_soap_malloc(soap,
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
				(struct tt__Mpeg4Configuration *) my_soap_malloc(soap,
						sizeof(struct tt__Mpeg4Configuration));
		videoEncoderConfiguration->MPEG4->GovLength =
				onvifVideoChannelInfo->govLength;
		videoEncoderConfiguration->MPEG4->Mpeg4Profile =
				onvifVideoChannelInfo->videoEncodeProfile;
	} else if (tt__VideoEncoding__H264 == videoEncoderConfiguration->Encoding) {
		videoEncoderConfiguration->H264 =
				(struct tt__H264Configuration *) my_soap_malloc(soap,
						sizeof(struct tt__H264Configuration));
		videoEncoderConfiguration->H264->GovLength =
				onvifVideoChannelInfo->govLength;
		videoEncoderConfiguration->H264->H264Profile =
				onvifVideoChannelInfo->videoEncodeProfile;
	}

	videoEncoderConfiguration->Multicast =
			(struct tt__MulticastConfiguration *) my_soap_malloc(soap,
					sizeof(struct tt__MulticastConfiguration));
	setMultiCastConfiguration(soap, videoEncoderConfiguration->Multicast, onvifVideoChannelInfo->rtspPort);
	videoEncoderConfiguration->SessionTimeout = DEFAULT_SESSION_TIME_OUT;
	videoEncoderConfiguration->__size = 0;
	videoEncoderConfiguration->__any = NULL;
	videoEncoderConfiguration->__anyAttribute = NULL;
#endif
}

void getResponseProfileInfoVideoSourceConfiguration(struct soap* soap,
		struct tt__VideoSourceConfiguration * videoSourceConfiguration,
		OnvifVideoChannelInfo* onvifVideoChannelInfo, int index) {
	videoSourceConfiguration->Name = getVideoSourceName(soap, index);
	videoSourceConfiguration->token = getVideoSourceConfigToken(soap, index);
	videoSourceConfiguration->SourceToken = getVideoSourceToken(soap, index);/*必须与__tmd__GetVideoSources中的token相同*/
	/*注意SourceToken*/
	videoSourceConfiguration->Extension = NULL;
	videoSourceConfiguration->__any = NULL;
	videoSourceConfiguration->__anyAttribute = NULL;
	videoSourceConfiguration->__size = 0;
	videoSourceConfiguration->Bounds =
			(struct tt__IntRectangle *) my_soap_malloc(soap,
					sizeof(struct tt__IntRectangle));
	videoSourceConfiguration->UseCount = 2;
	videoSourceConfiguration->Bounds->x = 0;
	videoSourceConfiguration->Bounds->y = 0;
	videoSourceConfiguration->Bounds->height = onvifVideoChannelInfo->height;
	videoSourceConfiguration->Bounds->width = onvifVideoChannelInfo->width;
}

void getResponseProfileInfoByVideoChannelInfo(struct soap* soap,
		struct tt__Profile* destProfile,
		OnvifVideoChannelInfo* onvifVideoChannelInfo, int index) {
	if (NULL == onvifVideoChannelInfo)
		return;
	if (FALSE == onvifVideoChannelInfo->stream_enable) {
		return;
	}
	/*必须包含VideoEncoderConfiguration的配置
	 不然不会出现live video 和 video streaming*/
	/*VideoEncoderConfiguration*/
	destProfile->VideoEncoderConfiguration = NULL;
#if 1
	destProfile->VideoEncoderConfiguration =
			(struct tt__VideoEncoderConfiguration *) my_soap_malloc(soap,
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
}

void getResponseProfileInfoByAudioChannelInfo(struct soap* soap,
		struct tt__Profile* destProfile,
		OnvifAudioChannelInfo* onvifAudioChannelInfo, int index) {
	if (NULL == onvifAudioChannelInfo)
		return;
	if (FALSE == onvifAudioChannelInfo->stream_enable) {
		return;
	}
	destProfile->AudioEncoderConfiguration =
			(struct tt__AudioEncoderConfiguration *) my_soap_malloc(soap,
					sizeof(struct tt__AudioEncoderConfiguration));
	getResponseProfileInfoAudioEncoderConfiguration(soap,
			destProfile->AudioEncoderConfiguration, onvifAudioChannelInfo,
			index);
	destProfile->AudioSourceConfiguration =
			(struct tt__AudioSourceConfiguration *) my_soap_malloc(soap,
					sizeof(struct tt__AudioSourceConfiguration));
	getResponseProfileInfoAudioSourceConfiguration(soap,
			destProfile->AudioSourceConfiguration, index);
}

void getResponseProfileInfo(struct soap* soap, struct tt__Profile* destProfile,
		OnvifVideoChannelInfo* onvifVideoChannelInfo,
		OnvifAudioChannelInfo* onvifAudioChannelInfo, int index) {
	destProfile->Name = getVideoMediaProfileName(soap, index);
	destProfile->token = getVideoMediaProfileToken(soap, index);
	destProfile->fixed = getxsdBoolean(soap, true);
	destProfile->__anyAttribute = NULL;
	destProfile->VideoAnalyticsConfiguration = NULL;
	destProfile->PTZConfiguration = getPTZConfiguration(soap);
	destProfile->MetadataConfiguration = NULL;
	destProfile->Extension = NULL;
	getResponseProfileInfoByVideoChannelInfo(soap, destProfile,
			onvifVideoChannelInfo, index);
	getResponseProfileInfoByAudioChannelInfo(soap, destProfile,
			onvifAudioChannelInfo, index);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetServiceCapabilities(
		struct soap* soap,
		struct _trt__GetServiceCapabilities *trt__GetServiceCapabilities,
		struct _trt__GetServiceCapabilitiesResponse *trt__GetServiceCapabilitiesResponse) {
	logInfo("__trt__GetServiceCapabilities");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media GetServiceCapabilities", "not support");
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSources(struct soap* soap,
		struct _trt__GetVideoSources *trt__GetVideoSources,
		struct _trt__GetVideoSourcesResponse *trt__GetVideoSourcesResponse) {
	logInfo("__trt__GetVideoSources");

	int size1 = 0;
	int ret = getVideoCount(&size1);
	if (!isRetCodeSuccess(ret)) {
		return getOnvifSoapActionFailedCode(soap, "GetVideoSources",
				"getVideoCount failed");
	}
	if (size1 > 1) {
		size1 = 1;
	}
	trt__GetVideoSourcesResponse->__sizeVideoSources = size1;
	trt__GetVideoSourcesResponse->VideoSources =
			(struct tt__VideoSource *) my_soap_malloc(soap,
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
		if (TRUE == onvifVideoChannelInfo.stream_enable) {
			getVideoSourcesResponseVideoSource(soap,
					&(trt__GetVideoSourcesResponse->VideoSources[i]),
					&onvifVideoChannelInfo, i);
		}
	}
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSources(struct soap* soap,
		struct _trt__GetAudioSources *trt__GetAudioSources,
		struct _trt__GetAudioSourcesResponse *trt__GetAudioSourcesResponse) {
	logInfo("__trt__GetAudioSources");
	int size1 = 0;
	int ret = getAudioCount(&size1);
	if (!isRetCodeSuccess(ret)) {
		return getOnvifSoapActionFailedCode(soap, "GetAudioSources",
				"getAudioCount failed");
	}
	if (size1 > 1) {
		size1 = 1;
	}
	trt__GetAudioSourcesResponse->__sizeAudioSources = size1;
	trt__GetAudioSourcesResponse->AudioSources =
			(struct tt__AudioSource *) my_soap_malloc(soap,
					sizeof(struct tt__AudioSource) * size1);
	int i = 0;
	for (i = 0; i < size1; i++) {
		OnvifAudioChannelInfo onvifAudioChannelInfo;
		onvifAudioChannelInfo.channelNo = i;
		ret = getAudioChannelInfo(&onvifAudioChannelInfo);
		if (!isRetCodeSuccess(ret)) {
			return getOnvifSoapActionFailedCode(soap, "GetAudioSources",
					"getAudioChannelInfo failed");
		}
		if (TRUE == onvifAudioChannelInfo.stream_enable)
			getAudioSourcesResponseAudioSource(soap,
					&(trt__GetAudioSourcesResponse->AudioSources[i]),
					&onvifAudioChannelInfo, i);
	}
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputs(struct soap* soap,
		struct _trt__GetAudioOutputs *trt__GetAudioOutputs,
		struct _trt__GetAudioOutputsResponse *trt__GetAudioOutputsResponse) {
	logInfo("__trt__GetAudioOutputs");
	return getOnvifSoapActionNotSupportCode(soap, "Media GetAudioOutputs", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__CreateProfile(struct soap* soap,
		struct _trt__CreateProfile *trt__CreateProfile,
		struct _trt__CreateProfileResponse *trt__CreateProfileResponse) {
	logInfo("__trt__CreateProfile");
	return getOnvifSoapActionNotSupportCode(soap, "Media CreateProfile", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetProfile(struct soap* soap,
		struct _trt__GetProfile *trt__GetProfile,
		struct _trt__GetProfileResponse *trt__GetProfileResponse) {
	logInfo("__trt__GetProfile");
	logInfo("__trt__GetProfile profile token %s",
			trt__GetProfile->ProfileToken);
	int vindex = getIndexFromVideoMediaProfileToken(
			trt__GetProfile->ProfileToken);
	int aindex = getIndexFromAudioMediaProfileToken(
			trt__GetProfile->ProfileToken);
	if ((vindex < 0) && (aindex < 0)) {
		return getOnvifSoapSendInvalidArgFailedCode(soap, "GetProfile",
				"profile token is invalid");
	}
	OnvifVideoChannelInfo* pOnvifVideoChannelInfo = NULL;
	OnvifVideoChannelInfo onvifVideoChannelInfo;
	if (vindex > -1) {
		onvifVideoChannelInfo.channelNo = vindex;
		if (!isRetCodeSuccess(getVideoChannelInfo(&onvifVideoChannelInfo))) {
			return getOnvifSoapActionFailedCode(soap, "GetProfile",
					"getVideoChannelInfo failed");
		}
		pOnvifVideoChannelInfo = &onvifVideoChannelInfo;
	}
	OnvifAudioChannelInfo* pOnvifAudioChannelInfo = NULL;
	OnvifAudioChannelInfo onvifAudioChannelInfo;
	if (aindex > -1) {
		onvifAudioChannelInfo.channelNo = aindex;
		if (!isRetCodeSuccess(getAudioChannelInfo(&onvifAudioChannelInfo))) {
			return getOnvifSoapActionFailedCode(soap, "GetProfile",
					"getAudioChannelInfo failed");
		}
		pOnvifAudioChannelInfo = &onvifAudioChannelInfo;
	}
	trt__GetProfileResponse->Profile = (struct tt__Profile *) my_soap_malloc(
			soap, sizeof(struct tt__Profile));
	getResponseProfileInfo(soap, trt__GetProfileResponse->Profile,
			pOnvifVideoChannelInfo, pOnvifAudioChannelInfo,
			vindex > -1 ? vindex : aindex);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetProfiles(struct soap* soap,
		struct _trt__GetProfiles *trt__GetProfiles,
		struct _trt__GetProfilesResponse *trt__GetProfilesResponse) {
	logInfo("__trt__GetProfiles");
	int vSize = 0;
	int ret = getVideoCount(&vSize);
	if (!isRetCodeSuccess(ret)) {
		return getOnvifSoapActionFailedCode(soap, "GetProfiles",
				"getVideoCount failed");
	}
	int aSize = 0;
	ret = getAudioCount(&aSize);
	if (!isRetCodeSuccess(ret)) {
		return getOnvifSoapActionFailedCode(soap, "GetProfiles",
				"getAudioCount failed");
	}
	int count = vSize > aSize ? vSize : aSize;
	trt__GetProfilesResponse->Profiles = (struct tt__Profile *) my_soap_malloc(
			soap, sizeof(struct tt__Profile) * count);
	trt__GetProfilesResponse->__sizeProfiles = count;
	int i;
	for (i = 0; i < count; i++) {
		OnvifVideoChannelInfo* pOnvifVideoChannelInfo = NULL;
		OnvifVideoChannelInfo onvifVideoChannelInfo;
		onvifVideoChannelInfo.channelNo = i;
		ret = getVideoChannelInfo(&onvifVideoChannelInfo);
		if (!isRetCodeSuccess(ret)) {
			logInfo("__trt__GetProfiles get Video Channel %d Info failed", i);
		} else {
			pOnvifVideoChannelInfo = &onvifVideoChannelInfo;
		}
		OnvifAudioChannelInfo* pOnvifAudioChannelInfo = NULL;
		OnvifAudioChannelInfo onvifAudioChannelInfo;
		onvifAudioChannelInfo.channelNo = i;
		ret = getAudioChannelInfo(&onvifAudioChannelInfo);
		if (!isRetCodeSuccess(ret)) {
			logInfo("__trt__GetProfiles get Audio Channel %d Info failed", i);
		} else {
			pOnvifAudioChannelInfo = &onvifAudioChannelInfo;
		}
		getResponseProfileInfo(soap, &(trt__GetProfilesResponse->Profiles[i]),
				pOnvifVideoChannelInfo, pOnvifAudioChannelInfo, i);
	}
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoEncoderConfiguration(
		struct soap* soap,
		struct _trt__AddVideoEncoderConfiguration *trt__AddVideoEncoderConfiguration,
		struct _trt__AddVideoEncoderConfigurationResponse *trt__AddVideoEncoderConfigurationResponse) {
	logInfo("__trt__AddVideoEncoderConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media AddVideoEncoderConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoSourceConfiguration(
		struct soap* soap,
		struct _trt__AddVideoSourceConfiguration *trt__AddVideoSourceConfiguration,
		struct _trt__AddVideoSourceConfigurationResponse *trt__AddVideoSourceConfigurationResponse) {
	logInfo("__trt__AddVideoSourceConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media AddVideoSourceConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioEncoderConfiguration(
		struct soap* soap,
		struct _trt__AddAudioEncoderConfiguration *trt__AddAudioEncoderConfiguration,
		struct _trt__AddAudioEncoderConfigurationResponse *trt__AddAudioEncoderConfigurationResponse) {
	logInfo("__trt__AddAudioEncoderConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media AddAudioEncoderConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioSourceConfiguration(
		struct soap* soap,
		struct _trt__AddAudioSourceConfiguration *trt__AddAudioSourceConfiguration,
		struct _trt__AddAudioSourceConfigurationResponse *trt__AddAudioSourceConfigurationResponse) {
	logInfo("__trt__AddAudioSourceConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media AddAudioSourceConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__AddPTZConfiguration(
		struct soap* soap,
		struct _trt__AddPTZConfiguration *trt__AddPTZConfiguration,
		struct _trt__AddPTZConfigurationResponse *trt__AddPTZConfigurationResponse) {
	logInfo("__trt__AddPTZConfiguration");
	return SOAP_OK;
	// return getOnvifSoapActionNotSupportCode(soap, "Media AddPTZConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__AddVideoAnalyticsConfiguration(
		struct soap* soap,
		struct _trt__AddVideoAnalyticsConfiguration *trt__AddVideoAnalyticsConfiguration,
		struct _trt__AddVideoAnalyticsConfigurationResponse *trt__AddVideoAnalyticsConfigurationResponse) {
	logInfo("__trt__AddVideoAnalyticsConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media AddVideoAnalyticsConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__AddMetadataConfiguration(
		struct soap* soap,
		struct _trt__AddMetadataConfiguration *trt__AddMetadataConfiguration,
		struct _trt__AddMetadataConfigurationResponse *trt__AddMetadataConfigurationResponse) {
	logInfo("__trt__AddMetadataConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media AddMetadataConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioOutputConfiguration(
		struct soap* soap,
		struct _trt__AddAudioOutputConfiguration *trt__AddAudioOutputConfiguration,
		struct _trt__AddAudioOutputConfigurationResponse *trt__AddAudioOutputConfigurationResponse) {
	logInfo("__trt__AddAudioOutputConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media AddAudioOutputConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__AddAudioDecoderConfiguration(
		struct soap* soap,
		struct _trt__AddAudioDecoderConfiguration *trt__AddAudioDecoderConfiguration,
		struct _trt__AddAudioDecoderConfigurationResponse *trt__AddAudioDecoderConfigurationResponse) {
	logInfo("__trt__AddAudioDecoderConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media AddAudioDecoderConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoEncoderConfiguration(
		struct soap* soap,
		struct _trt__RemoveVideoEncoderConfiguration *trt__RemoveVideoEncoderConfiguration,
		struct _trt__RemoveVideoEncoderConfigurationResponse *trt__RemoveVideoEncoderConfigurationResponse) {
	logInfo("__trt__RemoveVideoEncoderConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media RemoveVideoEncoderConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoSourceConfiguration(
		struct soap* soap,
		struct _trt__RemoveVideoSourceConfiguration *trt__RemoveVideoSourceConfiguration,
		struct _trt__RemoveVideoSourceConfigurationResponse *trt__RemoveVideoSourceConfigurationResponse) {
	logInfo("__trt__RemoveVideoSourceConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media RemoveVideoSourceConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioEncoderConfiguration(
		struct soap* soap,
		struct _trt__RemoveAudioEncoderConfiguration *trt__RemoveAudioEncoderConfiguration,
		struct _trt__RemoveAudioEncoderConfigurationResponse *trt__RemoveAudioEncoderConfigurationResponse) {
	logInfo("__trt__RemoveAudioEncoderConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media RemoveAudioEncoderConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioSourceConfiguration(
		struct soap* soap,
		struct _trt__RemoveAudioSourceConfiguration *trt__RemoveAudioSourceConfiguration,
		struct _trt__RemoveAudioSourceConfigurationResponse *trt__RemoveAudioSourceConfigurationResponse) {
	logInfo("__trt__RemoveAudioSourceConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media RemoveAudioSourceConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__RemovePTZConfiguration(
		struct soap* soap,
		struct _trt__RemovePTZConfiguration *trt__RemovePTZConfiguration,
		struct _trt__RemovePTZConfigurationResponse *trt__RemovePTZConfigurationResponse) {
	logInfo("__trt__RemovePTZConfiguration");
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveVideoAnalyticsConfiguration(
		struct soap* soap,
		struct _trt__RemoveVideoAnalyticsConfiguration *trt__RemoveVideoAnalyticsConfiguration,
		struct _trt__RemoveVideoAnalyticsConfigurationResponse *trt__RemoveVideoAnalyticsConfigurationResponse) {
	logInfo("__trt__RemoveVideoAnalyticsConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media RemoveVideoAnalyticsConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveMetadataConfiguration(
		struct soap* soap,
		struct _trt__RemoveMetadataConfiguration *trt__RemoveMetadataConfiguration,
		struct _trt__RemoveMetadataConfigurationResponse *trt__RemoveMetadataConfigurationResponse) {
	logInfo("__trt__RemoveMetadataConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media RemoveMetadataConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioOutputConfiguration(
		struct soap* soap,
		struct _trt__RemoveAudioOutputConfiguration *trt__RemoveAudioOutputConfiguration,
		struct _trt__RemoveAudioOutputConfigurationResponse *trt__RemoveAudioOutputConfigurationResponse) {
	logInfo("__trt__RemoveAudioOutputConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media RemoveAudioOutputConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__RemoveAudioDecoderConfiguration(
		struct soap* soap,
		struct _trt__RemoveAudioDecoderConfiguration *trt__RemoveAudioDecoderConfiguration,
		struct _trt__RemoveAudioDecoderConfigurationResponse *trt__RemoveAudioDecoderConfigurationResponse) {
	logInfo("__trt__RemoveAudioDecoderConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media RemoveAudioDecoderConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__DeleteProfile(struct soap* soap,
		struct _trt__DeleteProfile *trt__DeleteProfile,
		struct _trt__DeleteProfileResponse *trt__DeleteProfileResponse) {
	logInfo("__trt__DeleteProfile");
	return getOnvifSoapActionNotSupportCode(soap, "Media DeleteProfile", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfigurations(
		struct soap* soap,
		struct _trt__GetVideoSourceConfigurations *trt__GetVideoSourceConfigurations,
		struct _trt__GetVideoSourceConfigurationsResponse *trt__GetVideoSourceConfigurationsResponse) {
	logInfo("__trt__GetVideoSourceConfigurations");
	int vSize = 0;
	int ret = getVideoCount(&vSize);
	if (!isRetCodeSuccess(ret)) {
		return getOnvifSoapActionFailedCode(soap,
				"GetVideoSourceConfigurations", "getVideoCount failed");
	}
	trt__GetVideoSourceConfigurationsResponse->__sizeConfigurations = vSize;
	trt__GetVideoSourceConfigurationsResponse->Configurations =
			(struct tt__VideoSourceConfiguration *) my_soap_malloc(soap,
					sizeof(struct tt__VideoSourceConfiguration) * vSize);
	int i = 0;
	for (i = 0; i < vSize; i++) {
		OnvifVideoChannelInfo onvifVideoChannelInfo;
		onvifVideoChannelInfo.channelNo = i;
		ret = getVideoChannelInfo(&onvifVideoChannelInfo);
		if (!isRetCodeSuccess(ret)) {
			return getOnvifSoapActionFailedCode(soap,
					"GetVideoSourceConfigurations",
					"getVideoChannelInfo failed");
		}
		getResponseProfileInfoVideoSourceConfiguration(soap,
				&(trt__GetVideoSourceConfigurationsResponse->Configurations[i]),
				&onvifVideoChannelInfo, i);
	}
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfigurations(
		struct soap* soap,
		struct _trt__GetVideoEncoderConfigurations *trt__GetVideoEncoderConfigurations,
		struct _trt__GetVideoEncoderConfigurationsResponse *trt__GetVideoEncoderConfigurationsResponse) {
	logInfo("__trt__GetVideoEncoderConfigurations");
	int vSize = 0;
	int ret = getVideoCount(&vSize);
	if (!isRetCodeSuccess(ret)) {
		return getOnvifSoapActionFailedCode(soap,
				"GetVideoEncoderConfigurations", "getVideoCount failed");
	}
	trt__GetVideoEncoderConfigurationsResponse->__sizeConfigurations = vSize;
	trt__GetVideoEncoderConfigurationsResponse->Configurations =
			(struct tt__VideoEncoderConfiguration *) my_soap_malloc(soap,
					sizeof(struct tt__VideoEncoderConfiguration) * vSize);
	int i = 0;
	for (i = 0; i < vSize; i++) {
		OnvifVideoChannelInfo onvifVideoChannelInfo;
		onvifVideoChannelInfo.channelNo = i;
		ret = getVideoChannelInfo(&onvifVideoChannelInfo);
		if (!isRetCodeSuccess(ret)) {
			return getOnvifSoapActionFailedCode(soap,
					"GetVideoEncoderConfigurations",
					"getVideoChannelInfo failed");
		}
		getResponseProfileInfoVideoEncoderConfiguration(
				soap,
				&(trt__GetVideoEncoderConfigurationsResponse->Configurations[i]),
				&onvifVideoChannelInfo, i);
	}
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfigurations(
		struct soap* soap,
		struct _trt__GetAudioSourceConfigurations *trt__GetAudioSourceConfigurations,
		struct _trt__GetAudioSourceConfigurationsResponse *trt__GetAudioSourceConfigurationsResponse) {
	logInfo("__trt__GetAudioSourceConfigurations");
	int aSize = 0;
	int ret = getAudioCount(&aSize);
	if (!isRetCodeSuccess(ret)) {
		return getOnvifSoapActionFailedCode(soap,
				"__trt__GetAudioSourceConfigurations", "getAudioCount failed");
	}
	trt__GetAudioSourceConfigurationsResponse->__sizeConfigurations = aSize;
	trt__GetAudioSourceConfigurationsResponse->Configurations =
			(struct tt__AudioSourceConfiguration *) my_soap_malloc(soap,
					sizeof(struct tt__AudioSourceConfiguration) * aSize);
	int i = 0;
	for (i = 0; i < aSize; i++) {
		getResponseProfileInfoAudioSourceConfiguration(soap,
				&(trt__GetAudioSourceConfigurationsResponse->Configurations[i]),
				i);
	}
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfigurations(
		struct soap* soap,
		struct _trt__GetAudioEncoderConfigurations *trt__GetAudioEncoderConfigurations,
		struct _trt__GetAudioEncoderConfigurationsResponse *trt__GetAudioEncoderConfigurationsResponse) {
	logInfo("__trt__GetAudioEncoderConfigurations");
	int aSize = 0;
	int ret = getAudioCount(&aSize);
	if (!isRetCodeSuccess(ret)) {
		return getOnvifSoapActionFailedCode(soap,
				"GetAudioEncoderConfigurations", "getVideoCount failed");
	}
	trt__GetAudioEncoderConfigurationsResponse->__sizeConfigurations = aSize;
	trt__GetAudioEncoderConfigurationsResponse->Configurations =
			(struct tt__AudioEncoderConfiguration *) my_soap_malloc(soap,
					sizeof(struct tt__AudioEncoderConfiguration) * aSize);
	int i = 0;
	for (i = 0; i < aSize; i++) {
		OnvifAudioChannelInfo onvifAudioChannelInfo;
		onvifAudioChannelInfo.channelNo = i;
		ret = getAudioChannelInfo(&onvifAudioChannelInfo);
		if (!isRetCodeSuccess(ret)) {
			return getOnvifSoapActionFailedCode(soap,
					"GetAudioEncoderConfigurations",
					"getVideoChannelInfo failed");
		}
		getResponseProfileInfoAudioEncoderConfiguration(
				soap,
				&(trt__GetAudioEncoderConfigurationsResponse->Configurations[i]),
				&onvifAudioChannelInfo, i);
	}
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoAnalyticsConfigurations(
		struct soap* soap,
		struct _trt__GetVideoAnalyticsConfigurations *trt__GetVideoAnalyticsConfigurations,
		struct _trt__GetVideoAnalyticsConfigurationsResponse *trt__GetVideoAnalyticsConfigurationsResponse) {
	logInfo("__trt__GetVideoAnalyticsConfigurations");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media GetVideoAnalyticsConfigurations", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfigurations(
		struct soap* soap,
		struct _trt__GetMetadataConfigurations *trt__GetMetadataConfigurations,
		struct _trt__GetMetadataConfigurationsResponse *trt__GetMetadataConfigurationsResponse) {
	logInfo("__trt__GetMetadataConfigurations");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media GetMetadataConfigurations", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfigurations(
		struct soap* soap,
		struct _trt__GetAudioOutputConfigurations *trt__GetAudioOutputConfigurations,
		struct _trt__GetAudioOutputConfigurationsResponse *trt__GetAudioOutputConfigurationsResponse) {
	logInfo("__trt__GetAudioOutputConfigurations");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media GetAudioOutputConfigurations", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfigurations(
		struct soap* soap,
		struct _trt__GetAudioDecoderConfigurations *trt__GetAudioDecoderConfigurations,
		struct _trt__GetAudioDecoderConfigurationsResponse *trt__GetAudioDecoderConfigurationsResponse) {
	logInfo("__trt__GetAudioDecoderConfigurations");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media GetAudioDecoderConfigurations", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfiguration(
		struct soap* soap,
		struct _trt__GetVideoSourceConfiguration *trt__GetVideoSourceConfiguration,
		struct _trt__GetVideoSourceConfigurationResponse *trt__GetVideoSourceConfigurationResponse) {
	logInfo("__trt__GetVideoSourceConfiguration");
	logInfo("__trt__GetVideoSourceConfiguration ConfigurationToken %s",
			trt__GetVideoSourceConfiguration->ConfigurationToken);
	int index = getIndexFromVideoSourceConfigToken(
			trt__GetVideoSourceConfiguration->ConfigurationToken);
	if (index < 0) {
		return getOnvifSoapSendInvalidArgFailedCode(soap,
				"GetVideoSourceConfiguration", "ConfigurationToken is invalid");
	}

	trt__GetVideoSourceConfigurationResponse->Configuration =
			(struct tt__VideoSourceConfiguration *) my_soap_malloc(soap,
					sizeof(struct tt__VideoSourceConfiguration));
	OnvifVideoChannelInfo onvifVideoChannelInfo;
	onvifVideoChannelInfo.channelNo = index;
	if (!isRetCodeSuccess(getVideoChannelInfo(&onvifVideoChannelInfo))) {
		return getOnvifSoapActionFailedCode(soap, "GetVideoSourceConfiguration",
				"getVideoChannelInfo failed");
	}
	getResponseProfileInfoVideoSourceConfiguration(soap,
			trt__GetVideoSourceConfigurationResponse->Configuration,
			&onvifVideoChannelInfo, index);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfiguration(
		struct soap* soap,
		struct _trt__GetVideoEncoderConfiguration *trt__GetVideoEncoderConfiguration,
		struct _trt__GetVideoEncoderConfigurationResponse *trt__GetVideoEncoderConfigurationResponse) {
	logInfo("__trt__GetVideoEncoderConfiguration");
	logInfo("__trt__GetVideoEncoderConfiguration ConfigurationToken %s",
			trt__GetVideoEncoderConfiguration->ConfigurationToken);
	int index = getIndexFromVideoEncodeConfigToken(
			trt__GetVideoEncoderConfiguration->ConfigurationToken);
	if (index < 0) {
		return getOnvifSoapSendInvalidArgFailedCode(soap,
				"GetVideoEncoderConfiguration", "ConfigurationToken is invalid");
	}

	trt__GetVideoEncoderConfigurationResponse->Configuration =
			(struct tt__VideoEncoderConfiguration *) my_soap_malloc(soap,
					sizeof(struct tt__VideoEncoderConfiguration));
	OnvifVideoChannelInfo onvifVideoChannelInfo;
	onvifVideoChannelInfo.channelNo = index;
	if (!isRetCodeSuccess(getVideoChannelInfo(&onvifVideoChannelInfo))) {
		return getOnvifSoapActionFailedCode(soap,
				"GetVideoEncoderConfiguration", "getVideoChannelInfo failed");
	}
	getResponseProfileInfoVideoEncoderConfiguration(soap,
			trt__GetVideoEncoderConfigurationResponse->Configuration,
			&onvifVideoChannelInfo, index);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfiguration(
		struct soap* soap,
		struct _trt__GetAudioSourceConfiguration *trt__GetAudioSourceConfiguration,
		struct _trt__GetAudioSourceConfigurationResponse *trt__GetAudioSourceConfigurationResponse) {
	logInfo("__trt__GetAudioSourceConfiguration");
	logInfo("__trt__GetAudioSourceConfiguration ConfigurationToken %s",
			trt__GetAudioSourceConfiguration->ConfigurationToken);
	int index = getIndexFromAudioSourceConfigToken(
			trt__GetAudioSourceConfiguration->ConfigurationToken);
	if (index < 0) {
		return getOnvifSoapSendInvalidArgFailedCode(soap,
				"GetAudioSourceConfiguration", "ConfigurationToken is invalid");
	}

	trt__GetAudioSourceConfigurationResponse->Configuration =
			(struct tt__AudioSourceConfiguration *) my_soap_malloc(soap,
					sizeof(struct tt__AudioSourceConfiguration));
	getResponseProfileInfoAudioSourceConfiguration(soap,
			trt__GetAudioSourceConfigurationResponse->Configuration, index);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfiguration(
		struct soap* soap,
		struct _trt__GetAudioEncoderConfiguration *trt__GetAudioEncoderConfiguration,
		struct _trt__GetAudioEncoderConfigurationResponse *trt__GetAudioEncoderConfigurationResponse) {
	logInfo("__trt__GetAudioEncoderConfiguration");
	logInfo("__trt__GetAudioEncoderConfiguration ConfigurationToken %s",
			trt__GetAudioEncoderConfiguration->ConfigurationToken);
	int index = getIndexFromAudioEncodeConfigToken(
			trt__GetAudioEncoderConfiguration->ConfigurationToken);
	if (index < 0) {
		return getOnvifSoapSendInvalidArgFailedCode(soap,
				"GetAudioEncoderConfiguration", "ConfigurationToken is invalid");
	}

	trt__GetAudioEncoderConfigurationResponse->Configuration =
			(struct tt__AudioEncoderConfiguration *) my_soap_malloc(soap,
					sizeof(struct tt__AudioEncoderConfiguration));
	OnvifAudioChannelInfo onvifAudioChannelInfo;
	onvifAudioChannelInfo.channelNo = index;
	if (!isRetCodeSuccess(getAudioChannelInfo(&onvifAudioChannelInfo))) {
		return getOnvifSoapActionFailedCode(soap,
				"GetAudioEncoderConfiguration", "getAudioChannelInfo failed");
	}
	getResponseProfileInfoAudioEncoderConfiguration(soap,
			trt__GetAudioEncoderConfigurationResponse->Configuration,
			&onvifAudioChannelInfo, index);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoAnalyticsConfiguration(
		struct soap* soap,
		struct _trt__GetVideoAnalyticsConfiguration *trt__GetVideoAnalyticsConfiguration,
		struct _trt__GetVideoAnalyticsConfigurationResponse *trt__GetVideoAnalyticsConfigurationResponse) {
	logInfo("__trt__GetVideoAnalyticsConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media GetVideoAnalyticsConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfiguration(
		struct soap* soap,
		struct _trt__GetMetadataConfiguration *trt__GetMetadataConfiguration,
		struct _trt__GetMetadataConfigurationResponse *trt__GetMetadataConfigurationResponse) {
	logInfo("__trt__GetMetadataConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media GetMetadataConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfiguration(
		struct soap* soap,
		struct _trt__GetAudioOutputConfiguration *trt__GetAudioOutputConfiguration,
		struct _trt__GetAudioOutputConfigurationResponse *trt__GetAudioOutputConfigurationResponse) {
	logInfo("__trt__GetAudioOutputConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media GetAudioOutputConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfiguration(
		struct soap* soap,
		struct _trt__GetAudioDecoderConfiguration *trt__GetAudioDecoderConfiguration,
		struct _trt__GetAudioDecoderConfigurationResponse *trt__GetAudioDecoderConfigurationResponse) {
	logInfo("__trt__GetAudioDecoderConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media GetAudioDecoderConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoEncoderConfigurations(
		struct soap* soap,
		struct _trt__GetCompatibleVideoEncoderConfigurations *trt__GetCompatibleVideoEncoderConfigurations,
		struct _trt__GetCompatibleVideoEncoderConfigurationsResponse *trt__GetCompatibleVideoEncoderConfigurationsResponse) {
	logInfo("__trt__GetCompatibleVideoEncoderConfigurations");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media GetCompatibleVideoEncoderConfigurations", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoSourceConfigurations(
		struct soap* soap,
		struct _trt__GetCompatibleVideoSourceConfigurations *trt__GetCompatibleVideoSourceConfigurations,
		struct _trt__GetCompatibleVideoSourceConfigurationsResponse *trt__GetCompatibleVideoSourceConfigurationsResponse) {
	logInfo("__trt__GetCompatibleVideoSourceConfigurations");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media GetCompatibleVideoSourceConfigurations", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioEncoderConfigurations(
		struct soap* soap,
		struct _trt__GetCompatibleAudioEncoderConfigurations *trt__GetCompatibleAudioEncoderConfigurations,
		struct _trt__GetCompatibleAudioEncoderConfigurationsResponse *trt__GetCompatibleAudioEncoderConfigurationsResponse) {
	logInfo("__trt__GetCompatibleAudioEncoderConfigurations");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media GetCompatibleAudioEncoderConfigurations", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioSourceConfigurations(
		struct soap* soap,
		struct _trt__GetCompatibleAudioSourceConfigurations *trt__GetCompatibleAudioSourceConfigurations,
		struct _trt__GetCompatibleAudioSourceConfigurationsResponse *trt__GetCompatibleAudioSourceConfigurationsResponse) {
	logInfo("__trt__GetCompatibleAudioSourceConfigurations");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media GetCompatibleAudioSourceConfigurations", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleVideoAnalyticsConfigurations(
		struct soap* soap,
		struct _trt__GetCompatibleVideoAnalyticsConfigurations *trt__GetCompatibleVideoAnalyticsConfigurations,
		struct _trt__GetCompatibleVideoAnalyticsConfigurationsResponse *trt__GetCompatibleVideoAnalyticsConfigurationsResponse) {
	logInfo("__trt__GetCompatibleVideoAnalyticsConfigurations");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media GetCompatibleVideoAnalyticsConfigurations", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleMetadataConfigurations(
		struct soap* soap,
		struct _trt__GetCompatibleMetadataConfigurations *trt__GetCompatibleMetadataConfigurations,
		struct _trt__GetCompatibleMetadataConfigurationsResponse *trt__GetCompatibleMetadataConfigurationsResponse) {
	logInfo("__trt__GetCompatibleMetadataConfigurations");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media GetCompatibleMetadataConfigurations", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioOutputConfigurations(
		struct soap* soap,
		struct _trt__GetCompatibleAudioOutputConfigurations *trt__GetCompatibleAudioOutputConfigurations,
		struct _trt__GetCompatibleAudioOutputConfigurationsResponse *trt__GetCompatibleAudioOutputConfigurationsResponse) {
	logInfo("__trt__GetCompatibleAudioOutputConfigurations");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media GetCompatibleAudioOutputConfigurations", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetCompatibleAudioDecoderConfigurations(
		struct soap* soap,
		struct _trt__GetCompatibleAudioDecoderConfigurations *trt__GetCompatibleAudioDecoderConfigurations,
		struct _trt__GetCompatibleAudioDecoderConfigurationsResponse *trt__GetCompatibleAudioDecoderConfigurationsResponse) {
	logInfo("__trt__GetCompatibleAudioDecoderConfigurations");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media GetCompatibleAudioDecoderConfigurations", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoSourceConfiguration(
		struct soap* soap,
		struct _trt__SetVideoSourceConfiguration *trt__SetVideoSourceConfiguration,
		struct _trt__SetVideoSourceConfigurationResponse *trt__SetVideoSourceConfigurationResponse) {
	logInfo("__trt__SetVideoSourceConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media SetVideoSourceConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoEncoderConfiguration(
		struct soap* soap,
		struct _trt__SetVideoEncoderConfiguration *trt__SetVideoEncoderConfiguration,
		struct _trt__SetVideoEncoderConfigurationResponse *trt__SetVideoEncoderConfigurationResponse) {
	logInfo("__trt__SetVideoEncoderConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media SetVideoEncoderConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioSourceConfiguration(
		struct soap* soap,
		struct _trt__SetAudioSourceConfiguration *trt__SetAudioSourceConfiguration,
		struct _trt__SetAudioSourceConfigurationResponse *trt__SetAudioSourceConfigurationResponse) {
	logInfo("__trt__SetAudioSourceConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media SetAudioSourceConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioEncoderConfiguration(
		struct soap* soap,
		struct _trt__SetAudioEncoderConfiguration *trt__SetAudioEncoderConfiguration,
		struct _trt__SetAudioEncoderConfigurationResponse *trt__SetAudioEncoderConfigurationResponse) {
	logInfo("__trt__SetAudioEncoderConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media SetAudioEncoderConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__SetVideoAnalyticsConfiguration(
		struct soap* soap,
		struct _trt__SetVideoAnalyticsConfiguration *trt__SetVideoAnalyticsConfiguration,
		struct _trt__SetVideoAnalyticsConfigurationResponse *trt__SetVideoAnalyticsConfigurationResponse) {
	logInfo("__trt__SetVideoAnalyticsConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media SetVideoAnalyticsConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__SetMetadataConfiguration(
		struct soap* soap,
		struct _trt__SetMetadataConfiguration *trt__SetMetadataConfiguration,
		struct _trt__SetMetadataConfigurationResponse *trt__SetMetadataConfigurationResponse) {
	logInfo("__trt__SetMetadataConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media SetMetadataConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioOutputConfiguration(
		struct soap* soap,
		struct _trt__SetAudioOutputConfiguration *trt__SetAudioOutputConfiguration,
		struct _trt__SetAudioOutputConfigurationResponse *trt__SetAudioOutputConfigurationResponse) {
	logInfo("__trt__SetAudioOutputConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media SetAudioOutputConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__SetAudioDecoderConfiguration(
		struct soap* soap,
		struct _trt__SetAudioDecoderConfiguration *trt__SetAudioDecoderConfiguration,
		struct _trt__SetAudioDecoderConfigurationResponse *trt__SetAudioDecoderConfigurationResponse) {
	logInfo("__trt__SetAudioDecoderConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media SetAudioDecoderConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoSourceConfigurationOptions(
		struct soap* soap,
		struct _trt__GetVideoSourceConfigurationOptions *trt__GetVideoSourceConfigurationOptions,
		struct _trt__GetVideoSourceConfigurationOptionsResponse *trt__GetVideoSourceConfigurationOptionsResponse) {
	logInfo("__trt__GetVideoSourceConfigurationOptions");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media GetVideoSourceConfigurationOptions", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetVideoEncoderConfigurationOptions(
		struct soap* soap,
		struct _trt__GetVideoEncoderConfigurationOptions *trt__GetVideoEncoderConfigurationOptions,
		struct _trt__GetVideoEncoderConfigurationOptionsResponse *trt__GetVideoEncoderConfigurationOptionsResponse) {
	logInfo("__trt__GetVideoEncoderConfigurationOptions");
	int index = getIndexFromVideoEncodeConfigToken(trt__GetVideoEncoderConfigurationOptions->ConfigurationToken);
	if (index < 0) {
		index = getIndexFromVideoMediaProfileToken(trt__GetVideoEncoderConfigurationOptions->ProfileToken);

	}
	if (index < 0)
		index = 0;
	trt__GetVideoEncoderConfigurationOptionsResponse->Options =
			(struct tt__VideoEncoderConfigurationOptions *) my_soap_malloc(soap,
					sizeof(struct tt__VideoEncoderConfigurationOptions));
	OnvifVideoEncoderConfigurationOptionInfo onvifVideoEncoderConfigurationOptionInfo;
	onvifVideoEncoderConfigurationOptionInfo.channelNo = index;
	if (!isRetCodeSuccess(
			getVideoEncoderConfigurationOptionInfo(
					&onvifVideoEncoderConfigurationOptionInfo))) {
		return getOnvifSoapActionFailedCode(soap,
				"getVideoEncoderConfigurationOptions",
				"getVideoEncoderConfigurationOptionInfo failed");
	}
	trt__GetVideoEncoderConfigurationOptionsResponse->Options->QualityRange =
			getIntRangeByOnvifRange(soap,
					&onvifVideoEncoderConfigurationOptionInfo.quality);
	trt__GetVideoEncoderConfigurationOptionsResponse->Options->H264 =
			(struct tt__H264Options *) my_soap_malloc(soap,
					sizeof(struct tt__H264Options));
	trt__GetVideoEncoderConfigurationOptionsResponse->Options->H264->EncodingIntervalRange =
			getIntRangeByOnvifRange(soap,
					&onvifVideoEncoderConfigurationOptionInfo.encodingInterval);
	trt__GetVideoEncoderConfigurationOptionsResponse->Options->H264->FrameRateRange =
			getIntRangeByOnvifRange(soap,
					&onvifVideoEncoderConfigurationOptionInfo.frameRate);
	trt__GetVideoEncoderConfigurationOptionsResponse->Options->H264->GovLengthRange =
			getIntRangeByOnvifRange(soap,
					&onvifVideoEncoderConfigurationOptionInfo.govLength);
	trt__GetVideoEncoderConfigurationOptionsResponse->Options->H264->__sizeH264ProfilesSupported =
			onvifVideoEncoderConfigurationOptionInfo.profileCount;
	trt__GetVideoEncoderConfigurationOptionsResponse->Options->H264->H264ProfilesSupported =
			(enum tt__H264Profile *) my_soap_malloc(
					soap,
					sizeof(enum tt__H264Profile)
							* onvifVideoEncoderConfigurationOptionInfo.profileCount);
	int i = 0;
	for (i = 0; i < onvifVideoEncoderConfigurationOptionInfo.profileCount;
			i++) {
		switch (onvifVideoEncoderConfigurationOptionInfo.profiles[i]) {
		case H264_Baseline:
			trt__GetVideoEncoderConfigurationOptionsResponse->Options->H264->H264ProfilesSupported[i] =
					tt__H264Profile__Baseline;
			break;
		case H264_Main:
			trt__GetVideoEncoderConfigurationOptionsResponse->Options->H264->H264ProfilesSupported[i] =
					tt__H264Profile__Main;
			break;
		default:
			trt__GetVideoEncoderConfigurationOptionsResponse->Options->H264->H264ProfilesSupported[i] =
					tt__H264Profile__High;
			break;
		}

	}
	trt__GetVideoEncoderConfigurationOptionsResponse->Options->H264->__sizeResolutionsAvailable = onvifVideoEncoderConfigurationOptionInfo.resolutionCount;
	trt__GetVideoEncoderConfigurationOptionsResponse->Options->H264->ResolutionsAvailable = (struct tt__VideoResolution *) my_soap_malloc(
			soap,
			sizeof(struct tt__VideoResolution)
					* onvifVideoEncoderConfigurationOptionInfo.resolutionCount);
	for(i = 0; i < onvifVideoEncoderConfigurationOptionInfo.resolutionCount; i++) {
		trt__GetVideoEncoderConfigurationOptionsResponse->Options->H264->ResolutionsAvailable[i].Width = onvifVideoEncoderConfigurationOptionInfo.resolutions[i].width;
		trt__GetVideoEncoderConfigurationOptionsResponse->Options->H264->ResolutionsAvailable[i].Height = onvifVideoEncoderConfigurationOptionInfo.resolutions[i].height;
	}
	//该函数必要，video streaming需要
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioSourceConfigurationOptions(
		struct soap* soap,
		struct _trt__GetAudioSourceConfigurationOptions *trt__GetAudioSourceConfigurationOptions,
		struct _trt__GetAudioSourceConfigurationOptionsResponse *trt__GetAudioSourceConfigurationOptionsResponse) {
	logInfo("__trt__GetAudioSourceConfigurationOptions");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media GetAudioSourceConfigurationOptions", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioEncoderConfigurationOptions(
		struct soap* soap,
		struct _trt__GetAudioEncoderConfigurationOptions *trt__GetAudioEncoderConfigurationOptions,
		struct _trt__GetAudioEncoderConfigurationOptionsResponse *trt__GetAudioEncoderConfigurationOptionsResponse) {
	logInfo("__trt__GetAudioEncoderConfigurationOptions");
	//该函数必要，audio streaming需要
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetMetadataConfigurationOptions(
		struct soap* soap,
		struct _trt__GetMetadataConfigurationOptions *trt__GetMetadataConfigurationOptions,
		struct _trt__GetMetadataConfigurationOptionsResponse *trt__GetMetadataConfigurationOptionsResponse) {
	logInfo("__trt__GetMetadataConfigurationOptions");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media GetMetadataConfigurationOptions", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioOutputConfigurationOptions(
		struct soap* soap,
		struct _trt__GetAudioOutputConfigurationOptions *trt__GetAudioOutputConfigurationOptions,
		struct _trt__GetAudioOutputConfigurationOptionsResponse *trt__GetAudioOutputConfigurationOptionsResponse) {
	logInfo("__trt__GetAudioOutputConfigurationOptions");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media GetAudioOutputConfigurationOptions", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetAudioDecoderConfigurationOptions(
		struct soap* soap,
		struct _trt__GetAudioDecoderConfigurationOptions *trt__GetAudioDecoderConfigurationOptions,
		struct _trt__GetAudioDecoderConfigurationOptionsResponse *trt__GetAudioDecoderConfigurationOptionsResponse) {
	logInfo("__trt__GetAudioDecoderConfigurationOptions");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media GetAudioDecoderConfigurationOptions", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetGuaranteedNumberOfVideoEncoderInstances(
		struct soap* soap,
		struct _trt__GetGuaranteedNumberOfVideoEncoderInstances *trt__GetGuaranteedNumberOfVideoEncoderInstances,
		struct _trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse *trt__GetGuaranteedNumberOfVideoEncoderInstancesResponse) {
	logInfo("__trt__GetGuaranteedNumberOfVideoEncoderInstances");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media GetGuaranteedNumberOfVideoEncoderInstances", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetStreamUri(struct soap* soap,
		struct _trt__GetStreamUri *trt__GetStreamUri,
		struct _trt__GetStreamUriResponse *trt__GetStreamUriResponse) {
	logInfo("__trt__GetStreamUri");
	if (NULL == trt__GetStreamUri->ProfileToken) {
		return getOnvifSoapSendInvalidArgFailedCode(soap, "GetStreamUri",
				"profile token is null");
	}
	logInfo("__trt__GetStreamUri Profile token %s",
			trt__GetStreamUri->ProfileToken);
	int index = getIndexFromVideoMediaProfileToken(
			trt__GetStreamUri->ProfileToken);
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
	trt__GetStreamUriResponse->MediaUri =
			(struct tt__MediaUri *) my_soap_malloc(soap,
					sizeof(struct tt__MediaUri));
	trt__GetStreamUriResponse->MediaUri->Uri = soap_strdup(soap,
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
	return getOnvifSoapActionNotSupportCode(soap,
			"Media StartMulticastStreaming", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__StopMulticastStreaming(
		struct soap* soap,
		struct _trt__StopMulticastStreaming *trt__StopMulticastStreaming,
		struct _trt__StopMulticastStreamingResponse *trt__StopMulticastStreamingResponse) {
	logInfo("__trt__StopMulticastStreaming");
	return getOnvifSoapActionNotSupportCode(soap,
			"Media StopMulticastStreaming", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__SetSynchronizationPoint(
		struct soap* soap,
		struct _trt__SetSynchronizationPoint *trt__SetSynchronizationPoint,
		struct _trt__SetSynchronizationPointResponse *trt__SetSynchronizationPointResponse) {
	logInfo("__trt__SetSynchronizationPoint");
	logInfo("__trt__SetSynchronizationPoint Profile token %s",
			trt__SetSynchronizationPoint->ProfileToken);
	int index = getIndexFromVideoMediaProfileToken(
			trt__SetSynchronizationPoint->ProfileToken);
	if (index < 0) {
		return getOnvifSoapSendInvalidArgFailedCode(soap,
				"SetSynchronizationPoint", "profile token is invalid");
	}
	if (!isRetCodeSuccess(setVideoSynchronizationPoint(index))) {
		return getOnvifSoapActionFailedCode(soap, "SetSynchronizationPoint",
				"setVideoSynchronizationPoint failed");
	}
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __trt__GetSnapshotUri(struct soap* soap,
		struct _trt__GetSnapshotUri *trt__GetSnapshotUri,
		struct _trt__GetSnapshotUriResponse *trt__GetSnapshotUriResponse) {
	logInfo("__trt__GetSnapshotUri");
	trt__GetSnapshotUriResponse->MediaUri =
			(struct tt__MediaUri *) my_soap_malloc(soap,
					sizeof(struct tt__MediaUri));
	trt__GetSnapshotUriResponse->MediaUri->Uri = soap_strdup(soap,
			"http://avatar.csdn.net/7/E/1/1_ghostyu.jpg");
	trt__GetSnapshotUriResponse->MediaUri->InvalidAfterConnect = 0;
	trt__GetSnapshotUriResponse->MediaUri->InvalidAfterReboot = 0;
	trt__GetSnapshotUriResponse->MediaUri->Timeout = 2; //seconds
	trt__GetSnapshotUriResponse->MediaUri->__size = 0;
	trt__GetSnapshotUriResponse->MediaUri->__any = NULL;
	trt__GetSnapshotUriResponse->MediaUri->__anyAttribute = NULL;
	return SOAP_OK;
}

