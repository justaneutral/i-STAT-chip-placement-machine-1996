#if !defined(AFX_BAYSTATE_H__8DFBD024_14FC_11D2_94A5_000000000000__INCLUDED_)
#define AFX_BAYSTATE_H__8DFBD024_14FC_11D2_94A5_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// bayState.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// bayState dialog

class bayState : public CDialog
{
// Construction
public:
	bayState(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(bayState)
	enum { IDD = IDD_DIALOG_bay_number };
	int		m_bay;
	int		m_active;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(bayState)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(bayState)
	virtual void OnOK();
	afx_msg void OnBUTTONmachineconfig();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(bayState)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BAYSTATE_H__8DFBD024_14FC_11D2_94A5_000000000000__INCLUDED_)
