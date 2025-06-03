#if !defined(AFX_PRODUCTTYPEDLG_H__525B5611_307A_11D2_9191_00C0F02195F5__INCLUDED_)
#define AFX_PRODUCTTYPEDLG_H__525B5611_307A_11D2_9191_00C0F02195F5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ProductTypeDlg.h : header file
//


#include "ButtonColor.h"
#include "EditColor.h"



/////////////////////////////////////////////////////////////////////////////
// CProductTypeDlg dialog

class CProductTypeDlg : public CDialog
{
// Construction
public:
	CProductTypeDlg(CWnd* pParent, int *indicator);   // standard constructor

	int *m_indicator;

	CButtonColor m_BUTTON_enter_product_type;
	CEditColor m_EDIT_product_type;

	CButtonColor m_BUTTON_bay_1_status;
	CButtonColor m_BUTTON_bay_2_status;
	CButtonColor m_BUTTON_bay_3_status;
	CButtonColor m_BUTTON_bay_4_status;

	//labels.
	CButtonColor m_idcs[4];
	
	CButtonColor m_IDCANCEL;
	CButtonColor m_IDOK;

	UINT m_timer;

// Dialog Data
	//{{AFX_DATA(CProductTypeDlg)
	enum { IDD = IDD_product_type };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProductTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProductTypeDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PRODUCTTYPEDLG_H__525B5611_307A_11D2_9191_00C0F02195F5__INCLUDED_)
