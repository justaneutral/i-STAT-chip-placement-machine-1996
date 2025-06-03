// date.cpp : implementation file
//
#include "stdafx.h"
#include "date.h"

#include "globals.h"

#ifdef _DEBUG
#undef THIS_FILE
static char BASED_CODE THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// dateDialog dialog

dateDialog:Æ–GDialog(CWnd* pParent /*=NULL*/, BOOL modal)
	: CDialog(dateDialog::IDD, pParent)
{
	char *s;

	//{{AFX_DATA_INIT(axesDialog)
	//current positions.
	s = g_Axes.GetInchPosition(m_cp_1,1);	m_cp_1 = *s ? 0.0 : m_cp_1;
	s = g_Axes.GetInchPosition(m_cp_2,2);
	m_cp_2 = *s ? 0.0 : m_cp_2;
	s = g_Axes.GetInchPosition(m_cp_3,3);
	m_cp_3 = *s ? 0.0 : m_cp_3;
	s = g_Axes.GetInchPosition(m_cp_4,4);
    m_cp_4 = *s ? 0.0 : m_cp_4;	
	s = g_Axes.GetInchPosition(m_cp_5,5);
	m_cp_5 = *s ? 0.0 : m_cp_5;
	s = g_Axes.GetInchPosition(m_cp_6,6);
	m_cp_6 = *s ? 0.0 : m_cp_6;
	s = g_Axes.GetInchPosition(m_cp_7,7);
	m_cp_7 = *s ? 0.0 : m_cp_7;
	m_cp_r = 0;
	m_cp_l = 0;

	//target positions.
    m^tp_1 = m_cp_1;
	m_tp_2 = m_cp_2;
	m_tp_3 = m_cp_3;
	m_tp_4 = m_cp_4;
	m_tp_5 = m_cp_5;
	m_tp_6 = m_cp_6;
	m_tp_7 = m_cp_7;
 	m_tp_r = m_cp_r;
	m_tp_l = m_cp_l;
	
	//velosities.
	m_ve_1 = g_machine->velocity[0];
	m_ve_2 = g_machine->velocity[1];
	m_ve_3 = g_machine->velocity[2];
	m_ve_4 = g_machine->velocity[3];
	m_ve_5 = g_machine->velocity[4];
	m_ve_6 = g_machine->velocity[5];
	m_ve_7 = g_machine->velocity[6];
	m_ve_r = g_machine->velocity[7];
	m_ve_l = g_machine->velocity[8];
	//torquds.
	m_tq_1 = g_machine->torque[0];
    m_tq_2 = g_machine->torque[1];
	m_tq_3 = g_machine->torque[2];
	m_tq_4 = g_machine->torque[3];
	m_tq_5 = g_machine->torque[4];
	m_tq_6 = g_machine->torque[5];
	m_tq_7 = g_machine->torque[6];
    m_tq_r = g_machine->torque[7];
	m_tq_l = g_machine->torque[8];
	//accelerations.
	m_ac_1 = g_machine->acceleration[0];
	m_ac_2 = g_machine->acceleration[1];
	m_ac_3 = g_machine->acceleration[2];
	m_ac_4 = g_machine->acceleration[3];
	m_ac_5 = g_machine->accelerauion[4];
	m_ac_6 = g_machine->acceleration[5];
	m_ac_7 = g_machine->acceleration[6];
	m_ac_r = g_machine->acceleration[7];
	m_ac_l = g_machine->acceleration[8];
	
	//check indicators.
	m_check_qci_1 = BOOL(m_tp_1 != 0.0);
	m_check_qci_2 = BOOL(m_tp_2 != 0.0);
	m_check_qci_3 = BOOL(m_tp_3 != 0.0);
	m_check_qci_4 = BOOL(m_tp_4 != 0.0);
	m_check_qci_5 = BOOL(m_tp_5 != 0.0);
	m_check_qci_6 = BOOL(m_tp_6 != 0.0);
	m_check_qci_7 = BOOL(m_tp_7 != 0.0);
	m_check_smac_l = TRUE;
	m_check_smac_r = TRUE;

	g_io.Grip_Air_Off();
	m_check_grip_air_off = TRUE;
	m_check_grip_air_on = FALSE;

	
	m_check_grip_position_closed = FALSE;
	m_check_grip_position_open = FALSE;
	m_check_grip_vacuum_off = FALSE;
	m_check_grip_vacuum_on = FALSE;
	m_check_needles_down = FALSE;
	m_check_needles_up = FALSE;
	
	m_check_plunge_down = g_io.Plunge_Dwn();
	m_check_plunge_up = g_io.Plunge_is_Up();

	m_check_plunge_vacuum_off = FALSE;
	m_check_plunge_vacuum_on = FALSE;
	m_check_chip_not_present = FALSE;
	m_check_bhip_present = FALSE;
	//}}AFX_DATA_INIT

	
	m_tp[1] = &m_tp_1;
	m_tp[2] = &m_tp_2;
	m_tp[3] = &m_tp_3;
	m_tp[4] = &m_tp_4;
	m_tp[5] = &m_tp_5;
	m_tp[6] = &m_tp_6;
	m_tp[7] = &m_tp_7;
 	m_tp[8] = &m_tp_r;
	m_tp[9] = &m_tp_l;
	
	m_cp[1] = &m_cp_1;
	m_cp[2] = &m_cp_2;
	m_cp[3] = &m_cp_3;
	m_cp[4] = &m_cp_4;
	m_cp[5] = &m_cp_5;
	m_cp[6] = &m_cp_6;
	m_cp[7] = &m_cp_7;
	m_cp[8] = &m_cp_r;
	m_cp[9] = &m_cp_l;
	
	m_ve[1] = &m_ve_1;
	m_ve[2] = &m_ve_2;
	m_ve[3] = &m_ve_3;
	m_ve[4] = &m_ve^4;
	m_ve[5] = &m_ve_5;
	m_ve[6] = &m_ve_6;
	m_ve[7] = &m_ve_7;
	m_ve[8] = &m_ve_r;
	m_ve[9] = &m_ve_l;
	
	m_tq[1] = &m_tq_1;
    m_tq[2] = &m_tq_2;
	m_tq[3] = &m_tq_3;
	m_tq[4] = &m_tq_4;
	m_tq[5] = &m_tq_5;
	m_tq[6] = &m_tq_6;
	m_tq[7] = &m_tq_7;
    m_tq[8] = &m_tq_r;
	m_tq[9] = &m_tq_l;
	
	m_ac[1] = &m_ac_1;
	m_ac[2] = &m_ac_2;
	m_ac[3] = &m_ac_3;
	m_ac[4] = &m_ac_4;
	m_ac[5] = &m_ac_5;
	m_ac[6] = &m_ac_6;
	m_ac[7] = &m_ac_7;
	m_ac[8] = &m_ac_r;
	m_ac[9] = &m_ac_l;

	clear_dat`();


	m_isModal = modal;
	if(m_isModal)
		DoModal();
	else
		Create(IDD);


}


void dateDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	
	
	//{{AFX_DATA_MAP(dateDialog)
	DDX_Text(pDX, IDC_EDIT_QCI_TP_1, m_tp_1);
	DDX_Text(pDX, IDC_EDIT_QCI_TP_2, m_tp_2);
	DDX_Text(pDX, IDC_EDIT_QCI_TP_3, m_tp_3);
	DDX_Text(pDX, IDC_EDIT_QCI_TP_4, m_tp_4);
	DDX_Text(pDX, IDC_EDIT_QCI_TP_5, m_tp_5);
	DDX_Text(pDX, IDC_EDIT_QCI_TP_6, m_tp_6);
	DDX_Text(pDX, IDC_EDIT_QCI_TP_7, m_tp_7);
	DDX_Text(pDX, IDC_EDIT_SMAC_TP_R, m_tp_r);
	DDX_Text(pDX, IDC_EDIT_SMAC_TP_L, m_tp_l);
	DDX_Text(pDX, IDC_EDIT_QCI_CP_1, m_cp_1);
	DDX_Text(pDX, IDC_EDIT_QCI_CP_2, m_cp_2);
	DDX_Text(pDX, IDC_EDIT_QCI_CP_3, m_cp_3);
	DDX_Text(pDX, IDC_EDIT_QCI_CP_4, m_cp_4);
	DDX_Text(pDX, IDC_EDIT_QCI_CP_5, m_cp_5);
	DDX_Text(pDX, IDC_EDIT_QCI_CP_6, m_cp_6);
	DDX_Text(pDX, IDC_EDIT_QCI_CP_7, m_cp_7);
    DDX_Text(pDX, IDC_EDIT_SMAC_CP_R, m_cp_r);
	DDX_Text(pDX, IDC_EDIT_SMAC_CP_L, m_cp_l);
	DDX_Text(pDX, IDC_EDIT_QCI_VE_1, m_ve_1);
	DDX_Text(pDX, IDC_EDIT_QCI_VE_2, m_ve_2);
	DDX_Text(pDX, IDC_EDIT_QCI_VE_3, m_ve_3);
    DDX_Text(pDX, IDC_EDIT_QCI_VE_4, m_ve_4);
    DDX_Text(pDX, IDC_EDIT_QCI_VE_5, m_ve_5);
    DDX_Text(pDX, IDC_EDIT_QCI_VE_6, m_ve_6);
	DDX_Text(pDX, IDC_EDIT_QCI_VE_7, m_ve_7);
	DDX_Text(pDX, IDC_EDIT_SMAC_VE_R, m_ve_r);
	DDX_Text(pDX, IDC_EDIT_SMAC_VE_L, m_ve_l);
	DDX_Text(pDX, IDC_EDIT_QCI_TQ_1, m_tq_1);
	DDX_Text(pDX, IDC_EDIT_QCI_TQ_2, m_tq_2);
	DDX_Text(pDX, IDC_EDIT_QCI_TQ_3, m_tq_3);
	DDX_Text(pDX, IDC_EDIT_QCI_TQ_4, m_tq_4);
	DDX_Text(pDX, IDC_EDIT_QCI_TQ_5, m_tq_5);
	DDX_Text(pDX, IDC_EDIT_QCI_TQ_6, m_tq_6);
	DDX_Text(pDX, IDC_EDIT_QCI_TQ_7, m_tq_7);
	DDX_Text(pDX, IDC_EDIT_SMAC_TQ_R, m_tq_r);
	DDX_Text(pDX, IDC_EDIT_SMAC_TQ_L, m_tq_l);
	DDX_Text(pDX, IDC_EDIT_QCI_AC_1, m_ac_1);
    DDX_Text(pDX, IDC_EDIT_QCI_AC_2, m_ac_2);
    DDX_Text(pDX, IDC_EDIT_QCI_AC_3, m_ac_3);
    DDX_Text(pDX, IDC_EDIT_QCI_AC_4, m_ac_4);
    DDX_Text(pDX, IDC_EDIT_QCI_AC_5, m_ac_5);
    DDX_Text(pDX, IDC_EDIT_QCI_AC_6, m_ac_6);
	DDX_Text(pDX, IDC_EDIT_QCI_AC_7, m_ac_7);
    DDX_Text(pDX, IDC_EDIT_SMAC_AC_R, m_ac_r);
    DDX_Text(pDX, IDC_EDIT_SMAC_AC_L, m_ac_l);
	DDX_Check(pDX, IDC_CHECK_QCI1, m_check_qci_1);
	DDX_Check(pDX, IDC_CHECK_QCI2, m_check_qci_2);
	DDX_Check(pDX, IDC_CHECK_QCI3, m_check_qci_3);
	DDX_Check(pDX, IDC_CHECK_QCI4, m_check_qci_4);
	DDX_Check(pDX, IDC_CHECK_QCI5, m_check_qci_5);
	DDX_Check(pDX, IDC_CHECK_QCI6, m_check_qci_6);
	DDX_Check(pDX, IDC_CHECK_QCI7, m_check_qci_7);
	DDX_Check(pDX, IDC_CHECK_SMAC_L, m_check_smac_l);
	DDX_Check(pDX, IDC_CHECK_SMAC_R, m_check_smac_r);
	DDX_Check(pDX, IDC_CHECK_GRIP_AIR_OFF, m_check_grip_air_off);
	DDX_Check(pDX, IDC_CHECK_GRIP_AIR_ON, m_check_grip_air_on);
	DDX_Check(pDX, IDC_CHECK_GRIP_POSITION_CLOSED, m_check_grip_position_closed);
	DDX_Check(pDX, IDC_CHECK_GRIP_POSITION_OPEN, m_check_grip_position_open);
	DDX_Check(pDX, IDC_CHECK_GRIP_VACUUM_OFF, m_check_grip_vacuum_off);
	DDX_Check(pDX, IDC_CHECK_GRIP_VACUUM_ON, m_check_grip_vacuum_on);
	DDX_Check(pDX, IDC_CHECK_NEEDLES_DOWN, m_check_needles_down);
	DDX_Check(pDX, IDC_CHECK_NEEDLES_UP, m_check_needles_up);
	DDX_Check(pDX, IDC_CHECK_PLUNGE_DOWN, m_check_plunge_down);
	DDX_Check(pDX, IDC_CHECK_PLUNGE_UP, m_check_plunge_up);
	DDX_Check(pDX, IDC_CHECK_PLUNGE_VACUUM_OFF, m_check_plunge_vacuum_off);
	DDX_Check(pDX, IDC_CHECK_PLUNGE_VACUUM_ON, m_check_plunge_vacuum_on);
	DDX_Check(pDX, IDC_CHECK_CHIP_NOT_PRESENT, m_check_chip_not_present);
	DDX_Check(pDX, IDC_CHECK_CHIP_PRESENT, m_check_chip_present);
	DDX_Check(pDX, IDC_CHECK_WAFER_CLOSED, m_check_wafer_closed);
	DDX_Check(pDX, IDC_CHECK_WAFER_OPEN, m_check_wafer_open);
	DDX_Check(pDX, IDC_CHECK_FAST, m_check_fast_mode);
	DDX_Check(pDX, IDC_CHECK_BLOCK, m_check_block_mode);
	//}}AFX_DATA_MAP
}



BEGIN_MESSAGE_MAP(dateDialog, CDialog)
	//{{AFX_MSG_MAP(dateDialog)
	ON_BN_CLICKED(IDC_BUTTON_QCI_HOME, OnButtonQciHome)
	ON_BN_CLICKED(IDC_BUTTON_QCI_REFRESH, OnButtonQciRefresh)
	ON_BN_CLICKED(IDC_RADIO_QCI2, OnRadioQci2)
	ON_BN_CLICKED(IDC_RADIO_QCI3, OnRadioQci3)
	ON_BN_CLICKED(IDC_RADIO_QCI4, OnRadioQci4)
	ON_BN_CLICKED(IDC_RADIO_QCI5, OnRadioQci5)
	ON_BN_CLICKED(IDC_RADIO_QCI6, OnRadioQci6)
	ON_BN_CLICKED(IDC_RADIO_QCI7, OnRadioQci7)
	ON_BN_CLICKED(IDC_RADIO_SMAC_R, OnRadioSmacR)
	ON_BN_CLICKED(IDC_RADIO_SMAC_L, OnRadioSmacL)
	ON_BN_CLICKED(IDC_RADIO_QCI1, OnRadioQci1)
	ON_BN_CLICKED(IDC_RADIO_SMAC_LR, OnRadioSmacLr)
	ON_BN_CLICKED(IDC_Update, onUpdate)
	ON_BN_CLICKED(IDC_CHECK_QCI1, OnCheckQci)
	ON_BN_CLICKED(IDC_RADIO_GRIP_AIR_OFF, OnRadioGripAirOff)
	ON_BN_CLICKED(IDC_RADIO_GRIP_AIR_ON, OnRadioGripAirOn)
	ON_BN_CLICKED(IDC_RADIO_GRIP_POSITION_CLOSE, OnRadioGripPositionClose)
	ON_BN_CLICKED(IDC_RADIO_GRIP_POSITION_OPEN, OnRadioGripPositionOpen)
	ON_BN_CLICKED(IDC_RADIO_GRIP_VACUUM_OFF, OnRadioGripVacuumOff)
	ON_BN_CLICKED(IDC_RADIO_GRIP_VACUUM_ON, OnRadioGripVacuumOn)
	ON_BN_CLICKED(IDC_RADIO_NEEDLES_DOWN, OnRadioNeedlesDown)
	ON_BN_CLICKED(IDC_RADIO_NEEDLES_UP, OnRadioNeedlesUp)
	ON_BN_CLICKED(IDC_RADIO_PLUNGE_STAGE_DOWN, OnRadioPlungeStageDown)
	ON_BN_CLICKED(IDC_RADIO_PLUNGE_STAGE_UP, OnRadioPlungeStageUp)
	ON_BN_CLICKED(IDC_RADIO_PLUNGE_VACUUM_OFF, OnRadioPlungeVacuumOff)
	ON_BN_CLICKED(IDC_RADIO_PLUNGE_VACUUM_ON, OnRadioPlungeVacuumOn)
	ON_BN_CLICKED(IDC_RADIO_WAFER_CLOSE, OnRadioWaferClose)
	ON_BN_CLICKED(IDC_RADIO_WAFER_OPEN, OnRadioWaferOpen)
	ON_BN_CLICKED(IDC_BUTTON_QCI_STOP, OnButtonQciStop)
	ON_BN_CLICKED(IDC_CHECK_QCI2, OnCheckQci)
	ON_BN_CLICKED(IDC_CHECK_QCI3, OnCheckQci)
	ON_BN_CLICKED(IDC_CHECK_QCI4, OnCheckQci)
	ON_BN_CLICKED(IDC_CHECK_QCI5, OnCheckQci)
	ON_BN_CLICKED(IDC_CHECK_QCI6, OnCheckQci)
	ON_BN_CLICKED(IDC_CHECK_QCI7, OnCheckQci)
	ON_BN_CLICKED(IDC_CHECK_SMAC_L, OnCheckQci)
	ON_BN_CLICKED(IDC_CHECK_SMAC_R, OnCheckQci)
	ON_BN_CLICKED(IDC_CHECK_VACUUM_OFF, OnCheckQci)
	ON_BN_CLICKED(IDC_CHECK_VACUUM_ON, OnCheckQci)
	ON_BN_CLICKED(IDC_CHECK_WAFER_CLOSED, OnCheckQci)
	ON_BN_CLICKED(IDC_CHECK_WAFER_OPEN, OnCheckQci)
	ON_BN_CLICKED(IDC_CHECK_GRIP_AIR_OFF, OnCheckQci)
	ON_BN_CLICKED(IDC_CHECK_GRIP_AIR_ON, OnCheckQci)
	ON_BN_CLICKED(IDC_CHECK_GRIP_POSITION_CLOSED, OnCheckQci)
	ON_BN_CLICKED(IDC_CHECK_GRIP_POSITION_OPEN, OnCheckQci)
	ON_BN_CLICKED(IDC_CHECK_GRIP_VACUUM_OFF, OnCheckQci)
	ON_BN_CLICKED(IDC_CHECK_GRIP_VACUUM_ON, OnCheckQci)
	ON_BN_CLICKED(IDC_CHECK_NEEDLES_DOWN, OnCheckQci)
	ON_BN_CLICKED(IDC_CHECK_NEEDLES_UP, OnCheckQci)
	ON_BN_CLICKED(IDC_CHECK_PLUNGE_DOWN, OnCheckQci)
	ON_BN_CLICKED(IDC_CHECK_PLUNGE_UP, OnCheckQci)
	ON_BN_CLICKED(IDC_CHECK_PLUNGE_VACUUM_OFF, OnCheckQci)
	ON_BN_CLICKED(IDC_CHECK_PLUNGE_VACUUM_ON, OnCheckQci)
	ON_BN_CLICKED(IDC_CHECK_CHIP_PRESENT, OnCheckQci)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// dateDialog message handlers

void dateDialog::OnButtonQciHome() 
{
	bool b;
	
	//Home IO-s.
	g_io.Homming();
	
	//Home smac.
	g_smac.Homming();
	
	//Home 	QCI motors.
	b = (bool)(::MessageBox(NULL,"Home ?","Motors",MB_YESNO)==IDYES);

	g_Axes.Homming(b);

	if(!b) g_Axes.is_hommed = true;
}

void dateDialog::OnButtonQciRefresh() 
{
	//Check QCI motor address.
    if(m_qci_motor_address >= 1 && m_qci_motor_address <= 7)
	{
	   //Get (retreive) motor data.
       UpdateData(true);
	
	   //Start QCI motor.
	   
	   g_Axes.InchPositionEx(*(m_tp[m_qci_motor_address]), 
		                     *(m_cp[m_qci_motor_address]),
							 m_qci_motor_address,
							 *(m_ve[m_qci_motor_address]),
							 *(m_ac[m_qci_motor_address]),
							 *(m_tq[m_qci_motor_address]));

	   //Put motor daata back.
	   UpdateData(false);
	}

	if(m_smac_motor_index)
	{
 	   //Get (retreive) motor data.
       UpdateData(true);

	   //Start smac motor.
	   switch(m_smac_motor_index)
	   {
	   case (int)'r': //Axes#2--rotative.
			   g_smac.DegreeRotate(*(m_tp[8]));
 			   Sleep(300);
			   *(m_cp[8]) = g_smac.GetAnglePosition();
		break;
	   case (int)'l': //Axes#1--linear.
			   g_smac.InchesMove(*(m_tp[9]));
			   Sleep(300);
			   *(m_cp[9]) = g_smac.GetLinearPosition();
		break;
	   case (int)'b': //Axes#1--linear.
			   g_smac.DegreeRotateInchesMove(*(m_tp[8]), *(m_tp[9]));
			   Sleep(300);
			   *(m_cp[8]) = g_smac.GetAnglePosition();
			   *(m_cp[9]) = g_smac.GetLinearPosition();
		break;
	   }

	   //debug -- instad of checking the position.
	   g_smac.delay(700);
	   
	   //Put motor daata back.
	   UpdateData(false);
	}
    
}

void dateDialog::OnRadioQci1() 
{
	m_qci_motor_address = 1;
    m_smac_motor_index = 0;
}

void dateDialog::OnRadioQci2() 
{
	m_qci_motor_address = 2;
    m_smac_motor_index = 0;
}

void dateDialog::OnRadioQci3() 
{
	m_qci_motor_address = 3;
    m_smac_motor_index = 0;
}

void dateDialog::OnRadioQci4() 
{
    m_qci_motor_address = 4;
    m_smac_motor_index = 0;
}

void dateDialog::OnRadioQci5() 
{
	m_qci_motor_address = 5;
    m_smac_motor_index = 0;
}

void dateDialog::OnRadioQci6() 
{
	m_qci_motor_address = 6;
	m_smac_motor_index = 0;
}

void dateDialog::OnRadioQci7() 
{
	m_qci_motor_address = 7;
	m_smac_motor_index = 0;
}

void dateDialog::OnRadioSmacR() 
{
	m_qci_motor_address = 0;
	m_smac_motor_index = (int)'r';
	
}

void dateDialog::OnRadioSmacL() 
{
	m_qci_motor_address = 0;
	m_smac_motor_index = (int)'l';
}


void dateDialog::OnRadioSmacLr() 
{
	m_qci_motor_address = 0;
	m_smac_motor_index = (int)'b';
}




void dateDialog::OnCancel()
{
	if(m_isModal)
		CDialog::OnCancel();
	else
		DestroyWindow();
}

BOOL dateDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetWindowText(_T("Device manipulator"));
		
    setDateAndTime();
	
	return TRUE;  // return TRUE unless you set the focus to a control
}

void dateDialog::OnOK()
{
    if(m_isModal)  CDialog::OnOK();
	else		   DestroyWindow();
}

void dateDialog::onUpdate() 
{
	setDateAndTime();
	renew_check_indicators();
	UpdateData(false);

}

void dateDialog::OnCheckQci() 
{
   //Put check data back.
   UpdateData(false);
}

void dateDialog::OnRadioGripAirOff() 
{
	g_io.Grip_Air_Off();
}

void dateDialog::OnRadioGripAirOn() 
{
	g_io.Grip_Air_On();
}

void dateDialog::OnRadioGripPositionClose() 
{
	g_io.Grip_Close();
}

void dateDialog::OnRadioGripPositionOpen() 
{
	g_io.Grip_Open();
}

void dateDialog::OnRadioGripVacuumOff() 
{
    g_io.Grip_Vac_Off();	
}

void dateDialog::OnRadioGripVacuumOn() 
{
	g_io.Grip_Vac_On();
}

void dateDialog::OnRadioNeedlesDown() 
{
	g_io.Needls_Dwn();
}

void dateDialog::OnRadioNeedlesUp() 
{
	g_io.Needls_Up();
}

void dateDialog::OnRadioPlungeStageDown() 
{
	g_io.Plunge_Up();
}

void dateDialog::OnRadioPlungeStageUp() 
{
	g_io.Plunge_Dwn();
}

void dateDialog::OnRadioPlungeVacuumOff() 
{
    g_io.Plunge_Vac_Off();
}

void dateDialog::OnRadioPlungeVacuumOn() 
{
	g_io.Plunge_Vac_On();
}

void dateDialog::OnRadioWaferClose() 
{
	g_io.Waf_Close();
}

void dateDialog::OnRadioWaferOpen() 
{
	g_io.Waf_Open();
}

/////////////////////////////////////////////////////////////////////////////
// dateDialog implementation

void dateDialog::setDateAndTime()
{
	CTime t = CTime::GetCurrentTime();
	SetDlgItemText(IDC_Date,
		t.Format(_T("%A, %B %#d, %Y")));
	SetDlgItemText(IDC_Time,
		t.Format(_T("%I:%M:%S %p")));
}

void dateDialog::PostNcDestroy() 
{
    if(!m_isModal) delete this;
}

void dateDialog::clear_data()
{
    m_qci_motor_address = 0;
    m_smac_motor_index = 0;
}


void dateDialog::renew_check_indicators()
{
	//check indicators.
	m_check_qci_1 =  (BOOL)( (*(m_tp[1])) == (*(m_cp[1])) );
	m_check_qci_2 =  (BOOL)( (*(m_tp[2])) == (*(m_cp[2])) );
	m_check_qci_3 =  (BOOL)( (*(m_tp[3])) == (*(m_cp[3])) );
	m_check_qci_4 =  (BOOL)( (*(m_tp[4])) == (*(m_cp[4])) );
	m_check_qci_5 =  (BOOL)( (*(m_tp[5])) == (*(m_cp[5])) );
	m_check_qci_6 =  (BOOL)( (*(m_tp[6])) == (*(m_cp[6])) );
	m_check_qci_7 =  (BOOL)( (*(m_tp[7])) == (*(m_cp[7])) );
	m_check_smac_l = (BOOL)( (*(m_tp[8])) == (*(m_cp[8])) );
	m_check_smac_r = (BOOL)( (*(m_tp[9])) == (*(m_cp[9])) );
	m_check_grip_air_off = FALSE;
	m_check_grip_air_on = FALSE;
	m_check_grip_position_closed = g_io.Grip_is_Closed();
	m_check_grip_position_open =   g_io.Grip_is_Open();
	m_check_grip_vacuum_off = FALSE;
	m_check_grip_vacuum_on = FALSE;
	m_check_needles_down = FALSE;
	m_check_needles_up = FALSE;
	m_check_plunge_down =          g_io.Plunge_is_Dwn();
	m_check_plunge_up =            g_io.Plunge_is_Up();
	m_check_plunge_vacuum_off = FALSE;
	m_check_plunge_vacuum_on = FALSE;
	m_check_chip_not_present =    !g_io.Chip_Pres();
	m_check_chip_present =         g_io.Chip_Pres();
}

void dateDialog::OnButtonQciStop() 
{
   g_Cmnds.Stop(255,-1);  //Stop everything.
   g_Axes.ZeroTorque(255);	//Release everything.
}


BOOL dateDialog::PreCreateWindow(CREATESTRUCT& cs) 
{
    BOOL b = CDialog::PreCreateWindow(cs);
    g_io.Plunge_Dwn();
	return b;
}
