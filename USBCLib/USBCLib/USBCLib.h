#pragma once;

#include "GlobalDef.h"

#define LIB_VERSION_H 1
#define LIB_VERSION_L 1

#define HARDWARE_VERSION_H 1
#define HARDWARE_VERSION_L 0

#ifdef DLL_IMPLEMENT
	#define DLL_USBC __declspec(dllexport)
#else
	#define DLL_USBC __declspec(dllimport)
#endif

const int ERROR_NOUSBCDEVICE = -1;			//�����룺δ�����豸
const int ERROR_WITHOUTPOST = -2;			//�����룺û���Լ�
const int ERROR_SELECT_DEVICEID = -4;		//�����룺��ѡ�豸ID������
const int ERROR_OUTOFRANGE_DEVICEID = -8;	//�����룺�豸ID������Χ
const int ERROR_OUTOFRANGE_PORTID = -16;	//�����룺USB�˿�ID������Χ
const int ERROR_GENERIC = -128;			//�����룺����

extern "C" DLL_USBC int GetUSBCLibVersion();
extern "C" DLL_USBC int GetHardwareVersion();
extern "C" DLL_USBC int GetUSBCDevList(BYTE* pComNO, BYTE* pDevID, BYTE* pDevSize);
extern "C" DLL_USBC int RunUSBCLibPost();
extern "C" DLL_USBC int RunUSBCLibSelect(BYTE ucDevID, BYTE ucPortID);
