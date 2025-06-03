// Wafer.h: interface for the CWafer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WAFER_H__CD019390_D93E_11D1_8C50_00C04FAD4034__INCLUDED_)
#define AFX_WAFER_H__CD019390_D93E_11D1_8C50_00C04FAD4034__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CWafer  
{
public:
	CWafer();
	virtual ~CWafer();

	double centre[2];
	double dimension[2];
	double maxcnt[2];

	int currindex[2];
	double currcoord[2];

	void setwaferstruct();
	void correctwaferinfo(); //tie video system information.
	void setwaferinfo();
	void firstpos();
	void nextpos();
    bool islastpos();

};

#endif // !defined(AFX_WAFER_H__CD019390_D93E_11D1_8C50_00C04FAD4034__INCLUDED_)
