// Base.h: interface for the Base class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BASE_H__69B1CBC1_CEF4_11D1_8C49_00C04FAD4034__INCLUDED_)
#define AFX_BASE_H__69B1CBC1_CEF4_11D1_8C49_00C04FAD4034__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class Base  
{
public:
	Base();
	virtual ~Base();

    FILE *file;
	double x,y,q;

	bool save();
	bool retreive();
	bool restore();
};

#endif // !defined(AFX_BASE_H__69B1CBC1_CEF4_11D1_8C49_00C04FAD4034__INCLUDED_)
