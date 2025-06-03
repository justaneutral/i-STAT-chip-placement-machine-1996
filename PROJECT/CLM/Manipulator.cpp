// Manipulator.cpp : implementation file
//

#include "stdafx.h"
#include "globals.h"
#include "Manipulator.h"
#include <math.h>

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CManipulator dialog


CManipulator::CManipulator(CWnd* pParent /*=NULL*/)
	: CDialog(CManipulator::IDD, pParent)
{
	//{{AFX_DATA_INIT(CManipulator)
	//}}AFX_DATA_INIT
}


void CManipulator::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CManipulator)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CManipulator, CDialog)
	//{{AFX_MSG_MAP(CManipulator)
	ON_WM_MENUCHAR()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CManipulator message handlers


LRESULT CManipulator::OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu) 
{
	if(g_Axes.is_hommed) ChangeState(nChar);
	else 
	MessageBox("Motors are not hommed. Please home the motors first.","=Error=",MB_OK);
	
	return CDialog::OnMenuChar(nChar, nFlags, pMenu);
}




void CManipulator::ChangeState(UINT nChar)
{
	CString addr;
	CString s;
	
	GetDlgItem(IDC_EDIT_MANIPULATOR_ADDRESS)->GetWindowText(addr);
	GetDlgItem(IDC_EDIT_MANIPULATOR_POSITION)->GetWindowText(s);

	
	switch((char)nChar)
	{
	case '_' :
	case '-' : if(m_timer) KillTimer(m_timer);
		       position[address] = atof((LPCTSTR)s);
		       position[address] -= 0.002 * (double(1+10*nRepCnt++));
			   activated[address] = true;
			   m_timer = SetTimer(0,200,NULL);
			   break;
	
	case '=' :		   
	case '+' : if(m_timer) KillTimer(m_timer);
		       position[address] = atof((LPCTSTR)s);
		       position[address] += 0.002 * (double(1+10*nRepCnt++));
			   activated[address] = true;
			   m_timer = SetTimer(0,200,NULL);
			   break;

	case '1' : addr = '1'; address = 1; nRepCnt = 0; break;
	case '2' : addr = '2'; address = 2; nRepCnt = 0; break;
	case '3' : addr = '3'; address = 3; nRepCnt = 0; break;
	case '4' : addr = '4'; address = 4; nRepCnt = 0; break;
	case '5' : addr = '5'; address = 5; nRepCnt = 0; break;
	case '6' : addr = '6'; address = 6; nRepCnt = 0; break;
	case '7' : addr = '7'; address = 7; nRepCnt = 0; break;
	case '8' : addr = '8'; address = 8; nRepCnt = 0; break;
	case '9' : addr = '9'; address = 9; nRepCnt = 0; break;
	}	
		
	GetDlgItem(IDC_EDIT_MANIPULATOR_ADDRESS)->SetWindowText(addr);

	s.Format("%f",(position[address]));
	GetDlgItem(IDC_EDIT_MANIPULATOR_POSITION)->SetWindowText(s);
}

void CManipulator::OnTimer(UINT nIDEvent) 
{
    CString s;

	nRepCnt = 0;
	double pos = 0.0;

	KillTimer(m_timer);
	m_timer = 0;
		
	//qci...
	if(address>=1 && address <=7)
	{
	   if(activated[address])
	   {
		   g_Axes.InchPosition(position[address],pos,address);
		   activated[address] = false;

		   position[address] = pos;

		   s.Format("%f",(position[address]));
	       GetDlgItem(IDC_EDIT_MANIPULATOR_POSITION)->SetWindowText(s);
	   }
	}
	//smac Z.
	else
	if(address == 8)
	{
	   g_smac.InchesMove(position[address]);
	}
	//smac Angle.
	else
	if(address == 9)
	{
	   g_smac.DegreeRotate(position[address]);
	}
	
	
}

int CManipulator::DoModal() 
{
	int ret_code;
	
	nRepCnt = 0;
	m_timer = 0;

	for(address = 1; address <=  7; address++)
	{
		activated[address] = false;
	    g_Axes.GetInchPosition(position[address],address);
	}

	activated[8] = false;
	position[8] = g_smac.current_z;
	
	activated[9] = false;
	position[9]	= g_smac.current_a;
	
	ret_code =  CDialog::DoModal();

	if(m_timer) KillTimer(m_timer);
	m_timer = 0;

	return ret_code;
}
