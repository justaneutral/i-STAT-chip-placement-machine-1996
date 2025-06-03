#if !defined(AFX_USERDLG_H__7982E465_D53B_11D1_8C4D_00C04FAD4034__INCLUDED_)
#define AFX_USERDLG_H__7982E465_D53B_11D1_8C4D_00C04FAD4034__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// userDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// userDlg dialog

class userDlg : public CDialog
{
// Construction
public:
	userDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(userDlg)
	enum { IDD = IDD_USER };
	CString	m_password;
	CString	m_name;
	BOOL	m_permissions;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(userDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(userDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_uSERDLG_H__7982E465_D53B_11D1_8C4D_00C04FAD4034__INCLUDED_)
