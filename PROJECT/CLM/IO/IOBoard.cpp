// IOBoard.cpp: implementation of the CIOBoard class.
//
//////////////////////////////////////////////////////////////////////
#include "../globals.h"
#include <conio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CIOBoard::CIOBoard(unsigned short base_address)
{ 
	m_base = base_address;
    int i;
	//board # 1. 304
	for(i=24;i<48;i++) m_lut_table[i+1] = i-24;
	//board # 2. 300
	for(i=0;i<24;i++) m_lut_table[i+1] = i+32;
	//board #3
	for(i=48;i<72;i++) m_lut_table[i+1] = i+16;
	//board #4
	for(i=72;i<96;i++) m_lut_table[i+1] = i+24;
}


CIOBoard::CIOBoard(char * base_address_file)
{ 
#ifdef _PRINCETON_
	int i;
	FILE *f = fopen(base_address_file,"w+");
	if(f)
	{
	   fscanf(f,"%d\n",&m_base);
	   for(i=0;i<96;i++) fscanf(f,"%d\n",&m_lut_table[i]);
       fclose(f);
	}
	else
	{
	   f = fopen("chipload.log","w+");
	   if(f)
	   {  
		fprintf(f,"Couldn't access io device address file\n");
       	fclose(f);
	   }
	}
	//fscanf(base_address_file,"%d",m_base);
#endif
}



CIOBoard::~CIOBoard()
{
}



bool CIOBoard::Init()
{
#if defined _PRINCETON_

   int i;
   HANDLE h;

   h = CreateFile("\\\\.\\giveio", GENERIC_READ, 0, NULL,
					OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
   if(h == INVALID_HANDLE_VALUE)
   {
	  __Error("The I/O driver is not initialized",0);
	  return false;
   }
   CloseHandle(h);

   unsigned short control = m_base + 3;
   for(i=0;i<4;i++)
   {
	  _outp(control,0x99);
	  _outp(control-2,0xff);
	  control+=4;
   }


   //very critical:
   Plunge_Dwn();
   Sleep(500);
   return Plunge_is_Dwn();

#endif
   return true;
}


BYTE CIOBoard::ReadByte(unsigned short address)
{

#if defined _PRINCETON_
   return ~_inp(address+m_base);
#else
   return (BYTE)0;
#endif

}

bool CIOBoard::ReadBit(unsigned short address)
{
	BYTE m = 1;	
	if(ReadByte(address/8) & (m<<(address%8))) return true;
	return false;
}


void CIOBoard::WriteByte(unsigned short address, BYTE data)
{
#if defined _PRINCETON_
	_outp(m_base + address, ~data);
#endif
}


void CIOBoard::WriteBit(unsigned short address, bool data)
{	
	BYTE n=1,m = ReadByte(address/8);
	m = data ? m | (n << (address%8)) : m & (~(n<<(address%8)));
	WriteByte(address/8, m);
}


unsigned short CIOBoard::LUT(unsigned short index)
{
   return m_lut_table[(index-1)%96+1];
}

void CIOBoard::WriteBitIndirect(unsigned short index, bool data)
{	
    unsigned short address = LUT(index);
	WriteBit(address, data);
}

bool CIOBoard::ReadBitIndirect(unsigned short index)
{
	unsigned short address = LUT(index);
	return ReadBit(address);
}

////Griper.
bool CIOBoard::Grip_is_Open()
{
   if(!ReadBitIndirect(4) && ReadBitIndirect(5))
	return true;
   return false;

}

bool CIOBoard::Grip_is_Closed()
{
   if(ReadBitIndirect(4) && !ReadBitIndirect(5))
	return true;
   return false;
}

bool CIOBoard::Grip_Open()
{
  if(!Grip_is_Open() && Grip_is_Closed())
  {
     WriteBitIndirect(9,true);
	 WriteBitIndirect(13,false);
     return true;
  }
  return false;
}

bool CIOBoard::Grip_Close()
{
  if(Grip_is_Open() && !Grip_is_Closed())
  {
     WriteBitIndirect(9,false);
	 WriteBitIndirect(13,true);
     return true;
  }
  return false;
}


void CIOBoard::Grip_Vac_On()
{
   WriteBitIndirect(15,true);
}

void CIOBoard::Grip_Vac_Off()
{
   WriteBitIndirect(15,false);
}

void CIOBoard::Grip_Air_On()
{
   WriteBitIndirect(16,true);
}

void CIOBoard::Grip_Air_Off()
{
   WriteBitIndirect(16,false);
}


//Chip present on Gripper sensor.
bool CIOBoard::Chip_Pres()
{
	return !ReadBitIndirect(1);
}

//Plunge functions.
bool CIOBoard::Plunge_is_Up()
{
   return !ReadBitIndirect(2) && ReadBitIndirect(3);
}

bool CIOBoard::Plunge_is_Dwn()
{
   return ReadBitIndirect(2) && !ReadBitIndirect(3);
}

bool CIOBoard::Plunge_Up()
{
  if(!Plunge_is_Up() && Plunge_is_Dwn())
  {
  	WriteBitIndirect(11,true);
	WriteBitIndirect(33,false);
    return true;
  }
  return false;
}

bool CIOBoard::Plunge_Dwn()
{
  //if(Plunge_is_Up() && !Plunge_is_Dwn())
  //{
  	WriteBitIndirect(11,false);
	WriteBitIndirect(33,true);
    return true;
  //}
  //return false;
}

bool CIOBoard::Needls_Up()
{
  if(Plunge_is_Up() && !Plunge_is_Dwn())
  {
  	WriteBitIndirect(12,true);
	WriteBitIndirect(35,false);
	return true;
  }
  return false;
}

void CIOBoard::Needls_Dwn()
{
  WriteBitIndirect(12,false);
  WriteBitIndirect(35,true);
}

void CIOBoard::Plunge_Vac_On()
{
  WriteBitIndirect(34,true);
}

void CIOBoard::Plunge_Vac_Off()
{
  WriteBitIndirect(34,false);
}

//Wafer.
void CIOBoard::Waf_Open()
{
  WriteBitIndirect(10,false);
  WriteBitIndirect(14,true);
}

void CIOBoard::Waf_Close()
{
  WriteBitIndirect(10,true);
  WriteBitIndirect(14,false);
}



//Conveyor.
bool CIOBoard::Start_Cycle() //test if conveyor is ready.
{
   if(ReadBitIndirect(41))
	return true;

   if(Cycle_Cmp_is_On()) Cycle_Cmp_Off();
   return false;
}


void CIOBoard::Cycle_Cmp_Puls()
{	   
	Cycle_Cmp_On();
	while(Cycle_Cmp_is_Off());
    Cycle_Cmp_Off();
}


void CIOBoard::Cycle_Cmp_On()
{
  WriteBitIndirect(37,true);
}


void CIOBoard::Cycle_Cmp_Off()
{
  WriteBitIndirect(37,false);
}


bool CIOBoard::Cycle_Cmp_is_On()
{
  if(ReadBitIndirect(37)) return true;
  return false;
}

bool CIOBoard::Cycle_Cmp_is_Off()
{
  return !Cycle_Cmp_is_On();
}



//Barcode.
void CIOBoard::Barcode_Trigger_On()
{
  WriteBitIndirect(38,true);
}


void CIOBoard::Barcode_Trigger_Off()
{
  WriteBitIndirect(38,false);
}



//homming.
void CIOBoard::Homming()
{
   Grip_Air_Off();
   Grip_Close();
   Grip_Vac_Off();
   Needls_Dwn();
   Plunge_Dwn();
   Plunge_Vac_Off();
   Waf_Close();
}


//machina.
//pause button and light.
bool CIOBoard::Pause_is_Pressed()
{
   return ReadBitIndirect(48);
}

bool CIOBoard::Pause_Lt_is_On()
{
   return ReadBitIndirect(40);
}

bool CIOBoard::Pause_Lt_is_Off()
{
   return !Pause_Lt_is_On();
}

void CIOBoard::Pause_Lt_On()
{
  WriteBitIndirect(40,true);
}

void CIOBoard::Pause_Lt_Off()
{
  WriteBitIndirect(40,false);
}

bool CIOBoard::Motion_Lt_is_On()
{
   return ReadBitIndirect(39);
}

bool CIOBoard::Motion_Lt_is_Off()
{
   return !Motion_Lt_is_On();
}

void CIOBoard::Motion_Lt_On()
{
  WriteBitIndirect(39,true);
}

void CIOBoard::Motion_Lt_Off()
{
  WriteBitIndirect(39,false);
}

bool CIOBoard::Vision_Lt_is_On()
{
   return ReadBitIndirect(36);
}

bool CIOBoard::Vision_Lt_is_Off()
{
   return !Vision_Lt_is_On();
}

void CIOBoard::Vision_Lt_On()
{
  WriteBitIndirect(36,true);
}

void CIOBoard::Vision_Lt_Off()
{
  WriteBitIndirect(36,false);
}


//Start button.
bool CIOBoard::Start_is_Pressed()
{
   	if(!g_Axes.is_hommed) return false;
	
	return ReadBitIndirect(46);
}


//Home button.
bool CIOBoard::Home_is_Pressed()
{
   
	return ReadBitIndirect(45);
}


//Retry (error #1) button.
bool CIOBoard::Retry_is_Pressed()
{
   return ReadBitIndirect(44);
}


//Ignore (error #2) button.
bool CIOBoard::Ignore_is_Pressed()
{
   return ReadBitIndirect(47);
}
