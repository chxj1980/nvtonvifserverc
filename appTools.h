#ifndef APPTOOLS_H_
#define APPTOOLS_H_

#include <stdio.h>

#define __DEBUG
#ifdef __DEBUG
#define DBG(fmt,args...) fprintf(stdout,  fmt,  ##args)
#else
#define DBG(fmt,args...)
#endif
#define ERRLOG(fmt,args...) fprintf(stderr,  fmt,  ##args)

#define LOG(fmt,args...) fprintf(stdout,  fmt,  ##args)

#ifdef __cplusplus
	extern "C" {
#endif

	int getLocalIp(char* pIp);
	int getNetCardMac(unsigned char *pMac);
#ifdef __cplusplus
	}
#endif

#endif /* APPTOOLS_H_ */
