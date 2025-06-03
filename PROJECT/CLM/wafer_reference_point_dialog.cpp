// wafer_reference_point_dialog.cpp : implementation file
//

#include "afxdisp.h"
#include "stdafx.h"
#include "globals.h"
#include "wafer_reference_point_dialog.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// wafer_reference_point_dialog dialog


wafer_reference_point_dialog::wafer_reference_point_dialog(CWnd* pParent /*=NULL*/)
	: CDialog(wafer_reference_point_dialog::IDD, pParent)
{
	EnableAutomation();

	//{{AFX_DATA_INIT(wafer_reference_point_dialog)
	m_wafer_delta_x = g_machine ? g_machine->wafer_delta_x : 0.0;
	m_wafer_delta_y = g_machine ? g_machine->wafer_delta_y : 0.0;
	m_wafer_x = g_machine ? g_machine->wafer_x : 0.0;
	m_wafer_y = g_machine ? g_machine->wafer_y : 0.0;
	//}}AFX_DATA_INIT
}


void wafer_reference_point_dialog::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void wafer_reference_point_dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//m_wafer_x-=(5.38935-3.31395);
    //m_wafer_y-=(4.06355-2.41465);

	//{{AFX_DATA_MAP(wafer_reference_point_dialog)
	DDX_Text(pDX, IDC_wafer_reference_point_delta_x, m_wafer_delta_x);
	DDX_Text(pDX, IDC_wafer_reference_point_delta_y, m_wafer_delta_y);
	DDX_Text(pDX, IDC_wafer_reference_point_x, m_wafer_x);
	DDX_Text(pDX, IDC_wafer_reference_point_y, m_wafer_y);
	//}}AFX_DATA_MAP

	//m_wafer_x+=(5.38935-3.31395);
    //m_wafer_y+=(4.06355-2.41465);

}


BEGIN_MESSAGE_MAP(wafer_reference_point_dialog, CDialog)
	//{{AFX_MSG_MAP(wafer_reference_point_dialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(wafer_reference_point_dialog, CDialog)
	//{{AFX_DISPATCH_MAP(wafer_reference_point_dialog)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_Iwafer_reference_point_dialog to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {2408D217-0B70-11D2-8C96-00C04FAD4034}
static const IID IID_Iwafer_reference_point_dialog =
{ 0x2408d217, 0xb70, 0x11d2, { 0x8c, 0x96, 0x0, 0xc0, 0x4f, 0xad, 0x40, 0x34 } };

BEGIN_INTERFACE_MAP(wafer_reference_point_dialog, CDialog)
	INTERFACE_PART(wafer_reference_point_dialog, IID_Iwafer_reference_point_dialog, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// wafer_reference_point_dialog message handlers

void wafer_reference_point_dialog::OnOK() 
{
    Save();	
	CDialog::OnOK();
}


void wafer_reference_point_dialog::Save()
{
   if(!g_machine)
   {
      AfxMessageBox("Settings data corrupted");
	  return;
   }

   if(!UpdateData(TRUE))
   {
      AfxMessageBox("Incorrect data entered");
	  return;
   }

   g_machine->wafer_x = m_wafer_x;
   g_machine->wafer_y = m_wafer_y;
   g_machine->wafer_delta_x = m_wafer_delta_x;
   g_machine->wafer_delta_y = m_wafer_delta_y;
}