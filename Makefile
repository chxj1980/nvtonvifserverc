GSOAP_ROOT = /home/pc01/Source/c++/gsoap-2.8/gsoap
CFLAGS = -Wall -g -O2 -DDEBUG
CC = gcc
INCLUDE = -I ../libipnc/inc
LIBDIR = -L ../libipnc/src
CND_BUILDDIR=build
OBJECTDIR=${CND_BUILDDIR}
OBJECTFILES= soapC.o \
	soapServer.o \
	stdsoap2.o	\
	threads.o	\
	hashMap.o	\
	commIPC.o	\
	appTools.o	\
	logInfo.o	\
	parseCmdParam.o	\
	parseUserInputCmd.o	\
	runApp.o	\
	onvifHandle.o \
	onvifHandleProbe.o	\
	onvifHandleDeviceManagement.o	\
	onvifHandleMedia.o	\
	runProbeServer.o	\
	runDeviceService.o	\
	nvtonvifserverc.o
LIBS=-lpthread -lipc #-lssl -lcrypto
all: onvifserver

onvifserver: $(OBJECTFILES)  
	$(CC) $^ -o $@ $(LIBDIR) $(LIBS)
	
%.o: %.c
	$(CC) $(CFLAGS) -c $^ $(INCLUDE)
  
clean:
	rm -f onvifserver *.o *.a *.bak > /dev/null