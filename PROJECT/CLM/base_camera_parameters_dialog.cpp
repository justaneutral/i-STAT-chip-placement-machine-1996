// base_camera_parameters_dialog.cpp : implementation file
//

#include "afxdisp.h"
#include "stdafx.h"
#include "globals.h"
#include "base_camera_parameters_dialog.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// base_camera_parameters_dialog dialog


base_camera_parameters_dialog::base_camera_parameters_dialog(CWnd* pParent /*=NULL*/)
	: CDialog(base_camera_parameters_dialog::IDD, pParent)
{
	EnableAutomation();

	//{{AFX_DATA_INIT(base_camera_parameters_dialog)
	m_base_camera_angle = g_machine ? g_machine->base_camera_angle : 0.0;
	m_base_camera_x = g_machine ? g_machine->base_camera_x : 0.0;
	m_base_camera_y = g_machine ? g_machine->base_camera_y : 0.0;
	//}}AFX_DATA_INIT
}


void base_camera_parameters_dialog::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void base_camera_parameters_dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(base_camera_parameters_dialog)
	DDX_Text(pDX, IDC_base_cmera_parameters_angle, m_base_camera_angle);
	DDX_Text(pDX, IDC_base_cmera_parameters_x, m_base_camera_x);
	DDX_Text(pDX, IDC_base_cmera_parameters_y, m_base_camera_y);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(base_camera_parameters_dialog, CDialog)
	//{{AFX_MSG_MAP(base_camera_parameters_dialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(base_camera_parameters_dialog, CDialog)
	//{{AFX_DISPATCH_MAP(base_camera_parameters_dialog)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_Ibase_camera_parameters_dialog to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {2408D211-0B70-11D2-8C96-00C04FAD4034}
static const IID IID_Ibase_camera_parameters_dialog =
{ 0x2408d211, 0xb70, 0x11d2, { 0x8c, 0x96, 0x0, 0xc0, 0x4f, 0xad, 0x40, 0x34 } };

BEGIN_INTERFACE_MAP(base_camera_parameters_dialog, CDialog)
	INTERFACE_PART(base_camera_parameters_dialog, IID_Ibase_camera_parameters_dialog, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// base_camera_parameters_dialog message handlers

void base_camera_parameters_dialog::OnOK() 
{
	Save();
	CDialog::OnOK();
}


void base_camera_parameters_dialog::Save()
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


   g_machine->base_camera_x     = m_base_camera_x;
   g_machine->base_camera_y     = m_base_camera_y;
   g_machine->base_camera_angle = m_base_camera_angle;
}