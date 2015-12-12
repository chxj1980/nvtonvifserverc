#include "runDeviceService.h"
#include <pthread.h>
#include <string.h>

#include "appCommon.h"
#include "appTools.h"
#include "onvifHandle.h"
#include "logInfo.h"

#define BACKLOG 100
#define DEVICE_SERVICE_MAX_THREAD 3 // Size of thread pool
#define DEVICE_SERVICE_MAX_QUEUE 1000 // Max. size of request queue
SOAP_SOCKET deviceService_queue[DEVICE_SERVICE_MAX_QUEUE]; // The global request queue of sockets
int deviceService_queue_head = 0, deviceService_queue_tail = 0; // Queue deviceService_queue_head and deviceService_queue_tail
pthread_mutex_t deviceService_queue_mutex;
pthread_cond_t deviceService_queue_cond;
struct soap *deviceService_thread_soaps[DEVICE_SERVICE_MAX_THREAD];
pthread_t deviceService_threadIds[DEVICE_SERVICE_MAX_THREAD];

RunServiceInfo deviceServiceServiceInfo = { .m_Active = FALSE };

void *deviceService_process_queue(void*);
int deviceService_enqueue(SOAP_SOCKET);
SOAP_SOCKET deviceService_dequeue();

void * runDeviceServiceThreadMethod();

bool checkDeviceServiceSoapAcceptTimeOut() {
	return (!deviceServiceServiceInfo.m_Soap.errnum);
}

int startDeviceService() {
	stopDeviceService();
	soap_init(&deviceServiceServiceInfo.m_Soap);
	soap_set_namespaces(&deviceServiceServiceInfo.m_Soap, namespaces);
	deviceServiceServiceInfo.m_Soap.accept_timeout = SOAP_ACCEPT_TIMEOUT;
	debugInfo("startDeviceService bind");
	char* addr = onvifRunParam.address;
	if (strlen(addr) < 1) {
		addr = NULL;
	}
	logInfo("device service soap bind");
	if (!soap_valid_socket(soap_bind(&deviceServiceServiceInfo.m_Soap, addr, onvifRunParam.servicePort, BACKLOG))) {
		logInfo("device service bind error");
		return RET_CODE_ERROR_SOAP_BIND;
	}
	deviceServiceServiceInfo.m_Terminate = false;
	int err = pthread_create(&deviceServiceServiceInfo.m_RunThread, NULL,
			runDeviceServiceThreadMethod, NULL);
	if (0 != err) {
		stopSoap(&deviceServiceServiceInfo.m_Soap);
		soap_done(&deviceServiceServiceInfo.m_Soap);
		return RET_CODE_ERROR_CREATE_THREAD;
	}
	pthread_mutex_init(&deviceService_queue_mutex, NULL);
	pthread_cond_init(&deviceService_queue_cond, NULL);
	int i;
	for (i = 0; i < DEVICE_SERVICE_MAX_THREAD; i++) {
		deviceService_thread_soaps[i] = soap_copy(&deviceServiceServiceInfo.m_Soap);
		deviceService_thread_soaps[i]->recv_timeout = SOAP_RECV_TIMEOUT;
		deviceService_thread_soaps[i]->send_timeout = SOAP_SEND_TIMEOUT;
		pthread_create(&deviceService_threadIds[i], NULL, (void*(*)(void*)) deviceService_process_queue,
				(void*) deviceService_thread_soaps[i]);
	}
	deviceServiceServiceInfo.m_Active = true;
	return RET_CODE_SUCCESS;
}

void stopDeviceService() {
	if (!deviceServiceServiceInfo.m_Active)
		return;
	deviceServiceServiceInfo.m_Terminate = true;
	void* status;
	pthread_join(deviceServiceServiceInfo.m_RunThread, &status);
	soap_done(&deviceServiceServiceInfo.m_Soap);
	logInfo("stopDeviceService success");
	deviceServiceServiceInfo.m_Active = false;
}

void * runDeviceServiceThreadMethod() {
	SOAP_SOCKET remoteSocket;
	int i;
	while (!deviceServiceServiceInfo.m_Terminate) {
		myThreadSleep();
		remoteSocket = soap_accept(&deviceServiceServiceInfo.m_Soap);
		if (!soap_valid_socket(remoteSocket)) {
			if (checkDeviceServiceSoapAcceptTimeOut()) {
				continue;
			}
			soap_print_fault(&deviceServiceServiceInfo.m_Soap, stderr);
			return (void*) RET_CODE_ERROR_SOAP_ACCEPT;
		}
		while (SOAP_EOM == deviceService_enqueue(remoteSocket)) {
			if (deviceServiceServiceInfo.m_Terminate)
				break;
			myThreadSleep();
		}
	}
	for (i = 0; i < DEVICE_SERVICE_MAX_THREAD; i++) {
		while (deviceService_enqueue(SOAP_INVALID_SOCKET) == SOAP_EOM)
			sleep(1);
	}
	for (i = 0; i < DEVICE_SERVICE_MAX_THREAD; i++) {
		pthread_join(deviceService_threadIds[i], NULL);
		soap_done(deviceService_thread_soaps[i]);
		free(deviceService_thread_soaps[i]);
	}
	pthread_mutex_destroy(&deviceService_queue_mutex);
	pthread_cond_destroy(&deviceService_queue_cond);
	return (void*) RET_CODE_SUCCESS;
}

void *deviceService_process_queue(void *soap) {
	struct soap *tsoap = (struct soap*) soap;
	for (;;) {
		tsoap->socket = deviceService_dequeue();
		if (!soap_valid_socket(tsoap->socket))
			break;
		soap_serve(tsoap);
		stopSoap(tsoap);
	}
	return NULL;
}

int deviceService_enqueue(SOAP_SOCKET sock) {
	int status = SOAP_OK;
	int next = 0;
	pthread_mutex_lock(&deviceService_queue_mutex);
	next = deviceService_queue_tail + 1;
	if (next >= DEVICE_SERVICE_MAX_QUEUE)
		next = 0;
	if (next == deviceService_queue_head)
		status = SOAP_EOM;
	else {
		deviceService_queue[deviceService_queue_tail] = sock;
		deviceService_queue_tail = next;
	}
	pthread_cond_signal(&deviceService_queue_cond);
	pthread_mutex_unlock(&deviceService_queue_mutex);
	return status;
}

SOAP_SOCKET deviceService_dequeue() {
	SOAP_SOCKET sock;
	pthread_mutex_lock(&deviceService_queue_mutex);
	while (deviceService_queue_head == deviceService_queue_tail)
		pthread_cond_wait(&deviceService_queue_cond, &deviceService_queue_mutex);
	sock = deviceService_queue[deviceService_queue_head++];
	if (deviceService_queue_head >= DEVICE_SERVICE_MAX_QUEUE)
		deviceService_queue_head = 0;
	pthread_mutex_unlock(&deviceService_queue_mutex);
	return sock;
}
