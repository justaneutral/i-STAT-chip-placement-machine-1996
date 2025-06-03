// errorHandle.cpp : implementation file
//

#include "afxdisp.h"
#include "stdafx.h"
#include "globals.h"
#include "errorHandle.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// errorHandle dialog


errorHandle::errorHandle(CString question,CWnd* pParent /*=NULL*/)
	: CDialog(errorHandle::IDD, pParent)
{
	EnableAutomation();


    //result = 0;
	//TOCounter = 200; //10 seconds.

    Create(IDD);
	
	//SetTimer(0,20000,NULL);

	//{{AFX_DATA_INIT(errorHandle)
	m_error_string = &g_error_log ? g_error_log.szLastError : _T("No Error Handler");
	//}}AFX_DATA_INIT
}


void errorHandle::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void errorHandle::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(errorHandle)
	DDX_Text(pDX, IDC_EDIT_error_handle, m_error_string);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(errorHandle, CDialog)
	//{{AFX_MSG_MAP(errorHandle)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(errorHandle, CDialog)
	//{{AFX_DISPATCH_MAP(errorHandle)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IerrorHandle to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {3DC1BD21-1453-11D2-94A3-000000000000}
static const IID IID_IerrorHandle =
{ 0x3dc1bd21, 0x1453, 0x11d2, { 0x94, 0xa3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } };

BEGIN_INTERFACE_MAP(errorHandle, CDialog)
	INTERFACE_PART(errorHandle, IID_IerrorHandle, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// errorHandle message handlers

void errorHandle::OnTimer(UINT nIDEvent) 
{
	if( g_io.Retry_is_Pressed() )
	{
        result = IDOK;
		//KillTimer(0);
	    DestroyWindow();
	}
	
	if( g_io.Ignore_is_Pressed() )
	{
        result = IDCANCEL;
	    //KillTimer(0);
		DestroyWindow();
	}

	
	if(!TOCounter--)
	{
       result = IDCANCEL;
	   //KillTimer(0);
	   DestroyWindow();
	}
	
}

void errorHandle::OnCancel() 
{
    result = IDCANCEL;
	//KillTimer(0);
	DestroyWindow();
}

void errorHandle::OnOK() 
{
	// TODO: Add extra validation here
	result = IDOK;
	//KillTimer(0);
	DestroyWindow();
}

void errorHandle::PostNcDestroy()
{
   delete this;
}
