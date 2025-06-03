// QCICommands.cpp: implementation of the CQCICommands class.
//
//////////////////////////////////////////////////////////////////////
#define _WINNT

#include "../stdafx.h"

#include "QCICommands.h"
#include "QCINBS10CommDevice.h"

//globals.
#include "../globals.h"

#if defined _PRINCETON_
extern CQCINBS10CommDevice g_Comm_Dev;
#endif


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQCICommands::CQCICommands()
{

#if defined _PRINCETON_	
    assert(&g_Comm_Dev);
#endif

	m_status = NULL;
	m_increment = 0;
	m_count = 0;

	m_tx_packet = new char[256];
	m_rx_packet = new BYTE[256];
	motor = _T("0");


    assert(m_tx_packet);
	assert(m_rx_packet);

}

CQCICommands::~CQCICommands()
{
	if(m_tx_packet) delete m_tx_packet;
	if(m_rx_packet) delete m_rx_packet;
}


char * CQCICommands::Init()
{
#if defined _PRINCETON_
	assert(&g_Comm_Dev);
    // Initialize Comm Device
    char * statusStr = g_Comm_Dev.Init();
    assert(statusStr[0] == NULL); // error string will be NULL for success
	stop_flag = 0;
    stop_state = 0;
	return statusStr;
#else
	return "";
#endif
}




BYTE CQCICommands::tx_check_summ()
{
   int i;
   BYTE summ = 0;
   for(i=0;i<(m_count-1);i++) summ += (BYTE)(m_tx_packet[i]);
   summ = ~summ + 1;
   return summ;
}

BOOL CQCICommands::good_tx_check_summ()
{
   if(m_count && (BYTE)m_tx_packet[m_count-1] == tx_check_summ()) 
	 return TRUE;
   return FALSE;
}



BYTE CQCICommands::rx_check_summ()
{
   int i;
   BYTE summ = 0;
   for(i=0;i<(m_count-1);i++) summ += (BYTE)(m_rx_packet[i]);
   summ = ~summ + 1;
   return summ;
}

BOOL CQCICommands::good_rx_check_summ()
{
   if(m_count && (BYTE)m_rx_packet[m_count-1] == rx_check_summ()) 
	 return TRUE;
   return FALSE;
}




BOOL CQCICommands::good_address()
{
   if((BYTE)m_rx_packet[0] == (BYTE)m_tx_packet[0]) return TRUE;
   return FALSE;
}


BOOL CQCICommands::good_packet()
{

  if(good_rx_check_summ() && good_address()) return TRUE;
  return FALSE;

}


BOOL CQCICommands::ack()
{
   if(m_count != 3) return FALSE;
   if(m_rx_packet[1] != 0x80) return FALSE;
   return TRUE;
}


int CQCICommands::nak()
{  
   if(m_count == 7 && m_rx_packet[3] != 3) return 0;
   if(m_rx_packet[2] != 0xff) return 0;
   if(m_rx_packet[3] < 1 || m_rx_packet[3] > 8) return 0;
   if(!m_tx_packet[1]) return 0;
   if((BYTE)(m_rx_packet[4] - m_tx_packet[2])) return 0;
   return m_rx_packet[3];
}


BOOL CQCICommands::dp()
{  
   if((BYTE)m_count != (3 + (BYTE)m_rx_packet[1]) ) return FALSE;
   if((BYTE)m_rx_packet[2] != (BYTE)m_tx_packet[2]) return FALSE;
   return TRUE;
}


int CQCICommands::dp_length()
{
   if(!dp()) return 0;
   if((BYTE)m_rx_packet[1] > 0) 
	 return (int)((BYTE)m_rx_packet[1]);
   return 0;
}



BYTE CQCICommands::dp_data(int index)
{
   if(!dp()) return 0;
   if(index > dp_length()) return 0;
   return (m_rx_packet[3 + index]); 
}




unsigned short CQCICommands::prp()
{  
   if((BYTE)m_rx_packet[1] != (BYTE)3) return 0;
   if((BYTE)m_rx_packet[2] != (BYTE)0) return 0;
   
   WORD s_w = m_rx_packet[3];
   		s_w = (s_w << 8) & 0xff00;
        s_w = (s_w | (BYTE)m_rx_packet[4]);
   
   return (unsigned short)s_w;
}


#if defined _PRINCETON_

int CQCICommands::ExchangePackets()
{
    switch(m_tx_packet[0])
	{
	case 1: motor = _T("1"); break;
	case 2: motor = _T("2"); break;
	case 3: motor = _T("3"); break;
	case 4: motor = _T("4"); break;
	case 5: motor = _T("5"); break;
	case 6: motor = _T("6"); break;
	case 7: motor = _T("7"); break;
	}

	//DWORD time;
	char *last_stat = NULL;
	
	// input check
	if ( !good_tx_check_summ() ) 
	{
		AfxMessageBox("RS-485: bad outgoing packet check summ");
	    return -3;
	}

	//time test begin.
	//time = GetTickCount();
	
	//reinit
	//m_status = Init();
	//if(*m_status) AfxMessageBox("RS-485: Reinit error");
	
	//send.
	last_stat = m_status;
	m_status = g_Comm_Dev.TxPacket(m_count, m_tx_packet);

	//time test end.
	//time = GetTickCount() - time;
	//if(time)
	//{
	//	sprintf(s,"%d ms\n",(int)time);
	//    conout(s);
	//}

	//check sending.
	if(m_status[0]) 
	{
	    conout("RS-485: transmit packet error");
		return -2;
    }
	
	//if(command_mode()) return 0;
	
	
	//Get respose.
	m_count = 0;
	m_status = g_Comm_Dev.RxPacket(256,100, m_count);
	//if(*m_status == 0);

	m_increment = g_Comm_Dev.GetRxIncrement();
	m_count = m_increment ? m_count/m_increment : 0;
	  
	for(int i=0;i<m_count;i++) 
		m_rx_packet[i] = g_Comm_Dev.RxBuff(i*m_increment);

	//Check response.
	if(!good_packet()) 
	{
		char s[80];
		sprintf(s,"RS-485: no / bad receive packet, address = %d ( check cable)", m_tx_packet[0]);
		__Error(s,0);
		//AfxMessageBox(s);
		return -1;
    }
	return 0;
}
#else
int CQCICommands::ExchangePackets()
{
	return 0;
}
#endif


char s_status[80];
	
#if defined _PRINCETON_
char * CQCICommands::Communicate()
{
	
	int status = ExchangePackets();

	//check
	if(!status) 
	{
	  //parse.
	  if(ack()) return "";
	
	  if(int n = nak())
	  {
		sprintf( s_status, "NAK: error code %d", n);
		return s_status;
	  }
	}
	
	sprintf( s_status, "Comm. Error - code: %d", status);
	return s_status;
}
#else
char * CQCICommands::Communicate()
{
	return "";
}
#endif


void CQCICommands::doevents()
{	
   MSG qMsg;
   if(::GetMessage (&qMsg,NULL,0,0))
   {
	  ::TranslateMessage (&qMsg);
	  ::DispatchMessage (&qMsg);
   }
}






//////////////////////////////////////for///user/////////////


char * CQCICommands::Set_Identity(unsigned char address, 
	 	                          unsigned char group_identifier,
					              unsigned char identifier)
{
	m_count = 6;

	m_tx_packet[0] = (BYTE)address;
	m_tx_packet[1] = (BYTE)0x03; //length
	m_tx_packet[2] = (BYTE)0x9b; //command;
	m_tx_packet[3] = (BYTE)group_identifier;
	m_tx_packet[4] = (BYTE)identifier;
	m_tx_packet[5] = tx_check_summ();

	return Communicate();
}




char * CQCICommands::Read_Symbolic(unsigned char address,
								   unsigned short sym_loc,
								   long & return_data)
{
	static char m_status[80];

	m_count = 6;

	m_tx_packet[0] = (BYTE)address;
	m_tx_packet[1] = (BYTE)0x03; //length
	m_tx_packet[2] = (BYTE)0x0c; //command.
	m_tx_packet[3] = (BYTE)((WORD)sym_loc >> 8);
	m_tx_packet[4] = (BYTE)((WORD)sym_loc);
	m_tx_packet[5] = tx_check_summ();

	
	return_data = 0;

	//send / receive.
	int status = ExchangePackets();

	//check
	if(!status) 
	{
	  //parse.
	  if(ack()) return &"";
	
	  if(dp())
	  {
	  	for(int i=0; i<4;i++)
		{
		   return_data <<= 8; 
		   return_data &= 0xffffff00;
		   return_data |= dp_data(i);
		}
		return &"";
	  }
	  
	  if(int n = nak())
	  {
		sprintf( m_status, "NAK: error code %d", n);
		return m_status;
	  }
	}
	
	sprintf( m_status, "Comm. Error - code: %d", status);
	return m_status;
}





char * CQCICommands::Poll(unsigned char address, 
						  unsigned short & status_word)
{
	static char m_status[80];
	
	m_count = 3;

	m_tx_packet[0] = (BYTE)address;
	m_tx_packet[1] = (BYTE)0x00; //length
	m_tx_packet[2] = tx_check_summ();

	
	status_word = 0;

	//send / receive.
	int status = ExchangePackets();

	//check
	if(!status) 
	{
	  //parse.
	  if(ack()) return "";
	
	  unsigned short s_w = prp();
	  if(s_w)
	  {
	  	status_word = s_w;
		return &"";
	  }
	  
	  if(int n = nak())
	  {
		sprintf( m_status, "NAK: error code %d", n);
		return m_status;
	  }
	}
	
	sprintf( m_status, "Comm. Error - code: %d", status);
	return m_status;
}




char * CQCICommands::Clear_Poll(unsigned char address, 
	  				            unsigned short status_word)
{
	m_count = 6;

	m_tx_packet[0] = (BYTE)address;
	m_tx_packet[1] = (BYTE)0x03; //length
	m_tx_packet[2] = (BYTE)0x01; //command;
	m_tx_packet[3] = (BYTE)((WORD)status_word >> 8);
	m_tx_packet[4] = (BYTE)((WORD)status_word);
	m_tx_packet[5] = tx_check_summ();

	return Communicate();
}




char * CQCICommands::Zero_Target_Position(unsigned char address)
{
	m_count = 4;

	m_tx_packet[0] = (BYTE)address;
	m_tx_packet[1] = (BYTE)0x01; //length
	m_tx_packet[2] = (BYTE)0x91; //command;
	m_tx_packet[3] = tx_check_summ();

	return Communicate();
}


char * CQCICommands::Set_Target_Position(unsigned char address)
{
	m_count = 4;

	m_tx_packet[0] = (BYTE)address;
	m_tx_packet[1] = (BYTE)0x01; //length
	m_tx_packet[2] = (BYTE)0x92; //command;
	m_tx_packet[3] = tx_check_summ();

	return Communicate();
}




char * CQCICommands::Stop(unsigned char address,
						  long acceleration)
{
	m_count = 8;

	m_tx_packet[0] = (BYTE)address;
	m_tx_packet[1] = (BYTE)0x5; //length
	m_tx_packet[2] = (BYTE)0x03; //command;
  	m_tx_packet[3] = (BYTE)((DWORD)acceleration >> 24);
	m_tx_packet[4] = (BYTE)((DWORD)acceleration >> 16);
	m_tx_packet[5] = (BYTE)((DWORD)acceleration >> 8);
	m_tx_packet[6] = (BYTE)((DWORD)acceleration);
	m_tx_packet[7] = tx_check_summ();

	return Communicate();
}





char * CQCICommands::Motor_Torque(unsigned char address, 
						          unsigned short closed_hold,
						          unsigned short closed_running,
						          unsigned short open_hold,
						          unsigned short open_running)
{
	m_count = 12;

	m_tx_packet[0] = (BYTE)address;
	m_tx_packet[1] = (BYTE)0x09; //length
	m_tx_packet[2] = (BYTE)0x95; //command;
	m_tx_packet[3] = (BYTE)((WORD)closed_hold >> 8);
	m_tx_packet[4] = (BYTE)((WORD)closed_hold);
	m_tx_packet[5] = (BYTE)((WORD)closed_running >> 8);
	m_tx_packet[6] = (BYTE)((WORD)closed_running);
	m_tx_packet[7] = (BYTE)((WORD)open_hold >> 8);
	m_tx_packet[8] = (BYTE)((WORD)open_hold);
	m_tx_packet[9] = (BYTE)((WORD)open_running >> 8);
	m_tx_packet[10] = (BYTE)((WORD)open_running);
	m_tx_packet[11] = tx_check_summ();

	return Communicate();
}



char * CQCICommands::Error_Limits(unsigned char address, 
						          short moving_limit,
						          short hold_limit,
						          unsigned short delay_stopped)
{

	m_count = 10;

	m_tx_packet[0] = (BYTE)address;
	m_tx_packet[1] = (BYTE)0x07; //length
	m_tx_packet[2] = (BYTE)0x97; //command;
	m_tx_packet[3] = (BYTE)((WORD)moving_limit >> 8);
	m_tx_packet[4] = (BYTE)((WORD)moving_limit);
	m_tx_packet[5] = (BYTE)((WORD)hold_limit >> 8);
	m_tx_packet[6] = (BYTE)((WORD)hold_limit);
	m_tx_packet[7] = (BYTE)((WORD)delay_stopped >> 8);
	m_tx_packet[8] = (BYTE)((WORD)delay_stopped);
	m_tx_packet[9] = tx_check_summ();

	return Communicate();
}





////////////////////////////////////////////////
char * CQCICommands::DAV(unsigned char address, 
						 long distance,
						 unsigned long acceleration,
						 unsigned long velocity/*, 
						 WORD stop_flag, 
						 WORD stop_state*/)
{

	m_count = 20;

	m_tx_packet[0] = (BYTE)address;
	m_tx_packet[1] = (BYTE)0x11; //length
	m_tx_packet[2] = (BYTE)0x87; //command;
	m_tx_packet[3] = (BYTE)((DWORD)distance >> 24);
	m_tx_packet[4] = (BYTE)((DWORD)distance >> 16);
	m_tx_packet[5] = (BYTE)((DWORD)distance >> 8);
	m_tx_packet[6] = (BYTE)((DWORD)distance);
  	m_tx_packet[7] = (BYTE)((DWORD)acceleration >> 24);
	m_tx_packet[8] = (BYTE)((DWORD)acceleration >> 16);
	m_tx_packet[9] = (BYTE)((DWORD)acceleration >> 8);
	m_tx_packet[10] = (BYTE)((DWORD)acceleration);
  	m_tx_packet[11] = (BYTE)((DWORD)velocity >> 24);
	m_tx_packet[12] = (BYTE)((DWORD)velocity >> 16);
	m_tx_packet[13] = (BYTE)((DWORD)velocity >> 8);
	m_tx_packet[14] = (BYTE)((DWORD)velocity);
	m_tx_packet[15] = (BYTE)((WORD)stop_flag >> 8);
	m_tx_packet[16] = (BYTE)((WORD)stop_flag);
	m_tx_packet[17] = (BYTE)((WORD)stop_state >> 8);
	m_tx_packet[18] = (BYTE)((WORD)stop_state);
	m_tx_packet[19] = tx_check_summ();

	return Communicate();
}




char * CQCICommands::LAV(unsigned char address, 
						 long location,
						 unsigned long acceleration,
						 unsigned long velocity/*, 
						 WORD stop_flag, 
						 WORD stop_state*/)
{

	m_count = 20;

	m_tx_packet[0] = (BYTE)address;
	m_tx_packet[1] = (BYTE)0x11; //length
	m_tx_packet[2] = (BYTE)0x86; //command;
	m_tx_packet[3] = (BYTE)((DWORD)location >> 24);
	m_tx_packet[4] = (BYTE)((DWORD)location >> 16);
	m_tx_packet[5] = (BYTE)((DWORD)location >> 8);
	m_tx_packet[6] = (BYTE)((DWORD)location);
  	m_tx_packet[7] = (BYTE)((DWORD)acceleration >> 24);
	m_tx_packet[8] = (BYTE)((DWORD)acceleration >> 16);
	m_tx_packet[9] = (BYTE)((DWORD)acceleration >> 8);
	m_tx_packet[10] = (BYTE)((DWORD)acceleration);
  	m_tx_packet[11] = (BYTE)((DWORD)velocity >> 24);
	m_tx_packet[12] = (BYTE)((DWORD)velocity >> 16);
	m_tx_packet[13] = (BYTE)((DWORD)velocity >> 8);
	m_tx_packet[14] = (BYTE)((DWORD)velocity);
	m_tx_packet[15] = (BYTE)((WORD)stop_flag >> 8);
	m_tx_packet[16] = (BYTE)((WORD)stop_flag);
	m_tx_packet[17] = (BYTE)((WORD)stop_state >> 8);
	m_tx_packet[18] = (BYTE)((WORD)stop_state);
	m_tx_packet[19] = tx_check_summ();

	return Communicate();
}





char * CQCICommands::Home(unsigned char address,
			              long & position,
						  //long gap,
						  long distance,
			              unsigned long acceleration,
			              unsigned long velocity,
						  unsigned long timeout,
			              unsigned short closed_hold_torque,
			              unsigned short closed_running_torque,
			              unsigned short open_hold_torque,
			              unsigned short open_running_torque,
			              short moving_limit,
			              short hold_limit,
			              unsigned short delay_stopped)
{	
   
   long location;

   char *m_status = NULL;

   //clear stop conditions.
   stop_flag  = 0;
   stop_state = 0;

   position = 0;

   //m_status = Zero_Target_Position(address);
   //if(*m_status) return m_status;
   
   m_status = Distance(address,position,-distance,
	        acceleration,velocity,
	        timeout,closed_hold_torque,closed_running_torque,
			open_hold_torque,open_running_torque,
			moving_limit,hold_limit,delay_stopped);
   if(*m_status) return m_status;

   
   //m_status = Zero_Target_Position(address);
   //if(*m_status) return m_status;


   //set stop conditions to stop on the flag.
   stop_flag  = 0x0008;
   stop_state = 0x0008;
   
   //go to flag.
   m_status = Distance(address,position,distance,
	        acceleration,velocity,
	        timeout,closed_hold_torque,closed_running_torque,
			open_hold_torque,open_running_torque,
			moving_limit,hold_limit,delay_stopped);
   if(*m_status) return m_status;

   //clear stop conditions.
   stop_flag  = 0;
   stop_state = 0;
   

   //get last index position.
   m_status = Read_Symbolic(address, 2, position);
   if(*m_status) return m_status;

   //go to this position.
   m_status = Location(address,location,position,
	        acceleration,velocity,
	        timeout,closed_hold_torque,closed_running_torque,
			open_hold_torque,open_running_torque,
			moving_limit,hold_limit,delay_stopped);
   if(*m_status) return m_status;
   
   
   //go to the zero point.
   m_status = Distance(address,position,
	        long(g_machine->home_correction_start[address-1] * g_Axes.gear_ratio(address)),
	        acceleration,velocity,
	        timeout,closed_hold_torque,closed_running_torque,
			open_hold_torque,open_running_torque,
			moving_limit,hold_limit,delay_stopped);
   if(*m_status) return m_status;


   // fixing the ZERO.
   // zero target & position.
   m_status = Zero_Target_Position(address);
   if(*m_status) return m_status;

   
   
   // other border.
   m_status = Distance(address,position,distance,
	        acceleration,velocity,
	        timeout,closed_hold_torque,closed_running_torque,
			open_hold_torque,open_running_torque,
			moving_limit,hold_limit,delay_stopped);
   if(*m_status) return m_status;


   //m_status = Zero_Target_Position(address);
   //if(*m_status) return m_status;

   //set stop conditions to stop on the flag.
   stop_flag  = 0x0008;
   stop_state = 0x0008;
   
   //go to flag.
   m_status = Distance(address,position,-distance,
	        acceleration,velocity,
	        timeout,closed_hold_torque,closed_running_torque,
			open_hold_torque,open_running_torque,
			moving_limit,hold_limit,delay_stopped);
   if(*m_status) return m_status;

   //clear stop conditions.
   stop_flag  = 0;
   stop_state = 0;
   

   //get last index position.
   m_status = Read_Symbolic(address, 2, position);
   if(*m_status) return m_status;

   //go to this position.
   m_status = Location(address,location,position,
	        acceleration,velocity,
	        timeout,closed_hold_torque,closed_running_torque,
			open_hold_torque,open_running_torque,
			moving_limit,hold_limit,delay_stopped);
   if(*m_status) return m_status;


   //go to the second zero point.
   m_status = Distance(address,position,
	        long(g_machine->home_correction_end[address-1] * g_Axes.gear_ratio(address)),
	        acceleration,velocity,
	        timeout,closed_hold_torque,closed_running_torque,
			open_hold_torque,open_running_torque,
			moving_limit,hold_limit,delay_stopped);
   if(*m_status) return m_status;

   return m_status;
}




char * CQCICommands::Home5(long & position,
						  //long gap,
						  long distance,
			              unsigned long acceleration,
			              unsigned long velocity,
						  unsigned long timeout,
			              unsigned short closed_hold_torque,
			              unsigned short closed_running_torque,
			              unsigned short open_hold_torque,
			              unsigned short open_running_torque,
			              short moving_limit,
			              short hold_limit,
			              unsigned short delay_stopped)
{	
   
   long location;

   //clear stop conditions.
   stop_flag  = 0;
   stop_state = 0;

   position = 0;

   //m_status = Zero_Target_Position(address);
   //if(*m_status) return m_status;
   
   m_status = Distance(5,position,-distance,
	        acceleration,velocity,
	        timeout,closed_hold_torque/2,closed_running_torque/2,
			open_hold_torque/2,open_running_torque/2,
			moving_limit,hold_limit,delay_stopped);
   if(*m_status) return m_status;

   
   //m_status = Zero_Target_Position(address);
   //if(*m_status) return m_status;


   //set stop conditions to stop on the flag.
   stop_flag  = 0x0008;
   stop_state = 0x0008;
   
   //go to flag.
   m_status = Distance(5,position,distance,
	        acceleration,velocity,
	        timeout,closed_hold_torque*2,closed_running_torque*2,
			open_hold_torque*2,open_running_torque*2,
			moving_limit,hold_limit,delay_stopped);
   if(*m_status) return m_status;

   //clear stop conditions.
   stop_flag  = 0;
   stop_state = 0;
   

   //get last index position.
   m_status = Read_Symbolic(5, 2, position);
   if(*m_status) return m_status;

   //go to this position.
   m_status = Location(5,location,position,
	        acceleration,velocity,
	        timeout,closed_hold_torque*2,closed_running_torque*2,
			open_hold_torque*2,open_running_torque*2,
			moving_limit,hold_limit,delay_stopped);
   if(*m_status) return m_status;
   
   
   //go to the zero point.
   m_status = Distance(5,position,
	        long(g_machine->home_correction_start[5-1] * g_Axes.gear_ratio(5)),
	        acceleration,velocity,
	        timeout,closed_hold_torque*2,closed_running_torque*2,
			open_hold_torque*2,open_running_torque*2,
			moving_limit,hold_limit,delay_stopped);
   if(*m_status) return m_status;


   // fixing the ZERO.
   // zero target & position.
   m_status = Zero_Target_Position(5);
   if(*m_status) return m_status;

   
   
   // other border.
   m_status = Distance(5,position,distance,
	        acceleration,velocity,
	        timeout,closed_hold_torque,closed_running_torque,
			open_hold_torque,open_running_torque,
			moving_limit,hold_limit,delay_stopped);
   if(*m_status) return m_status;


   //m_status = Zero_Target_Position(address);
   //if(*m_status) return m_status;

   //set stop conditions to stop on the flag.
   stop_flag  = 0x0008;
   stop_state = 0x0008;
   
   //go to flag.
   m_status = Distance(5,position,-distance,
	        acceleration,velocity,
	        timeout,closed_hold_torque,closed_running_torque,
			open_hold_torque,open_running_torque,
			moving_limit,hold_limit,delay_stopped);
   if(*m_status) return m_status;

   //clear stop conditions.
   stop_flag  = 0;
   stop_state = 0;
   

   //get last index position.
   m_status = Read_Symbolic(5, 2, position);
   if(*m_status) return m_status;

   //go to this position.
   m_status = Location(5,location,position,
	        acceleration,velocity,
	        timeout,closed_hold_torque,closed_running_torque,
			open_hold_torque,open_running_torque,
			moving_limit,hold_limit,delay_stopped);
   if(*m_status) return m_status;


   //go to the second zero point.
   m_status = Distance(5,position,
	        long(g_machine->home_correction_end[5-1] * g_Axes.gear_ratio(5)),
	        acceleration,velocity,
	        timeout,closed_hold_torque,closed_running_torque,
			open_hold_torque,open_running_torque,
			moving_limit,hold_limit,delay_stopped);
   if(*m_status) return m_status;

   return m_status;
}






char * CQCICommands::Location(unsigned char address,
			                  long & position,
						      long location,
			                  unsigned long acceleration,
			                  unsigned long velocity,
						      unsigned long timeout,
			                  unsigned short closed_hold_torque,
			                  unsigned short closed_running_torque,
			                  unsigned short open_hold_torque,
			                  unsigned short open_running_torque,
			                  short moving_limit,
			                  short hold_limit,
			                  unsigned short delay_stopped)
{
	 
	unsigned short s_w;


	{
	  // stop the motor for any case.
      m_status = Stop(address, -1);
      if(*m_status) return m_status;

	
	  m_status = Set_Target_Position(address);
      if(*m_status) return m_status;

	
	
	  //Get the initial position.
      m_status = Read_Symbolic(address, 1, position);
      if(*m_status) return m_status;

	  //set motor torque.
	  m_status = Motor_Torque(address, 
	 	                      closed_hold_torque, 
		                      closed_running_torque,
			                  open_hold_torque, 
					          open_running_torque);
	  if(*m_status) return m_status;
	}

    
	{
	  //set error limits.
	  m_status = Error_Limits(address,moving_limit,hold_limit,delay_stopped);
	  if(*m_status) return m_status;


	  //initial poll and clear poll.
      s_w = 0;
	  m_status = Poll(address, s_w);
      if(*m_status) return m_status;
      m_status = Clear_Poll(address, 0xffff );
      if(*m_status) return m_status;
	}

	//go there.
	m_status = LAV(address,location,acceleration,velocity/*,0,0*/);
    if(*m_status) return m_status;
	
	
	//timeout setup.
	DWORD time_base = GetTickCount();
		
	//poll until condition.
	for(s_w=0; !(s_w) && ((GetTickCount()-time_base) < timeout) ;)
	{
	   m_status = Poll(address, s_w);
       if(*m_status) return m_status;

	   //doevents();
	}

	// stop the motor.
    m_status = Stop(address, -1);
    if(*m_status) return m_status;

	//Get the current position.
    m_status = Read_Symbolic(address, 1, position);

	return m_status;
}





bool CQCICommands::InPlace(unsigned char addr1,
						   unsigned char addr2,
						   unsigned char addr3,
						   unsigned char addr4,
						   unsigned char addr5,
						   unsigned char addr6,
						   unsigned char addr7)
{
  unsigned short s_w;
  //poll
  if(addr1) Poll(addr1, s_w); 
  if(s_w != 0) return false;
  else Stop(addr1, -1);
  
  if(addr2) Poll(addr2, s_w); 
  if(s_w != 0) return false;
  
  if(addr3) Poll(addr3, s_w); 
  if(s_w != 0) return false;
  
  if(addr4) Poll(addr4, s_w); 
  if(s_w != 0) return false;
  
  if(addr5) Poll(addr5, s_w); 
  if(s_w != 0) return false;
  
  if(addr6) Poll(addr6, s_w); 
  if(s_w != 0) return false;
  
  if(addr7) Poll(addr7, s_w); 
  if(s_w != 0) return false;
  
  return true;
}




char * CQCICommands::Distance(unsigned char address,
			                  long & position,
						      long distance,
			                  unsigned long acceleration,
			                  unsigned long velocity,
						      unsigned long timeout,
			                  unsigned short closed_hold_torque,
			                  unsigned short closed_running_torque,
			                  unsigned short open_hold_torque,
			                  unsigned short open_running_torque,
			                  short moving_limit,
			                  short hold_limit,
			                  unsigned short delay_stopped)
{
	 
	
	// stop the motor for any case.
    m_status = Stop(address, -1);
    if(*m_status) return m_status;


	m_status = Set_Target_Position(address);
    if(*m_status) return m_status;



	//Get the initial position position.
    m_status = Read_Symbolic(address, 1, position);
    if(*m_status) return m_status;

	//set motor torque.
	m_status = Motor_Torque(address, 
		                    closed_hold_torque, 
		                    closed_running_torque,
			                open_hold_torque, 
					        open_running_torque);
	if(*m_status) return m_status;

	//set error limits.
	m_status = Error_Limits(address,moving_limit,hold_limit,delay_stopped);
	if(*m_status) return m_status;


	//initial poll and clear poll.
    unsigned short s_w = 0;
	m_status = Poll(address, s_w);
    if(*m_status) return m_status;
    m_status = Clear_Poll(address, 0xffff );
    if(*m_status) return m_status;


	//go there.
	m_status = DAV(address,distance,acceleration,velocity/*,0,0*/);
    if(*m_status) return m_status;


	//timeout setup.
	DWORD time_base = GetTickCount();
		
	//poll until condition.
	for(s_w=0; !(s_w) && ((GetTickCount()-time_base) < timeout) ;)
	{
	   m_status = Poll(address, s_w);
       if(*m_status) return m_status;

	   //doevents();
	}

	// stop the motor.
    m_status = Stop(address, -1);
    if(*m_status) return m_status;

	//Get the current position.
    m_status = Read_Symbolic(address, 1, position);

	return m_status;
}

