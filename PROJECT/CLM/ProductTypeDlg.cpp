// ProductTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "globals.h"
#include "ProductTypeDlg.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProductTypeDlg dialog


CProductTypeDlg::CProductTypeDlg(CWnd* pParent /*=NULL*/,int *indicator)
	: CDialog(CProductTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProductTypeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_indicator = indicator;
	*m_indicator = 1;
	m_timer = 0;

	Create(IDD);

}


void CProductTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProductTypeDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProductTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CProductTypeDlg)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProductTypeDlg message handlers

HBRUSH CProductTypeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	/*
	static HBRUSH br = CreateSolidBrush(RGB(0,0,255));
	pDC->SetTextColor(RGB(0,0,0));
	pDC->SetBkColor(RGB(0,0,255));
	*/

	// TODO: Return a different brush if the default is not desired
	return hbr;
}
	
BOOL CProductTypeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	////////my custom staf.
	CRect rect;
	CString s;
	CWnd *placeholder;			
	

    //upper button.
	placeholder = GetDlgItem(IDC_BUTTON_enter_product_type);
	placeholder->GetWindowRect(&rect);
	placeholder->GetWindowText(s);
	placeholder->DestroyWindow();
	ScreenToClient(&rect);
	m_BUTTON_enter_product_type.SetColorSelected(RGB(192,192,0));
    m_BUTTON_enter_product_type.SetColorFocus(RGB(255,192,0));
    m_BUTTON_enter_product_type.SetColor(RGB(255,255,0));
	m_BUTTON_enter_product_type.Create((LPCTSTR)s,
		WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_OWNERDRAW,
		rect,this,IDC_BUTTON_enter_product_type);


	//upper edit control. -- let it be standart white one.
	//placeholder = GetDlgItem(IDC_EDIT_product_type);
	//placeholder->GetWindowRect(&rect);
	//placeholder->DestroyWindow();
	//ScreenToClient(&rect);
	//m_EDIT_product_type.Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP,rect,this,IDC_EDIT_product_type);
	


	//bay 1 condition indicator button.
	placeholder = GetDlgItem(IDC_BUTTON_bay_1_status);
	placeholder->GetWindowRect(&rect);
	placeholder->GetWindowText(s);
	placeholder->DestroyWindow();
	ScreenToClient(&rect);
	m_BUTTON_bay_1_status.SetColorSelected(RGB(192,192,0));
    m_BUTTON_bay_1_status.SetColorFocus(RGB(255,192,0));
    m_BUTTON_bay_1_status.SetColor(RGB(255,255,0));
	m_BUTTON_bay_1_status.Create((LPCTSTR)s,
		WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_OWNERDRAW,
		rect,this,IDC_BUTTON_bay_1_status);
	m_BUTTON_bay_1_status.SetTrigger( RGB(0,255,0),_T("Bay Can Run"),
									  RGB(255,0,0),_T("Bay Needs to Home"),
						  300,(BOOL *)&(g_Axes.is_hommed));
	
	/*
	//bay 2 condition indicator button.
	placeholder = GetDlgItem(IDC_BUTTON_bay_2_status);
	placeholder->GetWindowRect(&rect);
	placeholder->GetWindowText(s);
	placeholder->DestroyWindow();
	ScreenToClient(&rect);
	m_BUTTON_bay_2_status.SetColorSelected(RGB(192,192,0));
    m_BUTTON_bay_2_status.SetColorFocus(RGB(255,192,0));
    m_BUTTON_bay_2_status.SetColor(RGB(255,255,0));
	m_BUTTON_bay_2_status.Create((LPCTSTR)s,
		WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_OWNERDRAW,
		rect,this,IDC_BUTTON_bay_2_status);
	m_BUTTON_bay_2_status.SetTrigger( RGB(255,0,0),_T("Bay2   Run"),
									  RGB(0,255,0),_T("Bay2   Stop"),
						  300,(BOOL *)&(g_Axes.is_hommed));


	//bay 3 condition indicator button.
	placeholder = GetDlgItem(IDC_BUTTON_bay_3_status);
	placeholder->GetWindowRect(&rect);
	placeholder->GetWindowText(s);
	placeholder->DestroyWindow();
	ScreenToClient(&rect);
	m_BUTTON_bay_3_status.SetColorSelected(RGB(192,192,0));
    m_BUTTON_bay_3_status.SetColorFocus(RGB(255,192,0));
    m_BUTTON_bay_3_status.SetColor(RGB(255,255,0));
	m_BUTTON_bay_3_status.Create((LPCTSTR)s,
		WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_OWNERDRAW,
		rect,this,IDC_BUTTON_bay_3_status);
	m_BUTTON_bay_3_status.SetTrigger( RGB(255,0,0),_T("Bay3   Run"),
									  RGB(0,255,0),_T("Bay3   Stop"),
						  300,(BOOL *)&(g_Axes.is_hommed));
	

	//bay 4 condition indicator button.
	placeholder = GetDlgItem(IDC_BUTTON_bay_4_status);
	placeholder->GetWindowRect(&rect);
	placeholder->GetWindowText(s);
	placeholder->DestroyWindow();
	ScreenToClient(&rect);
	m_BUTTON_bay_4_status.SetColorSelected(RGB(192,192,0));
    m_BUTTON_bay_4_status.SetColorFocus(RGB(255,192,0));
    m_BUTTON_bay_4_status.SetColor(RGB(255,255,0));
	m_BUTTON_bay_4_status.Create((LPCTSTR)s,
		WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_OWNERDRAW,
		rect,this,IDC_BUTTON_bay_4_status);
	m_BUTTON_bay_4_status.SetTrigger( RGB(255,0,0),_T("Bay4   Run"),
									  RGB(0,255,0),_T("Bay4   Stop"),
						  300,(BOOL *)&(g_Axes.is_hommed));
	*/
	
	//"labels"
	DWORD idcs[4];
	idcs[0] = IDC_EDIT_bay_1_chip_type_text;
	/*
	idcs[1] = IDC_EDIT_bay_2_chip_type_text;
	idcs[2] = IDC_EDIT_bay_3_chip_type_text;
	idcs[3] = IDC_EDIT_bay_4_chip_type_text;
	*/
	for(int i=0;i<1;i++)
	{
	  placeholder = GetDlgItem(idcs[i]);
	  placeholder->GetWindowRect(&rect);
	  placeholder->GetWindowText(s);
	  placeholder->DestroyWindow();
	  ScreenToClient(&rect);
	  m_idcs[i].SetColorSelected(RGB(192,192,255));
      m_idcs[i].SetColorFocus(RGB(255,255,192));
      m_idcs[i].SetColor(RGB(255,255,235));
	  m_idcs[i].Create((LPCTSTR)s,
		WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON|BS_OWNERDRAW,
		rect,this,idcs[i]);
	}



	//IDCANCEL
	placeholder = GetDlgItem(IDCANCEL);
	placeholder->GetWindowRect(&rect);
	placeholder->GetWindowText(s);
	placeholder->DestroyWindow();
	ScreenToClient(&rect);
	m_IDCANCEL.SetColorSelected(RGB(255,255,0));
    m_IDCANCEL.SetColorFocus(RGB(255,191,0));
    m_IDCANCEL.SetColor(RGB(191,191,0));
	m_IDCANCEL.Create((LPCTSTR)s,
		WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_PUSHBUTTON|BS_OWNERDRAW,
		rect,this,IDCANCEL);



	//IDOK
	placeholder = GetDlgItem(IDOK);
	placeholder->GetWindowRect(&rect);
	placeholder->GetWindowText(s);
	placeholder->DestroyWindow();
	ScreenToClient(&rect);
	m_IDOK.SetColorSelected(RGB(0,255,191));
    m_IDOK.SetColorFocus(RGB(191,255,0));
    m_IDOK.SetColor(RGB(0,255,0));
	m_IDOK.Create((LPCTSTR)s,
		WS_CHILD|WS_VISIBLE|WS_TABSTOP|BS_PUSHBUTTON|BS_OWNERDRAW,
		rect,this,IDOK);




	//let the focus be on the product type field.
	GetDlgItem(IDC_EDIT_product_type)->SetFocus();
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProductTypeDlg::OnCancel() 
{
	
	if(m_timer) 
	{
		KillTimer(m_timer);
		m_timer = 0;
	}

	m_IDOK.m_text = _T(" System Production Run");
	m_IDOK.Invalidate();

	
	AfxMessageBox("Here will be processing of checking.");
	
	return;
	//This is for unrecreatable error condition - retrn with IDCANCEL.
	//CDialog::OnCancel();
}

void CProductTypeDlg::OnOK() 
{
	if(m_timer) return;

    m_IDOK.m_text = _T(" Press Start Button");
	m_IDOK.Invalidate();

	m_timer = SetTimer(0,50,NULL);
	
	return;

	//CDialog::OnOK();
}

void CProductTypeDlg::OnTimer(UINT nIDEvent) 
{
	 
	if(g_machine_state.is_started)
	{
	 	KillTimer(m_timer);
		//CDialog::OnOK();
		DestroyWindow();
	}
}

void CProductTypeDlg::PostNcDestroy() 
{
	*m_indicator = 0;
	delete this;
}
