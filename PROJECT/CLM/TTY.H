#ifdef __rs232__ 


#define USECOMM      // yes, we need the COMM API

//#undef NO_STRICT    // be bold!

//#define HINSTANCE HANDLE

#include <windows.h>
#include <commdlg.h>
#include <string.h>
#include <stdio.h>
#include <io.h>
#include <memory.h>

//#include "ttyresource.h"

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


// global stuff

DWORD    BaudTable[] =
         {
            CBR_110, CBR_300, CBR_600, CBR_1200, CBR_2400,
            CBR_4800, CBR_9600, CBR_14400, CBR_19200, CBR_38400,
            CBR_56000, CBR_128000, CBR_256000
         } ;

DWORD    ParityTable[] =
         {
            NOPARITY, EVENPARITY, ODDPARITY, MARKPARITY, SPACEPARITY
         } ;

DWORD    StopBitsTable[] =
         {
            ONESTOPBIT, ONE5STOPBITS, TWOSTOPBITS
         } ;

// CRT mappings to NT API

#define _fmemset   memset
#define _fmemmove  memmove


DWORD dwBlockSize;
char * szBuffer;


LPTTYINFO ttyinfo;


#endif  //__rs232__


LRESULT CreateTTYInfo() ;
BOOL DestroyTTYInfo() ;
BOOL ProcessTTYCharacter(BYTE) ;
BOOL ProcessTTYCharacters( LPSTR lpByte , DWORD dwBytesToWrite);
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
DWORD PASCAL CommWatchProc( LPSTR ) ;
BOOL WriteTTYBlock( LPSTR lpBlock, int nLength );
BOOL ReadTTYBlock( char *lpBlock );