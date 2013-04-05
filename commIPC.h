/*
 * commIPC.h
 *
 *  Created on: 2013-3-29
 *      Author: PC01
 */

#ifndef COMMIPC_H_
#define COMMIPC_H_

#include "appCommon.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct St_IPCRunInfo {
	int ipcConnectHandle;
}IPCRunInfo;

typedef struct ST_IPCCmdInfo{
	int key;
	char value[100];
}IPCCmdInfo;

typedef struct ST_IPCCmdInfoList {
	IPCCmdInfo* data;
	struct ST_IPCCmdInfoList* nextNode;
}IPCCmdInfoList;

IPCCmdInfoList* createCmdInfoList();
void destroyCmdInfoList(IPCCmdInfoList* list);
int getCmdInfoListStr(const IPCCmdInfoList* list);
int parseCmdInfoListStr(char* info, const IPCCmdInfoList* list);


extern IPCRunInfo ipcRunInfo;

int startIPCComm();
void stopIPCComm();
int getDeviceInformation();

#ifdef __cplusplus
}
#endif
#endif /* COMMIPC_H_ */
