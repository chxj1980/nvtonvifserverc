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
	
RUNAPPFILES = nvtonvifserverc.o
TARGET = onvifserver$(TARGET_EXTENSION)
LIBS=-lpthread -lipc #-lssl -lcrypto

CMOCK_DIR = ../cmock
CMOCK_SRC = $(CMOCK_DIR)/src
MOCK_DIR = ./mock

UNITY_DIR = ../Unity
UNITY_SRC = $(UNITY_DIR)/src
UNITYSYMBOLS = -DTEST -DUNITY_SUPPORT_64
UNITYFILES = $(UNITY_SRC)/unity.o
UNITYFILES1 = unity.o

TESTTARGETBASE = test
TESTTARGET = $(TESTTARGETBASE)$(TARGET_EXTENSION)
TESTSRC_DIR = ./$(TESTTARGETBASE)
TESTINCLUDE = $(INCLUDE) -I$(CMOCK_SRC) -I$(UNITY_SRC)  -I.
TESTRUNNERFILE = test_Runner
TESTOBJECTFILES = $(TESTSRC_DIR)/onvifHandleTest.o $(TESTSRC_DIR)/$(TESTRUNNERFILE).o 
TESTOBJECTFILES1 = onvifHandleTest.o $(TESTRUNNERFILE).o

all: $(TARGET)

$(TARGET): $(OBJECTFILES) $(RUNAPPFILES)  
	$(CC) $^ -o $@ $(LIBDIR) $(LIBS)

$(OBJECTFILES): %.o: %.c
	$(CC) $(CFLAGS) -c $^ $(INCLUDE)	

$(RUNAPPFILES): %.o: %.c
	$(CC) $(CFLAGS) -c $^ $(INCLUDE)

$(TESTOBJECTFILES): %.o: %.c
	$(CC) $(CFLAGS) -c $^ $(TESTINCLUDE)
	
$(UNITYFILES): %.o: %.c
	$(CC) $(CFLAGS) $(UNITYSYMBOLS) -c $^ 
	 	
$(TESTTARGET): testrunner  $(OBJECTFILES) $(UNITYFILES) $(TESTOBJECTFILES)
	$(CC) $(OBJECTFILES) $(UNITYFILES1) $(TESTOBJECTFILES1) -o $@ $(LIBDIR) $(LIBS) 
	
testrunner: 
	ruby $(UNITY_DIR)/auto/generate_test_runner.rb $(TESTSRC_DIR)/*.c $(TESTSRC_DIR)/$(TESTRUNNERFILE).c

testrun: cleantestdir $(TESTTARGET)
	./$(TESTTARGET)
	
.PHONY: cleantestdir clean testrun testrunner

cleantestdir:
	-rm -f $(TESTRUNNERFILE).o $(UNITYFILES) $(TESTSRC_DIR)/*.o $(TESTSRC_DIR)/*.a $(TESTSRC_DIR)/$(TESTRUNNERFILE).c > /dev/null		  

clean: cleantestdir
	-rm -f $(TARGET) $(TESTTARGET) *.o *.$(TARGET_EXTENSION) *.a *.bak > /dev/null
	-rm -rf $(MOCK_DIR)
	
	
help:
	@echo "makefile help:"
	@echo "make [all clean $(TARGET) $(TESTTARGET) testrun help]"
	@echo "\tall  \t\t Make Deault Action"
	@echo "\tclean  \t\t Clean All Temp Files"
	@echo "\t$(TARGET)  Create App File"
	@echo "\t$(TESTTARGET)  \t Create Test App File"
	@echo "\ttestrun  \t Run Test App"	
	@echo "\thelp  \t\t Display Help Message"
