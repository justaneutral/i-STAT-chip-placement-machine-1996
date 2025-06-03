// errorLog.cpp : implementation file
//

#include "afxdisp.h"
#include "stdafx.h"
#include "globals.h"
#include "errorLog.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// errorLog dialog


errorLog::errorLog(CWnd* pParent /*=NULL*/)
	: CDialog(errorLog::IDD, pParent)
{
	EnableAutomation();

	szLastError = CString(_T(""));

	//{{AFX_DATA_INIT(errorLog)
	m_log_string = GetErrorLog();
	//}}AFX_DATA_INIT
}


void errorLog::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void errorLog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(errorLog)
	DDX_Text(pDX, IDC_EDIT_error_log, m_log_string);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(errorLog, CDialog)
	//{{AFX_MSG_MAP(errorLog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(errorLog, CDialog)
	//{{AFX_DISPATCH_MAP(errorLog)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IerrorLog to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {8DFBD021-14FC-11D2-94A5-000000000000}
static const IID IID_IerrorLog =
{ 0x8dfbd021, 0x14fc, 0x11d2, { 0x94, 0xa5, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } };

BEGIN_INTERFACE_MAP(errorLog, CDialog)
	INTERFACE_PART(errorLog, IID_IerrorLog, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// errorLog message handlers

#define _GVE_ g_error_log.szLastError = CString(_T(""));


CString errorLog::GetErrorLog()
{
   char s[80];
   int i;
   FILE *f;
   f = fopen("error.log","r");
   
   m_log_string = CString(_T(""));

   if(f)
   {
      while(!feof(f))
	  {
	    for(i=0;i<4;i++)
		{
		   fscanf(f,"%s");
	       m_log_string += CString(s);
		}
		m_log_string += CString(_T("\r\n"));
	  }
	  fclose(f);
   }

   return m_log_string;
}


void errorLog::LogError()
{
   
   FILE *f;
   f = fopen("../error.log","a+");

   if(f)
   {
      fprintf(f,"Bay# %d Error: %s\n",g_machine->bay_number,szLastError);
      fclose(f);
   }
}
