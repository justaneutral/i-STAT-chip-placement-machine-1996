// SMAC.cpp: implementation of the CSMAC class.
//
//////////////////////////////////////////////////////////////////////
//#include <stdio.h>
#include "../stdafx.h"
#include "SMAC.h"

#include "../tty.h"


#include "../globals.h"

#include "../console.h"

//extern application theApp;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSMAC::CSMAC()
{
 	port = (serialPort*) new serialPort;
	
	Connect();

	Init();
}

CSMAC::~CSMAC()
{
    DisConnect();

	delete (serialPort*) port;
}


///methods.


bool CSMAC::Connect()
{
   //GetSharedMem(256,(LPSTR)&m_tty_handle,sizeof(HWND));
   port->port = 2;
   if(port->CreateTTYInfo() != (LRESULT) TRUE) return false;
   if(port->OpenConnection() != TRUE) return false;
	
   return true;
}


bool CSMAC::DisConnect()
{
   return (port->DestroyTTYInfo() == TRUE);
}


bool CSMAC::Init()
{
    current_z = 0.0;
    current_a = 0.0;
	return true;
	return false;
}



void CSMAC::delay(DWORD timeout)
{
	//timeout setup.
	DWORD time_base = GetTickCount();
		
	//poll until condition.
	while( (GetTickCount()-time_base) < timeout );
}



double CSMAC::GetLinearPosition()
{
	char str[80];
	int i, d, signum;
	double dist = 0.0;
	
	char data[4] = "1tp";
	data[3] = 0xd;

	Sleep(2*long(g_machine->smac_delay));
	
	port->ReceivedBuffer.Empty();
	

	if(!port->Send((LPSTR)data,(DWORD)4))
	{ 
		sprintf(str,"SMAC Transmit Error");
		__AbsoluteError(str);
	}
	
	Sleep(300);
	
	dist = 0.0;
	d=0;
	signum = 1;

	while(port->ReceivedBuffer.GetLength() < 5)
	{
		conout("waiting for response from SMAC");
		Sleep(300);
	}
	
	
	for( i=4; i<port->ReceivedBuffer.GetLength() && (d = port->ReceivedBuffer[i]) !='>'; i++ )
	{
	   if( dist == 0.0 &&  d == '-' ) signum = -1;
	   
	   if( d >= '0' && d <= '9' )
	   {
	   	 dist = dist*10 + (d-'0');
	   }
	}
	
	current_z = dist / 5080.0 * signum;	//distance in inches; 1 pitch = 0.005mm.
	
	return current_z;
}





double CSMAC::GetAnglePosition()
{
	char str[80];
	int i, d, signum;
	double ang = 0.0;
	
	char data[4] = "2tp";
	data[3] = 0xd;

	
	Sleep(2*long(g_machine->smac_delay));
	
	port->ReceivedBuffer.Empty();
	

	if(!port->Send((LPSTR)data,(DWORD)4))
	{ 
		sprintf(str,"SMAC Transmit Error");
		__AbsoluteError(str);
	}
	
	Sleep(300);
	
	ang = 0.0;
	d=0;
	signum = 1;
	for( i=4; (d = port->ReceivedBuffer[i]) !='>' && i<port->ReceivedBuffer.GetLength(); i++ )
	{
	   if( ang == 0.0 &&  d == '-' ) signum = -1;
	   
	   if( d >= '0' && d <= '9' )
	   {
	   	 ang = ang*10 + (d-'0');
	   }
	}
	
	current_a = ang / 13.51111111111 * signum;	//angle in degrees; 1 pitch =  0.07401315789474 deg.

	return current_a;
}




char * CSMAC::Send(char *data)
{
	unsigned int i;
	char *str = "";

	i=strlen(data);
	data[i] = 0xd;

	if(!port->Send((LPSTR)data,(DWORD)(i+1)))
	{ 
		sprintf(str,"SMAC Transmit Error");
		__AbsoluteError(str);
	}
		
	return str;
}



bool CSMAC::Homming()
{
   char * status = Send("ms0");
   current_z = 0.0;
   current_a = 0.0;
   return true;
   return false;
}



double CSMAC::restrict45(double &degree)
{
	degree = max(-45.0, min(45.0, degree));
	return degree;
}


double CSMAC::restrict2032mm(double &millimeter)
{
	millimeter = max(0.0, min(20.32, millimeter));
	return millimeter;
}



long CSMAC::Str2Long(char * szString)
{
   long data = 0;
   long signum = 1;
   int i;
      
   for(i=0;szString[i] && i < 80;i++)
   {
   	  switch(szString[i])
	  {
	  case '-': if(i==0) signum = -1;
		  break;
	  case '0': 
	  case '1':
	  case '2':
	  case '3':
	  case '4':
	  case '5':
	  case '6':
	  case '7':
	  case '8':
	  case '9':	data = 10*data + (szString[i]-'0');
		  break;
	  }	  
   }
   return data * signum;     
}


void CSMAC::DegreeRotateMillimeterMove(double degree, double millimeter)
{
	int i;
	char outstrng[100]="xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";

	double Ddcounts = restrict45(degree) * 13.51111111111;
	long Dcounts = ((long)Ddcounts) % 4864;

	double Mdcounts = millimeter * 200.0;
	long Mcounts = (long)Mdcounts;

	i = sprintf(outstrng,"2ma%d,1ma%d,0go",Dcounts,Mcounts);
	Send(outstrng);

	current_z = millimeter / 25.4;
    current_a = degree;

}



void CSMAC::DegreeRotateInchesMove(double degree, double inches)
{
   DegreeRotateMillimeterMove(degree, inches * 25.4);
}

double CSMAC::MillimeterMove(double millimeter)
{
	static char outstring[80] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
	double dcounts = restrict2032mm(millimeter) * 200.0;
	long counts = (long)dcounts;

	sprintf(outstring,"1ma%d,go",counts);
	Send(outstring);
	//strcpy(outstring,sendtosmac("1tp", cfl));
	//return double(Str2Long(outstring) / 200);
	current_z = millimeter / 25.4;
	return 0.0;
}


double CSMAC::InchesMove(double inches)
{
   return MillimeterMove(inches * 25.4) / 25.4;
}




double CSMAC::DegreeRotate(double degree)
{
	char outstring[80] = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx";
	double dcounts = restrict45(degree) * 13.51111111111;
	long counts = ((long)dcounts) % 4864;
	sprintf(outstring,"2ma%d,go",counts);
	Send(outstring);
    current_a = degree;
	return 0.0;
}


double CSMAC::RevolutionRotate(double revolution)
{
   return DegreeRotate(revolution * 360.0) / 360.0;
}

