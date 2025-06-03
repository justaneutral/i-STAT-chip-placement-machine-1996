#if !defined(AFX_GETUSERDLG_H__AB0D87A3_EE70_11D1_8C6D_00C04FAD4034__INCLUDED_)
#define AFX_GETUSERDLG_H__AB0D87A3_EE70_11D1_8C6D_00C04FAD4034__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// getuserDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// getuserDlg dialog

class getuserDlg : public CDialog
{
// Construction
public:
	getuserDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(getuserDlg)
	enum { IDD = IDD_get_user };
	CString	m_password;
	CString	m_name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(getuserDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(getuserDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETUSERDLG_H__AB0D87A3_EE70_11D1_8C6D_00C04FAD4034__INCLUDED_)
