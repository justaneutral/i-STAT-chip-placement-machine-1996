// vikingeditor.h : main header file for the VIKINGEDITOR application
//

#if !defined(AFX_VIKINGEDITOR_H__19179CF4_D948_11D1_8C50_00C04FAD4034__INCLUDED_)
#define AFX_VIKINGEDITOR_H__19179CF4_D948_11D1_8C50_00C04FAD4034__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CVikingeditorApp:
// See vikingeditor.cpp for the implementation of this class
//

class CVikingeditorApp : public CWinApp
{
public:
	CVikingeditorApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVikingeditorApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CVikingeditorApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIKINGEDITOR_H__19179CF4_D948_11D1_8C50_00C04FAD4034__INCLUDED_)
