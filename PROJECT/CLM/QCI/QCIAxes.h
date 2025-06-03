// QCIAxes.h: interface for the CQCIAxes class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_QCIAXES_H__AE99DC62_AED3_11D1_B2BB_00C04FAD4084__INCLUDED_)
#define AFX_QCIAXES_H__AE99DC62_AED3_11D1_B2BB_00C04FAD4084__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CQCIAxes  
{
public:
	CQCIAxes();
	virtual ~CQCIAxes();

//members.
long g_home_absolute[8];
bool is_hommed;
bool m_hommed[9];
	
//methods.
bool HommedWell();
void CollectHomeDistances();
char * Init();
char * Homming(bool refresh, int axis_number = 0);
double gear_ratio(int address);
long Inch2counts(int address, double inch);
double Counts2inch(int axis, long count);
char * InchPosition(double inch, double & hinch,int axis);
char * InchPosition(double inch, int axis);
char * InchPositionEx(double inch, double & hinch,int axis, 
    	double velocity, double acceleration, double torque);
char * ZeroTorque(int axis);
bool restore_torque();
char * SetTorque(int axis);
char * GetInchPosition(double & hinch,int axis);
bool Save_Home_Data();
bool Retreve_Home_Data();
};

#endif // !defined(AFX_QCIAXES_H__AE99DC62_AED3_11D1_B2BB_00C04FAD4084__INCLUDED_)
