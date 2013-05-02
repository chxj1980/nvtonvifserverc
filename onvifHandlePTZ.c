#include "soapH.h"
#include "soapStub.h"
#include "onvifHandle.h"
#include "appTools.h"
#include "logInfo.h"

SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetServiceCapabilities(
		struct soap* soap,
		struct _tptz__GetServiceCapabilities *tptz__GetServiceCapabilities,
		struct _tptz__GetServiceCapabilitiesResponse *tptz__GetServiceCapabilitiesResponse) {
	logInfo("__tptz__GetServiceCapabilities");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetServiceCapabilities", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetConfigurations(
		struct soap* soap,
		struct _tptz__GetConfigurations *tptz__GetConfigurations,
		struct _tptz__GetConfigurationsResponse *tptz__GetConfigurationsResponse) {
	logInfo("__tptz__GetConfigurations");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetConfigurations", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetPresets(struct soap* soap,
		struct _tptz__GetPresets *tptz__GetPresets,
		struct _tptz__GetPresetsResponse *tptz__GetPresetsResponse) {
	logInfo("__tptz__GetPresets");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetPresets", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__SetPreset(struct soap* soap,
		struct _tptz__SetPreset *tptz__SetPreset,
		struct _tptz__SetPresetResponse *tptz__SetPresetResponse) {
	logInfo("__tptz__SetPreset");
	return getOnvifSoapActionNotSupportCode(soap,
			"SetPreset", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__RemovePreset(struct soap* soap,
		struct _tptz__RemovePreset *tptz__RemovePreset,
		struct _tptz__RemovePresetResponse *tptz__RemovePresetResponse) {
	logInfo("__tptz__RemovePreset");
	return getOnvifSoapActionNotSupportCode(soap,
			"RemovePreset", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__GotoPreset(struct soap* soap,
		struct _tptz__GotoPreset *tptz__GotoPreset,
		struct _tptz__GotoPresetResponse *tptz__GotoPresetResponse) {
	logInfo("__tptz__GotoPreset");
	return getOnvifSoapActionNotSupportCode(soap,
			"GotoPreset", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetStatus(struct soap* soap,
		struct _tptz__GetStatus *tptz__GetStatus,
		struct _tptz__GetStatusResponse *tptz__GetStatusResponse) {
	logInfo("__tptz__GetStatus");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetStatus", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetConfiguration(struct soap* soap,
		struct _tptz__GetConfiguration *tptz__GetConfiguration,
		struct _tptz__GetConfigurationResponse *tptz__GetConfigurationResponse) {
	logInfo("__tptz__GetConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetNodes(struct soap* soap,
		struct _tptz__GetNodes *tptz__GetNodes,
		struct _tptz__GetNodesResponse *tptz__GetNodesResponse) {
	logInfo("__tptz__GetNodes");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetNodes", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetNode(struct soap* soap,
		struct _tptz__GetNode *tptz__GetNode,
		struct _tptz__GetNodeResponse *tptz__GetNodeResponse) {
	logInfo("__tptz__GetNode");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetNode", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__SetConfiguration(struct soap* soap,
		struct _tptz__SetConfiguration *tptz__SetConfiguration,
		struct _tptz__SetConfigurationResponse *tptz__SetConfigurationResponse) {
	logInfo("__tptz__SetConfiguration");
	return getOnvifSoapActionNotSupportCode(soap,
			"SetConfiguration", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetConfigurationOptions(
		struct soap* soap,
		struct _tptz__GetConfigurationOptions *tptz__GetConfigurationOptions,
		struct _tptz__GetConfigurationOptionsResponse *tptz__GetConfigurationOptionsResponse) {
	logInfo("__tptz__GetConfigurationOptions");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetConfigurationOptions", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__GotoHomePosition(struct soap* soap,
		struct _tptz__GotoHomePosition *tptz__GotoHomePosition,
		struct _tptz__GotoHomePositionResponse *tptz__GotoHomePositionResponse) {
	logInfo("__tptz__GotoHomePosition");
	return getOnvifSoapActionNotSupportCode(soap,
			"GotoHomePosition", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__SetHomePosition(struct soap* soap,
		struct _tptz__SetHomePosition *tptz__SetHomePosition,
		struct _tptz__SetHomePositionResponse *tptz__SetHomePositionResponse) {
	logInfo("__tptz__SetHomePosition");
	return getOnvifSoapActionNotSupportCode(soap,
			"SetHomePosition", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__ContinuousMove(struct soap* soap,
		struct _tptz__ContinuousMove *tptz__ContinuousMove,
		struct _tptz__ContinuousMoveResponse *tptz__ContinuousMoveResponse) {
	logInfo("__tptz__ContinuousMove");
	return getOnvifSoapActionNotSupportCode(soap,
			"ContinuousMove", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__RelativeMove(struct soap* soap,
		struct _tptz__RelativeMove *tptz__RelativeMove,
		struct _tptz__RelativeMoveResponse *tptz__RelativeMoveResponse) {
	logInfo("__tptz__RelativeMove");
	return getOnvifSoapActionNotSupportCode(soap,
			"RelativeMove", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__SendAuxiliaryCommand(
		struct soap* soap,
		struct _tptz__SendAuxiliaryCommand *tptz__SendAuxiliaryCommand,
		struct _tptz__SendAuxiliaryCommandResponse *tptz__SendAuxiliaryCommandResponse) {
	logInfo("__tptz__SendAuxiliaryCommand");
	return getOnvifSoapActionNotSupportCode(soap,
			"SendAuxiliaryCommand", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__AbsoluteMove(struct soap* soap,
		struct _tptz__AbsoluteMove *tptz__AbsoluteMove,
		struct _tptz__AbsoluteMoveResponse *tptz__AbsoluteMoveResponse) {
	logInfo("__tptz__AbsoluteMove");
	return getOnvifSoapActionNotSupportCode(soap,
			"AbsoluteMove", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__Stop(struct soap* soap,
		struct _tptz__Stop *tptz__Stop,
		struct _tptz__StopResponse *tptz__StopResponse) {
	logInfo("__tptz__Stop");
	return getOnvifSoapActionNotSupportCode(soap,
			"Stop", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetPresetTours(struct soap* soap,
		struct _tptz__GetPresetTours *tptz__GetPresetTours,
		struct _tptz__GetPresetToursResponse *tptz__GetPresetToursResponse) {
	logInfo("__tptz__GetPresetTours");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetPresetTours", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetPresetTour(struct soap* soap,
		struct _tptz__GetPresetTour *tptz__GetPresetTour,
		struct _tptz__GetPresetTourResponse *tptz__GetPresetTourResponse) {
	logInfo("__tptz__GetPresetTour");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetPresetTour", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__GetPresetTourOptions(
		struct soap* soap,
		struct _tptz__GetPresetTourOptions *tptz__GetPresetTourOptions,
		struct _tptz__GetPresetTourOptionsResponse *tptz__GetPresetTourOptionsResponse) {
	logInfo("__tptz__GetPresetTourOptions");
	return getOnvifSoapActionNotSupportCode(soap,
			"GetPresetTourOptions", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__CreatePresetTour(struct soap* soap,
		struct _tptz__CreatePresetTour *tptz__CreatePresetTour,
		struct _tptz__CreatePresetTourResponse *tptz__CreatePresetTourResponse) {
	logInfo("__tptz__CreatePresetTour");
	return getOnvifSoapActionNotSupportCode(soap,
			"CreatePresetTour", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__ModifyPresetTour(struct soap* soap,
		struct _tptz__ModifyPresetTour *tptz__ModifyPresetTour,
		struct _tptz__ModifyPresetTourResponse *tptz__ModifyPresetTourResponse) {
	logInfo("__tptz__ModifyPresetTour");
	return getOnvifSoapActionNotSupportCode(soap,
			"ModifyPresetTour", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__OperatePresetTour(
		struct soap* soap,
		struct _tptz__OperatePresetTour *tptz__OperatePresetTour,
		struct _tptz__OperatePresetTourResponse *tptz__OperatePresetTourResponse) {
	logInfo("__tptz__OperatePresetTour");
	return getOnvifSoapActionNotSupportCode(soap,
			"OperatePresetTour", NULL);
}

SOAP_FMAC5 int SOAP_FMAC6 __tptz__RemovePresetTour(struct soap* soap,
		struct _tptz__RemovePresetTour *tptz__RemovePresetTour,
		struct _tptz__RemovePresetTourResponse *tptz__RemovePresetTourResponse) {
	logInfo("__tptz__RemovePresetTour");
	return getOnvifSoapActionNotSupportCode(soap,
			"RemovePresetTour", NULL);
}

