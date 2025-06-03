// pick_place_point_dialog.cpp : implementation file
//

#include "afxdisp.h"
#include "stdafx.h"
#include "globals.h"
#include "pick_place_point_dialog.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// pick_place_point_dialog dialog


pick_place_point_dialog::pick_place_point_dialog(double * px,
												 double * py,
												 double * pz,
												 double * pangle,
												 CWnd* pParent /*=NULL*/)
	: CDialog(pick_place_point_dialog::IDD, pParent)

{
	EnableAutomation();

	x = px;
	y = py;
	z = pz;
	angle = pangle;

	//{{AFX_DATA_INIT(pick_place_point_dialog)
	m_angle =*angle;
	m_x = *x;
	m_y = *y;
	m_z = *z;
	//}}AFX_DATA_INIT
}


void pick_place_point_dialog::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void pick_place_point_dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(pick_place_point_dialog)
	DDX_Text(pDX, IDC_pick_place_angle, m_angle);
	DDX_Text(pDX, IDC_pick_place_x, m_x);
	DDX_Text(pDX, IDC_pick_place_y, m_y);
	DDX_Text(pDX, IDC_pick_place_z, m_z);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(pick_place_point_dialog, CDialog)
	//{{AFX_MSG_MAP(pick_place_point_dialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(pick_place_point_dialog, CDialog)
	//{{AFX_DISPATCH_MAP(pick_place_point_dialog)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_Ipick_place_point_dialog to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {2408D215-0B70-11D2-8C96-00C04FAD4034}
static const IID IID_Ipick_place_point_dialog =
{ 0x2408d215, 0xb70, 0x11d2, { 0x8c, 0x96, 0x0, 0xc0, 0x4f, 0xad, 0x40, 0x34 } };

BEGIN_INTERFACE_MAP(pick_place_point_dialog, CDialog)
	INTERFACE_PART(pick_place_point_dialog, IID_Ipick_place_point_dialog, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// pick_place_point_dialog message handlers

void pick_place_point_dialog::OnOK() 
{
    Save();	
	CDialog::OnOK();
}


void pick_place_point_dialog::Save()
{

	if(!UpdateData(TRUE))
	{
	   AfxMessageBox("Wrong data entered");
	   return;
	}
	
	*x = m_x;
	*y = m_y;
	*z = m_z;
	*angle = m_angle;
}
