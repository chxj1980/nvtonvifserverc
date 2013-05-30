#include "soapH.h"
#include "soapStub.h"
#include "onvifHandle.h"
#include "appTools.h"
#include "logInfo.h"
#define PTZ_NODE_TOKEN_PREFIX "PTZ_node_token"
#define PTZ_CONFIG_TOKEN_PREFIX "PTZ_config_token"

char* getPTZNodeToken(struct soap* soap, int index) {
	return getIndexTokeName(soap, PTZ_NODE_TOKEN_PREFIX, index);
}

char* getPTZConfigurationToken(struct soap* soap, int index) {
	return getIndexTokeName(soap, PTZ_CONFIG_TOKEN_PREFIX, index);
}

char* getPTZName(struct soap* soap, int index) {
	return getIndexTokeName(soap, "PTZ_Name", index);
}

int getIndexFromPTZNodeToken(char* token) {
	return getIndexFromTokenName(token, PTZ_NODE_TOKEN_PREFIX);
}

int getIndexFromPTZConfigToken(char* token) {
	return getIndexFromTokenName(token, PTZ_CONFIG_TOKEN_PREFIX);
}

int getOnvifPTZSoapActionNotSupport(struct soap *soap, const char *faultInfo, const char* faultDetail) {
	return getOnvifPTZSoapActionNotSupport(soap, faultInfo,
			faultDetail);
}


struct tt__Vector2D* getVector2D(struct soap* soap, char* uri, const float x,
		const float y) {
	struct tt__Vector2D* result = (struct tt__Vector2D*) my_soap_malloc(soap,
			sizeof(struct tt__Vector2D));
	result->space = uri;
	result->x = x;
	result->y = y;
	return result;
}

struct tt__Vector1D* getVector1D(struct soap* soap, char* uri, const float x) {
	struct tt__Vector1D* result = (struct tt__Vector1D*) my_soap_malloc(soap,
			sizeof(struct tt__Vector1D));
	result->space = uri;
	result->x = x;
	return result;
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetServiceCapabilities(
		struct soap* soap,
		struct _tptz__GetServiceCapabilities *tptz__GetServiceCapabilities,
		struct _tptz__GetServiceCapabilitiesResponse *tptz__GetServiceCapabilitiesResponse) {
	logInfo("__tptz__GetServiceCapabilities");
	return getOnvifPTZSoapActionNotSupport(soap, "PTZ GetServiceCapabilities",
			NULL);
}

char* getPanTiltSpaceURI(struct soap* soap, const char* part) {
	char uri[LARGE_INFO_LENGTH] = { 0 };
	sprintf(uri, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/%s", part);
	return soap_strdup(soap, uri);
}

char* getZoomPositionSpaceURI(struct soap* soap, const char* part) {
	char uri[LARGE_INFO_LENGTH] = { 0 };
	sprintf(uri, "http://www.onvif.org/ver10/tptz/ZoomSpaces/%s", part);
	return soap_strdup(soap, uri);
}

char* getAbsolutePantTiltPositionSpaceURI(struct soap* soap) {
	return getPanTiltSpaceURI(soap, "PositionGenericSpace");
}

char* getAbsoluteZoomPositionSpaceURI(struct soap* soap) {
	return getZoomPositionSpaceURI(soap, "PositionGenericSpace");
}

char* getContinuousPanTiltVelocitySpaceURI(struct soap* soap) {
	return getPanTiltSpaceURI(soap, "VelocityGenericSpace");
}

char* getContinuousZoomVelocitySpaceURI(struct soap* soap) {
	return getZoomPositionSpaceURI(soap, "VelocityGenericSpace");
}

char* getRelativePanTiltTranslationSpaceURI(struct soap* soap) {
	return getPanTiltSpaceURI(soap, "TranslationGenericSpace");
}

char* getRelativeZoomTranslationSpaceURI(struct soap* soap) {
	return getZoomPositionSpaceURI(soap, "TranslationGenericSpace");
}

struct tt__PTZSpeed* getPTZConfigurationPTZSpeed(struct soap* soap) {
	struct tt__PTZSpeed* result = (struct tt__PTZSpeed*) my_soap_malloc(soap,
			sizeof(struct tt__PTZSpeed));
	result->PanTilt = getVector2D(soap,
			getRelativePanTiltTranslationSpaceURI(soap), 0.0, 0.0);
	result->Zoom = getVector1D(soap, getRelativeZoomTranslationSpaceURI(soap),
			0.0);
	return result;
}

struct tt__PTZConfiguration* getPTZConfiguration(struct soap* soap) {
	struct tt__PTZConfiguration* result =
			(struct tt__PTZConfiguration*) my_soap_malloc(soap,
					sizeof(struct tt__PTZConfiguration));
	result->DefaultAbsolutePantTiltPositionSpace =
			getAbsolutePantTiltPositionSpaceURI(soap);
	result->DefaultAbsoluteZoomPositionSpace = getAbsoluteZoomPositionSpaceURI(
			soap);
	result->DefaultContinuousPanTiltVelocitySpace =
			getContinuousPanTiltVelocitySpaceURI(soap);
	result->DefaultContinuousZoomVelocitySpace =
			getContinuousZoomVelocitySpaceURI(soap);
	result->DefaultRelativePanTiltTranslationSpace =
			getRelativePanTiltTranslationSpaceURI(soap);
	result->DefaultRelativeZoomTranslationSpace =
			getRelativeZoomTranslationSpaceURI(soap);
	result->DefaultPTZSpeed = getPTZConfigurationPTZSpeed(soap);
	result->DefaultPTZTimeout = (LONG64*) my_soap_malloc(soap, sizeof(LONG64));
	*result->DefaultPTZTimeout = 0;
	result->Name = getPTZName(soap, 0);
	result->token = getPTZConfigurationToken(soap, 0);
	result->NodeToken = getPTZNodeToken(soap, 0);
	return result;
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetConfigurations(
		struct soap* soap,
		struct _tptz__GetConfigurations *tptz__GetConfigurations,
		struct _tptz__GetConfigurationsResponse *tptz__GetConfigurationsResponse) {
	logInfo("__tptz__GetConfigurations");
	tptz__GetConfigurationsResponse->__sizePTZConfiguration = 1;
	tptz__GetConfigurationsResponse->PTZConfiguration = getPTZConfiguration(
			soap);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetPresets(struct soap* soap,
		struct _tptz__GetPresets *tptz__GetPresets,
		struct _tptz__GetPresetsResponse *tptz__GetPresetsResponse) {
	logInfo("__tptz__GetPresets");
	return getOnvifPTZSoapActionNotSupport(soap, "PTZ GetPresets", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__SetPreset(struct soap* soap,
		struct _tptz__SetPreset *tptz__SetPreset,
		struct _tptz__SetPresetResponse *tptz__SetPresetResponse) {
	logInfo("__tptz__SetPreset");
	return getOnvifPTZSoapActionNotSupport(soap, "PTZ SetPreset", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__RemovePreset(struct soap* soap,
		struct _tptz__RemovePreset *tptz__RemovePreset,
		struct _tptz__RemovePresetResponse *tptz__RemovePresetResponse) {
	logInfo("__tptz__RemovePreset");
	return getOnvifPTZSoapActionNotSupport(soap, "PTZ RemovePreset", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__GotoPreset(struct soap* soap,
		struct _tptz__GotoPreset *tptz__GotoPreset,
		struct _tptz__GotoPresetResponse *tptz__GotoPresetResponse) {
	logInfo("__tptz__GotoPreset");
	return getOnvifPTZSoapActionNotSupport(soap, "PTZ GotoPreset", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetStatus(struct soap* soap,
		struct _tptz__GetStatus *tptz__GetStatus,
		struct _tptz__GetStatusResponse *tptz__GetStatusResponse) {
	logInfo("__tptz__GetStatus");
	return getOnvifPTZSoapActionNotSupport(soap, "PTZ GetStatus", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetConfiguration(struct soap* soap,
		struct _tptz__GetConfiguration *tptz__GetConfiguration,
		struct _tptz__GetConfigurationResponse *tptz__GetConfigurationResponse) {
	logInfo("__tptz__GetConfiguration %s", tptz__GetConfiguration->PTZConfigurationToken);
	int index = getIndexFromPTZConfigToken(tptz__GetConfiguration->PTZConfigurationToken);
	if (index < 0) {
		return getOnvifSoapSenderSubCode2Fault(soap, "ter:InvalidArgVal",
				"ter:ter:NoConfig", "ptz get configuration", "name token is invalid");
	}
	tptz__GetConfigurationResponse->PTZConfiguration = getPTZConfiguration(
			soap);
	return SOAP_OK;
}

struct tt__FloatRange* getFloatRange(struct soap* soap, int min, int max) {
	struct tt__FloatRange* result = (struct tt__FloatRange*) my_soap_malloc(
			soap, sizeof(struct tt__FloatRange));
	result->Min = min;
	result->Max = max;
	return result;
}

void getPTZNodeInfoSupportRelateSpace(struct soap* soap,
		struct tt__PTZSpaces* ptzSpaces) {
	ptzSpaces->RelativePanTiltTranslationSpace =
			(struct tt__Space2DDescription*) my_soap_malloc(soap,
					sizeof(struct tt__Space2DDescription));
	ptzSpaces->RelativePanTiltTranslationSpace->URI = soap_strdup(soap, "http://www.onvif.org/ver10/tptz/PanTiltSpaces/TranslationGenericSpace");
	ptzSpaces->RelativePanTiltTranslationSpace->XRange = getFloatRange(soap, 0,
			100);
	ptzSpaces->RelativePanTiltTranslationSpace->YRange = getFloatRange(soap, 0,
			100);
}

void getPTZNodeInfo(struct soap* soap, struct tt__PTZNode* ptzNode, int index) {
	ptzNode->Name = getPTZName(soap, index);
	ptzNode->token = getPTZNodeToken(soap, index);
	ptzNode->FixedHomePosition = (enum xsd__boolean *) my_soap_malloc(soap,
			sizeof(enum xsd__boolean));
	*ptzNode->FixedHomePosition = xsd__boolean__false_;
	ptzNode->MaximumNumberOfPresets = 1;
	ptzNode->SupportedPTZSpaces = (struct tt__PTZSpaces*) my_soap_malloc(soap,
			sizeof(struct tt__PTZSpaces));
	getPTZNodeInfoSupportRelateSpace(soap, ptzNode->SupportedPTZSpaces);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetNodes(struct soap* soap,
		struct _tptz__GetNodes *tptz__GetNodes,
		struct _tptz__GetNodesResponse *tptz__GetNodesResponse) {
	logInfo("__tptz__GetNodes");
	tptz__GetNodesResponse->__sizePTZNode = 1;
	tptz__GetNodesResponse->PTZNode = (struct tt__PTZNode*) my_soap_malloc(soap,
			sizeof(struct tt__PTZNode) * tptz__GetNodesResponse->__sizePTZNode);
	getPTZNodeInfo(soap, tptz__GetNodesResponse->PTZNode, 0);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetNode(struct soap* soap,
		struct _tptz__GetNode *tptz__GetNode,
		struct _tptz__GetNodeResponse *tptz__GetNodeResponse) {
	logInfo("__tptz__GetNode %s", tptz__GetNode->NodeToken);
	int index = getIndexFromPTZNodeToken(tptz__GetNode->NodeToken);
	if (index < 0) {
		return getOnvifSoapSenderSubCode2Fault(soap, "ter:InvalidArgVal",
				"ter:NoEntity", "ptz get node", "name token is invalid");
	}
	tptz__GetNodeResponse->PTZNode = (struct tt__PTZNode*) my_soap_malloc(soap,
			sizeof(struct tt__PTZNode));
	getPTZNodeInfo(soap, tptz__GetNodeResponse->PTZNode, index);
	return SOAP_OK;
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__SetConfiguration(struct soap* soap,
		struct _tptz__SetConfiguration *tptz__SetConfiguration,
		struct _tptz__SetConfigurationResponse *tptz__SetConfigurationResponse) {
	logInfo("__tptz__SetConfiguration");
	return getOnvifPTZSoapActionNotSupport(soap, "PTZ SetConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetConfigurationOptions(
		struct soap* soap,
		struct _tptz__GetConfigurationOptions *tptz__GetConfigurationOptions,
		struct _tptz__GetConfigurationOptionsResponse *tptz__GetConfigurationOptionsResponse) {
	logInfo("__tptz__GetConfigurationOptions");
	return getOnvifPTZSoapActionNotSupport(soap, "PTZ GetConfigurationOptions",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__GotoHomePosition(struct soap* soap,
		struct _tptz__GotoHomePosition *tptz__GotoHomePosition,
		struct _tptz__GotoHomePositionResponse *tptz__GotoHomePositionResponse) {
	logInfo("__tptz__GotoHomePosition");
	return getOnvifPTZSoapActionNotSupport(soap, "PTZ GotoHomePosition", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__SetHomePosition(struct soap* soap,
		struct _tptz__SetHomePosition *tptz__SetHomePosition,
		struct _tptz__SetHomePositionResponse *tptz__SetHomePositionResponse) {
	logInfo("__tptz__SetHomePosition");
	return getOnvifPTZSoapActionNotSupport(soap, "PTZ SetHomePosition", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__ContinuousMove(struct soap* soap,
		struct _tptz__ContinuousMove *tptz__ContinuousMove,
		struct _tptz__ContinuousMoveResponse *tptz__ContinuousMoveResponse) {
	logInfo("__tptz__ContinuousMove");
	return getOnvifPTZSoapActionNotSupport(soap, "PTZ ContinuousMove", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__RelativeMove(struct soap* soap,
		struct _tptz__RelativeMove *tptz__RelativeMove,
		struct _tptz__RelativeMoveResponse *tptz__RelativeMoveResponse) {
	logInfo("__tptz__RelativeMove");
	return getOnvifPTZSoapActionNotSupport(soap, "PTZ RelativeMove", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__SendAuxiliaryCommand(
		struct soap* soap,
		struct _tptz__SendAuxiliaryCommand *tptz__SendAuxiliaryCommand,
		struct _tptz__SendAuxiliaryCommandResponse *tptz__SendAuxiliaryCommandResponse) {
	logInfo("__tptz__SendAuxiliaryCommand");
	return getOnvifPTZSoapActionNotSupport(soap, "PTZ SendAuxiliaryCommand",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__AbsoluteMove(struct soap* soap,
		struct _tptz__AbsoluteMove *tptz__AbsoluteMove,
		struct _tptz__AbsoluteMoveResponse *tptz__AbsoluteMoveResponse) {
	logInfo("__tptz__AbsoluteMove");
	return getOnvifPTZSoapActionNotSupport(soap, "PTZ AbsoluteMove", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__Stop(struct soap* soap,
		struct _tptz__Stop *tptz__Stop,
		struct _tptz__StopResponse *tptz__StopResponse) {
	logInfo("__tptz__Stop");
	return getOnvifPTZSoapActionNotSupport(soap, "PTZ Stop", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetPresetTours(struct soap* soap,
		struct _tptz__GetPresetTours *tptz__GetPresetTours,
		struct _tptz__GetPresetToursResponse *tptz__GetPresetToursResponse) {
	logInfo("__tptz__GetPresetTours");
	return getOnvifPTZSoapActionNotSupport(soap, "PTZ GetPresetTours", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetPresetTour(struct soap* soap,
		struct _tptz__GetPresetTour *tptz__GetPresetTour,
		struct _tptz__GetPresetTourResponse *tptz__GetPresetTourResponse) {
	logInfo("__tptz__GetPresetTour");
	return getOnvifPTZSoapActionNotSupport(soap, "PTZ GetPresetTour", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetPresetTourOptions(
		struct soap* soap,
		struct _tptz__GetPresetTourOptions *tptz__GetPresetTourOptions,
		struct _tptz__GetPresetTourOptionsResponse *tptz__GetPresetTourOptionsResponse) {
	logInfo("__tptz__GetPresetTourOptions");
	return getOnvifPTZSoapActionNotSupport(soap, "PTZ GetPresetTourOptions",
			NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__CreatePresetTour(struct soap* soap,
		struct _tptz__CreatePresetTour *tptz__CreatePresetTour,
		struct _tptz__CreatePresetTourResponse *tptz__CreatePresetTourResponse) {
	logInfo("__tptz__CreatePresetTour");
	return getOnvifPTZSoapActionNotSupport(soap, "PTZ CreatePresetTour", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__ModifyPresetTour(struct soap* soap,
		struct _tptz__ModifyPresetTour *tptz__ModifyPresetTour,
		struct _tptz__ModifyPresetTourResponse *tptz__ModifyPresetTourResponse) {
	logInfo("__tptz__ModifyPresetTour");
	return getOnvifPTZSoapActionNotSupport(soap, "PTZ ModifyPresetTour", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__OperatePresetTour(
		struct soap* soap,
		struct _tptz__OperatePresetTour *tptz__OperatePresetTour,
		struct _tptz__OperatePresetTourResponse *tptz__OperatePresetTourResponse) {
	logInfo("__tptz__OperatePresetTour");
	return getOnvifPTZSoapActionNotSupport(soap, "PTZ OperatePresetTour", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__RemovePresetTour(struct soap* soap,
		struct _tptz__RemovePresetTour *tptz__RemovePresetTour,
		struct _tptz__RemovePresetTourResponse *tptz__RemovePresetTourResponse) {
	logInfo("__tptz__RemovePresetTour");
	return getOnvifPTZSoapActionNotSupport(soap, "PTZ RemovePresetTour", NULL);
}

