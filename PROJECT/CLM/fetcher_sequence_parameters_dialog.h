#if !defined(AFX_FETCHER_SEQUENCE_PARAMETERS_DIALOG_H__186F5912_0AAE_11D2_8C94_00C04FAD4034__INCLUDED_)
#define AFX_FETCHER_SEQUENCE_PARAMETERS_DIALOG_H__186F5912_0AAE_11D2_8C94_00C04FAD4034__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// fetcher_sequence_parameters_dialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// fetcher_sequence_parameters_dialog dialog

#include "resource.h"

class fetcher_sequence_parameters_dialog : public CDialog
{
// Construction
public:
	fetcher_sequence_parameters_dialog(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(fetcher_sequence_parameters_dialog)
	enum { IDD = IDD_fetcher_sequence_parameters };
	double	m_fetcher_clear;
	double	m_fetcher_justify;
	double	m_fetcher_magazine;
	double	m_fetcher_stretcher;
	double	m_magazine_capacity;
	double	m_magazine_increment;
	double	m_magazine_origin;
	double	m_stage_wafer_x;
	double	m_stage_wafer_y;
	double	m_wafer_x_justify;
	double	m_wafer_x_pick;
	double	m_wafer_y_justify;
	double	m_wafer_y_pick;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(fetcher_sequence_parameters_dialog)
	public:
	virtual void OnFinalRelease();
	void Save();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(fetcher_sequence_parameters_dialog)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(fetcher_sequence_parameters_dialog)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FETCHER_SEQUENCE_PARAMETERS_DIALOG_H__186F5912_0AAE_11D2_8C94_00C04FAD4034__INCLUDED_)
