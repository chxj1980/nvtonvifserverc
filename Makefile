GSOAP_ROOT = /home/pc01/Source/c++/gsoap-2.8/gsoap
CFLAGS = -Wall -g -O2 -DDEBUG
CC = gcc
INCLUDE = -I .
LIBDIR = -L .
CND_BUILDDIR=build
OBJECTDIR=${CND_BUILDDIR}
OBJECTFILES= soapC.o \
	soapServer.o \
	stdsoap2.o	\
	threads.o	\
	appTools.o	\
	runApp.o	\
	onvifHandle.o \
	onvifHandleProbe.o	\
	onvifHandleDeviceManagement.o	\
	onvifHandleMedia.o	\
	runProbeServer.o	\
	runDeviceService.o	\
	nvtonvifserverc.o
LIBS=-lpthread #-lssl -lcrypto
all: onvifserver

onvifserver: $(OBJECTFILES)  
	$(CC) $^ -o $@ $(LIBDIR) $(LIBS)
	
%.o: %.c
	$(CC) $(CFLAGS) -c $^ $(INCLUDE)
  
clean:
	rm -f onvifserver *.o *.a *.bak > /dev/null