// userDlg.cpp : implementation file
//

#include "stdafx.h"
#include "app.h"
#include "userDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// userDlg dialog


userDlg::userDlg(CWnd* pParent /*=NULL*/)
	: CDialog(userDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(userDlg)
	m_password = _T("");
	m_name = _T("");
	m_permissions = FALSE;
	//}}AFX_DATA_INIT
}


void userDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(userDlg)
	DDX_Text(pDX, IDC_PASSWORD, m_password);
	DDV_MaxChars(pDX, m_password, 15);
	DDX_Text(pDX, IDC_USER_NAME, m_name);
	DDV_MaxChars(pDX, m_name, 15);
	DDX_Check(pDX, IDC_CHECK_PERMISSIONS, m_permissions);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(userDlg, CDialog)
	//{{AFX_MSG_MAP(userDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// userDlg message handlers
