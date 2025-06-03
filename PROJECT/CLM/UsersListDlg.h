#if !defined(AFX_USERSLISTDLG_H__F4175321_33A5_11D2_9197_00C0F02195F5__INCLUDED_)
#define AFX_USERSLISTDLG_H__F4175321_33A5_11D2_9197_00C0F02195F5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// UsersListDlg.h : header file
//

#include <afxcmn.h>
#include <afxtempl.h>



typedef struct _USER_DATA
{
   CString FullName;
   CString UserID;
   CString Password;
   BOOL Restrictions;


} USER_DATA, *pUSER_DATA;



/////////////////////////////////////////////////////////////////////////////
// CUsersListDlg dialog

class CUsersListDlg : public CDialog
{
// Construction
public:
	CUsersListDlg(CWnd* pParent = NULL);   // standard constructor

	//CArray<CString,CString> m_Users;

    CArray<CString,CString>  FullName;
	CArray<CString,CString>	 UserID;
	CArray<CString,CString>	 Password;
	CArray<BOOL,BOOL>	     Restrictions;

	
	void SaveUsers();
	void ReadUsers();
	
	
	
// Dialog Data
	//{{AFX_DATA(CUsersListDlg)
	enum { IDD = IDD_user_list };
	CListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUsersListDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUsersListDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONdeleteuser();
	afx_msg void OnBUTTONedituser();
	afx_msg void OnBUTTONadduser();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_USERSLISTDLG_H__F4175321_33A5_11D2_9197_00C0F02195F5__INCLUDED_)
