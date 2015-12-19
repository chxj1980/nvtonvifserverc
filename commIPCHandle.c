#include <cmd_type.h>
#include "appTools.h"
#include "onvifHandle.h"
#include "commIPC.h"
#include "logInfo.h"

typedef int (*Push_CommIPC_Command_Func)(const Map, const void*);
typedef int (*Parse_CommIPC_Command_Func)(const Map, const void*);

void printIPCCmdInfoEach(MapNode node, void* arg) {
	logInfo("key %s value %s", node->key,
			((IPCCmdInfo*) (node->element))->value);
}

void logIPCCmdInfoList(Map map) {
	map->forEach(map, printIPCCmdInfoEach, NULL);
}

int sendCommIPCFunc(const int type, const void* info,
		const Push_CommIPC_Command_Func pushFunc,
		const Parse_CommIPC_Command_Func parseFunc) {
	if (NULL == pushFunc) {
		return RET_CODE_ERROR_NULL_OBJECT;
	}
	Map inList = createIPCCmdInfoMapList();
	int result = pushFunc(inList, info);
	if (!isRetCodeSuccess(result)) {
		destroyIPCCmdInfoMapList(inList);
		return result;
	}
	Map outList = createIPCCmdInfoMapList();
	result = sendAndRetList(type, inList, outList);
	if (!isRetCodeSuccess(result)) {
		destroyIPCCmdInfoMapList(inList);
		destroyIPCCmdInfoMapList(outList);
		return result;
	}
	if (NULL != parseFunc) {
		result = parseFunc(outList, info);
	}
	destroyIPCCmdInfoMapList(inList);
	destroyIPCCmdInfoMapList(outList);
	return result;
}

int setNTPInfo_PushCmd(const Map inList, const void* info1) {
	OnvifNTPInfo* info = (OnvifNTPInfo*) info1;
	char* address = info->address;
	if (strlen(address) < 1) {
		return RET_CODE_ERROR_INVALID_IP;
	}
	if (strlen(address) > 0) {
		putIntValueInList(inList, e_time_ntpenable, ENABLE_YES);
		putStrValueInList(inList, e_time_ntpserver, address);
	} else
		putIntValueInList(inList, e_time_ntpenable, ENABLE_NO);
	return RET_CODE_SUCCESS;
}

int setNTPInfo(OnvifNTPInfo* info) {
	return sendCommIPCFunc(T_Set, info, setNTPInfo_PushCmd, NULL);
}

int getNTPInfo_PushCmd(const Map inList, const void* info1) {
	putNullValueInList(inList, e_time_ntpenable);
	putNullValueInList(inList, e_time_ntpserver);
	return RET_CODE_SUCCESS;
}

int getNTPInfo_ParseCmd(const Map outList, const void* info1) {
	OnvifNTPInfo* info = (OnvifNTPInfo*) info1;
	int result = getStrValueFromList(outList, e_time_ntpserver, info->address);
	result = getIntValueFromList(outList, e_time_ntpenable, &info->enable);
	return result;
}

int getNTPInfo(OnvifNTPInfo* info) {
	return sendCommIPCFunc(T_Get, info, getNTPInfo_PushCmd, getNTPInfo_ParseCmd);
}

int getNetCardInfo_PushCmd(const Map inList, const void* info1) {
	putNullValueInList(inList, e_net_cardname);
	putNullValueInList(inList, e_net_macaddr);
	putNullValueInList(inList, e_net_ip);
	putNullValueInList(inList, e_net_netmask);
	putNullValueInList(inList, e_net_gateway);
	return RET_CODE_SUCCESS;
}

int getNetCardInfo_ParseCmd(const Map outList, const void* info1) {
	OnvifNetCardInfo* info = (OnvifNetCardInfo*) info1;
	memset(info, 0, sizeof(OnvifNetCardInfo));
	char macaddr[INFO_LENGTH] = { 0 };
	int result = getStrValueFromList(outList, e_net_macaddr, macaddr);
	if (strlen(macaddr) < 1) {
		return RET_CODE_ERROR_INVALID_VALUE;
	}
	info->size = 1;
	strcpy(info->netCardInfos[0].mac, macaddr);
	getStrValueFromList(outList, e_net_cardname, info->netCardInfos[0].name);
	getStrValueFromList(outList, e_net_ip, info->netCardInfos[0].ip);
	strcpy(onvifRunParam.ip, info->netCardInfos[0].ip);
	if (strlen(onvifRunParam.ip) < 1) {
		getLocalIp(onvifRunParam.ip);
	}
	getStrValueFromList(outList, e_net_netmask, info->netCardInfos[0].mask);
	getStrValueFromList(outList, e_net_gateway, info->netCardInfos[0].gateway);
	return result;
}

int getNetCardInfo(OnvifNetCardInfo* info) {
	return sendCommIPCFunc(T_Get, info, getNetCardInfo_PushCmd,
			getNetCardInfo_ParseCmd);
}

int getDeviceInfo_PushCmd(const Map inList, const void* info1) {
	putNullValueInList(inList, e_sys_hardwareId);
	putNullValueInList(inList, e_sys_manufacturer);
	putNullValueInList(inList, e_sys_Model);
	putNullValueInList(inList, e_sys_serialNumber);
	putNullValueInList(inList, e_sys_hdversion);
	putNullValueInList(inList, e_net_macaddr);
	return RET_CODE_SUCCESS;
}

int getDeviceInfo_ParseCmd(const Map outList, const void* info1) {
	OnvifDeviceInfo* info = (OnvifDeviceInfo*) info1;
	memset(info, 0, sizeof(OnvifDeviceInfo));
	getStrValueFromList(outList, e_sys_hardwareId, info->hardwareId);
	getStrValueFromList(outList, e_sys_manufacturer, info->manufacturer);
	getStrValueFromList(outList, e_sys_Model, info->model);
	getStrValueFromList(outList, e_sys_serialNumber, info->serialNumber);
	getStrValueFromList(outList, e_sys_hdversion, info->firmwareVersion);
	getStrValueFromList(outList, e_net_macaddr, info->macAddr);
	if (strlen(info->hardwareId) > 0) {
		strcpy(onvifRunParam.hardwareId, info->hardwareId);
	}
	if (strlen(info->macAddr) > 0) {
		squeezeChar(info->macAddr, '-');
		memset(onvifRunParam.urnHardwareId, 0, INFO_LENGTH);
		setRunParamUrnHardwareId(info->macAddr);
	}
	return RET_CODE_SUCCESS;
}

int getDeviceInfo(OnvifDeviceInfo* info) {
	return sendCommIPCFunc(T_Get, info, getDeviceInfo_PushCmd,
			getDeviceInfo_ParseCmd);
}

int deviceReboot_PushCmd(const Map inList, const void* info1) {
	putIntValueInList(inList, e_reboot, ENABLE_YES);
	return RET_CODE_SUCCESS;
}

int deviceReboot() {
	return sendCommIPCFunc(T_Set, NULL, deviceReboot_PushCmd, NULL);
}

int setSystemFactoryDefault_PushCmd(const Map inList, const void* info1) {
	putIntValueInList(inList, e_reset, ENABLE_YES);
	return RET_CODE_SUCCESS;
}

int setSystemFactoryDefault() {
	return sendCommIPCFunc(T_Set, NULL, setSystemFactoryDefault_PushCmd, NULL);
}

int getVideoCount_PushCmd(const Map inList, const void* info1) {
	putNullValueInList(inList, e_video_chn_num);
	return RET_CODE_SUCCESS;
}

int getVideoCount_ParseCmd(const Map outList, const void* info1) {
	return getIntValueFromList(outList, e_video_chn_num, (int*) info1);
}

int getVideoCount(int* count) {
	return sendCommIPCFunc(T_Get, count, getVideoCount_PushCmd,
			getVideoCount_ParseCmd);
}

int getAudioCount_PushCmd(const Map inList, const void* info1) {
	putNullValueInList(inList, e_video_chn_num);
	return RET_CODE_SUCCESS;
}

int getAudioCount_ParseCmd(const Map outList, const void* info1) {
	return getIntValueFromList(outList, e_video_chn_num, (int*) info1);
}

int getAudioCount(int* count) {
	return sendCommIPCFunc(T_Get, count, getAudioCount_PushCmd,
			getAudioCount_ParseCmd);
}

int getDeviceTime_PushCmd(const Map inList, const void* info1) {
	putNullValueInList(inList, e_time_ntpenable);
	putNullValueInList(inList, e_time_utc_systime);
	putNullValueInList(inList, e_time_Zone);
	return RET_CODE_SUCCESS;
}

int getDeviceTime_ParseCmd(const Map outList, const void* info1) {
	OnvifSystemDateTime* info = (OnvifSystemDateTime*) info1;
	int ntp;
	int result = getIntValueFromList(outList, e_time_ntpenable, &ntp);
	if (!isRetCodeSuccess(result)) {
		logInfo("getDeviceTime_ParseCmd get e_time_ntpenable failed");
		return result;
	}
	info->ntpSet = ntp;
	int zone = 8;
	result = getIntValueFromList(outList, e_time_Zone, &zone);
	if (!isRetCodeSuccess(result))
		return result;
	if (zone > 24) {
		zone = zone / ONE_HOUR_MIN;
	}
	info->timeZone = zone;
	char localTime1[INFO_LENGTH] = { 0 };
	result = getStrValueFromList(outList, e_time_utc_systime, localTime1);
	if (!isRetCodeSuccess(result)) {
		logInfo("getDeviceTime_ParseCmd get e_time_utc_systime failed");
		return result;
	}
	if (strlen(localTime1) < 1)
		return RET_CODE_ERROR_NULL_VALUE;
	time_t tim;
	result = parseTimeStr(localTime1, &tim);
	if (!isRetCodeSuccess(result)) {
		logInfo("getDeviceTime_ParseCmd parseTimeStr %s failed",
				localTime1);
		return result;
	}
	info->utcTime = tim;
	return result;
}

int getDeviceTime(OnvifSystemDateTime* info) {
	return sendCommIPCFunc(T_Get, info, getDeviceTime_PushCmd,
			getDeviceTime_ParseCmd);
}

int setDeviceTime_PushCmd(const Map inList, const void* info1) {
	OnvifSystemDateTime* info = (OnvifSystemDateTime*) info1;
	if (info->ntpSet) {
		putIntValueInList(inList, e_time_ntpenable, ENABLE_YES);
	} else {
		putIntValueInList(inList, e_time_ntpenable, ENABLE_NO);
		if (INVALID_TIME_ZONE != info->timeZone)
			putIntValueInList(inList, e_time_Zone, info->timeZone);
		char dt[INFO_LENGTH] = { 0 };
		getDateTimeStr(dt, INFO_LENGTH, info->utcTime);
		putStrValueInList(inList, e_time_utc_systime, dt);
	}
	return RET_CODE_SUCCESS;
}

int setDeviceTime(OnvifSystemDateTime* info) {
	if (NULL == info)
		return RET_CODE_ERROR_NULL_VALUE;
	return sendCommIPCFunc(T_Set, info, setDeviceTime_PushCmd, NULL);
}

int getVideoChannelInfo_PushCmd(const Map inList, const void* info1) {
	OnvifVideoChannelInfo* info = (OnvifVideoChannelInfo*) info1;
	if (NULL == info) {
		return RET_CODE_ERROR_NULL_OBJECT;
	}
	if (info->channelNo < 0) {
		return RET_CODE_ERROR_INVALID_VALUE;
	}
	putIntValueInList(inList, e_Chn, info->channelNo);
	putIntValueInList(inList, e_Sub_Chn, 0);
	putNullValueInList(inList, e_Stream_enable);
	putNullValueInList(inList, e_enc_type);
	putNullValueInList(inList, e_frame_rate);
	putNullValueInList(inList, e_bit_rate);
	putNullValueInList(inList, e_width);
	putNullValueInList(inList, e_height);
	putNullValueInList(inList, e_brightness);
	putNullValueInList(inList, e_saturation);
	putNullValueInList(inList, e_contrast);
	putNullValueInList(inList, e_sharpness);
	putNullValueInList(inList, e_wb_mode);
	putNullValueInList(inList, e_wb_cbgain);
	putNullValueInList(inList, e_wb_crgain);
	putNullValueInList(inList, e_backlightcomp_mode);
	putNullValueInList(inList, e_backlightcomp_level);
	putNullValueInList(inList, e_wdrange_mode);
	putNullValueInList(inList, e_wdrange_level);
	putNullValueInList(inList, e_ip_interval);
	putNullValueInList(inList, e_encode_profile);
	putNullValueInList(inList, e_init_quant);
	putNullValueInList(inList, e_port_rtspport);
	return RET_CODE_SUCCESS;
}

int getVideoChannelInfo_ParseCmd(const Map outList, const void* info1) {
	OnvifVideoChannelInfo* info = (OnvifVideoChannelInfo*) info1;
	memset(info, 0, sizeof(OnvifVideoChannelInfo));
	int value;
	int result = getIntValueFromList(outList, e_Chn, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->channelNo = value;
	result = getIntValueFromList(outList, e_Stream_enable, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->stream_enable = value;
	result = getIntValueFromList(outList, e_enc_type, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->enc_type = value;
	result = getIntValueFromList(outList, e_frame_rate, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->frame_rate = value;
	result = getIntValueFromList(outList, e_bit_rate, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->bit_rate = value;
	result = getIntValueFromList(outList, e_width, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->width = value;
	result = getIntValueFromList(outList, e_height, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->height = value;
	result = getIntValueFromList(outList, e_brightness, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->bright = value;
	result = getIntValueFromList(outList, e_saturation, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->saturation = value;
	result = getIntValueFromList(outList, e_contrast, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->contrast = value;
	result = getIntValueFromList(outList, e_sharpness, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->sharpness = value;
	result = getIntValueFromList(outList, e_wb_mode, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->wbMode = value;
	result = getIntValueFromList(outList, e_wb_cbgain, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->wbCbGain = value;
	result = getIntValueFromList(outList, e_wb_crgain, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->wbCrGain = value;
	result = getIntValueFromList(outList, e_backlightcomp_mode, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->backLightCompMode = value;
	result = getIntValueFromList(outList, e_backlightcomp_level, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->backLightCompLevel = value;
	result = getIntValueFromList(outList, e_wdrange_mode, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->wideDynamicMode = value;
	result = getIntValueFromList(outList, e_wdrange_level, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->wideDynamicLevel = value;
	result = getIntValueFromList(outList, e_ip_interval, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->govLength = value;
	result = getIntValueFromList(outList, e_encode_profile, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->videoEncodeProfile = value;
	result = getIntValueFromList(outList, e_init_quant, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->quality = value;
	result = getIntValueFromList(outList, e_port_rtspport, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->rtspPort = value;
	return result;
}

int getVideoChannelInfo(OnvifVideoChannelInfo* info) {
	return sendCommIPCFunc(T_Get, info, getVideoChannelInfo_PushCmd,
			getVideoChannelInfo_ParseCmd);
}

int getAudioChannelInfo_PushCmd(const Map inList, const void* info1) {
	OnvifAudioChannelInfo* info = (OnvifAudioChannelInfo*) info1;
	if (NULL == info) {
		return RET_CODE_ERROR_NULL_OBJECT;
	}
	if (info->channelNo < 0) {
		return RET_CODE_ERROR_INVALID_VALUE;
	}
	putIntValueInList(inList, e_Chn, info->channelNo);
	putNullValueInList(inList, e_audio_enable);
	putNullValueInList(inList, e_audio_enc_type);
	putNullValueInList(inList, e_audio_bitrate);
	putNullValueInList(inList, e_audio_samplesize);
	putNullValueInList(inList, e_audio_samplerate);
	putNullValueInList(inList, e_audio_rtspport);
	return RET_CODE_SUCCESS;
}

int getAudioChannelInfo_ParseCmd(const Map outList, const void* info1) {
	OnvifAudioChannelInfo* info = (OnvifAudioChannelInfo*) info1;
	memset(info, 0, sizeof(OnvifAudioChannelInfo));
	int value;
	int result = getIntValueFromList(outList, e_Chn, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->channelNo = value;
	result = getIntValueFromList(outList, e_audio_enable, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->stream_enable = value;
	if (FALSE == info->stream_enable)
		return result;
	result = getIntValueFromList(outList, e_audio_enc_type, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->enc_type = value;
	result = getIntValueFromList(outList, e_audio_bitrate, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->bit_rate = value;
	result = getIntValueFromList(outList, e_audio_samplesize, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->sample_size = value;
	result = getIntValueFromList(outList, e_audio_samplerate, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->sample_rate = value;
	result = getIntValueFromList(outList, e_audio_rtspport, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->rtspPort = value;
	return result;
}

int getAudioChannelInfo(OnvifAudioChannelInfo* info) {
	return sendCommIPCFunc(T_Get, info, getAudioChannelInfo_PushCmd,
			getAudioChannelInfo_ParseCmd);
}

int getVideoChannelStreamInfo_PushCmd(const Map inList, const void* info1) {
	OnvifVideoChannelInfo* info = (OnvifVideoChannelInfo*) info1;
	if (NULL == info) {
		return RET_CODE_ERROR_NULL_OBJECT;
	}
	if (info->channelNo < 0) {
		return RET_CODE_ERROR_INVALID_VALUE;
	}
	putIntValueInList(inList, e_Chn, info->channelNo);
	putIntValueInList(inList, e_Sub_Chn, onvifRunParam->subVideoChannelNo);
	putNullValueInList(inList, e_video_addr);
	return RET_CODE_SUCCESS;
}

int getVideoChannelStreamInfo_ParseCmd(const Map outList, const void* info1) {
	OnvifVideoChannelInfo* info = (OnvifVideoChannelInfo*) info1;
	memset(info, 0, sizeof(OnvifVideoChannelInfo));
	int value;
	int result = getIntValueFromList(outList, e_Chn, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->channelNo = value;
	result = getStrValueFromList(outList, e_video_addr, info->videoAddr);
	if (!isRetCodeSuccess(result))
		return result;
	return result;
}

int getVideoChannelStreamInfo(OnvifVideoChannelInfo* info) {
	return sendCommIPCFunc(T_Get, info, getVideoChannelStreamInfo_PushCmd,
			getVideoChannelStreamInfo_ParseCmd);
}

int getAudioChannelStreamInfo_PushCmd(const Map inList, const void* info1) {
	OnvifAudioChannelInfo* info = (OnvifAudioChannelInfo*) info1;
	if (NULL == info) {
		return RET_CODE_ERROR_NULL_OBJECT;
	}
	if (info->channelNo < 0) {
		return RET_CODE_ERROR_INVALID_VALUE;
	}
	putIntValueInList(inList, e_Chn, info->channelNo);
	putNullValueInList(inList, e_video_addr);
	return RET_CODE_SUCCESS;
}

int getAudioChannelStreamInfo_ParseCmd(const Map outList, const void* info1) {
	OnvifAudioChannelInfo* info = (OnvifAudioChannelInfo*) info1;
	memset(info, 0, sizeof(OnvifAudioChannelInfo));
	int value;
	int result = getIntValueFromList(outList, e_Chn, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->channelNo = value;
	result = getStrValueFromList(outList, e_video_addr, info->audioAddr);
	if (!isRetCodeSuccess(result))
		return result;
	return result;
}

int getAudioChannelStreamInfo(OnvifAudioChannelInfo* info) {
	return sendCommIPCFunc(T_Get, info, getAudioChannelStreamInfo_PushCmd,
			getAudioChannelStreamInfo_ParseCmd);
}

int setPTZStop_PushCmd(const Map inList, const void* info1) {
	OnvifPTZStopInfo* info = (OnvifPTZStopInfo*) info1;
	putIntValueInList(inList, e_ptz_stop_pt, info->stopPt);
	putIntValueInList(inList, e_ptz_stop_zoom, info->stopZoom);
	return RET_CODE_SUCCESS;
}

int setPTZStopInfo(OnvifPTZStopInfo* info) {
	if (NULL == info)
		return RET_CODE_ERROR_NULL_VALUE;
	return sendCommIPCFunc(T_Set, info, setPTZStop_PushCmd, NULL);
}

int setPTZContinousMove_PushCmd(const Map inList, const void* info1) {
	OnvifPTZContinousMoveInfo* info = (OnvifPTZContinousMoveInfo*) info1;
	if (info->setPt) {
		putFloatValueInList(inList, e_ptz_continue_move_vx, info->x);
		putFloatValueInList(inList, e_ptz_continue_move_vy, info->y);
	}
	if (info->setZoom) {
		putFloatValueInList(inList, e_ptz_continue_move_vzoom, info->zoom);
	}
	if (info->setTimeOut) {
		putIntValueInList(inList, e_ptz_continue_move_timeout, info->timeOut);
	}
	return RET_CODE_SUCCESS;
}

int setPTZContinousMoveInfo(OnvifPTZContinousMoveInfo* info) {
	if (NULL == info)
		return RET_CODE_ERROR_NULL_VALUE;
	return sendCommIPCFunc(T_Set, info, setPTZContinousMove_PushCmd, NULL);
}

int getPTZConfigurationInfo_PushCmd(const Map inList, const void* info1) {
	OnvifPTZConfigurationInfo* info = (OnvifPTZConfigurationInfo*) info1;
	if (NULL == info) {
		return RET_CODE_ERROR_NULL_OBJECT;
	}
	putNullValueInList(inList, e_ptz_continue_move_default_timeout);
	return RET_CODE_SUCCESS;
}

int getPTZConfigurationInfo_ParseCmd(const Map outList, const void* info1) {
	OnvifPTZConfigurationInfo* info = (OnvifPTZConfigurationInfo*) info1;
	memset(info, 0, sizeof(OnvifPTZConfigurationInfo));
	int value;
	int result = getIntValueFromList(outList, e_ptz_continue_move_default_timeout, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->defaultTimeout = value;
	return result;
}

int getPTZConfigurationInfo(OnvifPTZConfigurationInfo* info) {
	return sendCommIPCFunc(T_Get, info, getPTZConfigurationInfo_PushCmd,
			getPTZConfigurationInfo_ParseCmd);
}

int getPTZPresetsCapacity_PushCmd(const Map inList, const void* info1) {
	int* info = (int*) info1;
	if (NULL == info) {
		return RET_CODE_ERROR_NULL_OBJECT;
	}
	putNullValueInList(inList, e_ptz_presets_capacity);
	return RET_CODE_SUCCESS;
}

int getPTZPresetsCapacity_ParseCmd(const Map outList, const void* info1) {
	int* info = (int*) info1;
	memset(info, 0, sizeof(int));
	int value;
	int result = getIntValueFromList(outList, e_ptz_presets_capacity, &value);
	if (!isRetCodeSuccess(result))
		return result;
	*info = value;
	return result;
}

int getPTZPresetsCapacity(int* info) {
	return sendCommIPCFunc(T_Get, info, getPTZPresetsCapacity_PushCmd,
			getPTZPresetsCapacity_ParseCmd);
}

int getPTZAllPresets_PushCmd(const Map inList, const void* info1) {
	OnvifPTZAllPresets* info = (OnvifPTZAllPresets*) info1;
	if (NULL == info) {
		return RET_CODE_ERROR_NULL_OBJECT;
	}
	putNullValueInList(inList, e_ptz_allpresets);
	return RET_CODE_SUCCESS;
}

int getPTZAllPresets_ParseCmd(const Map outList, const void* info1) {
	OnvifPTZAllPresets* info = (OnvifPTZAllPresets*) info1;
	memset(info, 0, sizeof(OnvifPTZAllPresets));
	char value[2048] = {0};
	int result = getStrValueFromList(outList, e_ptz_allpresets, value);
	if (!isRetCodeSuccess(result))
		return result;
	int sz = 0;
	int iv = 0;
	OnvifPTZPreset* opreset;
	char* preset;
	PStrList list = newStrList();
	parseListByDiv(list, value, "/");
	int i;
	for(i = 0; i < list->size(list); i++) {
		preset = list->get(list, i);
		if (NULL == preset)
			continue;
		if (strlen(preset) < 1)
			continue;
		if (!isRetCodeSuccess(convertHexStrToDec(preset, &iv)))
			continue;
		opreset = &(info->presets[sz++]);
		opreset->index = iv;
		getPTZPreset(opreset);
	}
	info->size = sz;
	delStrList(list);
	return result;
}

int getPTZAllPresets(OnvifPTZAllPresets* info){
	return sendCommIPCFunc(T_Get, info, getPTZAllPresets_PushCmd,
			getPTZAllPresets_ParseCmd);
}

void getPTZErrorFromList(const Map outList, OnvifPTZPreset* info) {
	info->error = RESULT_OK;
	getIntValueFromList(outList, e_error, &(info->error));
}

int gotoPTZPreset_PushCmd(const Map inList, const void* info1) {
	OnvifPTZPreset* info = (OnvifPTZPreset*) info1;
	putIntValueInList(inList, e_ptz_goto_preset, info->index);
	return RET_CODE_SUCCESS;
}

int gotoPTZPreset_ParseCmd(const Map outList, const void* info1) {
	OnvifPTZPreset* info = (OnvifPTZPreset*) info1;
	int result = getIntValueFromList(outList, e_ptz_goto_preset, &(info->index));
	if (!isRetCodeSuccess(result))
		return result;
	getPTZErrorFromList(outList, info);
	return result;
}

int gotoPTZPreset(OnvifPTZPreset* info) {
	if (NULL == info)
		return RET_CODE_ERROR_NULL_VALUE;
	return sendCommIPCFunc(T_Set, info, gotoPTZPreset_PushCmd, gotoPTZPreset_ParseCmd);
}

int getPTZPreset_PushCmd(const Map inList, const void* info1) {
	OnvifPTZPreset* info = (OnvifPTZPreset*) info1;
	putIntValueInList(inList, e_ptz_preset, info->index);
	putNullValueInList(inList, e_ptz_presetname);
	return RET_CODE_SUCCESS;
}

int getPTZPreset_ParseCmd(const Map outList, const void* info1) {
	OnvifPTZPreset* info = (OnvifPTZPreset*) info1;
	int result = getIntValueFromList(outList, e_ptz_preset, &(info->index));
	if (!isRetCodeSuccess(result))
		return result;
	getPTZErrorFromList(outList, info);
	getStrValueFromList(outList, e_ptz_presetname, info->name);
	return result;
}

int getPTZPreset(OnvifPTZPreset* info) {
	if (NULL == info)
		return RET_CODE_ERROR_NULL_VALUE;
	return sendCommIPCFunc(T_Get, info, getPTZPreset_PushCmd, getPTZPreset_ParseCmd);
}

int removePTZPreset_PushCmd(const Map inList, const void* info1) {
	OnvifPTZPreset* info = (OnvifPTZPreset*) info1;
	putIntValueInList(inList, e_ptz_deletepreset, info->index);
	return RET_CODE_SUCCESS;
}

int removePTZPreset_ParseCmd(const Map outList, const void* info1) {
	OnvifPTZPreset* info = (OnvifPTZPreset*) info1;
	int result = getIntValueFromList(outList, e_ptz_deletepreset, &(info->index));
	if (!isRetCodeSuccess(result)) {
		return result;
	}
	getPTZErrorFromList(outList, info);
	return result;
}

int removePTZPreset(OnvifPTZPreset* info) {
	if (NULL == info)
		return RET_CODE_ERROR_NULL_VALUE;
	return sendCommIPCFunc(T_Set, info, removePTZPreset_PushCmd, removePTZPreset_ParseCmd);
}

int setPTZPreset_PushCmd(const Map inList, const void* info1) {
	OnvifPTZPreset* info = (OnvifPTZPreset*) info1;
	putIntValueInList(inList, e_ptz_preset, info->index);
	putStrValueInList(inList, e_ptz_presetname, info->name);
	return RET_CODE_SUCCESS;
}

int setPTZPreset_ParseCmd(const Map outList, const void* info1) {
	OnvifPTZPreset* info = (OnvifPTZPreset*) info1;
	int result = getIntValueFromList(outList, e_ptz_preset, &(info->index));
	if (!isRetCodeSuccess(result))
		return result;
	getPTZErrorFromList(outList, info);
	getStrValueFromList(outList, e_ptz_presetname, info->name);
	return RET_CODE_SUCCESS;
}

int setPTZPreset(OnvifPTZPreset* info) {
	if (NULL == info)
		return RET_CODE_ERROR_NULL_VALUE;
	return sendCommIPCFunc(T_Set, info, setPTZPreset_PushCmd, setPTZPreset_ParseCmd);
}

int setVideoSynchronizationPoint_PushCmd(const Map inList, const void* info1) {
	int* info = (int*) info1;
	putIntValueInList(inList, e_video_SynchronizationPoint, *info);
	return RET_CODE_SUCCESS;
}

int setVideoSynchronizationPoint(int index) {
	return sendCommIPCFunc(T_Set, &index, setVideoSynchronizationPoint_PushCmd, NULL);
}

int getVideoEncoderConfigurationOptionInfo_PushCmd(const Map inList, const void* info1) {
	OnvifVideoEncoderConfigurationOptionInfo* info = (OnvifVideoEncoderConfigurationOptionInfo*) info1;
	if (NULL == info) {
		return RET_CODE_ERROR_NULL_OBJECT;
	}
	if (info->channelNo < 0) {
		return RET_CODE_ERROR_INVALID_VALUE;
	}
	putIntValueInList(inList, e_Chn, info->channelNo);
	putNullValueInList(inList, e_min_quant);
	putNullValueInList(inList, e_max_quant);
	putNullValueInList(inList, e_videoResolutions);
	putNullValueInList(inList, e_min_ip_interval);
	putNullValueInList(inList, e_max_ip_interval);
	putNullValueInList(inList, e_min_frame_rate);
	putNullValueInList(inList, e_max_frame_rate);
	putNullValueInList(inList, e_min_videoEncodingInterval);
	putNullValueInList(inList, e_max_videoEncodingInterval);
	putNullValueInList(inList, e_profile_levels);
	return RET_CODE_SUCCESS;
}

int getVideoEncoderConfigurationOptionInfo_ParseCmd(const Map outList, const void* info1) {
	OnvifVideoEncoderConfigurationOptionInfo* info = (OnvifVideoEncoderConfigurationOptionInfo*) info1;
	memset(info, 0, sizeof(OnvifVideoEncoderConfigurationOptionInfo));
	int value;
	int result = getIntValueFromList(outList, e_Chn, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->channelNo = value;
	result = getIntValueFromList(outList, e_min_quant, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->quality.min = value;
	result = getIntValueFromList(outList, e_max_quant, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->quality.max = value;
	result = getIntValueFromList(outList, e_min_ip_interval, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->govLength.min = value;
	result = getIntValueFromList(outList, e_max_ip_interval, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->govLength.max = value;
	result = getIntValueFromList(outList, e_min_frame_rate, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->frameRate.min = value;
	result = getIntValueFromList(outList, e_max_frame_rate, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->frameRate.max = value;
	result = getIntValueFromList(outList, e_min_videoEncodingInterval, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->encodingInterval.min = value;
	result = getIntValueFromList(outList, e_max_videoEncodingInterval, &value);
	if (!isRetCodeSuccess(result))
		return result;
	info->encodingInterval.max = value;
	result = getIntValueFromList(outList, e_profile_levels, &value);
	if (!isRetCodeSuccess(result))
		return result;
	int pCount = 0;
	if (value & 0x01) {
		info->profiles[pCount++] = H264_Baseline;
	}
	if (value & 0x02) {
		info->profiles[pCount++] = H264_Main;
	}
	if (value & 0x04) {
		info->profiles[pCount++] = H264_High;
	}
	info->profileCount = pCount;
	char profilesv[INFO_LENGTH] = {0};
	getStrValueFromList(outList, e_videoResolutions, profilesv);
	if (strlen(profilesv) < 1) {
		return RET_CODE_ERROR_INVALID_VALUE;
	}
	PStrList list = newStrList();
	PStrList list1 = newStrList();
	parseListByDiv(list, profilesv, "/");
	info->resolutionCount = list->size(list);
	int i;
	char* pData;
	for(i = 0; i < info->resolutionCount; i++) {
		list1->clearAll(list1);
		pData = list->get(list, i);
		parseListByDiv(list1, pData, ",");
		if (list1->size(list1) < 2)
			continue;
		info->resolutions[i].width = atoi(list1->get(list1, 0));
		info->resolutions[i].height = atoi(list1->get(list1, 1));
	}
	delStrList(list);
	delStrList(list1);
	return result;
}

int getVideoEncoderConfigurationOptionInfo(OnvifVideoEncoderConfigurationOptionInfo* info) {
	if (NULL == info)
		return RET_CODE_ERROR_NULL_VALUE;
	return sendCommIPCFunc(T_Get, info, getVideoEncoderConfigurationOptionInfo_PushCmd, getVideoEncoderConfigurationOptionInfo_ParseCmd);
}
