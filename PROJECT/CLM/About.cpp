// About.cpp : implementation file
//

#include "stdafx.h"
#include "app.h"
#include "About.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAbout dialog


CAbout::CAbout(CWnd* pParent /*=NULL*/)
	: CDialog(CAbout::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAbout)
	//}}AFX_DATA_INIT
}


void CAbout::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAbout)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAbout, CDialog)
	//{{AFX_MSG_MAP(CAbout)
	ON_BN_CLICKED(IDC_BUTTON_colored, OnBUTTONcolored)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAbout message handlers


BOOL CAbout::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CRect rect;
	CString s;
	CWnd *placeholder;
	
	//edit
	placeholder = GetDlgItem(IDC_STATIC_colored);
	placeholder->GetWindowRect(&rect);
	placeholder->DestroyWindow();
	ScreenToClient(&rect);
	m_editcolor.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP,rect,this,IDC_STATIC_colored);

    //button.
	placeholder = GetDlgItem(IDC_BUTTON_colored);
	placeholder->GetWindowRect(&rect);
	placeholder->GetWindowText(s);
	placeholder->DestroyWindow();
	ScreenToClient(&rect);

	m_buttoncolor.Create((LPCTSTR)s,WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_PUSHBUTTON|BS_OWNERDRAW,rect,this,IDC_BUTTON_colored);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAbout::OnBUTTONcolored() 
{
   AfxMessageBox("vali(d)");	
}
