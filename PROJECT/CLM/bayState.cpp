// bayState.cpp : implementation file
//

#include <afxdisp.h>
#include "stdafx.h"
#include "globals.h"
#include "bayState.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// bayState dialog


bayState::bayState(CWnd* pParent /*=NULL*/)
	: CDialog(bayState::IDD, pParent)
{
	EnableAutomation();

	//{{AFX_DATA_INIT(bayState)
	m_bay = g_machine->bay_number;
	m_active = g_machine->bay_active;
	//}}AFX_DATA_INIT
}


void bayState::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void bayState::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(bayState)
	DDX_Radio(pDX, IDC_RADIO_bay_1, m_bay);
	DDX_Radio(pDX, IDC_RADIO_Active, m_active);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(bayState, CDialog)
	//{{AFX_MSG_MAP(bayState)
	ON_BN_CLICKED(IDC_BUTTON_machine_config, OnBUTTONmachineconfig)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(bayState, CDialog)
	//{{AFX_DISPATCH_MAP(bayState)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IbayState to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {8DFBD023-14FC-11D2-94A5-000000000000}
static const IID IID_IbayState =
{ 0x8dfbd023, 0x14fc, 0x11d2, { 0x94, 0xa5, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 } };

BEGIN_INTERFACE_MAP(bayState, CDialog)
	INTERFACE_PART(bayState, IID_IbayState, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// bayState message handlers


void bayState::OnOK() 
{
	UpdateData(TRUE);
	CDialog::OnOK();
}

void bayState::OnBUTTONmachineconfig() 
{
    	
}
