GSOAP_ROOT = /home/pc01/Source/c++/gsoap-2.8/gsoap
CFLAGS = -Wall -g -O2
CC = gcc
INCLUDE = -I$(GSOAP_ROOT)
CND_BUILDDIR=build
OBJECTDIR=${CND_BUILDDIR}
OBJECTFILES= soapC.o \
	soapServer.o \
	stdsoap2.o	\
	appTools.o	\
	runApp.o	\
	onvifHandle.o \
	onvifHandleProbe.o	\
	onvifHandleDeviceManagement.o	\
	onvifHandleMedia.o	\
	nvtonvifserverc.o
LIBS=-lpthread #-lssl -lcrypto
all: onvifserver

onvifserver: $(OBJECTFILES)  
	$(CC) $^ -o $@ $(LIBS)
	
%*.o : %*.c
	$(CC) $(CFLAGS) -c $^ $(INCLUDE)
  
clean:
	rm -f nvtOnvifServer *.o *.a *.bak > /dev/null