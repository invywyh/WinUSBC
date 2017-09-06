#ifndef _LeoSerialPort_H
#define _LeoSerialPort_H

#include <Windows.h>
#include <string.h>

#define  uint8_t unsigned char
#define  uint16_t unsigned short

    class CLeoSerialPort
    {
    public:
        CLeoSerialPort();
        ~CLeoSerialPort();

        BOOL Open ( int nPort = 8, int nBaud = 9600 );//�򿪴��ڣ�Ĭ��Com2&9600
        BOOL Close ( void );//�رմ���

        int ReadData ( void *, int );//��������
		int WriteData ( const char *, int );//��������

        int ReadDataWaiting ( void );	// ��ѯ���������Ƿ���δ��ȡ�����ݣ���ѯ��־λ

        BOOL IsOpened ( void )//����Com��
		{
            return ( m_bOpened );
        }

    protected:
        HANDLE m_hIDComDev;
        OVERLAPPED m_OverlappedRead, m_OverlappedWrite;
        BOOL m_bOpened;
    };
#endif
