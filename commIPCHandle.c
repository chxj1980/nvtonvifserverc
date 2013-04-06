#include <cmd_type.h>
#include "appCommon.h"
#include "onvifHandle.h"
#include "commIPC.h"


int setNTPInfo(OnvifNTPInfo* info) {
	if (NULL == info) {
		return RET_CODE_ERROR_NULL_OBJECT;
	}

	char* address = info->address;
	if (strlen(address) < 1) {
		return RET_CODE_ERROR_INVALID_IP;
	}
	hmap_t* inList = hashmap_create();

	if (strlen(address) > 0) {
		putIntValueInList(inList, e_time_ntpenable, ENABLE_YES);
		putStrValueInList(inList, e_time_ntpserver, address);
	} else
		putIntValueInList(inList, e_time_ntpenable, ENABLE_NO);
	hmap_t* outList = hashmap_create();
	int result = sendAndRetList(T_Set, inList, outList);
	if (RET_CODE_SUCCESS == result) {

	}
	destroyHashMapList(inList);
	destroyHashMapList(outList);
	return result;
}

int getNTPInfo(OnvifNTPInfo* info) {
	if (NULL == info) {
		return RET_CODE_ERROR_NULL_OBJECT;
	}

	hmap_t* inList = hashmap_create();
	putNullValueInList(inList, e_time_ntpenable);
	putNullValueInList(inList, e_time_ntpserver);
	hmap_t* outList = hashmap_create();
	int result = sendAndRetList(T_Set, inList, outList);
	if (RET_CODE_SUCCESS == result) {
		getIntValueFromList(outList, e_time_ntpenable, &info->enable);
		getStrValueFromList(outList, e_time_ntpserver, info->address);
	}
	destroyHashMapList(inList);
	destroyHashMapList(outList);
	return result;
}

int getNetCardInfo(OnvifNetCardInfo* info) {
	if (NULL == info) {
		return RET_CODE_ERROR_NULL_OBJECT;
	}

	hmap_t* inList = hashmap_create();
	putNullValueInList(inList, e_net_cardname);
	putNullValueInList(inList, e_net_macaddr);
	putNullValueInList(inList, e_net_ip);
	putNullValueInList(inList, e_net_netmask);
	putNullValueInList(inList, e_net_gateway);
	hmap_t* outList = hashmap_create();
	int result = sendAndRetList(T_Set, inList, outList);
	if (RET_CODE_SUCCESS == result) {

	}
	destroyHashMapList(inList);
	destroyHashMapList(outList);
	return result;
}

