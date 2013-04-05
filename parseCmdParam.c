#include "parseCmdParam.h"
#include "onvifHandle.h"

int parseCmd(int argc, char **argv) {
	int result = RET_CODE_SUCCESS;
	int cmdFlag = 0;
	opterr = 0;
	int port = 0;
	while ((cmdFlag = getopt(argc, argv, "dhp:")) != -1) {
		switch (cmdFlag) {
		case 'h':
			cmdParam.help = true;
			if (cmdParam.help) {
				logInfo("Usage: onvifserver [options]");
				logInfo("Options");
				logInfo("-d\t Display debug information");
				logInfo("-p <port>\t Onvif service port");
				logInfo("-h\t Display this informateion");
			}
			result = RET_CODE_ERROR_UNKNOWN;
			break;
		case 'd':
			cmdParam.debug = true;
			break;
		case 'p':
			port = atoi(optarg);
			if (port < 1024) {
				logInfo("Please input port exceed 1024");
				result = RET_CODE_ERROR_INVALID_VALUE;
			} else {
				cmdParam.port = port;
			}
			onvifRunParam.runFlag = true;
			onvifRunParam.servicePort = cmdParam.port;
			break;
		default:
			break;
		}

	}
	return result;
}

