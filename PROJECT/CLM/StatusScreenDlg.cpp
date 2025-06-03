// StatusScreenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "globals.h"
#include "StatusScreenDlg.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStatusScreenDlg dialog


CStatusScreenDlg::CStatusScreenDlg(CWnd* pParent /*=NULL*/, int *indicator)
	: CDialog(CStatusScreenDlg::IDD, pParent)
{
	
	//{{AFX_DATA_INIT(CStatusScreenDlg)
	m_operator = g_current_user.FullName;
	m_product  = g_machine_state.product;
	//}}AFX_DATA_INIT

	m_indicator = indicator;
	*m_indicator = 1;
    
	for(int i=0;i<4;i++) m_magazine_is_active[i] = false;

	Create(IDD);
}


void CStatusScreenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStatusScreenDlg)
	DDX_Text(pDX, IDC_status_screen_operator, m_operator);
	DDV_MaxChars(pDX, m_operator, 15);
	DDX_Text(pDX, IDC_status_screen_product_type, m_product);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStatusScreenDlg, CDialog)
	//{{AFX_MSG_MAP(CStatusScreenDlg)
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStatusScreenDlg message handlers

BOOL CStatusScreenDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int i;
	CRect rect;
	CString s;
	CWnd *placeholder;
	CFont *font;
	LOGFONT LogFont;
	//static fields replacement.
	DWORD idcs[] = { IDC_STATIC_status_colored_1,
		        0,   //IDC_STATIC_status_colored_2,
					 IDC_STATIC_status_colored_3,
					 IDC_STATIC_status_colored_4,
					 IDC_STATIC_status_colored_5,
				0,	 //IDC_STATIC_status_colored_6,
				0,	 //IDC_STATIC_status_colored_7,
				0,	 //IDC_STATIC_status_colored_8,
					 IDC_STATIC_status_colored_9,
					 IDC_STATIC_status_colored_10,
					 IDC_STATIC_status_colored_11,
					 IDC_STATIC_status_colored_12,
					 IDC_STATIC_status_colored_13 };

	for(i=0;i<13;i++)
	{
	  if(idcs[i])
	  {
	    font = (CFont *) new CFont;
	    placeholder = GetDlgItem(idcs[i]);
	    placeholder->GetWindowRect(&rect);
	    placeholder->GetWindowText(s);
	  
	    placeholder->GetFont()->GetLogFont(&LogFont);
	    font->CreateFontIndirect(&LogFont);

	    placeholder->DestroyWindow();
	    ScreenToClient(&rect);
	  	  
	    m_STATIC_status_colored[i].SetColorSelected(RGB(192,192,192));
	    m_STATIC_status_colored[i].SetColorFocus(RGB(192,255,192));
	    m_STATIC_status_colored[i].SetColor(RGB(255,255,192));
	 
	    m_STATIC_status_colored[i].Create((LPCTSTR)s,
		  WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_OWNERDRAW,rect,this,idcs[i]);
	    m_STATIC_status_colored[i].SetFont(font);

	    delete font;
	  }
	}
	


	//some information fields (static windows).

	//active bays indicators.
	idcs[0] = IDC_status_screen_bay_1_running;
	/*idcs[1] = IDC_status_screen_bay_2_running;
	idcs[2] = IDC_status_screen_bay_3_running;
	idcs[3] = IDC_status_screen_bay_4_running;
	*/
		 
	for(i=0;i<1;i++)
	{
	  font = (CFont *) new CFont;
	  placeholder = GetDlgItem(idcs[i]);
	  placeholder->GetWindowRect(&rect);
	  placeholder->GetWindowText(s);
	  
	  placeholder->GetFont()->GetLogFont(&LogFont);
	  font->CreateFontIndirect(&LogFont);

	  placeholder->DestroyWindow();
	  ScreenToClient(&rect);
	  
	  m_bay_running[i].Create((LPCTSTR)s,
		WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_OWNERDRAW,rect,this,idcs[i]);
	  m_bay_running[i].SetFont(font);
	  
	  delete font;
	}

	m_bay_running[0].SetTrigger(RGB(255,0,0),_T("*"),
		                        RGB(255,255,255),_T(""),
								1000,
								(BOOL *)&(g_Axes.is_hommed));	
	/*
	m_bay_running[1].SetTrigger(RGB(255,0,0),_T("2"),
		                        RGB(255,255,255),_T(""),
								1000,
								&(g_machine_state.m_chip_is_picked_up));	

	m_bay_running[2].SetTrigger(RGB(255,0,0),_T("3"),
		                        RGB(255,255,255),_T(""),
								1000,
								&(g_machine_state.m_chip_is_picked_up));	

	m_bay_running[3].SetTrigger(RGB(255,0,0),_T("4"),
		                        RGB(255,255,255),_T(""),
								1000,
								&(g_machine_state.m_chip_is_picked_up));	
	*/  


	//passive bays indicators.
	idcs[0] = IDC_status_screen_bay_1_stopped;
	/*idcs[1] = IDC_status_screen_bay_2_stopped;
	idcs[2] = IDC_status_screen_bay_3_stopped;
	idcs[3] = IDC_status_screen_bay_4_stopped;
	*/

	for(i=0;i<1;i++)
	{
	  font = (CFont *) new CFont;
	  placeholder = GetDlgItem(idcs[i]);
	  placeholder->GetWindowRect(&rect);
	  placeholder->GetWindowText(s);
	  
	  placeholder->GetFont()->GetLogFont(&LogFont);
	  font->CreateFontIndirect(&LogFont);

	  placeholder->DestroyWindow();
	  ScreenToClient(&rect);
	  
	  m_bay_stopped[i].Create((LPCTSTR)s,
		WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_OWNERDRAW,rect,this,idcs[i]);
	  m_bay_stopped[i].SetFont(font);
	  
	  delete font;
	}

	m_bay_stopped[0].SetTrigger(RGB(255,255,255),_T(""),
		                        RGB(0,255,0),_T("*"),
								1000,
								&(g_machine_state.m_chip_is_picked_up));	

	/*m_bay_stopped[1].SetTrigger(RGB(255,255,255),_T(""),
		                        RGB(0,255,0),_T("2"),
								1000,
								&(g_machine_state.m_chip_is_picked_up));	

	m_bay_stopped[2].SetTrigger(RGB(255,255,255),_T(""),
		                        RGB(0,255,0),_T("3"),
								1000,
								&(g_machine_state.m_chip_is_picked_up));	

	m_bay_stopped[3].SetTrigger(RGB(255,255,255),_T(""),
		                        RGB(0,255,0),_T("4"),
								1000,
								&(g_machine_state.m_chip_is_picked_up));	

   */

	
	//magazines.
	idcs[0] = IDC_status_screen_magazine_1;
	/*idcs[1] = IDC_status_screen_magazine_2;
	idcs[2] = IDC_status_screen_magazine_3;
	idcs[3] = IDC_status_screen_magazine_4;
	*/	 

	for(i=0;i<1;i++)
	{
	  font = (CFont *) new CFont;
	  placeholder = GetDlgItem(idcs[i]);
	  placeholder->GetWindowRect(&rect);
	  placeholder->GetWindowText(s);
	  
	  placeholder->GetFont()->GetLogFont(&LogFont);
	  font->CreateFontIndirect(&LogFont);

	  placeholder->DestroyWindow();
	  ScreenToClient(&rect);
	  
	  m_magazine[i].SetColorFocus(RGB(255,255,250));
	  m_magazine[i].SetColorSelected(RGB(255,255,250));
	  m_magazine[i].SetColor(RGB(255,255,250));

	  m_magazine[i].Create((LPCTSTR)s,
		WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_OWNERDRAW,rect,this,idcs[i]);
	  m_magazine[i].SetFont(font);
	  
	  delete font;
	}
	
	m_magazine[0].SetTrigger(RGB(255,0,0), _T("Empty"),
							 RGB(0,255,0), _T("Ready"),
				             1000,
							 (BOOL *)&(g_machine_state.m_magazine_is_ready));
	
	
	
	//extended scrolable field for error logging.
	placeholder = GetDlgItem(IDC_status_screen_edit);
	placeholder->GetWindowRect(&rect);
	placeholder->DestroyWindow();
	ScreenToClient(&rect);
	m_edit.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_VSCROLL|
		          ES_MULTILINE|ES_AUTOVSCROLL|ES_READONLY,
		          rect,this,IDC_status_screen_edit);

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CStatusScreenDlg::OnEraseBkgnd(CDC* pDC) 
{
	if(!CDialog::OnEraseBkgnd(pDC)) return FALSE;
	CBrush brush, wbrush;
	CRect rect;
	int i;
	CWnd *placeholder;

	
	//Background. (R,G)
	if(m_background_color_state) brush.CreateSolidBrush(RGB(255,0,0));
	else                         brush.CreateSolidBrush(RGB(0,255,0));
    
	pDC->SelectObject(brush);
	
	GetWindowRect(&rect);
	ScreenToClient(&rect);
	pDC->Rectangle(rect);

	//White isles.
	DWORD idcs[5] = { IDC_status_white_box_1,
	                  IDC_status_white_box_2,
					  IDC_status_white_box_3,
					  IDC_status_white_box_4,
					  IDC_status_white_box_5 };
	
	wbrush.CreateSolidBrush(RGB(255,255,255));
	pDC->SelectObject(wbrush);
	
	for(i=0;i<5;i++)
	{
	  placeholder = GetDlgItem(idcs[i]);
	  placeholder->GetWindowRect(&rect);
	  ScreenToClient(&rect);
	  pDC->Rectangle(rect);
	}
	
	
	return TRUE;
}


void CStatusScreenDlg::OnTimer(UINT nIDEvent) 
{

	bool inv = false;
	DWORD interval;

	if(g_machine_state.m_error != m_background_color_state)
	{
	   inv = true;
	   m_background_color_state = g_machine_state.m_error;
	}


	for(int i=0;i<1;i++) 
	  if(g_machine_state.m_wafer_is_loaded != m_magazine_is_active[i])
	  {
		 inv = true;
	     m_magazine_is_active[i] = !m_magazine_is_active[i];
		 
		 if(m_magazine_is_active[i]) interval = 1000;
		 else interval = 0;
		 
		 m_magazine[i].SetTrigger(RGB(255,255,255),_T("Empty"),
			                      RGB(255,255,255),_T("OK"),
								  interval,
								  &(g_machine_state.m_magazine_is_ready));
	  }
	
	if(inv) Invalidate();  
	
	
	if(g_machine_state.m_error)
	{
	  GetDlgItem(IDC_status_screen_motor_address)->SetWindowText(g_Cmnds.motor);
	  GetDlgItem(IDC_status_screen_status)->SetWindowText(_T("Error"));
	  GetDlgItem(IDC_status_screen_vision_system)->SetWindowText(g_machine_state.vision);

	  if(!g_current_error.IsEmpty())
	  {
	    CString Err_Lines;	  
	    char line_ending[3];
	    line_ending[0]=0xd;
	    line_ending[1]=0xa;
	    line_ending[2]=0;
	    m_edit.GetWindowText(Err_Lines);
	    Err_Lines += g_current_error;
		g_current_error.Empty();
	    Err_Lines += line_ending;
	    m_edit.SetWindowText(Err_Lines);
	  }
	}
	else
	{
	  GetDlgItem(IDC_status_screen_motor_address)->SetWindowText(_T(""));
	  GetDlgItem(IDC_status_screen_status)->SetWindowText(g_machine_state.status);
	  GetDlgItem(IDC_status_screen_vision_system)->SetWindowText(_T(""));
	}
}



void CStatusScreenDlg::OnDestroy() 
{
	if(m_timer) KillTimer(m_timer);
	m_timer = 0;
	
	CDialog::OnDestroy();
}



int CStatusScreenDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	m_timer = SetTimer(0,1000,NULL);
	
	return 0;
}

void CStatusScreenDlg::OnOK() 
{
	DestroyWindow();
}

void CStatusScreenDlg::OnCancel() 
{
}

void CStatusScreenDlg::PostNcDestroy() 
{
	*m_indicator = 0;
	delete this;
}
