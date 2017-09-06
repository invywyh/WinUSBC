#include "LeoSerialPort.h"
#include <tchar.h>

// ��ʹ�ö���ʱ
#define TIMEOUT_READ_INTERVAL	0xFFFFFFFF
#define TIMEOUT_READ_TOTAL_MULTIPLIER	10
#define TIMEOUT_READ_TOTAL_CONSTANT		100

// д��ʱΪ��
#define TIMEOUT_WRITE_TOTAL_MULTIPLIER	10
#define TIMEOUT_WRITE_TOTAL_CONSTANT	100

// �Ƽ�������/�����������ע�⣺ʵ��ֵ��ϵͳ���ã�
#define BUFFER_INPUT_RECOMMEND	1024
#define BUFFER_OUTPUT_RECOMMEND	1024

// �첽��ȡ/д�����ʱ�ȴ��¼��ĳ�ʱʱ��
#define TIMEOUT_READCOMM_EVENT	1000
#define TIMEOUT_WRITECOMM_EVENT	1000

// һЩͨѶЭ��ʹ�õĺ�
#define FC_DTRDSR       0x01
#define FC_RTSCTS       0x02
#define FC_XONXOFF      0x04
#define ASCII_BEL       0x07
#define ASCII_BS        0x08
#define ASCII_LF        0x0A
#define ASCII_CR        0x0D
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

CLeoSerialPort::CLeoSerialPort()
{
    memset ( &m_OverlappedRead, 0, sizeof ( OVERLAPPED ) );
    memset ( &m_OverlappedWrite, 0, sizeof ( OVERLAPPED ) );
    m_hIDComDev = NULL;
    m_bOpened = FALSE;
}

CLeoSerialPort::~CLeoSerialPort()
{
    Close();
}

// �򿪴���
BOOL CLeoSerialPort::Open ( int nPort, int nBaud )
{
    if ( m_bOpened )
        return TRUE;

    TCHAR szPort[50];
    TCHAR szComParams[50];
    DCB dcb;

	if(nPort<10)
		wsprintf ( szPort, _T ( "COM%d" ), nPort );
	else
		wsprintf ( szPort, _T ( "\\\\.\\COM%d" ), nPort );
	// API�������ļ���Windows�н������豸�����ļ��Դ�
    m_hIDComDev = CreateFile (
                      szPort,
                      GENERIC_READ | GENERIC_WRITE,
                      0,
                      NULL,
                      OPEN_EXISTING,
                      FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,	// �첽��д
                      NULL
                  );

    if ( m_hIDComDev == NULL ) return ( FALSE );

    memset ( &m_OverlappedRead, 0, sizeof ( OVERLAPPED ) );
    memset ( &m_OverlappedWrite, 0, sizeof ( OVERLAPPED ) );

    // ���ó�ʱ
    COMMTIMEOUTS CommTimeOuts;
    CommTimeOuts.ReadIntervalTimeout	= TIMEOUT_READ_INTERVAL;
    CommTimeOuts.ReadTotalTimeoutMultiplier = TIMEOUT_READ_TOTAL_MULTIPLIER;
    CommTimeOuts.ReadTotalTimeoutConstant	= TIMEOUT_READ_TOTAL_CONSTANT;
    CommTimeOuts.WriteTotalTimeoutMultiplier= TIMEOUT_WRITE_TOTAL_MULTIPLIER;
    CommTimeOuts.WriteTotalTimeoutConstant	= TIMEOUT_WRITE_TOTAL_CONSTANT;
    SetCommTimeouts ( m_hIDComDev, &CommTimeOuts );

	if(nPort<10)
		wsprintf ( szComParams, _T ( "COM%d:%d,n,8,1" ), nPort, nBaud );
	else 
		wsprintf ( szComParams, _T ( "\\\\.\\COM%d:%d,n,8,1" ), nPort, nBaud );
// �����첽��ȡ/д������¼�
    m_OverlappedRead.hEvent = CreateEvent ( NULL, TRUE, FALSE, NULL );
    m_OverlappedWrite.hEvent = CreateEvent ( NULL, TRUE, FALSE, NULL );

// ��ȡ/���ô����豸����
    dcb.DCBlength = sizeof ( DCB );
    GetCommState ( m_hIDComDev, &dcb );
    dcb.BaudRate = nBaud;
    dcb.ByteSize = 8;
    unsigned char ucSet;
    ucSet = ( unsigned char ) ( ( FC_RTSCTS & FC_DTRDSR ) != 0 );
    ucSet = ( unsigned char ) ( ( FC_RTSCTS & FC_RTSCTS ) != 0 );
    ucSet = ( unsigned char ) ( ( FC_RTSCTS & FC_XONXOFF ) != 0 );

    if ( !SetCommState ( m_hIDComDev, &dcb ) ||
            !SetupComm ( m_hIDComDev, BUFFER_INPUT_RECOMMEND, BUFFER_OUTPUT_RECOMMEND ) ||
            m_OverlappedRead.hEvent == NULL ||
            m_OverlappedWrite.hEvent == NULL ) {
        DWORD dwError = GetLastError();

        if ( m_OverlappedRead.hEvent != NULL ) CloseHandle ( m_OverlappedRead.hEvent );

        if ( m_OverlappedWrite.hEvent != NULL ) CloseHandle ( m_OverlappedWrite.hEvent );

        CloseHandle ( m_hIDComDev );
        return ( FALSE );
    }

    m_bOpened = TRUE;

    return ( m_bOpened );
}

// �رմ���
BOOL CLeoSerialPort::Close ( void )
{
    if ( !m_bOpened || m_hIDComDev == NULL ) return ( TRUE );

    if ( m_OverlappedRead.hEvent != NULL ) CloseHandle ( m_OverlappedRead.hEvent );

    if ( m_OverlappedWrite.hEvent != NULL ) CloseHandle ( m_OverlappedWrite.hEvent );

    CloseHandle ( m_hIDComDev );
    m_bOpened = FALSE;
    m_hIDComDev = NULL;

    return ( TRUE );
}

// ��ѯ���ܻ��������Ƿ������ݣ�ֻ��ѯ������ȡ��
int CLeoSerialPort::ReadDataWaiting ( void )
{
    if ( !m_bOpened || m_hIDComDev == NULL ) return ( 0 );

    DWORD dwErrorFlags;
    COMSTAT ComStat;

    ClearCommError ( m_hIDComDev, &dwErrorFlags, &ComStat );

    return ( ( int ) ComStat.cbInQue );
}

// ��ȡ���Դ��ڵ�����
int CLeoSerialPort::ReadData ( void *buffer, int limit )
{

	if ( !m_bOpened || m_hIDComDev == NULL ) {return ( 0 );};

    DWORD dwBytes, dwErrorFlags;

    COMSTAT ComStat;
    ClearCommError ( m_hIDComDev, &dwErrorFlags, &ComStat );
    if ( !ComStat.cbInQue ) return ( 0 );

    dwBytes = ( DWORD ) ComStat.cbInQue;
    if ( limit < ( int ) dwBytes ) dwBytes = ( DWORD ) limit;

	if(dwBytes<8){return ( 0 );};

	DCB dcb;	
	GetCommState(m_hIDComDev,&dcb);
	dcb.fDtrControl = 1;//����
	SetCommState(m_hIDComDev,&dcb);
	
    BOOL bStatus  = ReadFile ( m_hIDComDev, buffer, dwBytes, &dwBytes, &m_OverlappedRead );

// ��ѯ�첽��ȡ�Ƿ���ɣ�δ��������ȴ�
    if ( !bStatus ) {
        if ( GetLastError() == ERROR_IO_PENDING ) {
            WaitForSingleObject ( m_OverlappedRead.hEvent, TIMEOUT_READCOMM_EVENT );
            return ( ( int ) dwBytes );
        }

        return ( 0 );
    }

    return ( ( int ) dwBytes );
}

// �򴮿ڷ�������
int CLeoSerialPort::WriteData ( const char *buffer, int size )
{
    if ( !m_bOpened || m_hIDComDev == NULL ) return ( 0 );

    DWORD dwBytes;

    dwBytes = ( DWORD ) size;

	DCB dcb;	
	GetCommState(m_hIDComDev,&dcb);
	dcb.fDtrControl = 0;//����
	SetCommState(m_hIDComDev,&dcb);
	
	BOOL bStatus = WriteFile(m_hIDComDev,buffer,dwBytes,&dwBytes,&m_OverlappedWrite);

	// ��ѯ�첽��ȡ�Ƿ���ɣ�δ��������ȴ�
    if ( !bStatus ) {
        if ( GetLastError() == ERROR_IO_PENDING ) {
				WaitForSingleObject(m_OverlappedWrite.hEvent,TIMEOUT_WRITECOMM_EVENT);
	            return ( ( int ) dwBytes );
        }

        return ( 0 );
    }

    return ( ( int ) dwBytes );
}
