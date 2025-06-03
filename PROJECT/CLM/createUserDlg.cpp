// createUserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "app.h"
#include "createUserDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// createUserDlg dialog


createUserDlg::createUserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(createUserDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(createUserDlg)
	m_password = _T("");
	m_password_confirm = _T("");
	m_name = _T("");
	m_permissions = FALSE;
	m_id = _T("");
	//}}AFX_DATA_INIT
}


void createUserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(createUserDlg)
	DDX_Text(pDX, IDC_PASSWORD, m_password);
	DDV_MaxChars(pDX, m_password, 15);
	DDX_Text(pDX, IDC_PASSWORD_CONFIRM, m_password_confirm);
	DDV_MaxChars(pDX, m_password_confirm, 15);
	DDX_Text(pDX, IDC_USER_NAME, m_name);
	DDV_MaxChars(pDX, m_name, 15);
	DDX_Check(pDX, IDC_CHECK_PERMISSIONS, m_permissions);
	DDX_Text(pDX, IDC_USER_ID, m_id);
	DDV_MaxChars(pDX, m_id, 15);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(createUserDlg, CDialog)
	//{{AFX_MSG_MAP(createUserDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// createUserDlg message handlers
