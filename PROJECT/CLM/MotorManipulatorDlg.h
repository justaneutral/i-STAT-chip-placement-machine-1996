#if !defined(AFX_MOTORMANIPULATORDLG_H__28F1DD71_519A_11D2_B36B_00C04FAD4084__INCLUDED_)
#define AFX_MOTORMANIPULATORDLG_H__28F1DD71_519A_11D2_B36B_00C04FAD4084__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MotorManipulatorDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMotorManipulatorDlg dialog

class CMotorManipulatorDlg : public CDialog
{
// Construction
public:
	CMotorManipulatorDlg(CWnd* pParent = NULL);   // standard constructor

	int address;
	int activated[10];
	double position[10];


// Dialog Data
	//{{AFX_DATA(CMotorManipulatorDlg)
	enum { IDD = IDD_MANIPULATOR };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMotorManipulatorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMotorManipulatorDlg)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MOTORMANIPULATORDLG_H__28F1DD71_519A_11D2_B36B_00C04FAD4084__INCLUDED_)
