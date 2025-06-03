// serialPort.h: interface for the serialPort class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERIALPORT_H__613043B0_FB14_11D1_8C7C_00C04FAD4034__INCLUDED_)
#define AFX_SERIALPORT_H__613043B0_FB14_11D1_8C7C_00C04FAD4034__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#define USECOMM 
#include <windows.h>
#include <commdlg.h>
#include <string.h>
#include <stdio.h>
#include <io.h>
#include <memory.h>

// constant definitions
#define GWL_NPTTYINFO        0
#define TTYEXTRABYTES        sizeof( LONG )
#define ABOUTDLG_USEBITMAP  1
#define ATOM_TTYINFO       0x100

// hard coded maximum number of ports for device under Win32
#define MAXPORTS        4

// terminal size
#define MAXROWS         25
#define MAXCOLS         80
#define MAXBLOCK        80
#define MAXLEN_TEMPSTR  81
#define RXQUEUE         4096
#define TXQUEUE         4096

// cursor states
#define CS_HIDE         0x00
#define CS_SHOW         0x01

// Flow control flags
#define FC_DTRDSR       0x01
#define FC_RTSCTS       0x02
#define FC_XONXOFF      0x04

// ascii definitions
#define ASCII_BEL       0x07
#define ASCII_BS        0x08
#define ASCII_LF        0x0A
#define ASCII_CR        0x0D
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

// data structures
typedef struct tagTTYINFO
{
   HANDLE  idComDev ;
   BYTE    bPort;
   BOOL    fConnected, fXonXoff, fLocalEcho, fNewLine, fAutoWrap,
           fUseCNReceive, fDisplayErrors;
   BYTE    bByteSize, bFlowCtrl, bParity, bStopBits ;
   DWORD   dwBaudRate ;
   int     Size;
   HANDLE      hPostEvent, hWatchThread, hWatchEvent ;
   DWORD       dwThreadID ;
   OVERLAPPED  osWrite, osRead ;

} TTYINFO, *LPTTYINFO ;

// macros ( for easier readability )
#define COMDEV( x ) (x -> idComDev)
#define PORT( x )   (x -> bPort)
#define CONNECTED( x ) (x -> fConnected)
#define XONXOFF( x ) (x -> fXonXoff)
#define LOCALECHO( x ) (x -> fLocalEcho)
#define BYTESIZE( x ) (x -> bByteSize)
#define FLOWCTRL( x ) (x -> bFlowCtrl)
#define PARITY( x ) (x -> bParity)
#define STOPBITS( x ) (x -> bStopBits)
#define BAUDRATE( x ) (x -> dwBaudRate)
#define SIZE( x ) (x -> Size)
#define USECNRECEIVE( x ) (x -> fUseCNReceive)
#define POSTEVENT( x ) (x -> hPostEvent)
#define HTHREAD( x ) (x -> hWatchThread)
#define THREADID( x ) (x -> dwThreadID)
#define WRITE_OS( x ) (x -> osWrite)
#define READ_OS( x ) (x -> osRead)

// CRT mappings to NT API
#define _fmemset   memset
#define _fmemmove  memmove




class serialPort  
{

friend DWORD CALLBACK CommWatchProc( LPVOID );

public:
	serialPort();
	virtual ~serialPort();

    DWORD    BaudTable[13];
    DWORD    ParityTable[5];
    DWORD    StopBitsTable[4];
	DWORD dwBlockSize;
    int port;
	char * szBuffer;
	CRITICAL_SECTION rs232;
	CString ReceivedBuffer;
	LPTTYINFO ttyinfo;

	bool CommWatchProcess();
    LRESULT CreateTTYInfo() ;
    BOOL DestroyTTYInfo() ;
    BOOL Send(BYTE) ;
    BOOL Send( LPSTR lpByte , DWORD dwBytesToWrite);
    int ReadCommBlock(LPSTR, int ) ;
    BOOL WriteCommBlock(LPSTR, DWORD);
    BOOL OpenConnection() ;
    BOOL SetupConnection() ;
    BOOL CloseConnection() ;
    VOID GoModalDialogBoxParam( HINSTANCE, LPCSTR, HWND, DLGPROC, LPARAM ) ;
    VOID FillComboBox( HINSTANCE, HWND, int, DWORD NEAR *, WORD, DWORD ) ;
    BOOL SelectTTYFont( HWND ) ;
    BOOL SettingsDlgInit( HWND ) ;
    BOOL SettingsDlgTerm( HWND ) ;
    BOOL PASCAL SettingsDlgProc( HWND, UINT, WPARAM, LPARAM ) ;
    BOOL Receive( LPSTR lpBlock, int nLength );
};

#endif // !defined(AFX_SERIALPORT_H__613043B0_FB14_11D1_8C7C_00C04FAD4034__INCLUDED_)
