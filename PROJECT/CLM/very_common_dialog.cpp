// very_common_dialog.cpp : implementation file
//

#include "stdafx.h"
#include "app.h"
#include "very_common_dialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// very_common_dialog dialog


very_common_dialog::very_common_dialog(CString *ParamName,
									   CWnd* pParent /*=NULL*/)
	: CDialog(very_common_dialog::IDD, pParent)
{
	//SetWindowText((LPSTR)ParamName);
	//{{AFX_DATA_INIT(very_common_dialog)
	m_double = 0.0;
	//}}AFX_DATA_INIT
}
						 

void very_common_dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(very_common_dialog)
	DDX_Text(pDX, idc_very_common_edit, m_double);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(very_common_dialog, CDialog)
	//{{AFX_MSG_MAP(very_common_dialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// very_common_dialog message handlers
