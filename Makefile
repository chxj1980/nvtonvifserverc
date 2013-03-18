GSOAP_ROOT = /home/pc01/Source/c++/gsoap-2.8/gsoap
CC = gcc -g -O2 #-DWITH_OPENSSL -DHAVE_PTHREAD_H
INCLUDE = -I$(GSOAP_ROOT)
CND_BUILDDIR=build
OBJECTDIR=${CND_BUILDDIR}
OBJECTFILES= nvtonvifserverc.o
LIBS=-lpthread #-lssl -lcrypto
all: onvifserver

onvifserver: $(OBJECTFILES)  
	$(CC) $^ -o $@ $(LIBS)
	
%*.o : %*.c
 $(CC) -c $^ $(INCLUDE)
  
clean:
	rm -f nvtOnvifServer *.o *.a *.bak > /dev/null