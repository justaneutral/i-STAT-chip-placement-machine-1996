// motor_parameters_dialog.cpp : implementation file
//

#include "afxdisp.h"
#include "stdafx.h"
#include "globals.h"
#include "motor_parameters_dialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// motor_parameters_dialog dialog


motor_parameters_dialog::motor_parameters_dialog(int index, CWnd* pParent /*=NULL*/)
	: CDialog(motor_parameters_dialog::IDD, pParent)
{
	
	EnableAutomation();

	m_index = index;

	if(m_index < 0 || m_index > 9) 
	{
		AfxMessageBox("Incorrect Axes Number");
	}
	

	//{{AFX_DATA_INIT(motor_parameters_dialog)
	m_acceleration = g_machine && m_index>0 && m_index<9 ? g_machine->acceleration[m_index-1] : 0.0;
	m_homming_acceleration = g_machine && m_index>0 && m_index<9 ? g_machine->home_acceleration[m_index-1] : 0.0;
	m_hommind_correction_end = g_machine && m_index>0 && m_index<9 ? g_machine->home_correction_end[m_index-1] : 0.0;
	m_homming_correction_start = g_machine && index>0 && m_index<9 ? g_machine->home_correction_start[m_index-1] : 0.0;
	m_homming_distance = g_machine && m_index>0 && m_index<9 ? g_machine->home_distance[m_index-1] : 0.0;
	m_homming_tolerance = g_machine && m_index>0 && m_index<9 ? g_machine->home_tolerance[m_index-1] : 0.0;
	m_homming_torque = g_machine && m_index>0 && m_index<9 ? g_machine->home_torque[m_index-1] : 0.0;
	m_homming_velocity = g_machine && m_index>0 && m_index<9 ? g_machine->home_velocity[m_index-1] : 0.0;
	m_torque = g_machine && m_index>0 && m_index<9 ? g_machine->torque[m_index-1] : 0.0;
	m_velocity = g_machine && m_index>0 && m_index<9 ? g_machine->velocity[m_index-1] : 0.0;
	//}}AFX_DATA_INIT
}


void motor_parameters_dialog::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void motor_parameters_dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(motor_parameters_dialog)
	DDX_Text(pDX, IDC_motor_parameters_acceleration, m_acceleration);
	DDX_Text(pDX, IDC_motor_parameters_homming_acceleration, m_homming_acceleration);
	DDX_Text(pDX, IDC_motor_parameters_homming_correction_end, m_hommind_correction_end);
	DDX_Text(pDX, IDC_motor_parameters_homming_correction_start, m_homming_correction_start);
	DDX_Text(pDX, IDC_motor_parameters_homming_distance, m_homming_distance);
	DDX_Text(pDX, IDC_motor_parameters_homming_tolerance, m_homming_tolerance);
	DDX_Text(pDX, IDC_motor_parameters_homming_torque, m_homming_torque);
	DDX_Text(pDX, IDC_motor_parameters_homming_velocity, m_homming_velocity);
	DDX_Text(pDX, IDC_motor_parameters_torque, m_torque);
	DDX_Text(pDX, IDC_motor_parameters_velocity, m_velocity);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(motor_parameters_dialog, CDialog)
	//{{AFX_MSG_MAP(motor_parameters_dialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(motor_parameters_dialog, CDialog)
	//{{AFX_DISPATCH_MAP(motor_parameters_dialog)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_Imotor_parameters_dialog to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {186F5913-0AAE-11D2-8C94-00C04FAD4034}
static const IID IID_Imotor_parameters_dialog =
{ 0x186f5913, 0xaae, 0x11d2, { 0x8c, 0x94, 0x0, 0xc0, 0x4f, 0xad, 0x40, 0x34 } };

BEGIN_INTERFACE_MAP(motor_parameters_dialog, CDialog)
	INTERFACE_PART(motor_parameters_dialog, IID_Imotor_parameters_dialog, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// motor_parameters_dialog message handlers

void motor_parameters_dialog::Save()
{
	
   if(!g_machine) 
   {
	   AfxMessageBox("Settings structure is not initialized");
	   return;
   }

   if(!(m_index>0 && m_index<9))
   {
	   AfxMessageBox("Axes number is not valid : must be 1 to 9");
	   return;
   }

   
   if(!UpdateData(TRUE))
   {
	   AfxMessageBox("Settings are not retreived");
	   return;
   }

	g_machine->acceleration[m_index-1] = m_acceleration;
	g_machine->home_acceleration[m_index-1] = m_homming_acceleration;
	g_machine->home_correction_end[m_index-1] = m_hommind_correction_end;
	g_machine->home_correction_start[m_index-1] = m_homming_correction_start;
	g_machine->home_distance[m_index-1] = m_homming_distance;
	g_machine->home_tolerance[m_index-1] = m_homming_tolerance;
	g_machine->home_torque[m_index-1] = m_homming_torque;
	g_machine->home_velocity[m_index-1] = m_homming_velocity;
	g_machine->torque[m_index-1] = m_torque;
	g_machine->velocity[m_index-1] = m_velocity;
}


void motor_parameters_dialog::OnOK() 
{
    Save();	
	CDialog::OnOK();
}
