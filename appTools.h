#ifndef APPTOOLS_H_
#define APPTOOLS_H_

#include <stdio.h>
#include <sys/time.h>
#include "appCommon.h"

#ifdef __cplusplus
	extern "C" {
#endif

	int getLocalIp(char* pIp);
	int getNetCardMac(unsigned char *pMac);
	void stopSoap(struct soap* soap1);
	int getServiceURL(char* value, const int port);
	bool isValidHandle(int handle);
	bool isRetCodeSuccess(int retCode);
	void getDateTimeStr(char* info, const int len, const time_t dtValue);
	void getCurrentDateTimeStr(char* info, const int len);
#ifdef __cplusplus
	}
#endif

#endif /* APPTOOLS_H_ */
