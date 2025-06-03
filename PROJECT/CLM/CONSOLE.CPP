#include "stdafx.h"
#include "globals.h"

//#define _CONSOLE_


void conout(char *str)
{
#ifdef _CONSOLE_
	
	ULONG n;
	WriteFile(g_out,str,strlen(str),&n,0);

#else
	
	int i;
	for(i=0;i<11;i++) 
	{
	   strcpy(g_wcons.outp[i],g_wcons.outp[i+1]);
	}
	
	for(i=0; i<64 && str[i] && str[i] != 0xa && str[i] != 0xd; i++);
	str[i] = (char)'\0';

	strcpy(g_wcons.outp[11],str);
	
#endif
	CString s;
	char e[3];
	e[0]=0xd;
	e[1]=0xa;
	e[2]=0;

	if(g_text_output) g_text_output->LineScroll(1);
	if(g_text_output) g_text_output->GetWindowText(s);
	str[strlen(str)] = 0xd;
	if(g_text_output) g_text_output->SetWindowText(CString(s+CString(str)+CString(e)));
}



int conin(char *str)
{
	ULONG n;
#ifdef _CONSOLE_
	
	ReadFile(g_in,str,80,&n,0);

#else

	g_Wnd->Invalidate();
	
	kbHit=false;
    
    DWORD t, timeout = GetTickCount() + 30000; // 30 secunds.

    do 
	{
		//doevents();
		t = GetTickCount();
	}
	while(!kbHit && t<timeout);


	strcpy(str,g_wcons.inp[1]);	   //data is in the middle..
	n = strlen(str);

#endif

	return (int)n;
}




void __AbsoluteError(char *error_s)
{
   g_current_error = error_s;
    
   conout("__ERROR__");
   conout(error_s);
   conout("Process will be stopped automaticaly.");
   
   g_Wnd->SendMessage(WM_COMMAND,IDM_PROC_START,0);   
}


/*

void __Error(char *error_s)
{
   //char s[80];
   
   if(strstr(error_s,"vision") || strstr(error_s,"Vision") || strstr(error_s,"VISION")) 
	   g_io.Vision_Lt_On();
   else 					    
	   g_io.Motion_Lt_On();


   g_current_error = error_s;

   g_machine_state.m_error = true;
   
   conout("__ERROR__");
   conout(error_s);
   conout("Fix the Problem and Press Retry Button");
   
   
   while(g_machine_state.m_error)
   {
     //conin(s);
     //if(strstr(s,"<F1>"));
	 Sleep(300);
   }
   g_io.Vision_Lt_Off();
   g_io.Motion_Lt_Off();
   
   g_machine_state.m_error = false;

   Sleep(1000);
}
*/



bool __Error(char *error_s, unsigned int time_out) //time out in milliseconds.
{
   DWORD time;
   bool b;


   if(!machine_is_initialized) 
   { 
	 AfxMessageBox(error_s);
	 return false;
   }
   
   if(strstr(error_s,"vision") || strstr(error_s,"Vision") || strstr(error_s,"VISION")) 
	   g_io.Vision_Lt_On();
   else 					    
	   g_io.Motion_Lt_On();


   g_current_error = error_s;

   g_machine_state.m_error = true;
   
   //conout("__ERROR__");
   conout(error_s);
   //conout("If You Want to Correct the problem - Press Retry Button.");
   
   //set timeout.
   time = GetTickCount();
   b = false;

   while(g_machine_state.m_error /*Cancel isn't pressed*/ && !b /*time isn't out*/)
   {
     //conin(s);
     //if(strstr(s,"<F1>"));
	 Sleep(300);
	 b = (bool)((time_out>0) && ((GetTickCount()-time)>time_out));
   }
   
   g_io.Vision_Lt_Off();
   g_io.Motion_Lt_Off();
   
   g_machine_state.m_error = false;
   
   return b;
}



void __Warning(char *error_s, unsigned int time_out) //time out in milliseconds.
{
   DWORD time;
   bool b;


   if(!machine_is_initialized) 
   { 
	 AfxMessageBox(error_s);
   }
   

   g_current_error = error_s;

   g_machine_state.m_error = true;
   
   conout(error_s);
   
   //set timeout.
   time = GetTickCount();
   b = false;

   while(!b /*time isn't out*/)
   {
	 Sleep(1000);
	 b = (bool)((time_out==0) || ((GetTickCount()-time)>time_out));
   }
   
   g_machine_state.m_error = false;
   g_current_error.Empty();
}
