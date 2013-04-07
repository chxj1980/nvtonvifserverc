GSOAP_ROOT = /home/pc01/Source/c++/gsoap-2.8/gsoap
CFLAGS = -Wall -g -O2 -DDEBUG
CC = gcc
INCLUDE = -I../libipnc/inc
LIBDIR = -L../libipnc/src
CND_BUILDDIR=build
OBJECTDIR=${CND_BUILDDIR}

ifeq ($(OS),Windows_NT)
	TARGET_EXTENSION=.exe
else
	TARGET_EXTENSION=.out
endif

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
TARGET = onvifserver
LIBS=-lpthread -lipc #-lssl -lcrypto

TESTTARGETBASE = test
TESTTARGET = $(TESTTARGETBASE)$(TARGET_EXTENSION)
TESTSRC_DIR = ./$(TESTTARGETBASE)
CMOCK_DIR = ../cmock
CMOCK_SRC = $(CMOCK_DIR)/src
MOCK_DIR = ./mock

UNITY_DIR = ../Unity
UNITY_SRC = $(UNITY_DIR)/src
UNITYSYMBOLS = -DTEST -DUNITY_SUPPORT_64

TESTINCLUDE = $(INCLUDE) -I$(CMOCK_SRC) -I$(UNITY_SRC)  -I.
TESTRUNNERFILE = test_Runner
TESTOBJECTFILES = $(OBJECTFILES) $(TESTSRC_DIR)/onvifHandleTest.o $(TESTSRC_DIR)/$(TESTRUNNERFILE).o 

all: $(TARGET)

$(TARGET): $(RUNAPPFILES)  
	$(CC) $^ -o $@ $(LIBDIR) $(LIBS)
	
%.o: %.c
	$(CC) $(CFLAGS) -c $^ $(TESTINCLUDE)

$(TESTTARGET): testrunner $(OBJECTFILES)
	$(CC) $^ -o $@ $(LIBDIR) $(LIBS) 

testrunner: 
	ruby $(UNITY_DIR)/auto/generate_test_runner.rb $(TESTSRC_DIR)/*.c $(TESTSRC_DIR)/$(TESTRUNNERFILE).c
		  
clean:
	rm -f $(TARGET) $(TESTTARGET) *.o *.a *.bak > /dev/null
	rm -rf $(MOCK_DIR)
	rm -f $(TESTSRC_DIR)/*.o $(TESTSRC_DIR)/*.a $(TESTSRC_DIR)/$(TESTRUNNERFILE).c > /dev/null
	
help:
	@echo "makefile help:"
	@echo "make [all clean $(TARGET) $(TESTTARGET)]"
