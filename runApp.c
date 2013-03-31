#include "runApp.h"
#include "appTools.h"
#include "parseCmdParam.h"
#include "onvifHandle.h"
#include "parseUserInputCmd.h"

CmdParam cmdParam = {false, false, DEVICE_WEBSERVICE_PORT};
bool runAppTerminate;

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
	while(!runAppTerminate) {
		parseUserInputCmd();
	}
	LOG("Quit program...\n");
	stopOnvifApp();
	return RET_CODE_SUCCESS;
}

