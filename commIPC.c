#include "commIPC.h"
#include <interface.h>
#include "appTools.h"

IPCRunInfo ipcRunInfo;

int startIPCComm() {
	if (isValidHandle(ipcRunInfo.ipcConnectHandle))
		stopIPCComm();
	int cHandle = connect_local("localhost");
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
	if (-1 == send_local(ipcRunInfo.ipcConnectHandle, (char*)cmd, len)) {
		return RET_CODE_ERROR_SEND;
	}

	return RET_CODE_SUCCESS;
}

int sendAndRecvIPCCmd(void* incmd, int inlen, void* outInfo, int* outlen) {
	int result = sendIPCCmd(incmd, inlen);
	if (!isRetCodeSuccess(result)) {
		return result;
	}
	int size = recv_local(ipcRunInfo.ipcConnectHandle, (char*)outInfo, outlen);
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
