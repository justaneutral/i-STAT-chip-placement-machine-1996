// SetupScreenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "app.h"
#include "SetupScreenDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetupScreenDlg dialog


CSetupScreenDlg::CSetupScreenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetupScreenDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetupScreenDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSetupScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetupScreenDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetupScreenDlg, CDialog)
	//{{AFX_MSG_MAP(CSetupScreenDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetupScreenDlg message handlers

BOOL CSetupScreenDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
  ReplacButton(RGB(255,255,0),IDC_BUTTON_setup_screen_motor_setup_fetcher,m_setup_screen_motor_setup_fetcher);
  ReplacButton(RGB(255,255,0),IDC_BUTTON_setup_screen_fetcher_clamp,m_setup_screen_fetcher_clamp);
  ReplacButton(RGB(255,255,0),IDC_BUTTON_setup_screen_motor_setup_stretcher,m_setup_screen_motor_setup_stretcher);
  ReplacButton(RGB(255,255,0),IDC_BUTTON_setup_screen_motor_setup_overhead_gantry_x,m_setup_screen_motor_setup_overhead_gantry_x);
  ReplacButton(RGB(255,255,0),IDC_BUTTON_setup_screen_motor_setup_overhead_gantry_y,m_setup_screen_motor_setup_overhead_gantry_y);
  ReplacButton(RGB(255,255,0),IDC_BUTTON_setup_screen_motor_setup_overhead_gantry_smac,m_setup_screen_motor_setup_overhead_gantry_smac);
  ReplacButton(RGB(255,255,0),IDC_BUTTON_setup_screen_overhead_gantry_jaws,m_setup_screen_overhead_gantry_jaws);
  ReplacButton(RGB(255,255,0),IDC_BUTTON_setup_screen_motor_setup_wafer_x,m_setup_screen_motor_setup_wafer_x);
  ReplacButton(RGB(255,255,0),IDC_BUTTON_setup_screen_motor_setup_wafer_y,m_setup_screen_motor_setup_wafer_y);
  ReplacButton(RGB(255,255,0),IDC_BUTTON_setup_screen_vision_wafer,m_setup_screen_vision_wafer);
  ReplacButton(RGB(255,255,0),IDC_BUTTON_setup_screen_vision_base,m_setup_screen_vision_base);
  ReplacButton(RGB(255,255,0),IDC_BUTTON_setup_screen_plunge_up_timing,m_setup_screen_plunge_up_timing);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CSetupScreenDlg::ReplacButton(COLORREF color,DWORD idc,CButtonColor &wnd)
{
	CRect rect;
	CString s;
	CWnd *placeholder;
	
    //button.
	placeholder = GetDlgItem(idc);
	placeholder->GetWindowRect(&rect);
	placeholder->GetWindowText(s);
	placeholder->DestroyWindow();
	ScreenToClient(&rect);

	wnd.SetColorSelected(RGB(255,255,255);
	wnd.SetColorFocus(RGB(255,192,192);
	wnd.SetColor(color);

	wnd.Create((LPCTSTR)s,WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_PUSHBUTTON|BS_OWNERDRAW,
		rect,this,idc);
}