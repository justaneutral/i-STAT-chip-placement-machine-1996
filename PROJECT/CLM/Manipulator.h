#if !defined(AFX_MANIPULATOR_H__9F87C271_51A8_11D2_B36C_00C04FAD4084__INCLUDED_)
#define AFX_MANIPULATOR_H__9F87C271_51A8_11D2_B36C_00C04FAD4084__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Manipulator.h : header file
//

#include <afxcmn.h>

/////////////////////////////////////////////////////////////////////////////
// CManipulator dialog

class CManipulator : public CDialog
{
// Construction
public:
	CManipulator(CWnd* pParent = NULL);   // standard constructor
	
	int address;
	int activated[10];
	double position[10];

	void ChangeState(UINT nChar);

	int nRepCnt;

	DWORD m_timer;

// Dialog Data
	//{{AFX_DATA(CManipulator)
	enum { IDD = IDD_MANIPULATOR };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CManipulator)
	public:
	virtual int DoModal();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CManipulator)
	afx_msg LRESULT OnMenuChar(UINT nChar, UINT nFlags, CMenu* pMenu);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MANIPULATOR_H__9F87C271_51A8_11D2_B36C_00C04FAD4084__INCLUDED_)
