#include "runProbeServer.h"
#include <pthread.h>

#include "RemoteDiscoveryBinding.nsmap"
#include "appCommon.h"
#include "appTools.h"
#include "onvifHandle.h"
#include "logInfo.h"

RunServiceInfo probeServerServiceInfo = { .m_Active = FALSE };

bool checkProbeServerSoapAcceptTimeOut() {
	bool result = false;
	const char **s = NULL;
	if (NULL != probeServerServiceInfo.m_Soap.fault) {
		s = soap_faultstring(&probeServerServiceInfo.m_Soap);
		if ((NULL != s) && (NULL != (*s)) && (NULL != strstr(*s, "timed out"))) {
			result = true;
		}
	}
	return result;
}

void runProbeSoapServe() {
	if (soap_serve(&probeServerServiceInfo.m_Soap)) {
		if (checkProbeServerSoapAcceptTimeOut()) {
			stopSoap(&probeServerServiceInfo.m_Soap);
			return;
		}
		soap_print_fault(&probeServerServiceInfo.m_Soap, stderr);
	}
	stopSoap(&probeServerServiceInfo.m_Soap);
	logInfo(
			"Accepted connection from IP = %lu.%lu.%lu.%lu socket = %d \r",
			((probeServerServiceInfo.m_Soap.ip) >> 24) & 0xFF,
			((probeServerServiceInfo.m_Soap.ip) >> 16) & 0xFF,
			((probeServerServiceInfo.m_Soap.ip) >> 8) & 0xFF,
			(probeServerServiceInfo.m_Soap.ip) & 0xFF,
			(probeServerServiceInfo.m_Soap.socket));
}

void * runProbeServerThreadMethod() {
	while (!probeServerServiceInfo.m_Terminate) {
		myThreadSleep();
		runProbeSoapServe();
	}
	return (void*) RET_CODE_SUCCESS;
}

int startProbeServer() {
	stopProbeServer();
	struct ip_mreq mcast;
	probeServerServiceInfo.m_Terminate = false;
	soap_init1(&probeServerServiceInfo.m_Soap, SOAP_IO_UDP | SOAP_XML_IGNORENS);
	soap_set_namespaces(&probeServerServiceInfo.m_Soap, namespaces);
	probeServerServiceInfo.m_Soap.recv_timeout = SOAP_RECV_TIMEOUT;
	logInfo("probe soap bind");
	char* addr = onvifRunParam.address;
	if (strlen(addr) < 1) {
		addr = NULL;
	} else {
		probeServerServiceInfo.m_Soap.bind_flags = SO_REUSEADDR;
	}
	if (NULL != addr) {
		struct in_addr if_req;
		if_req.s_addr = inet_addr(addr);  // 想绑定的IP地址
		probeServerServiceInfo.m_Soap.ipv4_multicast_if = (char*)my_soap_malloc(&probeServerServiceInfo.m_Soap, sizeof(struct in_addr));
		memcpy(probeServerServiceInfo.m_Soap.ipv4_multicast_if, (char*)&if_req, sizeof(if_req));
	}
	if (!soap_valid_socket(soap_bind(&probeServerServiceInfo.m_Soap, NULL, MULTI_CAST_PORT, 100))) {
		soap_print_fault(&probeServerServiceInfo.m_Soap, stderr);
		return RET_CODE_ERROR_SOAP_BIND;
	}
	mcast.imr_multiaddr.s_addr = inet_addr(MULTI_CAST_IP);
	if (NULL != addr) {
		mcast.imr_interface.s_addr = inet_addr(addr);  // 想绑定的IP地址
	} else {
		mcast.imr_interface.s_addr = htonl(INADDR_ANY);
	}
	if (setsockopt(probeServerServiceInfo.m_Soap.master, IPPROTO_IP,
			IP_ADD_MEMBERSHIP, (char*) &mcast, sizeof(mcast)) < 0) {
		logInfo("setsockopt error! error code = %d,err string = %s", errno,
				strerror(errno));
		return RET_CODE_ERROR_SETSOCKOPT;
	}




//	int err = pthread_create(&probeServerServiceInfo.m_RunThread, NULL,
//			runProbeServerThreadMethod, NULL);
//	if (0 != err) {
//		stopSoap(&probeServerServiceInfo.m_Soap);
//		soap_done(&probeServerServiceInfo.m_Soap);
//		return RET_CODE_ERROR_CREATE_THREAD;
//	}
	probeServerServiceInfo.m_Active = true;
	return RET_CODE_SUCCESS;

}

void stopProbeServer() {
	if (!probeServerServiceInfo.m_Active)
		return;
	probeServerServiceInfo.m_Terminate = true;
//	void* status;
//	pthread_join(probeServerServiceInfo.m_RunThread, &status);
	soap_done(&probeServerServiceInfo.m_Soap);
	logInfo("stopProbeServer success");
	probeServerServiceInfo.m_Active = false;
}
