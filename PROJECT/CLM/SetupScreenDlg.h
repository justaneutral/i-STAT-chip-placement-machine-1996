#if !defined(AFX_SETUPSCREENDLG_H__5B9EE4E2_337C_11D2_9197_00C0F02195F5__INCLUDED_)
#define AFX_SETUPSCREENDLG_H__5B9EE4E2_337C_11D2_9197_00C0F02195F5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SetupScreenDlg.h : header file
//

#include "ButtonColor.h"

/////////////////////////////////////////////////////////////////////////////
// CSetupScreenDlg dialog

class CSetupScreenDlg : public CDialog
{
// Construction
public:
	CSetupScreenDlg(CWnd* pParent = NULL);   // standard constructor

    void ReplacButton(COLORREF color,DWORD idc,CButtonColor &wnd);

	//different buttons with different colors.
	CButtonColor	m_setup_screen_motor_setup_fetcher,
					m_setup_screen_fetcher_clamp,
					m_setup_screen_motor_setup_stretcher,
					m_setup_screen_motor_setup_overhead_gantry_x,
					m_setup_screen_motor_setup_overhead_gantry_y,
					m_setup_screen_motor_setup_overhead_gantry_smac,
					m_setup_screen_overhead_gantry_jaws,
					m_setup_screen_motor_setup_wafer_x,
					m_setup_screen_motor_setup_wafer_y,
					m_setup_screen_vision_wafer,
					m_setup_screen_vision_base,
					m_setup_screen_plunge_up_timing;



// Dialog Data
	//{{AFX_DATA(CSetupScreenDlg)
	enum { IDD = IDD_setup_screen };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetupScreenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetupScreenDlg)
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETUPSCREENDLG_H__5B9EE4E2_337C_11D2_9197_00C0F02195F5__INCLUDED_)
