// MotorManipulatorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "globals.h"
#include "MotorManipulatorDlg.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMotorManipulatorDlg dialog


CMotorManipulatorDlg::CMotorManipulatorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMotorManipulatorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMotorManipulatorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	for(address = 1; address <=  7; address++)
	{
		activated[address] = false;
	    g_Axes.GetInchPosition(position[address],address);
	}

	activated[8] = false;
	position[8] = g_smac.current_z;
	
	activated[9] = false;
	position[9]	= g_smac.current_a;


}


void CMotorManipulatorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMotorManipulatorDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMotorManipulatorDlg, CDialog)
	//{{AFX_MSG_MAP(CMotorManipulatorDlg)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMotorManipulatorDlg message handlers

void CMotorManipulatorDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CString addr;
	CString s;
	
	GetDlgItem(IDC_EDIT_MANIPULATOR_ADDRESS)->GetWindowText(addr);
	GetDlgItem(IDC_EDIT_MANIPULATOR_POSITION)->GetWindowText(s);

	
	position[address] = atof((LPCTSTR)s);
	
		  
	
	switch(nChar)
	{
	case '-' : position[address] -= exp(double(max(0,nRepCnt-1))); break; 	
	case '+' : position[address] += exp(double(max(0,nRepCnt-1))); break;
	case '1' : addr = '1'; address = 1; break;
	case '2' : addr = '2'; address = 2; break;
	case '3' : addr = '3'; address = 3; break;
	case '4' : addr = '4'; address = 4; break;
	case '5' : addr = '5'; address = 5; break;
	case '6' : addr = '6'; address = 6; break;
	case '7' : addr = '7'; address = 7; break;
	case '8' : addr = '8'; address = 8; break;
	case '9' : addr = '9'; address = 9; break;
	}	
		
	GetDlgItem(IDC_EDIT_MANIPULATOR_ADDRESS)->SetWindowText(addr);

	s.Format("%f",float(position[address]));
	GetDlgItem(IDC_EDIT_MANIPULATOR_POSITION)->SetWindowText(s);

	
	CDialog::OnChar(nChar, nRepCnt, nFlags);
}
