#if !defined(AFX_STATUSSCREENDLG_H__FEF39C91_31EA_11D2_9193_00C0F02195F5__INCLUDED_)
#define AFX_STATUSSCREENDLG_H__FEF39C91_31EA_11D2_9193_00C0F02195F5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// StatusScreenDlg.h : header file
//


#include "ButtonColor.h"
#include "EditColor.h"
#include "ColorStatic.h"



/////////////////////////////////////////////////////////////////////////////
// CStatusScreenDlg dialog

class CStatusScreenDlg : public CDialog
{
// Construction
public:
	CStatusScreenDlg(CWnd* pParent/* = NULL*/, int *indicator);   // standard constructor

	int *m_indicator;

	//information fields.
	CButtonColor m_bay_running[4],
				 m_bay_stopped[4],
				 m_magazine[4];
	
	//extended scrolable field for error logging.
	CEditColor	 m_edit;

	
	
	//dialog's static with white background.
	CButtonColor m_STATIC_status_colored[13];


	//controlled background.
	UINT m_timer;
	BOOL m_background_color_state;
	BOOL m_magazine_is_active[4];


// Dialog Data
	//{{AFX_DATA(CStatusScreenDlg)
	enum { IDD = IDD_status_screen };
	CString	m_operator;
	CString	m_product;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStatusScreenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CStatusScreenDlg)
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATUSSCREENDLG_H__FEF39C91_31EA_11D2_9193_00C0F02195F5__INCLUDED_)
