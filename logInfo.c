#include "logInfo.h"
#include <stdio.h>
#include <stdarg.h>
#include "appTools.h"


void logIntoFile(FILE* file, char* level, const char* fmt, ...) {
	char value[500] = {0};
	char dt[30];
	getCurrentDateTimeStr(dt, 30);
	va_list argptr;
	int cnt;
	va_start(argptr, fmt);
	cnt = vsnprintf(value, 500,fmt, argptr);
	va_end(argptr);
	fprintf(file, "%s %s: %s\n", dt, level, value);
}

void logInfo(const char* fmt, ...) {
#ifdef __DEBUG
	va_list argptr;
	va_start(argptr, fmt);
	logIntoFile(stdout, "NORMAL", fmt, argptr);
	va_end(argptr);
#endif
}

void debugInfo(const char* fmt, ...) {
	va_list argptr;
	va_start(argptr, fmt);
	logIntoFile(stdout, "DEBUG", fmt, argptr);
	va_end(argptr);

}

void errorInfo(const char* fmt, ...) {
	va_list argptr;
	va_start(argptr, fmt);
	logIntoFile(stderr, "ERROR", fmt, argptr);
	va_end(argptr);
}

