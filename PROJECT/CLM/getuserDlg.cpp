// getuserDlg.cpp : implementation file
//

#include "stdafx.h"
#include "app.h"
#include "getuserDlg.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// getuserDlg dialog


getuserDlg::getuserDlg(CWnd* pParent /*=NULL*/)
	: CDialog(getuserDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(getuserDlg)
	m_password = _T("");
	m_name = _T("");
	//}}AFX_DATA_INIT
}


void getuserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(getuserDlg)
	DDX_Text(pDX, IDC_CURRENT_PASSWORD, m_password);
	DDV_MaxChars(pDX, m_password, 15);
	DDX_Text(pDX, IDC_CURRENT_USER_NAME, m_name);
	DDV_MaxChars(pDX, m_name, 15);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(getuserDlg, CDialog)
	//{{AFX_MSG_MAP(getuserDlg)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// getuserDlg message handlers

HBRUSH getuserDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	static HBRUSH br = CreateSolidBrush(RGB(0,255,0));
	pDC->SetTextColor(RGB(0,0,0));
	pDC->SetBkColor(RGB(0,255,0));


	
	// TODO: Return a different brush if the default is not desired
	return br;
}

BOOL getuserDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	GetDlgItem(IDC_CURRENT_USER_NAME)->SetFocus();
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
