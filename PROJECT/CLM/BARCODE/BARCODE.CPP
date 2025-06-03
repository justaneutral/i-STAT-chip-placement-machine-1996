// barcode.cpp: implementation of the CBarcode class.
//
//////////////////////////////////////////////////////////////////////
//#include <stdio.h>
#include "../stdafx.h"
#include "barcode.h"


#include "../globals.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBarcode::CBarcode()
{
}

CBarcode::~CBarcode()
{
}


///methods.
CString CBarcode::Get_File_Name()
{
   CString fn;
      
   CFileDialog dlg(TRUE,"vik");
   DWORD status = 0;

   FILE *f = NULL;

   int i = 0;

   //Get file name.
   
   if(g_machine_state.m_wafer_is_loaded)
   {
     fn = g_machine_state.m_file_name;
   }
   else
   {
     fn = CString("");

     for(i=0;i<3 && (fn == CString("") || fn == CString("ERROR"));i++)
	 {
        fn = Read();
	 }
   }
			 
   
   
   
   //error handling.
trynexttime:
   if(fn == CString("") || fn == CString("ERROR") || fn == CString("<cancel>"))
   {
      
	  conout("Wafer file information is invalid.");

	  do 
	  { 
		status = dlg.DoModal();
	  } while(!(status == IDOK || status == IDCANCEL));

	  switch(status)
	  {
	    case IDOK: fn = dlg.GetPathName();
			  break;
	    
		case IDCANCEL:
		      //we need to remove the wafer.
		      fn = _T("<cancel>");
		      return fn;
	  }
   }

   else //good.
   {
	  fn = _T("A:\\") + fn + _T(".VIK");
   }
   
   
   fn.MakeUpper();

   // Check - Open for read (will fail if file "data" does not exist).
   if((fn.Right(4) != ".VIK") || ((f = fopen( fn, "r" )) == NULL)) 
   {   
	  if(__Error("Map file does not exist - try again.",20000))
	  {
	  	fn = _T("");
	  }
   	  else
	  {
	    fn = _T("");
        goto trynexttime;
	  }
   }

   //close.
   if(f) fclose(f);
   
   return fn;
}



void CBarcode::Trigger_On()
{
   g_io.Barcode_Trigger_On();
}


void CBarcode::Trigger_Off()
{
   g_io.Barcode_Trigger_Off();
}


CString & CBarcode::Read()
{
    static char str[100];
	static CString s;
#ifdef _PRINCETON_

	HANDLE comHandle;
	BOOL success;
	DCB dcb;
	DWORD numRead;
	COMMTIMEOUTS timeouts;

	//Trigger off the barcode.
	Trigger_Off();


	// Open the comm port. Can open COM, LPT,
	// or \\\\.\\TELNET
	comHandle = CreateFile("COM1", 
		GENERIC_READ|GENERIC_WRITE, 
		0, 0, OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL, 0);
	if (comHandle == INVALID_HANDLE_VALUE) 
		__Error("<Barcode port (COM1) opening error>",0);
	
	// Get the current settings of the COMM port 
	success = GetCommState(comHandle, &dcb);
	if (!success) __Error("<Barcode port error>",0);
   // Modify the baud rate, etc.
	dcb.BaudRate = 9600;
	dcb.ByteSize = 7;
	dcb.Parity = EVENPARITY;
	dcb.StopBits = ONESTOPBIT;

	// Apply the new comm port settings
	success = SetCommState(comHandle, &dcb);
	if (!success) __Error("<Barcode port configuration error>",0);

	// Change the ReadIntervalTimeout so that
	// ReadFile will return immediately. See
	// help file
	timeouts.ReadIntervalTimeout = MAXDWORD; 
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts( comHandle, &timeouts );

	// Set the Data Terminal Ready line
	EscapeCommFunction(comHandle, SETDTR);

	// Trigger Bar Code reader.
	Trigger_On();

	Sleep(300);

	// Trigger Bar Code reader off.
	Trigger_Off();


	//Read the data from bar code reader.
	success = ReadFile(comHandle,str,100,&numRead, 0);
	if (!success) __Error("error in barcode reader - could not read data.",0);

	// Print the string received
	str[numRead] = '\0';
	
	conout(str);
	
	// Clear the DTR line
	EscapeCommFunction(comHandle, CLRDTR);

	CloseHandle(comHandle);  

#else

	strcpy(str,"ERROR");

#endif
	s = CString(str);
	return s;
}
