#if !defined(AFX_ERRORLOG_H__8DFBD022_14FC_11D2_94A5_000000000000__INCLUDED_)
#define AFX_ERRORLOG_H__8DFBD022_14FC_11D2_94A5_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// errorLog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// errorLog dialog

class errorLog : public CDialog
{
// Construction
public:
	errorLog(CWnd* pParent = NULL);   // standard constructor
	CString szLastError;
	void LogError();
	CString GetErrorLog();

// Dialog Data
	//{{AFX_DATA(errorLog)
	enum { IDD = IDD_DIALOG_error_log };
	CString	m_log_string;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(errorLog)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(errorLog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(errorLog)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ERRORLOG_H__8DFBD022_14FC_11D2_94A5_000000000000__INCLUDED_)
