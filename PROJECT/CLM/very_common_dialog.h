#if !defined(AFX_VERY_COMMON_DIALOG_H__E7388B91_E151_11D1_B2F2_00C04FAD4084__INCLUDED_)
#define AFX_VERY_COMMON_DIALOG_H__E7388B91_E151_11D1_B2F2_00C04FAD4084__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// very_common_dialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// very_common_dialog dialog

#include "afxwin.h"
#include "resource.h"

class very_common_dialog : public CDialog
{
// Construction
public:
	very_common_dialog(CString *ParamName,CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(very_common_dialog)
	enum { IDD = idd_very_common_dialog };
	double	m_double;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(very_common_dialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(very_common_dialog)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VERY_COMMON_DIALOG_H__E7388B91_E151_11D1_B2F2_00C04FAD4084__INCLUDED_)
