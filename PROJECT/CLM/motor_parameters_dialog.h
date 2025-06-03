#if !defined(AFX_MOTOR_PARAMETERS_DIALOG_H__186F5914_0AAE_11D2_8C94_00C04FAD4034__INCLUDED_)
#define AFX_MOTOR_PARAMETERS_DIALOG_H__186F5914_0AAE_11D2_8C94_00C04FAD4034__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// motor_parameters_dialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// motor_parameters_dialog dialog

class motor_parameters_dialog : public CDialog
{
// Construction
public:
	motor_parameters_dialog(int index, CWnd* pParent = NULL);   // standard constructor
	
	void Save();
// Dialog Data
	//{{AFX_DATA(motor_parameters_dialog)
	enum { IDD = IDD_motor_parameters };
	double	m_acceleration;
	double	m_homming_acceleration;
	double	m_hommind_correction_end;
	double	m_homming_correction_start;
	double	m_homming_distance;
	double	m_homming_tolerance;
	double	m_homming_torque;
	double	m_homming_velocity;
	double	m_torque;
	double	m_velocity;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(motor_parameters_dialog)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int m_index;
	// Generated message map functions
	//{{AFX_MSG(motor_parameters_dialog)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(motor_parameters_dialog)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOTOR_PARAMETERS_DIALOG_H__186F5914_0AAE_11D2_8C94_00C04FAD4034__INCLUDED_)
