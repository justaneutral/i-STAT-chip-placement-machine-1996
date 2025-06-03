// serialPort.cpp: implementation of the serialPort class.
//
//////////////////////////////////////////////////////////////////////

//#include ""
#include "app.h"
#include "serialPort.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

serialPort::serialPort()
{
	port = 0;
	ReceivedBuffer = CString("");
	BaudTable[0] = CBR_110;
	BaudTable[1] = CBR_300;
	BaudTable[2] = CBR_600;
	BaudTable[3] = CBR_1200;
	BaudTable[4] = CBR_2400;
	BaudTable[5] = CBR_4800;
	BaudTable[6] = CBR_9600;
	BaudTable[7] = CBR_14400;
	BaudTable[8] = CBR_19200;
	BaudTable[9] = CBR_38400;
	BaudTable[10] = CBR_56000;
	BaudTable[11] = CBR_128000;
	BaudTable[12] = CBR_256000;

    ParityTable[0] = NOPARITY;
	ParityTable[1] =	EVENPARITY;
	ParityTable[2] = ODDPARITY;
	ParityTable[3] = MARKPARITY;
	ParityTable[4] =	SPACEPARITY;

	StopBitsTable[0] = ONESTOPBIT;
	StopBitsTable[1] = ONE5STOPBITS;
	StopBitsTable[2] = TWOSTOPBITS;
}

serialPort::~serialPort()
{

}




bool serialPort::CommWatchProcess()
{

   DWORD       dwEvtMask ;
   OVERLAPPED  os ;
   int        nLength ;
   BYTE       abIn[ MAXBLOCK + 1] ;

   memset( &os, 0, sizeof( OVERLAPPED ) ) ;

   // create I/O event used for overlapped read

   os.hEvent = CreateEvent( NULL,    // no security
                            TRUE,    // explicit reset req
                            FALSE,   // initial event reset
                            NULL ) ; // no name
   if (os.hEvent == NULL)
   {
      MessageBox( NULL, "Failed to create event for thread!", "TTY Error!",
                  MB_ICONEXCLAMATION | MB_OK ) ;
      return ( FALSE ) ;
   }

   if (!SetCommMask( COMDEV( ttyinfo ), EV_RXCHAR ))
      return ( FALSE ) ;

   while ( CONNECTED( ttyinfo ) )
   {
      dwEvtMask = 0 ;

      WaitCommEvent( COMDEV( ttyinfo ), &dwEvtMask, NULL );

      if ((dwEvtMask & EV_RXCHAR) == EV_RXCHAR)
      {
         do
         {
           if (nLength = ReadCommBlock((LPSTR) abIn, MAXBLOCK ))
           { 
   	          //Write to 'screen'.	
		      Receive( (LPSTR) abIn, nLength ) ;
           }
         }
         while ( nLength > 0 ) ;
      }
   }

   // get rid of event handle

   CloseHandle( os.hEvent ) ;

   // clear information in structure (kind of a "we're done flag")

   THREADID( ttyinfo ) = 0 ;
   HTHREAD( ttyinfo ) = NULL ;

   return( TRUE ) ;

} // end of CommWatchProcess()






LRESULT serialPort::CreateTTYInfo()
{
   
	InitializeCriticalSection(&rs232);
	
	
	if (NULL == (ttyinfo = 
                   (LPTTYINFO) LocalAlloc( LPTR, sizeof( TTYINFO ) )))
      return ( (LRESULT) -1 ) ;

   // initialize TTY info structure

   COMDEV( ttyinfo )        = 0 ;
   CONNECTED( ttyinfo )     = FALSE ;
   PORT( ttyinfo )          = port ;
   BAUDRATE( ttyinfo )      = CBR_9600 ;
   BYTESIZE( ttyinfo )      = 8 ;
   //FLOWCTRL( ttyinfo )    = FC_RTSCTS ;
   FLOWCTRL( ttyinfo )      = FC_XONXOFF ;
   PARITY( ttyinfo )        = NOPARITY ;
   STOPBITS( ttyinfo )      = ONESTOPBIT ;
   XONXOFF( ttyinfo )       = FALSE ;
   USECNRECEIVE( ttyinfo )  = TRUE ;
   WRITE_OS( ttyinfo ).Offset = 0 ;
   WRITE_OS( ttyinfo ).OffsetHigh = 0 ;
   READ_OS( ttyinfo ).Offset = 0 ;
   READ_OS( ttyinfo ).OffsetHigh = 0 ;

   // create I/O event used for overlapped reads / writes

   READ_OS( ttyinfo ).hEvent = CreateEvent( NULL,    // no security
                                              TRUE,    // explicit reset req
                                              FALSE,   // initial event reset
                                              NULL ) ; // no name
   if (READ_OS( ttyinfo ).hEvent == NULL)
   {
      LocalFree( ttyinfo ) ;
      return ( -1 ) ;
   }
   WRITE_OS( ttyinfo ).hEvent = CreateEvent( NULL,    // no security
                                               TRUE,    // explicit reset req
                                               FALSE,   // initial event reset
                                               NULL ) ; // no name
   if (NULL == WRITE_OS( ttyinfo ).hEvent)
   {
      CloseHandle( READ_OS( ttyinfo ).hEvent ) ;
      LocalFree( ttyinfo ) ;
      return ( -1 ) ;
   }

   return ( (LRESULT) TRUE ) ;

} // end of CreateTTYInfo()


BOOL serialPort::DestroyTTYInfo()
{

   DeleteCriticalSection(&rs232);
	
   // force connection closed (if not already closed)

   if (CONNECTED( ttyinfo ))
      CloseConnection() ;

   // clean up event objects

   CloseHandle( READ_OS( ttyinfo ).hEvent ) ;
   CloseHandle( WRITE_OS( ttyinfo ).hEvent ) ;
   CloseHandle( POSTEVENT( ttyinfo ) ) ;

   LocalFree( ttyinfo ) ;
   return ( TRUE ) ;

} // end of DestroyTTYInfo()


BOOL  serialPort::Send( BYTE bOut )
{
   if (NULL == ttyinfo) return ( FALSE ) ;
   if (!CONNECTED( ttyinfo )) return ( FALSE ) ;

   // a robust app would take appropriate steps if WriteCommBlock failed
   WriteCommBlock((char *)&bOut, 1 ) ;

   return ( TRUE ) ;

} // end of ProcessTTYCharacter()



BOOL  serialPort::Send( LPSTR lpByte , DWORD dwBytesToWrite)
{
   if (NULL == ttyinfo) return ( FALSE ) ;
   if (!CONNECTED( ttyinfo )) return ( FALSE ) ;

   WriteCommBlock(lpByte, dwBytesToWrite ) ;

   return ( TRUE ) ;
}


BOOL  serialPort::OpenConnection()
{
   char       szPort[ 15 ];
   BOOL       fRetVal ;

   HANDLE        hCommWatchThread ;
   DWORD         dwThreadID ;
   COMMTIMEOUTS  CommTimeOuts ;

   if (NULL == ttyinfo) return ( FALSE ) ;

   // load the COM prefix string and append port number

   wsprintf( szPort, "com%d",PORT( ttyinfo ) ) ;

   // open COMM device

   if ((COMDEV( ttyinfo ) =
      CreateFile( szPort, GENERIC_READ | GENERIC_WRITE,
                  0,                    // exclusive access
                  NULL,                 // no security attrs
                  OPEN_EXISTING,
                  FILE_ATTRIBUTE_NORMAL |
                  FILE_FLAG_OVERLAPPED, // overlapped I/O
                  NULL )) == (HANDLE) -1 )
      return ( FALSE ) ;
   else
   {
      // get any early notifications

      SetCommMask( COMDEV( ttyinfo ), EV_RXCHAR ) ;

      // setup device buffers

      SetupComm( COMDEV( ttyinfo ), 4096, 4096 ) ;

      // purge any information in the buffer

      PurgeComm( COMDEV( ttyinfo ), PURGE_TXABORT | PURGE_RXABORT |
                                      PURGE_TXCLEAR | PURGE_RXCLEAR ) ;

      // set up for overlapped I/O

      CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF ;
      CommTimeOuts.ReadTotalTimeoutMultiplier = 0 ;
      CommTimeOuts.ReadTotalTimeoutConstant = 1000 ;
      // CBR_9600 is approximately 1byte/ms. For our purposes, allow
      // double the expected time per character for a fudge factor.
      CommTimeOuts.WriteTotalTimeoutMultiplier = 2*CBR_9600/BAUDRATE( ttyinfo ) ;
      CommTimeOuts.WriteTotalTimeoutConstant = 0 ;
      SetCommTimeouts( COMDEV( ttyinfo ), &CommTimeOuts ) ;
   }

   fRetVal = SetupConnection() ;

   if (fRetVal)
   {
      CONNECTED( ttyinfo ) = TRUE ;

      // Create a secondary thread
      // to watch for an event.

      if (NULL == (hCommWatchThread =
		              CreateThread( (LPSECURITY_ATTRIBUTES) NULL,
                                    0,
                                    (LPTHREAD_START_ROUTINE)CommWatchProc,
                                    (LPVOID) this,
                                    0, &dwThreadID )))
      {
         CONNECTED( ttyinfo ) = FALSE ;
         CloseHandle( COMDEV( ttyinfo ) ) ;
         fRetVal = FALSE ;
      }
      else
      {
         THREADID( ttyinfo ) = dwThreadID ;
         HTHREAD( ttyinfo ) = hCommWatchThread ;

         // assert DTR
         EscapeCommFunction( COMDEV( ttyinfo ), SETDTR ) ;
      }
   }
   else
   {
      CONNECTED( ttyinfo ) = FALSE ;
      CloseHandle( COMDEV( ttyinfo ) ) ;
   }

   return ( fRetVal ) ;

} // end of OpenConnection()



BOOL  serialPort::SetupConnection()
{
   BOOL       fRetVal ;
   BYTE       bSet ;
   DCB        dcb ;
   
   if (NULL == ttyinfo) return ( FALSE ) ;

   dcb.DCBlength = sizeof( DCB ) ;

   GetCommState( COMDEV( ttyinfo ), &dcb ) ;

   dcb.BaudRate = BAUDRATE( ttyinfo ) ;
   dcb.ByteSize = BYTESIZE( ttyinfo ) ;
   dcb.Parity = PARITY( ttyinfo ) ;
   dcb.StopBits = STOPBITS( ttyinfo ) ;

   // setup hardware flow control

   bSet = (BYTE) ((FLOWCTRL( ttyinfo ) & FC_DTRDSR) != 0) ;
   dcb.fOutxDsrFlow = bSet ;
   if (bSet)
      dcb.fDtrControl = DTR_CONTROL_HANDSHAKE ;
   else
      dcb.fDtrControl = DTR_CONTROL_ENABLE ;

   bSet = (BYTE) ((FLOWCTRL( ttyinfo ) & FC_RTSCTS) != 0) ;
	dcb.fOutxCtsFlow = bSet ;
   if (bSet)
      dcb.fRtsControl = RTS_CONTROL_HANDSHAKE ;
   else
      dcb.fRtsControl = RTS_CONTROL_ENABLE ;

   // setup software flow control

   bSet = (BYTE) ((FLOWCTRL( ttyinfo ) & FC_XONXOFF) != 0) ;

   dcb.fInX = dcb.fOutX = bSet ;
   dcb.XonChar = ASCII_XON ;
   dcb.XoffChar = ASCII_XOFF ;
   dcb.XonLim = 100 ;
   dcb.XoffLim = 100 ;

   // other various settings

   dcb.fBinary = TRUE ;
   dcb.fParity = TRUE ;

   fRetVal = SetCommState( COMDEV( ttyinfo ), &dcb ) ;

   return ( fRetVal ) ;

} // end of SetupConnection()


BOOL  serialPort::CloseConnection()
{
   if (NULL == ttyinfo) return ( FALSE ) ;

   // set connected flag to FALSE

   CONNECTED( ttyinfo ) = FALSE ;

   // disable event notification and wait for thread
   // to halt

   SetCommMask( COMDEV( ttyinfo ), 0 ) ;

   // block until thread has been halted

   while(THREADID(ttyinfo) != 0);

   // drop DTR

   EscapeCommFunction( COMDEV( ttyinfo ), CLRDTR ) ;

   // purge any outstanding reads/writes and close device handle

   PurgeComm( COMDEV( ttyinfo ), PURGE_TXABORT | PURGE_RXABORT |
                                   PURGE_TXCLEAR | PURGE_RXCLEAR ) ;
   CloseHandle( COMDEV( ttyinfo ) ) ;

   return ( TRUE ) ;

} // end of CloseConnection()



int  serialPort::ReadCommBlock( LPSTR lpszBlock, int nMaxLength )
{
   BOOL       fReadStat ;
   COMSTAT    ComStat ;
   DWORD      dwErrorFlags;
   DWORD      dwLength;
   DWORD      dwError;
   char       szError[ 10 ] ;
   

   if (NULL == ttyinfo)	return ( FALSE ) ;

   // only try to read number of bytes in queue
   ClearCommError( COMDEV( ttyinfo ), &dwErrorFlags, &ComStat ) ;
   dwLength = min( (DWORD) nMaxLength, ComStat.cbInQue ) ;

   if (dwLength > 0)
   {
      fReadStat = ReadFile( COMDEV( ttyinfo ), lpszBlock,
		                    dwLength, &dwLength, &READ_OS( ttyinfo ) ) ;
      if (!fReadStat)
      {
         if (GetLastError() == ERROR_IO_PENDING)
         {
            OutputDebugString("\n\rIO Pending");
            // We have to wait for read to complete.
            // This function will timeout according to the
            // CommTimeOuts.ReadTotalTimeoutConstant variable
            // Every time it times out, check for port errors
            while(!GetOverlappedResult( COMDEV( ttyinfo ),
               &READ_OS( ttyinfo ), &dwLength, TRUE ))
            {
               dwError = GetLastError();
               if(dwError == ERROR_IO_INCOMPLETE)
                  // normal result if not finished
                  continue;
               else
               {
                  // an error occurred, try to recover
                  wsprintf( szError, "<CE-%u>", dwError ) ;
                  Receive(szError, lstrlen( szError ) ) ;
                  ClearCommError( COMDEV( ttyinfo ), &dwErrorFlags, &ComStat ) ;
                  if (dwErrorFlags > 0)
                  {
	                  wsprintf( szError, "<CE-%u>", dwErrorFlags ) ;
	                  Receive(szError, lstrlen( szError ) ) ;
                  }
                  break;
               }

            }

	      }
         else
         {
            // some other error occurred
            dwLength = 0 ;
            ClearCommError( COMDEV( ttyinfo ), &dwErrorFlags, &ComStat ) ;
            if (dwErrorFlags > 0)
            {
	            wsprintf( szError, "<CE-%u>", dwErrorFlags ) ;
	            Receive( szError, lstrlen( szError ) ) ;
            }
         }
      }
   }

   return ( dwLength ) ;

} // end of ReadCommBlock()



BOOL  serialPort::WriteCommBlock( LPSTR lpByte , DWORD dwBytesToWrite)
{

   BOOL        fWriteStat ;
   DWORD       dwBytesWritten ;
   DWORD       dwErrorFlags;
   DWORD   	   dwError;
   DWORD       dwBytesSent=0;
   COMSTAT     ComStat;
   char        szError[ 128 ] ;


   if (NULL == ttyinfo) return ( FALSE ) ;

   fWriteStat = WriteFile( COMDEV( ttyinfo ), lpByte, dwBytesToWrite,
                           &dwBytesWritten, &WRITE_OS( ttyinfo ) ) ;

   // Note that normally the code will not execute the following
   // because the driver caches write operations. Small I/O requests
   // (up to several thousand bytes) will normally be accepted
   // immediately and WriteFile will return true even though an
   // overlapped operation was specified

   if (!fWriteStat)
   {
      if(GetLastError() == ERROR_IO_PENDING)
      {
         // We should wait for the completion of the write operation
         // so we know if it worked or not

         // This is only one way to do this. It might be beneficial to
         // place the write operation in a separate thread
         // so that blocking on completion will not negatively
         // affect the responsiveness of the UI

         // If the write takes too long to complete, this
         // function will timeout according to the
         // CommTimeOuts.WriteTotalTimeoutMultiplier variable.
         // This code logs the timeout but does not retry
         // the write.

         while(!GetOverlappedResult( COMDEV( ttyinfo ),
            &WRITE_OS( ttyinfo ), &dwBytesWritten, TRUE ))
         {
            dwError = GetLastError();
            if(dwError == ERROR_IO_INCOMPLETE)
            {
               // normal result if not finished
               dwBytesSent += dwBytesWritten;
               continue;
            }
            else
            {
               // an error occurred, try to recover
               wsprintf( szError, "<CE-%u>", dwError ) ;
               Receive( szError, lstrlen( szError ) ) ;
               ClearCommError( COMDEV( ttyinfo ), &dwErrorFlags, &ComStat ) ;
               if (dwErrorFlags > 0)
               {
                  wsprintf( szError, "<CE-%u>", dwErrorFlags ) ;
                  Receive(szError, lstrlen( szError ) ) ;
               }
               break;
            }
         }

         dwBytesSent += dwBytesWritten;

         if( dwBytesSent != dwBytesToWrite )
             wsprintf(szError,"\nProbable Write Timeout: Total of %ld bytes sent", dwBytesSent);
         else
             wsprintf(szError,"\n%ld bytes written", dwBytesSent);

         OutputDebugString(szError);

      }
      else
      {
         // some other error occurred
         ClearCommError( COMDEV( ttyinfo ), &dwErrorFlags, &ComStat ) ;
         if (dwErrorFlags > 0)
         {
            wsprintf( szError, "<CE-%u>", dwErrorFlags ) ;
            Receive( szError, lstrlen( szError ) ) ;
         }
         return ( FALSE );
      }
   }
   return ( TRUE ) ;

} // end of WriteCommBlock()


BOOL serialPort::Receive( LPSTR lpBlock, int nLength)
{
	EnterCriticalSection(&rs232);

	ReceivedBuffer += lpBlock;
	if (ReceivedBuffer.GetLength() > 4096)
	{
	   	ReceivedBuffer = ReceivedBuffer.Right(4096);
	}
	LeaveCriticalSection(&rs232);
	return ( TRUE ) ;
} // end of WriteTTYBlock()


DWORD CALLBACK CommWatchProc( LPVOID lpData)
{
   //must be reenterable....
   if(((serialPort *)lpData)->CommWatchProcess())
	   return 0;
   else
	   return -1;
} // end of CommWatchProc()
