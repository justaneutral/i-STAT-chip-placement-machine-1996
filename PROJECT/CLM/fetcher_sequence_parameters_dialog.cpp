// fetcher_sequence_parameters_dialog.cpp : implementation file
//

#include "afxdisp.h"
#include "stdafx.h"
#include "fetcher_sequence_parameters_dialog.h"


#include "globals.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// fetcher_sequence_parameters_dialog dialog


fetcher_sequence_parameters_dialog::fetcher_sequence_parameters_dialog(CWnd* pParent /*=NULL*/)
	: CDialog(fetcher_sequence_parameters_dialog::IDD, pParent)
{
	EnableAutomation();
	
	//{{AFX_DATA_INIT(fetcher_sequence_parameters_dialog)
	
	m_fetcher_clear = g_machine ? g_machine->fetcher_clear : 0.0;
	m_fetcher_justify = g_machine ? g_machine->fetcher_justify : 0.0;
	m_fetcher_magazine = g_machine ? g_machine->fetcher_magazine : 0.0;
	m_fetcher_stretcher = g_machine ? g_machine->fetcher_stretcher : 0.0;
	m_magazine_capacity = g_machine ? g_machine->magazine_capacity : 0.0;
	m_magazine_increment = g_machine ? g_machine->magazine_increment : 0.0;
	m_magazine_origin = g_machine ? g_machine->magazine_origin : 0.0;
	m_stage_wafer_x = g_machine ? g_machine->stage_wafer_x : 0.0;
	m_stage_wafer_y = g_machine ? g_machine->stage_wafer_y : 0.0;
	m_wafer_x_justify = g_machine ? g_machine->wafer_x_justify : 0.0;
	m_wafer_x_pick = g_machine ? g_machine->wafer_x_pick : 0.0;
	m_wafer_y_justify = g_machine ? g_machine->wafer_y_justify : 0.0;
	m_wafer_y_pick = g_machine ? g_machine->wafer_y_pick : 0.0;
	//}}AFX_DATA_INIT
}


void fetcher_sequence_parameters_dialog::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void fetcher_sequence_parameters_dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(fetcher_sequence_parameters_dialog)
	DDX_Text(pDX, IDC_fetcher_sequence_parameters_fetcher_clear, m_fetcher_clear);
	DDX_Text(pDX, IDC_fetcher_sequence_parameters_fetcher_justify, m_fetcher_justify);
	DDX_Text(pDX, IDC_fetcher_sequence_parameters_fetcher_magazine, m_fetcher_magazine);
	DDX_Text(pDX, IDC_fetcher_sequence_parameters_fetcher_stretcher, m_fetcher_stretcher);
	DDX_Text(pDX, IDC_fetcher_sequence_parameters_magazine_capacity, m_magazine_capacity);
	DDX_Text(pDX, IDC_fetcher_sequence_parameters_magazine_increment, m_magazine_increment);
	DDX_Text(pDX, IDC_fetcher_sequence_parameters_magazine_origin, m_magazine_origin);
	DDX_Text(pDX, IDC_fetcher_sequence_parameters_stage_wafer_x, m_stage_wafer_x);
	DDX_Text(pDX, IDC_fetcher_sequence_parameters_stage_wafer_y, m_stage_wafer_y);
	DDX_Text(pDX, IDC_fetcher_sequence_parameters_wafer_x_justify, m_wafer_x_justify);
	DDX_Text(pDX, IDC_fetcher_sequence_parameters_wafer_x_pick, m_wafer_x_pick);
	DDX_Text(pDX, IDC_fetcher_sequence_parameters_wafer_y_justify, m_wafer_y_justify);
	DDX_Text(pDX, IDC_fetcher_sequence_parameters_wafer_y_pick, m_wafer_y_pick);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(fetcher_sequence_parameters_dialog, CDialog)
	//{{AFX_MSG_MAP(fetcher_sequence_parameters_dialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(fetcher_sequence_parameters_dialog, CDialog)
	//{{AFX_DISPATCH_MAP(fetcher_sequence_parameters_dialog)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_Ifetcher_sequence_parameters_dialog to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {186F5911-0AAE-11D2-8C94-00C04FAD4034}
static const IID IID_Ifetcher_sequence_parameters_dialog =
{ 0x186f5911, 0xaae, 0x11d2, { 0x8c, 0x94, 0x0, 0xc0, 0x4f, 0xad, 0x40, 0x34 } };

BEGIN_INTERFACE_MAP(fetcher_sequence_parameters_dialog, CDialog)
	INTERFACE_PART(fetcher_sequence_parameters_dialog, IID_Ifetcher_sequence_parameters_dialog, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// fetcher_sequence_parameters_dialog message handlers

void fetcher_sequence_parameters_dialog::OnOK() 
{
    Save();	
	CDialog::OnOK();
}

void fetcher_sequence_parameters_dialog::Save()
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

	
	g_machine->fetcher_clear      = m_fetcher_clear;
	g_machine->fetcher_justify    = m_fetcher_justify;
	g_machine->fetcher_magazine   = m_fetcher_magazine;
	g_machine->fetcher_stretcher  = m_fetcher_stretcher;
	g_machine->magazine_capacity  = m_magazine_capacity;
	g_machine->magazine_increment = m_magazine_increment;
	g_machine->magazine_origin    = m_magazine_origin;
	g_machine->stage_wafer_x      = m_stage_wafer_x;
	g_machine->stage_wafer_y      = m_stage_wafer_y;
	g_machine->wafer_x_justify    = m_wafer_x_justify;
	g_machine->wafer_x_pick       = m_wafer_x_pick;
    g_machine->wafer_y_justify    = m_wafer_y_justify;
	g_machine->wafer_y_pick       = m_wafer_y_pick;
}
