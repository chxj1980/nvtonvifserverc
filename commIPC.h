/*
 * commIPC.h
 *
 *  Created on: 2013-3-29
 *      Author: PC01
 */

#ifndef COMMIPC_H_
#define COMMIPC_H_

#include "appCommon.h"
#include "hashMap.h"

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

extern IPCRunInfo ipcRunInfo;

int startIPCComm();
void stopIPCComm();
void putStrValueInList(const hmap_t inList, const int key, const char* value);
void putIntValueInList(const hmap_t inList, const int key, const int value);
void putNullValueInList(const hmap_t inList, const int key);
void getStrValueFromList(const hmap_t outList, const int key, char* value);
void getIntValueFromList(const hmap_t outList, const int key, int* value);
hmap_t createHashMapList();
void destroyHashMapList(hmap_t list);
int sendAndRetList(const int type, const hmap_t inList, hmap_t outList);

#ifdef __cplusplus
}
#endif

#endif /* COMMIPC_H_ */
