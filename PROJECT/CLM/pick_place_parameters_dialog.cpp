// pick_place_parameters_dialog.cpp : implementation file
//
#include "afxdisp.h"
#include "stdafx.h"
#include "globals.h"
#include "pick_place_parameters_dialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// pick_place_parameters_dialog dialog


pick_place_parameters_dialog::pick_place_parameters_dialog(CWnd* pParent /*=NULL*/)
	: CDialog(pick_place_parameters_dialog::IDD, pParent)
{
	EnableAutomation();

	//{{AFX_DATA_INIT(pick_place_parameters_dialog)
	m_grip_air_delay = g_machine ? g_machine->grip_air_delay : 0.0;
	m_needle_timer = g_machine ? g_machine->needle_timer : 0.0;
	m_smac_delay = g_machine ? g_machine->smac_delay : 0.0;
	m_smac_z_clearance = g_machine ? g_machine->smac_z_clearance : 0.0;
	m_grip_close_delay = g_machine ? g_machine->grip_close_delay : 0.0;
	//}}AFX_DATA_INIT
}


void pick_place_parameters_dialog::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void pick_place_parameters_dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(pick_place_parameters_dialog)
	DDX_Text(pDX, IDC_pick_place_parameters_grip_air_delay, m_grip_air_delay);
	DDX_Text(pDX, IDC_pick_place_parameters_needle_timer, m_needle_timer);
	DDX_Text(pDX, IDC_pick_place_parameters_smac_delay, m_smac_delay);
	DDX_Text(pDX, IDC_pick_place_parameters_smac_z_clearance, m_smac_z_clearance);
	DDX_Text(pDX, IDC_pick_place_parameters_grip_close_delay, m_grip_close_delay);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(pick_place_parameters_dialog, CDialog)
	//{{AFX_MSG_MAP(pick_place_parameters_dialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(pick_place_parameters_dialog, CDialog)
	//{{AFX_DISPATCH_MAP(pick_place_parameters_dialog)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_Ipick_place_parameters_dialog to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {2408D213-0B70-11D2-8C96-00C04FAD4034}
static const IID IID_Ipick_place_parameters_dialog =
{ 0x2408d213, 0xb70, 0x11d2, { 0x8c, 0x96, 0x0, 0xc0, 0x4f, 0xad, 0x40, 0x34 } };

BEGIN_INTERFACE_MAP(pick_place_parameters_dialog, CDialog)
	INTERFACE_PART(pick_place_parameters_dialog, IID_Ipick_place_parameters_dialog, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// pick_place_parameters_dialog message handlers

void pick_place_parameters_dialog::OnOK() 
{
   if(!g_machine) 
   {
	   AfxMessageBox("Settings structure is not initialized");
	   return;
   }


   if(!UpdateData(TRUE))
   {
	   AfxMessageBox("Settings are not retreived");
	   return;
   }

	g_machine->grip_close_delay = m_grip_close_delay;
    g_machine->grip_air_delay = m_grip_air_delay;
	g_machine->needle_timer = m_needle_timer;
	g_machine->smac_delay = m_smac_delay;
    g_machine->smac_z_clearance = m_smac_z_clearance;	
	
	CDialog::OnOK();
}
