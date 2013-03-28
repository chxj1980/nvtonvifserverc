#include "appTools.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/route.h>
#include <netinet/in.h>
#include <arpa/inet.h>


int getNetCardMac(unsigned char *pMac) {
	if (NULL == pMac)
		return RET_CODE_ERROR_NULLVALUE;
	struct ifreq ifreq;
	int sockfd = 0;
	unsigned char mac[6] = { 0 };

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		ERRLOG( "netGetMac socket");
		return RET_CODE_ERROR_CREATESOCKET;
	}

	strcpy(ifreq.ifr_name, NET_CARD_NAME);

	if (ioctl(sockfd, SIOCGIFHWADDR, &ifreq) < 0) {
		ERRLOG( "netGetMac ioctl");
		close(sockfd);
		return RET_CODE_ERROR_SOCKETIOCTL;
	}

	memcpy(mac, ifreq.ifr_hwaddr.sa_data, 6);
	LOG( "MAC:%02x:%02x:%02x:%02x:%02x:%02x\n",
			mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

	if (pMac != NULL) {
		memcpy(pMac, mac, 6);
	}

	close(sockfd);

	return RET_CODE_SUCCESS;
}

int getLocalIp(char* pIp) {
	if (NULL == pIp)
		return RET_CODE_ERROR_NULLVALUE;
	int sock = 0;
	struct ifreq ifr;
	unsigned int localIp = 0;
	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, NET_CARD_NAME, IFNAMSIZ);
	sock = socket(AF_INET, SOCK_DGRAM, 0);

	if (sock <= 0) {
		return RET_CODE_ERROR_CREATESOCKET;
	}

	((struct sockaddr_in*) &ifr.ifr_addr)->sin_family = PF_INET;

	if (ioctl(sock, SIOCGIFADDR, &ifr) < 0) {
		ERRLOG("get ip error: %s\r\n", strerror(errno));
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

int getServiceURL(char* value) {
	char ip_list[21] = { 0 };
	int result = getLocalIp(ip_list);
	if (RET_CODE_SUCCESS != result)
		return result;
	sprintf(value, "http://%s:%d/",	ip_list, DEVICE_WEBSERVICE_PORT);
	return RET_CODE_SUCCESS;
}
