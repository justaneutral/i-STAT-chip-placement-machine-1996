// IOBoard.h: interface for the CIOBoard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IOBOARD_H__D0150131_ADFE_11D1_9626_00C04FAD4034__INCLUDED_)
#define AFX_IOBOARD_H__D0150131_ADFE_11D1_9626_00C04FAD4034__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CIOBoard  
{
public:
	CIOBoard(unsigned short base_address);
	CIOBoard(char * base_address_file);
	virtual ~CIOBoard();

//methods.

bool Init();
BYTE ReadByte(unsigned short address);
bool ReadBit(unsigned short address);
void WriteByte(unsigned short address, BYTE data);
void WriteBit(unsigned short address, bool data);
unsigned short LUT(unsigned short index);
void WriteBitIndirect(unsigned short index, bool data);
bool ReadBitIndirect(unsigned short index);

//members
unsigned short m_base;
unsigned short m_lut_table[97];

//methods of the machina.
////Buttons & Lights.
bool Pause_is_Pressed();
bool Pause_Lt_is_On();
bool Pause_Lt_is_Off();
void Pause_Lt_On();
void Pause_Lt_Off();
bool Motion_Lt_is_On();
bool Motion_Lt_is_Off();
void Motion_Lt_On();
void Motion_Lt_Off();
bool Vision_Lt_is_On();
bool Vision_Lt_is_Off();
void Vision_Lt_On();
void Vision_Lt_Off();
bool Start_is_Pressed();
bool Home_is_Pressed();
bool Retry_is_Pressed();
bool Ignore_is_Pressed();
////Gripper functions.
bool Grip_is_Closed();
bool Grip_is_Open();
bool Grip_Close();
bool Grip_Open();
void Grip_Vac_On();
void Grip_Vac_Off();
void Grip_Air_On();
void Grip_Air_Off();
//Chip present on Gripper sensor.
bool Chip_Pres();
//Plunge functions.
bool Plunge_is_Up();
bool Plunge_is_Dwn();
bool Plunge_Up();
bool Plunge_Dwn();
bool Needls_Up();
void Needls_Dwn();
void Plunge_Vac_On();
void Plunge_Vac_Off();
//Wafer.
void Waf_Open();
void Waf_Close();
//Conveyor
bool Start_Cycle(); //test if conveyor is ready and turns Cycle_Cmp down.
void Cycle_Cmp_Puls();
void Cycle_Cmp_On();
void Cycle_Cmp_Off();
bool Cycle_Cmp_is_On();
bool Cycle_Cmp_is_Off();
//Barcode reader.
void Barcode_Trigger_On();
void Barcode_Trigger_Off();



//homming.
void Homming();
};

#endif // !defined(AFX_IOBOARD_H__D0150131_ADFE_11D1_9626_00C04FAD4034__INCLUDED_)
