#include "commIPC.h"
#include <interface.h>
#include <cmdpares.h>
#include <cmd_type.h>

#include "appTools.h"
#include "onvifHandle.h"

IPCRunInfo ipcRunInfo = {.ipcConnectHandle = INVALID_HANDLE};

int startIPCComm() {
	if (isValidHandle(ipcRunInfo.ipcConnectHandle))
		stopIPCComm();
	int cHandle = connect_local("/tmp/avserver.domain");
	if (!isValidHandle(cHandle)) {
		logInfo("connect ipc failed");
		return RET_CODE_ERROR_CONNECT;
	}
	ipcRunInfo.ipcConnectHandle = cHandle;
	return RET_CODE_SUCCESS;
}

int sendIPCCmd(const void_ptr cmd, const int len) {
	if (!isValidHandle(ipcRunInfo.ipcConnectHandle)) {
		return RET_CODE_ERROR_NOT_RUN;
	}
	if (-1 == send_local(ipcRunInfo.ipcConnectHandle, (char*) cmd, len)) {
		return RET_CODE_ERROR_SEND;
	}

	return RET_CODE_SUCCESS;
}

int sendAndRecvIPCCmd(const void_ptr incmd, const int inlen, void_ptr outInfo,
		int* outlen) {
	int result = sendIPCCmd(incmd, inlen);
	if (!isRetCodeSuccess(result)) {
		return result;
	}
	int size = recv_local(ipcRunInfo.ipcConnectHandle, (char*) outInfo,
			*outlen);
	logInfo("sendAndRecvIPCCmd recv_local %d", size);
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

int getSendListIterate(void_ptr data, void_ptr arg) {
	char* v = (char*)arg;
	IPCCmdInfo* cmdInfo = data;
	char invalue[120] = { 0 };
	sprintf(invalue, "&%d=%s", cmdInfo->key, cmdInfo->value);
	strcat(v, invalue);
	return HMAP_S_OK;
}

int freeListIterate(void_ptr data, void_ptr arg) {
	IPCCmdInfo* cmdInfo = data;
	free(cmdInfo);
	return HMAP_S_OK;
}

void putStrValueInList(const hmap_t inList, const int key, const char* value) {
	IPCCmdInfo* cmdInfo = (IPCCmdInfo*) malloc(sizeof(IPCCmdInfo));
	cmdInfo->key = key;
	if (NULL != value)
		sprintf(cmdInfo->value, "%s", value);
	else
		strcpy(cmdInfo->value, "");
	char* skey = malloc(20);
	sprintf(skey, "%d", key);
	hashmap_put(inList, skey, cmdInfo);
}

void putIntValueInList(const hmap_t inList, const int key, const int value) {
	char cValue[20];
	sprintf(cValue, "%d", value);
	putStrValueInList(inList, key, cValue);
}

void putNullValueInList(const hmap_t inList, const int key) {
	putStrValueInList(inList, key, NULL);
}

int getStrValueFromList(const hmap_t list, const int key, char* value) {
	int result = RET_CODE_ERROR_NULL_VALUE;
	char cKey[20];
	sprintf(cKey, "%d", key);
	void_ptr* v1 = NULL;
	if (HMAP_S_OK == hashmap_get(list, cKey, &v1)) {
		if (NULL != v1) {
			strcpy(value, (char*) v1);
		}
		result = RET_CODE_SUCCESS;
	}
	return result;
}

int getIntValueFromList(const hmap_t outList, const int key, int* value) {
	char cKey[20];
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

void getSendListCmd(char* invalue, const int type, const hmap_t inList) {
	sprintf(invalue, "$%d=%d", e_TYPE, type);
	hashmap_iterate(inList, getSendListIterate, invalue);
	strcat(invalue, "#");
}

void parseRecvListCmd(const char* outValue, const hmap_t outList) {
	char* pInput = outValue;
	char* pkey;
	char* pvalue;
	int parseIndex = 0;
	do {
		pkey = ParseVars(pInput, &parseIndex);
		logInfo("parseRecvListCmd parsekey %s", pkey);
		pvalue = ParseVars(pInput, &parseIndex);
		logInfo("parseRecvListCmd parse value %s", pvalue);
		if (pkey == NULL || pvalue == NULL) {
			break;
		}
		putStrValueInList(outList, atoi(pkey), pvalue);
	} while (parseIndex != -1);
}

int sendAndRetList(const int type, const hmap_t inList, hmap_t outList) {
	char invalue[1000] = { 0 };
	char outvalue[1000] = { 0 };
	int outlen = 1000;
	memset(outvalue, 0, 1000);
	getSendListCmd(invalue, type, inList);
	int result = sendAndRecvIPCCmd(invalue, strlen(invalue), outvalue, &outlen);
	if (!isRetCodeSuccess(result))
		return result;
	if (outlen > 0) {
		logInfo("sendAndRetList %s", outvalue);
		parseRecvListCmd(outvalue, outList);
	}
	return result;
}

void destroyHashMapList(hmap_t list) {
	hashmap_destroy(list, freeListIterate, 0);
}

hmap_t createHashMapList() {
	return hashmap_create();
}

