// date.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// dateDialog dialog

class dateDialog : public CDialog
{
// Construction
public:
	dateDialog(CWnd* pParent=NULL,BOOL modal=TRUE);   // standard constructor

// Dialog Data
	//{{AFX_DATA(dateDialog)
	enum { IDD = IDD_Diagnostic };
    double  m_tp_1;
	double  m_tp_2;
	double	m_tp_3;
	double	m_tp_4;
	double	m_tp_5;
	double	m_tp_6;
	double	m_tp_7;
 	double	m_tp_r;
	double	m_tp_l;
	double	m_cp_1;
	double	m_cp_2;
	double	m_cp_3;
	double	m_cp_4;
	double	m_cp_5;
	double	m_cp_6;
	double	m_cp_7;
	double	m_cp_r;
	double	m_cp_l;
	double	m_ve_1;
	double	m_ve_2;
	double	m_ve_3;
	double	m_ve_4;
	double	m_ve_5;
	double	m_ve_6;
	double	m_ve_7;
	double	m_ve_r;
	double	m_ve_l;
	double	m_tq_1;
    double	m_tq_2;
	double	m_tq_3;
	double	m_tq_4;
	double	m_tq_5;
	double	m_tq_6;
	double	m_tq_7;
    double	m_tq_r;
	double	m_tq_l;
	double	m_ac_1;
	double	m_ac_2;
	double	m_ac_3;
	double	m_ac_4;
	double	m_ac_5;
	double	m_ac_6;
	double	m_ac_7;
	double	m_ac_r;
	double	m_ac_l;
	BOOL	m_check_qci_1;
	BOOL	m_check_qci_2;
	BOOL	m_check_qci_3;
	BOOL	m_check_qci_4;
	BOOL	m_check_qci_5;
	BOOL	m_check_qci_6;
	BOOL	m_check_qci_7;
	BOOL	m_check_smac_l;
	BOOL	m_check_smac_r;
	BOOL	m_check_grip_air_off;
	BOOL	m_check_grip_air_on;
	BOOL	m_check_grip_position_closed;
	BOOL	m_check_grip_position_open;
	BOOL	m_check_grip_vacuum_off;
	BOOL	m_check_grip_vacuum_on;
	BOOL	m_check_needles_down;
	BOOL	m_check_needles_up;
	BOOL	m_check_plunge_down;
	BOOL	m_check_plunge_up;
	BOOL	m_check_plunge_vacuum_off;
	BOOL	m_check_plunge_vacuum_on;
	BOOL	m_check_chip_not_present;
	BOOL	m_check_chip_present;
	BOOL	m_check_wafer_closed;
	BOOL	m_check_wafer_open;
	//}}AFX_DATA


// Overrides
	//{{AFX_VIRTUAL(dateDialog)
	protected:
        virtual void DoDataExchange(CDataExchange* pDX);
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
   	void clear_data();
	void renew_check_indicators();
	int m_qci_motor_address;
	int m_smac_motor_index;
	double * m_tp[10];
	double * m_cp[10];
	double * m_ve[10];
	double * m_tq[10];
	double * m_ac[10];

protected:
	BOOL m_isModal;	// True when created as a modal dialog
	void setDateAndTime();

	// Generated message map functions
	//{{AFX_MSG(dateDialog)
	afx_msg void OnButtonQciHome();
	afx_msg void OnButtonQciRefresh();
	afx_msg void OnRadioQci2();
	afx_msg void OnRadioQci3();
	afx_msg void OnRadioQci4();
	afx_msg void OnRadioQci5();
	afx_msg void OnRadioQci6();
	afx_msg void OnRadioQci7();
	afx_msg void OnRadioSmacR();
	afx_msg void OnRadioSmacL();
	afx_msg void OnRadioQci1();
	afx_msg void OnRadioSmacLr();
	afx_msg void onUpdate();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCheckQci();
	afx_msg void OnRadioGripAirOff();
	afx_msg void OnRadioGripAirOn();
	afx_msg void OnRadioGripPositionClose();
	afx_msg void OnRadioGripPositionOpen();
	afx_msg void OnRadioGripVacuumOff();
	afx_msg void OnRadioGripVacuumOn();
	afx_msg void OnRadioNeedlesDown();
	afx_msg void OnRadioNeedlesUp();
	afx_msg void OnRadioPlungeStageDown();
	afx_msg void OnRadioPlungeStageUp();
	afx_msg void OnRadioPlungeVacuumOff();
	afx_msg void OnRadioPlungeVacuumOn();
	afx_msg void OnRadioWaferClose();
	afx_msg void OnRadioWaferOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
