#include "runApp.h"
#include "appTools.h"
#include "parseCmdParam.h"
#include "onvifHandle.h"
#include "parseUserInputCmd.h"
#include "logInfo.h"
#include <signal.h>

CmdParam cmdParam = {false, false, DEVICE_WEBSERVICE_PORT};
bool runAppTerminate;

void catchSignalInt(int signo) {
	runAppTerminate = true;
}

void catchSignalTerm(int signo) {
	runAppTerminate = true;
}

int runApp(int argc, char **argv) {
	runAppTerminate = FALSE;
	int result = parseCmd(argc, argv);
	if (RET_CODE_SUCCESS != result) {
		return result;
	}
	result = startOnvifApp();
	if (!isRetCodeSuccess(result)) {
		return result;
	}
	signal(SIGINT, &catchSignalInt);
	signal(SIGTERM, &catchSignalTerm);
	while(!runAppTerminate) {
		usleep(10000);
		// parseUserInputCmd();
	}
	logInfo("Quit Program...");
	stopOnvifApp();
	return RET_CODE_SUCCESS;
}

