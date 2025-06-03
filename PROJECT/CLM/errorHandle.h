#if !defined(AFX_ERRORHANDLE_H__3DC1BD22_1453_11D2_94A3_000000000000__INCLUDED_)
#define AFX_ERRORHANDLE_H__3DC1BD22_1453_11D2_94A3_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// errorHandle.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// errorHandle dialog

class errorHandle : public CDialog
{
// Construction
public:
	errorHandle(CString question, CWnd* pParent = NULL);   // standard constructor
	int result;
	int TOCounter;

// Dialog Data
	//{{AFX_DATA(errorHandle)
	enum { IDD = IDD_DIALOG_error_handle };
	CString	m_error_string;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(errorHandle)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void PostNcDestroy();

	// Generated message map functions
	//{{AFX_MSG(errorHandle)
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(errorHandle)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ERRORHANDLE_H__3DC1BD22_1453_11D2_94A3_000000000000__INCLUDED_)
