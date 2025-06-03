#if !defined(AFX_MACHINESTATE_H__02029AE2_1030_11D2_8C9A_00C04FAD4034__INCLUDED_)
#define AFX_MACHINESTATE_H__02029AE2_1030_11D2_8C9A_00C04FAD4034__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// machineState.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// machineState dialog

#include "resource.h"

class machineState : public CDialog
{
// Construction
public:
	machineState(CWnd* pParent = NULL);   // standard constructor
	~machineState();
	bool update();
	bool recreate();
	bool Save();
	BOOL m_error;
	CString status;
	CString product;
	CString vision;


	char str_buffer[80];
	FILE    *settingsfile;

	double m_camX;
	double m_camY;

	bool is_started;

// Dialog Data
	//{{AFX_DATA(machineState)
	enum { IDD = IDD_DIALOG_machine_state };
	BOOL	m_chip_is_picked_up;
	BOOL	m_magazine_is_ready;
	BOOL	m_wafer_is_loaded;
	UINT	m_chip_position;
	CString	m_file_name;
	UINT	m_wafer_position;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(machineState)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(machineState)
	afx_msg void OnBUTTONmachinestatefilename();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(machineState)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MACHINESTATE_H__02029AE2_1030_11D2_8C9A_00C04FAD4034__INCLUDED_)
