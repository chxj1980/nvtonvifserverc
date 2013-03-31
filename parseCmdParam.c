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
				LOG("Usage: onvifserver [options]\n");
				LOG("Options\n");
				LOG("-d\t Display debug information\n");
				LOG("-p <port>\t Onvif service port");
				LOG("-h\t Display this informateion");
			}
			result = RET_CODE_ERROR_UNKNOWN;
			break;
		case 'd':
			cmdParam.debug = true;
			break;
		case 'p':
			port = atoi(optarg);
			if (port < 1024) {
				LOG("Please input port exceed 1024\n");
				result = RET_CODE_ERROR_INVALIDVALUE;
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

