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

extern IPCRunInfo ipcRunInfo;

int startIPCComm();
void stopIPCComm();


#ifdef __cplusplus
}
#endif
#endif /* COMMIPC_H_ */
