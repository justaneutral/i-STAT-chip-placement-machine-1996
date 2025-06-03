// QCICommands.h: interface for the CQCICommands class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QCICOMMANDS_H__3BC41201_98B6_11D1_B2F2_00C04FAD4034__INCLUDED_)
#define AFX_QCICOMMANDS_H__3BC41201_98B6_11D1_B2F2_00C04FAD4034__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "QCINBS10CommDevice.h"

class CQCICommands : public CObject  
{
public:
    CQCICommands();
	virtual ~CQCICommands();


//internal members
private:

	char *m_status;
	char *m_tx_packet;
	BYTE *m_rx_packet;
	int m_increment, m_count;

//internal methods
private:
BYTE tx_check_summ();
BOOL good_tx_check_summ();
BYTE rx_check_summ();
BOOL good_rx_check_summ();
BOOL good_address();
BOOL good_packet();
BOOL ack();
int  nak();
BOOL dp();
int  dp_length();
BYTE dp_data(int index);
unsigned short prp();
int    ExchangePackets();
char * Communicate();

public:
void doevents();

//external methods
public:

WORD stop_flag; 
WORD stop_state;
CString motor;


char * Init();



char * Set_Identity(unsigned char address, 
	 	            unsigned char group_identifier,
					unsigned char identifier);


char * Poll(unsigned char address, 
	 	    unsigned short & status_word);


char * Clear_Poll(unsigned char address, 
	              unsigned short status_word);


char * Zero_Target_Position(unsigned char address);


char * Set_Target_Position(unsigned char address);


char * Read_Symbolic(unsigned char address,
					 unsigned short sym_loc,
					 long & return_data);


char * Stop(unsigned char address,
		    long acceleration);

	
char * Motor_Torque(unsigned char address, 
		            unsigned short closed_hold,
					unsigned short closed_running,
					unsigned short open_hold,
					unsigned short open_running);

char * Error_Limits(unsigned char address, 
					short moving_limit,
					short hold_limit,
					unsigned short delay_stopped);
	
char * DAV(unsigned char address, 
		   long distance,
		   unsigned long acceleration,
		   unsigned long velocity/*, 
		   WORD stop_flag, 
		   WORD stop_state*/);


char * LAV(unsigned char address, 
		   long location,
		   unsigned long acceleration,
		   unsigned long velocity/*, 
		   WORD stop_flag, 
		   WORD stop_state*/);




// use these methods.

char * Home5(long & position,
			//long gap,
		    long distance = 2147418110,
			unsigned long acceleration = 100000,
			unsigned long velocity = 200000000,
			unsigned long timeout = 17000,
			unsigned short closed_hold_torque = 4000,
			unsigned short closed_running_torque = 6000,
			unsigned short open_hold_torque = 4000,
			unsigned short open_running_torque = 6000,
			short moving_limit = 40,
			short hold_limit = 40,
			unsigned short delay_stopped = 800);


char * Home(unsigned char address,
            long & position,
			//long gap,
		    long distance = 2147418110,
			unsigned long acceleration = 100000,
			unsigned long velocity = 200000000,
			unsigned long timeout = 17000,
			unsigned short closed_hold_torque = 4000,
			unsigned short closed_running_torque = 6000,
			unsigned short open_hold_torque = 4000,
			unsigned short open_running_torque = 6000,
			short moving_limit = 40,
			short hold_limit = 40,
			unsigned short delay_stopped = 800);


bool InPlace(unsigned char addr1 = 0,
			 unsigned char addr2 = 0,
			 unsigned char addr3 = 0,
			 unsigned char addr4 = 0,
			 unsigned char addr5 = 0,
			 unsigned char addr6 = 0,
			 unsigned char addr7 = 0);



char * Location(unsigned char address,
                long & position,
		        long location = 2147418110,
			    unsigned long acceleration = 1000000,
			    unsigned long velocity = 200000000,
			    unsigned long timeout = 17000,
			    unsigned short closed_hold_torque = 4000,
			    unsigned short closed_running_torque = 6000,
			    unsigned short open_hold_torque = 4000,
			    unsigned short open_running_torque = 6000,
			    short moving_limit = 140,
			    short hold_limit = 140,
			    unsigned short delay_stopped = 800);


char * Distance(unsigned char address,
			    long & position,
				long distance = 2147418110,
			    unsigned long acceleration = 1000000,
			    unsigned long velocity = 200000000,
			    unsigned long timeout = 17000,
			    unsigned short closed_hold_torque = 4000,
			    unsigned short closed_running_torque = 6000,
			    unsigned short open_hold_torque = 4000,
			    unsigned short open_running_torque = 6000,
			    short moving_limit = 140,
			    short hold_limit = 140,
			    unsigned short delay_stopped = 800);


};
#endif // !defined(AFX_QCICOMMANDS_H__3BC41201_98B6_11D1_B2F2_00C04FAD4034__INCLUDED_)
