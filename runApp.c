#include "runApp.h"
#include "appCommon.h"
#include "runProbeServer.h"
#include "runDeviceService.h"
#include "appTools.h"

int runApp() {
	initProbeServer();
	initDeviceService();
	int result = startProbeServer();
	if (RET_CODE_SUCCESS  != result)
		return result;
	LOG("runApp Device Service");
	result = startDeviceService();
	if (RET_CODE_SUCCESS != result) {
		stopProbeServer();
		return result;
	}
	while(1) {

	}
	return RET_CODE_SUCCESS;
}

