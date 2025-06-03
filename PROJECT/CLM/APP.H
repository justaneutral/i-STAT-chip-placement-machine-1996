// app.h : main header file for the MENU application
//

#include "stdafx.h"

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

#include "../vikingeditor/vikingeditordoc.h"

/////////////////////////////////////////////////////////////////////////////
// application:
// See app.cpp for the implementation of this class
//

class application : public CWinApp
{
public:
	CVikingEditorDoc *map;
	
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(application)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL
};


/////////////////////////////////////////////////////////////////////////////
