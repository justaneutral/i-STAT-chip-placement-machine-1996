// machineState.cpp : implementation file
//

#include "stdafx.h"
#include "globals.h"
#include <assert.h>
#include <afxdisp.h>
#include "machineState.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// machineState dialog


machineState::machineState(CWnd* pParent /*=NULL*/)
	: CDialog(machineState::IDD, pParent)
{
	EnableAutomation();

	is_started = false;
	m_error = false;
	status = _T("st");
	product =_T("pr");
	vision = _T("vis");


	settingsfile = (FILE *)NULL;
	
    if(!update()) 
	{	
       recreate();
       update();
	}


	
	// {{AFX_DATA_INIT(machineState)
	//m_chip_is_picked_up = FALSE;
	//m_magazine_is_ready = FALSE;
	//m_wafer_is_loaded = FALSE;
	//m_chip_position = 0;
	//m_file_name = _T("");
	//m_wafer_position = 0;
	// }}AFX_DATA_INIT
}


machineState::~machineState()
{
  if(settingsfile) 
  {
    fclose(settingsfile);
	settingsfile = (FILE *)NULL;
  }
  
  CDialog::~CDialog();
}



void machineState::OnFinalRelease()
{
	CDialog::OnFinalRelease();
}


void machineState::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(machineState)
	DDX_Check(pDX, IDC_CHECK_machine_state_chip_is_picked_up, m_chip_is_picked_up);
	DDX_Check(pDX, IDC_CHECK_magazine_is_ready, m_magazine_is_ready);
	DDX_Check(pDX, IDC_CHECK_wafer_is_loaded, m_wafer_is_loaded);
	DDX_Text(pDX, IDC_EDIT_machine_state_chip_position, m_chip_position);
	DDX_Text(pDX, IDC_EDIT_machine_state_file_name, m_file_name);
	DDX_Text(pDX, IDC_EDIT_machine_state_wafer_position, m_wafer_position);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(machineState, CDialog)
	//{{AFX_MSG_MAP(machineState)
	ON_BN_CLICKED(IDC_BUTTON_machine_state_file_name, OnBUTTONmachinestatefilename)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(machineState, CDialog)
	//{{AFX_DISPATCH_MAP(machineState)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ImachineState to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {02029AE1-1030-11D2-8C9A-00C04FAD4034}
static const IID IID_ImachineState =
{ 0x2029ae1, 0x1030, 0x11d2, { 0x8c, 0x9a, 0x0, 0xc0, 0x4f, 0xad, 0x40, 0x34 } };

BEGIN_INTERFACE_MAP(machineState, CDialog)
	INTERFACE_PART(machineState, IID_ImachineState, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// machineState message handlers

void machineState::OnBUTTONmachinestatefilename() 
{
   //get the current state.
   UpdateData(TRUE);	
   CFileDialog dlg(TRUE);
   if(dlg.DoModal() == IDOK)
   {
	 m_file_name = dlg.GetPathName();
     //Save();
     //correct the file name on the screen.
     UpdateData(FALSE);
   }
}


//////functions...
bool machineState::recreate()
{
	  int i = 0;

	  //initial case.
	  
	  m_chip_is_picked_up = false;
	  m_chip_position = 0;
	  m_file_name = CString("");
	  m_magazine_is_ready = true;
	  m_wafer_is_loaded = false;
	  m_wafer_position = 0;

	  return Save();
}



bool machineState::Save()
{
	machina_state_DS m_ds;

    m_ds.m_chip_is_picked_up = (BOOL)(m_chip_is_picked_up == TRUE);
	m_ds.m_chip_position = m_chip_position;
	m_ds.m_magazine_is_ready = (BOOL)(m_magazine_is_ready == TRUE);
	m_ds.m_wafer_is_loaded = (BOOL)(m_wafer_is_loaded == TRUE);
	m_ds.m_wafer_position = m_wafer_position;
	strcpy(m_ds.m_file_name,(LPCTSTR)m_file_name);

	bool b=false;
    b=write_data("c:\\machine.snp", &m_ds);
	if(b) b=read_data("c:\\machine.snp", &m_ds);
    return b;
}


bool machineState::update()
{
	machina_state_DS m_ds;
	
	bool b=false;
    b=read_data("c:\\machine.snp", &m_ds);
    
	if(b)
	{
	   m_chip_is_picked_up = m_ds.m_chip_is_picked_up;
	   m_chip_position = m_ds.m_chip_position;
	   m_magazine_is_ready = m_ds.m_magazine_is_ready;
	   m_wafer_is_loaded = m_ds.m_wafer_is_loaded;
	   m_wafer_position = m_ds.m_wafer_position;
	   m_file_name = CString(m_ds.m_file_name);

	   
	}
	else
    {
	   m_chip_is_picked_up = 0;
	   m_chip_position = 1;
	   m_magazine_is_ready = 1;
	   m_wafer_is_loaded = 0;
	   m_wafer_position = 1;
	   m_file_name = CString("<File Read Error>");
	}
	return b;
}








void machineState::OnOK() 
{
    UpdateData(TRUE);
	Save();	

	CDialog::OnOK();
}
