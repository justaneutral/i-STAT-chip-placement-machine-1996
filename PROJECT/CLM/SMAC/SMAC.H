// SMAC.h: interface for the CSMAC class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SMAC_H__AE99DC61_AED3_11D1_B2BB_00C04FAD4084__INCLUDED_)
#define AFX_SMAC_H__AE99DC61_AED3_11D1_B2BB_00C04FAD4084__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "../serialPort.h"


class CSMAC  
{
public:
	CSMAC();
	virtual ~CSMAC();

//members.
serialPort* port;
HWND m_tty_handle;
double  current_z, current_a;

//methods.
bool Connect();
bool DisConnect();
bool Init();
void delay(DWORD timeout);
double GetLinearPosition();
double GetAnglePosition();
char * Send(char * data);
bool Homming();
double restrict45(double & degree);
double restrict2032mm(double & millimeter);
long Str2Long(char * szString);
void DegreeRotateMillimeterMove(double degree, double millimeter);
void DegreeRotateInchesMove(double degree, double inches);
double MillimeterMove(double millimeter);
double InchesMove(double inches);
double DegreeRotate(double degree);
double RevolutionRotate(double revolution);

};


#endif // !defined(AFX_SMAC_H__AE99DC61_AED3_11D1_B2BB_00C04FAD4084__INCLUDED_)
