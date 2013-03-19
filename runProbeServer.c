#include "runProbeServer.h"
#include <pthread.h>

#include "RemoteDiscoveryBinding.nsmap"
#include "appCommon.h"
#include "appTools.h"

RunServiceInfo probeServerServiceInfo;

void initProbeServer() {
	probeServerServiceInfo.m_Active = false;
}

void * runProbeServerThreadMethod() {
    int count = 0;
    while (!probeServerServiceInfo.m_Terminate) {
        if (soap_serve(&probeServerServiceInfo.m_Soap)) {
            soap_print_fault(&probeServerServiceInfo.m_Soap, stderr);
        }

        stopSoap(&probeServerServiceInfo.m_Soap);

        //�ͻ��˵�IP��ַ
        LOG("Accepted count %d, connection from IP = %lu.%lu.%lu.%lu socket = %d \r\n",
                count, ((probeServerServiceInfo.m_Soap.ip) >> 24)&0xFF, ((probeServerServiceInfo.m_Soap.ip) >> 16)&0xFF, ((probeServerServiceInfo.m_Soap.ip) >> 8)&0xFF, (probeServerServiceInfo.m_Soap.ip)&0xFF, (probeServerServiceInfo.m_Soap.socket));
        count++;
    }
    soap_done(&probeServerServiceInfo.m_Soap);
    return (void*)RET_CODE_SUCCESS;
}


int startProbeServer() {
    stopProbeServer();
	struct ip_mreq mcast;
	probeServerServiceInfo.m_Terminate = false;
	soap_init1(&probeServerServiceInfo.m_Soap, SOAP_IO_UDP | SOAP_XML_IGNORENS);
	soap_set_namespaces(&probeServerServiceInfo.m_Soap, namespaces);
	if (!soap_valid_socket(soap_bind(&probeServerServiceInfo.m_Soap, NULL, MULTI_CAST_PORT, 10))) {
		soap_print_fault(&probeServerServiceInfo.m_Soap, stderr);
		return RET_CODE_ERROR_SOAP_BIND;
	}
	mcast.imr_multiaddr.s_addr = inet_addr(MULTI_CAST_IP);
	mcast.imr_interface.s_addr = htonl(INADDR_ANY);

	if (setsockopt(probeServerServiceInfo.m_Soap.master, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*) &mcast,
			sizeof(mcast)) < 0) {
		LOG("setsockopt error! error code = %d,err string = %s\n",
				errno, strerror(errno));
		return RET_CODE_ERROR_SETSOCKOPT;
	}
	int err = pthread_create(&probeServerServiceInfo.m_RunThread, NULL, runProbeServerThreadMethod, NULL);
	if (0 != err) {
		stopSoap(&probeServerServiceInfo.m_Soap);
		return RET_CODE_ERROR_CREATETHREAD;
	}
	probeServerServiceInfo.m_Active = true;
	return RET_CODE_SUCCESS;

}

void stopProbeServer() {
	if (!probeServerServiceInfo.m_Active)
		return;
	probeServerServiceInfo.m_Terminate =true;
    void* status;
    pthread_join(probeServerServiceInfo.m_RunThread, &status);
    probeServerServiceInfo.m_Active = false;
}
