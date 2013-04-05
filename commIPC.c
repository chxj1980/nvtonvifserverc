#include "commIPC.h"
#include <interface.h>
#include <cmd_type.h>
#include <cmdpares.h>

#include "appTools.h"
#include "hashMap.h"
#include "onvifHandle.h"

IPCRunInfo ipcRunInfo;

int startIPCComm() {
	if (isValidHandle(ipcRunInfo.ipcConnectHandle))
		stopIPCComm();
	int cHandle = connect_local("/tmp/avserver.domain");
	if (!isValidHandle(cHandle)) {
		return RET_CODE_ERROR_CONNECT;
	}
	ipcRunInfo.ipcConnectHandle = cHandle;
	return RET_CODE_SUCCESS;
}

int sendIPCCmd(void* cmd, int len) {
	if (!isValidHandle(ipcRunInfo.ipcConnectHandle)) {
		return RET_CODE_ERROR_NOT_RUN;
	}
	if (-1 == send_local(ipcRunInfo.ipcConnectHandle, (char*) cmd, len)) {
		return RET_CODE_ERROR_SEND;
	}

	return RET_CODE_SUCCESS;
}

int sendAndRecvIPCCmd(void* incmd, int inlen, void* outInfo, int* outlen) {
	int result = sendIPCCmd(incmd, inlen);
	if (!isRetCodeSuccess(result)) {
		return result;
	}
	int size = recv_local(ipcRunInfo.ipcConnectHandle, (char*) outInfo, outlen);
	if (-1 == size) {
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
	char* v = arg;
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

void putStrValueInList(const hmap_t* inList, const int key, char* value) {
	IPCCmdInfo* cmdInfo = (IPCCmdInfo*) malloc(sizeof(IPCCmdInfo));
	cmdInfo->key = key;
	if (NULL != value)
		sprintf(cmdInfo->value, "%s", value);
	else
		sprintf(cmdInfo->value, "");
	char* skey = malloc(20);
	sprintf(skey, key);
	hashmap_put(inList, skey, cmdInfo);
}

void putIntValueInList(const hmap_t* inList, const int key, const int value) {
	char cValue[20];
	sprintf(cValue, "%d", value);
	putStrValueInList(inList, key, cValue);
}

void putNullValueInList(const hmap_t* inList, const int key) {
	putStrValueInList(inList, key, NULL);
}

void getSendListCmd(char* invalue, const int type, const hmap_t* inList) {
	sprintf(invalue, "$%d=%d", e_TYPE, type);
	hashmap_iterate(inList, getSendListIterate, invalue);
	strcat(invalue, "#");
}

void parseRecvListCmd(char* outValue, const hmap_t* outList) {
	char* pInput = outValue;
	char* pkey, pvalue;
	int parseIndex;
	int ikey;
	do {
		pkey = ParseVars(pInput, &parseIndex);
		pvalue = ParseVars(pInput, &parseIndex);
		if (pkey == NULL || pvalue == NULL)
			break;
		putStrValueInList(outList, atoi(pkey), pvalue);
	} while (parseIndex != -1);
}

int sendAndRetList(const int type, const hmap_t* inList, hmap_t* outList) {
	char invalue[1000] = { 0 };
	char outvalue[1000] = { 0 };
	int outlen = 1000;
	getSendListCmd(invalue, type, inList);
	int result = sendAndRecvIPCCmd(invalue, strlen(invalue), outvalue, outlen);
	if (RET_CODE_SUCCESS != result)
		return result;
	parseRecvListCmd(outvalue, outList);
	return result;
}

void destroyList(hmap_t* list) {
	hashmap_destroy(list, freeListIterate, 0);
}

int setNTPInfo(OnvifNTPInfo* info) {
	if (NULL == info) {
		return RET_CODE_ERROR_NULL_OBJECT;
	}

	char* address = info->address;
	if (NULL == address) {
		return RET_CODE_ERROR_NULL_OBJECT;
	}
	hmap_t* inList = hashmap_create();

	if (strlen(address) > 0) {
		putIntValueInList(inList, e_time_ntpenable, ENABLE_YES);
		putStrValueInList(inList, e_time_ntpserver, address);
	} else
		putIntValueInList(inList, e_time_ntpenable, ENABLE_NO);
	hmap_t* outList = hashmap_create();
	int result = sendAndRetList(T_Set, inList, outList);
	if (RET_CODE_SUCCESS == result) {

	}
	destroyList(inList);
	destroyList(outList);
	return result;
}

int getNTPInfo(OnvifNTPInfo* info) {
	if (NULL == info) {
			return RET_CODE_ERROR_NULL_OBJECT;
		}

		char* address = info->address;
		if (NULL == address) {
			return RET_CODE_ERROR_NULL_OBJECT;
		}
		hmap_t* inList = hashmap_create();
		putNullValueInList(inList, e_time_ntpenable);
		putNullValueInList(inList, e_time_ntpserver);
		hmap_t* outList = hashmap_create();
		int result = sendAndRetList(T_Set, inList, outList);
		if (RET_CODE_SUCCESS == result) {

		}
		destroyList(inList);
		destroyList(outList);
		return result;
}
