#include "appTools.h"
//#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/time.h>

#include "logInfo.h"

int getNetCardMac(unsigned char *pMac) {
	if (NULL == pMac)
		return RET_CODE_ERROR_NULL_VALUE;
	struct ifreq ifreq;
	int sockfd = 0;
	unsigned char mac[6] = { 0 };

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		errorInfo("netGetMac socket");
		return RET_CODE_ERROR_CREATE_SOCKET;
	}

	strcpy(ifreq.ifr_name, NET_CARD_NAME);

	if (ioctl(sockfd, SIOCGIFHWADDR, &ifreq) < 0) {
		errorInfo("netGetMac ioctl");
		close(sockfd);
		return RET_CODE_ERROR_SOCKETIOCTL;
	}

	memcpy(mac, ifreq.ifr_hwaddr.sa_data, 6);
	logInfo("MAC:%02x:%02x:%02x:%02x:%02x:%02x", mac[0], mac[1], mac[2], mac[3],
			mac[4], mac[5]);

	if (pMac != NULL) {
		memcpy(pMac, mac, 6);
	}

	close(sockfd);

	return RET_CODE_SUCCESS;
}

int getLocalIp(char* pIp) {
	if (NULL == pIp)
		return RET_CODE_ERROR_NULL_VALUE;
	int sock = 0;
	struct ifreq ifr;
	unsigned int localIp = 0;
	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, NET_CARD_NAME, IFNAMSIZ);
	sock = socket(AF_INET, SOCK_DGRAM, 0);

	if (sock <= 0) {
		return RET_CODE_ERROR_CREATE_SOCKET;
	}

	((struct sockaddr_in*) &ifr.ifr_addr)->sin_family = PF_INET;

	if (ioctl(sock, SIOCGIFADDR, &ifr) < 0) {
		errorInfo("get ip error: %s", strerror(errno));
		close(sock);
		return RET_CODE_ERROR_SOCKETIOCTL;
	} else {
		localIp = ((struct sockaddr_in*) &ifr.ifr_addr)->sin_addr.s_addr;
	}
	close(sock);
	strcpy(pIp, inet_ntoa(*((struct in_addr *) &localIp)));
	return RET_CODE_SUCCESS;
}

void stopSoap(struct soap* soap1) {
	soap_destroy(soap1);
	soap_end(soap1);
}

int getServiceURL(char* value, const char* ip, const int port) {
	if (strlen(ip) < 1)
		return RET_CODE_ERROR_INVALID_IP;
	sprintf(value, "http://%s:%d/", ip, port);
	return RET_CODE_SUCCESS;
}

void getAppointServiceURL(char* value, const char* serviceURL, const char* service) {
	sprintf(value, "%s%s/", serviceURL, service);
}

bool isValidHandle(int handle) {
	return (INVALID_HANDLE != handle);
}

bool isRetCodeSuccess(int retCode) {
	return RET_CODE_SUCCESS == retCode;
}

void getDateTimeStr(char* info, const int len, const time_t dtValue) {
	struct tm* today = localtime(&dtValue);
	strftime(info, len, "%F %T", today);
}

void getCurrentDateTimeStr(char* info, const int len) {
	getDateTimeStr(info, len, time(NULL));
}

void getCurrentDateTimeMSecStr(char* info, const int len) {
	struct timeval tv;
	struct tm tm;
	gettimeofday(&tv, NULL);
	getDateTimeStr(info, len, tv.tv_sec);
	int len1 = strlen(info);
	sprintf(info + len1, ".%06.6d", (int)(tv.tv_usec));
}

int parseTimeZoneTimeStr(const char* timeinfo, const short srcTimeZone, const short destTimeZone,
		time_t* value) {
	if (NULL == value) {
		return RET_CODE_ERROR_NULL_VALUE;
	}
	if (NULL == timeinfo) {
		return RET_CODE_ERROR_NULL_VALUE;
	}
	if (strlen(timeinfo) < 19) {
		return RET_CODE_ERROR_INVALID_VALUE;
	}
	struct timeb timeb1;
	ftime(&timeb1);
	int localZone = -timeb1.timezone / 60;
	char pt[5];
	memset(pt, 0, 5);
	struct tm tm1;
	strncpy(pt, timeinfo, 4);
	tm1.tm_year = atoi(pt) - 1900;
	memset(pt, 0, 5);
	strncpy(pt, timeinfo + 5, 2);
	tm1.tm_mon = atoi(pt) - 1;
	strncpy(pt, timeinfo + 8, 2);
	tm1.tm_mday = atoi(pt);
	strncpy(pt, timeinfo + 11, 2);
	tm1.tm_hour = atoi(pt);
	strncpy(pt, timeinfo + 14, 2);
	tm1.tm_min = atoi(pt);
	strncpy(pt, timeinfo + 17, 2);
	tm1.tm_sec = atoi(pt);
	tm1.tm_isdst=0;
	time_t ltx = mktime(&tm1);
	char dd[200];
	time_t srctx = (localZone - srcTimeZone) * 60 * 60 + ltx;
	getDateTimeStr(dd, 200, srctx);
	*value = (destTimeZone - localZone) * 60 * 60 + srctx;
	getDateTimeStr(dd, 200, *value);
	return RET_CODE_SUCCESS;
}
