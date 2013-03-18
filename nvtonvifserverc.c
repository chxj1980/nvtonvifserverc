#include <stdio.h>
#include <stdlib.h>
#include "RemoteDiscoveryBinding.nsmap"
#include "soapH.h"
#include "soapStub.h"
void runDeviceProbe() {
	struct soap ServerSoap;
	struct ip_mreq mcast;
	int count = 0;

	//��ʼ������ʱ����
	//soap_init2(&ServerSoap, SOAP_IO_UDP | SOAP_IO_FLUSH | SOAP_IO_LENGTH, SOAP_IO_UDP | SOAP_IO_FLUSH | SOAP_IO_LENGTH);
	soap_init1(&ServerSoap, SOAP_IO_UDP | SOAP_XML_IGNORENS);
//	ServerSoap.version = 2;
//	soap_init(&ServerSoap);
//	ServerSoap.accept_timeout = 10;
//	ServerSoap.recv_timeout = 10;

	soap_set_namespaces(&ServerSoap, namespaces);

	if(!soap_valid_socket(soap_bind(&ServerSoap, NULL, 3702, 10)))
	{
		soap_print_fault(&ServerSoap, stderr);
		exit(1);
	}

	mcast.imr_multiaddr.s_addr = inet_addr("239.255.255.250");
	mcast.imr_interface.s_addr = htonl(INADDR_ANY);

	if(setsockopt(ServerSoap.master, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mcast, sizeof(mcast)) < 0)
	{
		printf("setsockopt error! error code = %d,err string = %s\n",errno,strerror(errno));
		return;
	}

	for(;;)
	{
		if(soap_serve(&ServerSoap))
		{
			soap_print_fault(&ServerSoap, stderr);
		}

		soap_destroy(&ServerSoap);
		soap_end(&ServerSoap);

		//�ͻ��˵�IP��ַ
		printf("Accepted count %d, connection from IP = %lu.%lu.%lu.%lu socket = %d \r\n",
				count, ((ServerSoap.ip)>>24)&0xFF, ((ServerSoap.ip)>>16)&0xFF, ((ServerSoap.ip)>>8)&0xFF, (ServerSoap.ip)&0xFF, (ServerSoap.socket));
		count++;
	}

	//��������ʱ�Ļ���
	soap_done(&ServerSoap);
}

int main(void) {
	runDeviceProbe();
	return EXIT_SUCCESS;
}
