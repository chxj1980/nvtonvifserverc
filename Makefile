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
	commIPCHandle.o	\
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
	runDeviceService.o	
	
RUNAPPFILES = $(OBJECTFILES) nvtonvifserverc.o

LIBS=-lpthread -lipc #-lssl -lcrypto

CMOCK_DIR = ../cmock
CMOCK_SRC = $(CMOCK_DIR)/src
MOCK_DIR = ./mock

UNITY_DIR = ../Unity
UNITY_SRC = $(UNITY_DIR)/src
UNITYSYMBOLS = -DTEST -DUNITY_SUPPORT_64

TESTINCLUDE = -I$(CMOCK_SRC) -I$(UNITY_SRC) -I$(MOCKS_DIR) -I../libipnc/inc -I.

all: onvifserver

onvifserver: $(RUNAPPFILES)  
	$(CC) $^ -o $@ $(LIBDIR) $(LIBS)
	
%.o: %.c
	$(CC) $(CFLAGS) -c $^ $(INCLUDE)

test:
	
	  
clean:
	rm -f onvifserver *.o *.a *.bak > /dev/null