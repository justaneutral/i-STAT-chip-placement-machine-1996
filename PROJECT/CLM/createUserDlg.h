#if !defined(AFX_CREATEUSERDLG_H__7982E465_D53B_11D1_8C4D_00C04FAD4034__INCLUDED_)
#define AFX_CREATEUSERDLG_H__7982E465_D53B_11D1_8C4D_00C04FAD4034__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// createUserDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// createUserDlg dialog

class createUserDlg : public CDialog
{
// Construction
public:
	createUserDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(createUserDlg)
	enum { IDD = IDD_CREATE_USER };
	CString	m_password;
	CString	m_password_confirm;
	CString	m_name;
	BOOL	m_permissions;
	CString	m_id;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(createUserDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(createUserDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREATEUSERDLG_H__7982E465_D53B_11D1_8C4D_00C04FAD4034__INCLUDED_)
