/*
 * commIPC.h
 *
 *  Created on: 2013-3-29
 *      Author: PC01
 */

#ifndef COMMIPC_H_
#define COMMIPC_H_

#include "appCommon.h"
#include "map.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct St_IPCRunInfo {
	int ipcConnectHandle;
}IPCRunInfo;

typedef struct ST_IPCCmdInfo{
	int key;
	char value[500];
}IPCCmdInfo;

extern IPCRunInfo ipcRunInfo;

int startIPCComm();
void stopIPCComm();
void putStrValueInList(const Map inList, const int key, const char* value);
void putIntValueInList(const Map inList, const int key, const int value);
void putFloatValueInList(const Map inList, const int key, const float value);
void putNullValueInList(const Map inList, const int key);
int getStrValueFromList(const Map outList, const int key, char* value);
int getIntValueFromList(const Map outList, const int key, int* value);
Map createIPCCmdInfoMapList();
void destroyIPCCmdInfoMapList(Map list);
int sendAndRetList(const int type, const Map inList, Map outList);

#ifdef __cplusplus
}
#endif

#endif /* COMMIPC_H_ */
