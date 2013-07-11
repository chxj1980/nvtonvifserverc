#include "commIPC.h"
#include <interface.h>
#include <cmdpares.h>
#include <cmd_type.h>

#include "appTools.h"
#include "onvifHandle.h"
#include "logInfo.h"

IPCRunInfo ipcRunInfo = {.ipcConnectHandle = INVALID_HANDLE};

int startIPCComm() {
	if (isValidHandle(ipcRunInfo.ipcConnectHandle))
		stopIPCComm();
	int cHandle = INVALID_HANDLE;
	int i;
	for(i = 0; i < 3; i++) {
		cHandle = connect_local("/tmp/avserver.domain");
		if (isValidHandle(cHandle)) {
			logInfo("connect ipc success");
			break;
		}
		sleep(5);
	}
	if (!isValidHandle(cHandle)) {
		return RET_CODE_ERROR_CONNECT;
	}
	ipcRunInfo.ipcConnectHandle = cHandle;
	return RET_CODE_SUCCESS;
}

int sendIPCCmd(const void* cmd, const int len) {
	if (!isValidHandle(ipcRunInfo.ipcConnectHandle)) {
		return RET_CODE_ERROR_NOT_RUN;
	}
	if (-1 == send_local(ipcRunInfo.ipcConnectHandle, (char*) cmd, len)) {
		return RET_CODE_ERROR_SEND;
	}
	return RET_CODE_SUCCESS;
}

int sendAndRecvIPCCmd(const void* incmd, const int inlen, void* outInfo,
		int* outlen) {
	int result = sendIPCCmd(incmd, inlen);
	if (!isRetCodeSuccess(result)) {
		return result;
	}
	int size = recv_local(ipcRunInfo.ipcConnectHandle, (char*) outInfo,
			*outlen);
	if (size < 0) {
		return RET_CODE_ERROR_RECV;
	}
	*outlen = size;
	return RET_CODE_SUCCESS;
}

void stopIPCComm() {
	if (!isValidHandle(ipcRunInfo.ipcConnectHandle)) {
		return;
	}
	close_local(ipcRunInfo.ipcConnectHandle);
	ipcRunInfo.ipcConnectHandle = INVALID_HANDLE;
}

void putStrValueInList(const Map inList, const int key, const char* value) {
	IPCCmdInfo* cmdInfo = (IPCCmdInfo*) malloc(sizeof(IPCCmdInfo));
	cmdInfo->key = key;
	if (NULL != value)
		sprintf(cmdInfo->value, "%s", value);
	else
		strcpy(cmdInfo->value, "");
	char skey[SMALL_INFO_LENGTH];
	sprintf(skey, "%d", key);
	inList->put(inList, skey, (MapElement)cmdInfo);
}

void putIntValueInList(const Map inList, const int key, const int value) {
	char cValue[20];
	sprintf(cValue, "%d", value);
	putStrValueInList(inList, key, cValue);
}

void putFloatValueInList(const Map inList, const int key, const float value) {
	char cValue[20];
	sprintf(cValue, "%f", value);
	putStrValueInList(inList, key, cValue);
}

void putNullValueInList(const Map inList, const int key) {
	putStrValueInList(inList, key, NULL);
}

int getStrValueFromList(const Map list, const int key, char* value) {
	int result = RET_CODE_ERROR_NULL_VALUE;
	char cKey[SMALL_INFO_LENGTH];
	sprintf(cKey, "%d", key);
	MapNode mapNode = list->get(list, cKey);
	if(NULL == mapNode) {
		return result;
	}
	if (NULL == mapNode->element)
		return result;
	IPCCmdInfo* info1 = (IPCCmdInfo*)mapNode->element;
	strcpy(value, info1->value);
	result = RET_CODE_SUCCESS;
	return result;
}

int getIntValueFromList(const Map outList, const int key, int* value) {
	char cKey[SMALL_INFO_LENGTH];
	sprintf(cKey, "%d", key);
	char cValue[520] = { 0 };
	int result = getStrValueFromList(outList, key, cValue);
	if (isRetCodeSuccess(result)) {
		if (strlen(cValue) > 0) {
			*value = atoi(cValue);
		}
		else
			result = RET_CODE_ERROR_NULL_VALUE;
	}
	return result;
}

void getSendListForEach(MapNode node, void* arg) {
	char* v = (char*)arg;
	if (NULL == node)
		return;
	if (NULL == node->element) {
		return;
	}
	IPCCmdInfo* cmdInfo = (IPCCmdInfo*)node->element;
	char invalue[120] = { 0 };
	sprintf(invalue, "&%d=%s", cmdInfo->key, cmdInfo->value);
	strcat(v, invalue);
}

void getSendListCmd(char* invalue, const int type, const Map inList) {
	sprintf(invalue, "$%d=%d", e_TYPE, type);
	inList->forEach(inList, getSendListForEach, invalue);
	strcat(invalue, "#");
}

void parseRecvListCmd(const char* outValue, const Map outList) {
	char* pInput = (char*)outValue;
	char* pkey;
	char* pvalue;
	int parseIndex = 0;
	do {
		pkey = ParseVars(pInput, &parseIndex);
		pvalue = ParseVars(pInput, &parseIndex);
		if (NULL == pkey || NULL == pvalue) {
			break;
		}
		putStrValueInList(outList, atoi(pkey), pvalue);
	} while (parseIndex != -1);
}

int sendAndRetList(const int type, const Map inList, Map outList) {
	char invalue[1000] = { 0 };
	char outvalue[1000] = { 0 };
	int outlen = 1000;
	memset(outvalue, 0, 1000);
	getSendListCmd(invalue, type, inList);
	int result = sendAndRecvIPCCmd(invalue, strlen(invalue), outvalue, &outlen);
	if (!isRetCodeSuccess(result))
		return result;
	if (outlen > 0) {
		parseRecvListCmd(outvalue, outList);
	}
	return result;
}

void destroyIPCCmdInfoMapList(Map list) {
	delMap(list);
}

void freeIPCCmdInfo(MapElement data) {
	IPCCmdInfo* cmdInfo = data;
	free(cmdInfo);
}

Map createIPCCmdInfoMapList() {
	return newMap(freeIPCCmdInfo);
}

