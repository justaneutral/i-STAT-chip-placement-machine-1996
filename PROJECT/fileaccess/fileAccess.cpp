#include "..\\clm\\stdafx.h"
#include "fileaccess.h"
/*
#include <fstream.h>

#include "fileaccess.h"

bool read_data(char * filename, machina_state_DS * cs)
{
	ifstream f(filename, ios::binary);
  	if(!f) return false;
    f.read((char*)cs,sizeof(machina_state_DS));
    return true;

	open()

}


bool write_data(char * filename, machina_state_DS * cs)
{
	ofstream f(filename, ios::binary);
  	if(!f) return false;
    f.write((char*)cs,sizeof(machina_state_DS));
    return true;
}


#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>


bool read_data(char * filename, machina_state_DS * cs)
{
   int fh;
   bool b;
   unsigned int nbytes = sizeof(machina_state_DS), bytesread;

   // Open file for input:
   if( (fh = _open( filename, _O_RDONLY | _O_BINARY)) == -1 )
     return false;
   // Read in input:
   if( ( bytesread = _read( fh, (char*)cs, nbytes ) ) <= 0 )
    b=false;
   else b = true;  
   _close( fh );
   return true;
}


bool write_data(char * filename, machina_state_DS * cs)
{
   int fh;
   bool b;
   unsigned byteswritten;

   if( (fh = _open( filename, _O_RDWR | _O_CREAT | _O_TRUNC | _O_BINARY, 
                               _S_IREAD | _S_IWRITE )) != -1 )
   {
      if(( byteswritten = _write( fh, (char*)cs, 
		     sizeof( machina_state_DS ))) == -1 )
         b = false;
	  else
	     b = (bool)(FlushFileBuffers((void *)fh) == TRUE);
      _close( fh );
   }
   return b;
}
*/


bool write_data(char * filename, machina_state_DS * cs)
{
   HANDLE f;
   BOOL b;
   DWORD nbytes = sizeof(machina_state_DS), bytesread;

   // Open file for input:
   
   f = CreateFile(filename,GENERIC_WRITE,0,0,CREATE_ALWAYS,0,0);

   if(f==INVALID_HANDLE_VALUE)
   {
      //__Error("snap file for writing creation error");
	  //conout(GetLastError());
	  return false;
   }
   
   
   b = WriteFile(f,cs,sizeof(machina_state_DS),&bytesread,0);

   if(!b)
   {
      //__Error("snap file writing error");
	  //conout(GetLastError());
   }

   CloseHandle(f);

   return b!=FALSE;
}



bool read_data(char * filename, machina_state_DS * cs)
{
   HANDLE f;
   BOOL b;
   DWORD nbytes = sizeof(machina_state_DS), bytesread;

   // Open file for input:
   
   f = CreateFile(filename,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);

   if(f==INVALID_HANDLE_VALUE)
   {
      //__Error("snap file for reading creation error");
	  //conout(GetLastError());
	  return false;
   }
   
   
   b = ReadFile(f,cs,sizeof(machina_state_DS),&bytesread,0);

   if(!b)
   {
      //__Error("snap file reading error");
	  //conout(GetLastError());
   }
   
   CloseHandle(f);

   return b != FALSE;
}
