// wafer_camera_parameters_dialog.cpp : implementation file
//

#include "afxdisp.h"
#include "stdafx.h"
#include "globals.h"
#include "wafer_camera_parameters_dialog.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// wafer_camera_parameters_dialog dialog


wafer_camera_parameters_dialog::wafer_camera_parameters_dialog(CWnd* pParent /*=NULL*/)
	: CDialog(wafer_camera_parameters_dialog::IDD, pParent)
{
	EnableAutomation();

	//{{AFX_DATA_INIT(wafer_camera_parameters_dialog)
	m_wafer_camera_angle = g_machine ? g_machine->wafer_camera_angle : 0.0;
	m_wafer_camera_back_x = g_machine ? g_machine->wafer_camera_back_x : 0.0;
	m_wafer_camera_back_y = g_machine ? g_machine->wafer_camera_back_y : 0.0;
	m_wafer_camera_forward_x = g_machine ? g_machine->wafer_camera_forward_x : 0.0;
	m_wafer_camera_forward_y = g_machine ? g_machine->wafer_camera_forward_y : 0.0;
	m_wafer_camera_x = g_machine ? g_machine->wafer_camera_x : 0.0;
	m_wafer_camera_y = g_machine ? g_machine->wafer_camera_y : 0.0;
	//}}AFX_DATA_INIT
}


void wafer_camera_parameters_dialog::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void wafer_camera_parameters_dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(wafer_camera_parameters_dialog)
	DDX_Text(pDX, IDC_wafer_cmera_parameters_angle, m_wafer_camera_angle);
	DDX_Text(pDX, IDC_wafer_cmera_parameters_back_x, m_wafer_camera_back_x);
	DDX_Text(pDX, IDC_wafer_cmera_parameters_back_y, m_wafer_camera_back_y);
	DDX_Text(pDX, IDC_wafer_cmera_parameters_forward_x, m_wafer_camera_forward_x);
	DDX_Text(pDX, IDC_wafer_cmera_parameters_forward_y, m_wafer_camera_forward_y);
	DDX_Text(pDX, IDC_wafer_cmera_parameters_x, m_wafer_camera_x);
	DDX_Text(pDX, IDC_wafer_cmera_parameters_y, m_wafer_camera_y);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(wafer_camera_parameters_dialog, CDialog)
	//{{AFX_MSG_MAP(wafer_camera_parameters_dialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(wafer_camera_parameters_dialog, CDialog)
	//{{AFX_DISPATCH_MAP(wafer_camera_parameters_dialog)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_Iwafer_camera_parameters_dialog to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {2408D219-0B70-11D2-8C96-00C04FAD4034}
static const IID IID_Iwafer_camera_parameters_dialog =
{ 0x2408d219, 0xb70, 0x11d2, { 0x8c, 0x96, 0x0, 0xc0, 0x4f, 0xad, 0x40, 0x34 } };

BEGIN_INTERFACE_MAP(wafer_camera_parameters_dialog, CDialog)
	INTERFACE_PART(wafer_camera_parameters_dialog, IID_Iwafer_camera_parameters_dialog, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// wafer_camera_parameters_dialog message handlers


void wafer_camera_parameters_dialog::OnOK() 
{
    Save();	
	CDialog::OnOK();
}

void wafer_camera_parameters_dialog::Save()
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

	g_machine->wafer_camera_angle = m_wafer_camera_angle;
	g_machine->wafer_camera_x = m_wafer_camera_x;
	g_machine->wafer_camera_y = m_wafer_camera_y;
	g_machine->wafer_camera_back_x = m_wafer_camera_back_x;
	g_machine->wafer_camera_back_y = m_wafer_camera_back_y;
	g_machine->wafer_camera_forward_x = m_wafer_camera_forward_x;
	g_machine->wafer_camera_forward_y = m_wafer_camera_forward_y;
}
