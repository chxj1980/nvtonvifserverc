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
UNITYFILES = unity.o

TESTTARGETBASE = test

TESTSRC_DIR = $(TESTTARGETBASE)
TESTINCLUDE = $(INCLUDE) -I$(CMOCK_SRC) -I$(UNITY_SRC)  -I.
TESTFILES = commIPCHandleTest onvifHandleTest
TESTOBJECTFILES = $(foreach n, $(TESTFILES), $(n).o)

TESTRUNNERFILE = test_Runner
TESTRUNNERFILES = $(foreach n, $(TESTFILES), $(TESTRUNNERFILE)$(n).o)
TESTTARGETS = $(foreach n, $(TESTFILES), $(TESTRUNNERFILE)$(n)$(TARGET_EXTENSION))

all: $(TARGET)

$(TARGET): $(OBJECTFILES) $(RUNAPPFILES)  
	$(CC) $^ -o $@ $(LIBDIR) $(LIBS)

$(OBJECTFILES): %.o: %.c
	$(CC) $(CFLAGS) -c $^ $(INCLUDE)	

$(RUNAPPFILES): %.o: %.c
	$(CC) $(CFLAGS) -c $^ $(INCLUDE)

$(TESTOBJECTFILES): %.o: $(TESTSRC_DIR)/%.c
	$(CC) $(CFLAGS) $(UNITYSYMBOLS) -c $^ $(TESTINCLUDE)
	
$(UNITYFILES): %.o: $(UNITY_SRC)/%.c
	$(CC) $(CFLAGS) $(UNITYSYMBOLS) -c $^ 

$(TESTRUNNERFILES): %.o: $(TESTSRC_DIR)/%.c
	$(CC) $(CFLAGS) $(UNITYSYMBOLS) -c $^ $(TESTINCLUDE)
	-rm -f $^

$(TESTTARGETS): %$(TARGET_EXTENSION): %.o
	$(CC) $(OBJECTFILES) $(UNITYFILES) $^ $(subst $(TESTRUNNERFILE), ,$^) -o $@ $(LIBDIR) $(LIBS)
	 	
testrunner:  
	@for testrun1 in $(TESTFILES); do \
		ruby $(UNITY_DIR)/auto/generate_test_runner.rb $(TESTSRC_DIR)/$$testrun1.c $(TESTSRC_DIR)/$(TESTRUNNERFILE)$$testrun1.c ; \
	done
	
buildtest: $(OBJECTFILES) $(UNITYFILES) $(TESTOBJECTFILES) testrunner $(TESTRUNNERFILES) $(TESTTARGETS)

testrun: cleantest buildtest
	@for testrun1 in $(TESTTARGETS); do \
		./$$testrun1 ; \
	done

runapp: $(TARGET)
		./$(TARGET)
		
.PHONY: cleantest clean testrun testrunner buildtesttarget runapp

cleantest:
	-rm -f $(UNITYFILES) $(TESTSRC_DIR)/*.o $(TESTSRC_DIR)/*.out $(TESTSRC_DIR)/*.a $(TESTSRC_DIR)/$(TESTRUNNERFILE).c > /dev/null
	-rm -f $(TESTTARGETS)		  

clean: cleantest
	-rm -f $(TARGET) $(TESTTARGET) *.o *.$(TARGET_EXTENSION) *.a *.bak > /dev/null
	-rm -rf $(MOCK_DIR)
	
	
help:
	@echo "makefile help:"
	@echo "make [all clean $(TARGET) $(TESTTARGET) testrun help]"
	@echo "  all                    Make Deault Action $(TARGET)"
	@echo "  clean                  Clean All Temp Files"
	@echo "  cleantest              Clean All Test Files"            
	@echo "  $(TARGET)        Create App File"
	@echo "  buildtest              Create Test App File"
	@echo "  runapp                 Run App"
	@echo "  testrun                Run Test App"	
	@echo "  help                   Display Help Message"
