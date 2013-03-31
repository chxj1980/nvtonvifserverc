#ifndef APPTOOLS_H_
#define APPTOOLS_H_

#include <stdio.h>
#include "appCommon.h"

#ifdef __cplusplus
	extern "C" {
#endif

	int getLocalIp(char* pIp);
	int getNetCardMac(unsigned char *pMac);
	void stopSoap(struct soap* soap1);
	int getServiceURL(char* value);
	bool isValidHandle(int handle);
	bool isRetCodeSuccess(int retCode);

#ifdef __cplusplus
	}
#endif

#endif /* APPTOOLS_H_ */
