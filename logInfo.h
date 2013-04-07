/*
 * logInfo.h
 *
 *  Created on: 2013-4-2
 *      Author: PC01
 */

#ifndef LOGINFO_H_
#define LOGINFO_H_

#ifdef __cplusplus
extern "C" {
#endif

void logInfo(const char*, ...);
void debugInfo(const char*, ...);
void errorInfo(const char*, ...);
void logRawLineInfo(const char*, ...);

#ifdef __cplusplus
}
#endif

#endif /* LOGINFO_H_ */
