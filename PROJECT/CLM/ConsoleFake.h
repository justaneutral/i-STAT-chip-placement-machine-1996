// ConsoleFake.h: interface for the CConsoleFake class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CONSOLEFAKE_H__7BEDBDB1_2022_11D2_8CA5_00C04FAD4034__INCLUDED_)
#define AFX_CONSOLEFAKE_H__7BEDBDB1_2022_11D2_8CA5_00C04FAD4034__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CConsoleFake  
{
public:
	CConsoleFake();
	virtual ~CConsoleFake();

	char  inp[3][64];
	char  outp[12][64];

};

#endif // !defined(AFX_CONSOLEFAKE_H__7BEDBDB1_2022_11D2_8CA5_00C04FAD4034__INCLUDED_)
