#ifndef __USBC_LIB_H
#define __USBC_LIB_H

#include<Windows.h>

const char USBC_SUCCEEDED = 0;					//�����ɹ�
const char USBC_ERROR_NOUSBCDEVICE = -1;		//�����룺δ�����豸
const char USBC_ERROR_WITHOUTPOST = -2;			//�����룺û���Լ�
const char USBC_ERROR_SELECT_DEVICEID = -4;		//�����룺��ѡ�豸ID������
const char USBC_ERROR_OUTOFRANGE_DEVICEID = -8;	//�����룺�豸ID������Χ
const char USBC_ERROR_OUTOFRANGE_PORTID = -16;	//�����룺USB�˿�ID������Χ
const char USBC_ERROR_GENERIC = -128;			//�����룺����

typedef int (*GetUSBCLibVersion)();
typedef int (*GetHardwareVersion)();
typedef int (*GetUSBCDevList)(BYTE* pComNO, BYTE* pDevID, BYTE* pDevSize);
typedef int (*RunUSBCLibPost)();
typedef int (*RunUSBCLibSelect)(BYTE ucDevID, BYTE ucPortID);

static HINSTANCE  hMod;

static GetUSBCLibVersion	dllGetUSBCLibVersion;
static GetHardwareVersion	dllGetHardwareVersion;
static GetUSBCDevList		dllGetUSBCDevList;
static RunUSBCLibPost		dllRunUSBCLibPost;
static RunUSBCLibSelect		dllRunUSBCLibSelect;

static void ParseError(char error)
{
	switch(error)
	{
		case USBC_SUCCEEDED:
			printf("�ɹ�!\r\n");
			break;
		case USBC_ERROR_NOUSBCDEVICE:
			printf("�����룺δ�����豸!\r\n");
			break;
		case USBC_ERROR_WITHOUTPOST:
			printf("�����룺û���Լ�!\r\n");
			break;
		case USBC_ERROR_SELECT_DEVICEID:
			printf("�����룺��ѡ�豸ID������!\r\n");
			break;
		case USBC_ERROR_OUTOFRANGE_DEVICEID:
			printf("�����룺�豸ID������Χ!\r\n");
			break;
		case USBC_ERROR_OUTOFRANGE_PORTID:
			printf("�����룺USB�˿�ID������Χ!\r\n");
			break;
		case USBC_ERROR_GENERIC:
			printf("�����룺USB�˿�ID������Χ!\r\n");
			break;
		default:
			printf("δ֪\r\n");
			break;
	}
};

static BYTE InitUSBCLib()
{
	hMod = LoadLibrary("USBCLib.DLL");

	BYTE bFlag=0x00;

	dllGetUSBCLibVersion  =  (GetUSBCLibVersion)GetProcAddress(hMod, "GetUSBCLibVersion");
	if( dllGetUSBCLibVersion )
	{
		bFlag|=0x01;
	}
				
	dllGetHardwareVersion  =  (GetHardwareVersion)GetProcAddress(hMod, "GetHardwareVersion");
	if( dllGetHardwareVersion )
	{
		bFlag|=0x02;
	}

	dllGetUSBCDevList = (GetUSBCDevList)GetProcAddress(hMod, "GetUSBCDevList");

	dllRunUSBCLibPost = (RunUSBCLibPost)GetProcAddress(hMod, "RunUSBCLibPost");
	if( dllRunUSBCLibPost )
	{
		bFlag|=0x04;
	}

	dllRunUSBCLibSelect = (RunUSBCLibSelect)GetProcAddress(hMod, "RunUSBCLibSelect");
	if( dllRunUSBCLibPost )
	{
		bFlag|=0x08;
	}

	return bFlag;
};

static void UninitUSBCLib()
{
	if(hMod)
		FreeLibrary(hMod);
};
#endif
