#ifndef RUNAPP_H_
#define RUNAPP_H_

#include "appCommon.h"

#ifdef __cplusplus
	extern "C" {
#endif
	extern bool runAppTerminate;
	int runApp(int argc, char **argv);

#ifdef __cplusplus
	}
#endif


#endif /* RUNAPP_H_ */
