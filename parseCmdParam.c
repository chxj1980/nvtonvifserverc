#include "parseCmdParam.h"
#include "onvifHandle.h"
#include "logInfo.h"

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
				logRawLineInfo("Usage: onvifserver [options]");
				logRawLineInfo("Options");
				logRawLineInfo("-d\t Display debug information");
				logRawLineInfo("-p <port>\t Onvif service port");
				logRawLineInfo("-h\t Display this informateion");
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
			strcpy(onvifRunParam.ip, "");
			break;
		default:
			break;
		}

	}
	return result;
}

