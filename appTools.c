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
#include <math.h>

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
	sprintf(value, "http://%s:%d/onvif/device_service", ip, port);
	return RET_CODE_SUCCESS;
}

void getAppointServiceURL(char* value, const char* serviceURL,
		const char* service) {
	sprintf(value, "%s/%s", serviceURL, service);
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

void myThreadSleep() {
	usleep(10000);
}

void squeezeChar(char s[], char c) {
	int i, j;
	for (i = 0, j = 0; s[i] != '\0'; i++) {
		if (s[i] != c) {
			s[j++] = s[i];
		}
	}
	s[j] = '\0';
}

void getCurrentDateTimeStr(char* info, const int len) {
	getDateTimeStr(info, len, time(NULL));
}

void getCurrentDateTimeMSecStr(char* info, const int len) {
	struct timeval tv;
	gettimeofday(&tv, NULL);
	getDateTimeStr(info, len, tv.tv_sec);
	int len1 = strlen(info);
	sprintf(info + len1, ".%6.6d", (int) (tv.tv_usec));
}

int parseTimeZoneTimeStr(const char* timeinfo, const short srcTimeZone,
		const short destTimeZone, time_t* value) {
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
	int localZone = -timeb1.timezone / ONE_HOUR_MIN;
	char pt[5];
	memset(pt, 0, 5);
	struct tm tm1;
	strncpy(pt, timeinfo, 4);
	tm1.tm_year = atoi(pt) - START_TIME_YEAR;
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
	tm1.tm_isdst = 0;
	time_t ltx = mktime(&tm1);
	char dd[200];
	time_t srctx = (localZone - srcTimeZone) * ONE_HOUR_SEC + ltx;
	getDateTimeStr(dd, 200, srctx);
	*value = (destTimeZone - localZone) * ONE_HOUR_SEC + srctx;
	getDateTimeStr(dd, 200, *value);
	return RET_CODE_SUCCESS;
}

int parseTimeStr(const char* timeInfo, time_t* value) {
	if (NULL == value) {
		return RET_CODE_ERROR_NULL_VALUE;
	}
	if (NULL == timeInfo) {
		return RET_CODE_ERROR_NULL_VALUE;
	}
	if (strlen(timeInfo) < 19) {
		return RET_CODE_ERROR_INVALID_VALUE;
	}
	struct tm p;
	sscanf(timeInfo, "%d-%d-%d %d:%d:%d", &(p.tm_year), &(p.tm_mon),
			&(p.tm_mday), &(p.tm_hour), &(p.tm_min), &(p.tm_sec));
	p.tm_year -= START_TIME_YEAR;
	*value = mktime(&p);
	return RET_CODE_SUCCESS;
}

char *reverseStr(char *str, int len) {

	char *start = str;
	char *end = str + len - 1;
	char ch;

	if (str != NULL)
	{
		while (start < end) {
			ch = *start;
			*start++ = *end;
			*end-- = ch;
		}
	}
	return str;
}

int convertBCDToDec(const unsigned char *bcd, int length) {
	int tmp;
	int dec = 0;
	int i;
	for (i = 0; i < length; i++) {
		tmp = ((bcd[i] >> 4) & 0x0F) * 10 + (bcd[i] & 0x0F);
		dec += tmp * pow(100, length - 1 - i);
	}
	return dec;
}

int convertDecToBCD(int value, unsigned char *bcd) {
	int tmp;
	int pos = 0;
	if (0 == value) {
		bcd[0] = 0x0;
		return 1;
	}
	while (value != 0) {
		tmp = value % 100;
		bcd[pos++] = ((tmp / 10) << 4) + ((tmp % 10) & 0x0F);
		pos /= 100;
	}
	reverseStr((char*)bcd, pos);
	return pos;
}

void convertDecToHexStr(const int src, char* dest) {
	sprintf(dest, "%x", src);
}

int convertHexStrToDec(const char* src, int* dest) {
	if (NULL == src) {
		return RET_CODE_ERROR_NULL_VALUE;
	}
	if (NULL == dest) {
		return RET_CODE_ERROR_NULL_VALUE;
	}
	if (strlen(src) < 1)
		return RET_CODE_ERROR_NULL_VALUE;
	int value;
	sscanf(src, "%x", &value);
	*dest = value;
	return RET_CODE_SUCCESS;
}

void parseListByDiv(const PStrList list, const char* value, const char* div) {
	if (NULL == list)
		return;
	char *preset = NULL;
	preset = strtok(value, (char*)div);
	while (preset != NULL) {
		list->addLast(list, preset);
		preset = strtok(NULL, (char*)div);
	}
}

bool charIsPositive(char c) {
	return (c == '+');
}

bool charIsNegative(char c) {
	return (c == '-');
}

bool charIsNumber(char c) {
	return ((c >= '0') && (c <= '9'));
}

int parsePosixTimeZone(const char* tzone, int* tz) {
	if (NULL == tzone) {
		return RET_CODE_ERROR_NULL_VALUE;
	}
	if (NULL == tz) {
		return RET_CODE_ERROR_NULL_VALUE;
	}
	if (strlen(tzone) < 4) {
		return RET_CODE_ERROR_INVALID_VALUE;
	}
	bool hasNumber = charIsNumber(tzone[3]);
	if ((!hasNumber) && (!charIsNegative(tzone[3]))
			&& (!charIsPositive(tzone[3])))
		return RET_CODE_ERROR_INVALID_VALUE;
	char val[4] = { 0, 0, 0, 0 };
	val[0] = tzone[3];
	int i = 1;
	for (i = 1; i < 3; i++) {
		if (!charIsNumber(tzone[i + 3])) {
			if (!hasNumber)
				return RET_CODE_ERROR_INVALID_VALUE;
			break;
		}
		hasNumber = true;
		val[i] = tzone[i + 3];
	}
	*tz = atoi(val);
	if (*tz > 23) {
		return RET_CODE_ERROR_INVALID_VALUE;
	}
	return RET_CODE_SUCCESS;
}
