#if !defined(AFX_ABOUT_H__FDE33601_2E0C_11D2_9190_00C0F02195F5__INCLUDED_)
#define AFX_ABOUT_H__FDE33601_2E0C_11D2_9190_00C0F02195F5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// About.h : header file
//


#include "EditColor.h"
#include "ButtonColor.h"


/////////////////////////////////////////////////////////////////////////////
// CAbout dialog

class CAbout : public CDialog
{
// Construction
public:
	CAbout(CWnd* pParent = NULL);   // standard constructor
	CEditColor m_editcolor;
	CButtonColor m_buttoncolor;
	CWnd m_staticcolor;

// Dialog Data
	//{{AFX_DATA(CAbout)
	enum { IDD = IDD_About };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAbout)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAbout)
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONcolored();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUT_H__FDE33601_2E0C_11D2_9190_00C0F02195F5__INCLUDED_)
