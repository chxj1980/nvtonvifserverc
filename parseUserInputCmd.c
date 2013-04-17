#include "parseUserInputCmd.h"
#include "appCommon.h"
#include "runApp.h"

int getUserInput(char* cmd, int* len) {
	char c = '\0';
	int pos = 0;
	int result = RET_CODE_SUCCESS;
	while (true) {
		c = getchar();
		if ((pos < *len) && (c != '\n')) {
			cmd[pos] = c;
			pos++;
		} else {
			if (pos >= *len) {
				result = RET_CODE_ERROR_INVALID_VALUE;
			}
			else {
				*len = pos - 1;
				cmd[pos] = '\0';
			}
			break;
		}
	}
	return result;
}

void parseUserInputCmd() {
	char cmd[INFO_LENGTH] = {0};
	int len = 0;
	while(!runAppTerminate) {
		len = INFO_LENGTH - 1;
		if (RET_CODE_SUCCESS == getUserInput(cmd, &len)) {
			if (0 == strncmp(cmd, "quit", len)) {
				runAppTerminate = true;
				break;
			}

		}
	}
}

