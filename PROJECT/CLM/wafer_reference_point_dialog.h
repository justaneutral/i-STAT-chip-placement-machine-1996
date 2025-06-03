#if !defined(AFX_WAFER_REFERENCE_POINT_DIALOG_H__2408D218_0B70_11D2_8C96_00C04FAD4034__INCLUDED_)
#define AFX_WAFER_REFERENCE_POINT_DIALOG_H__2408D218_0B70_11D2_8C96_00C04FAD4034__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// wafer_reference_point_dialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// wafer_reference_point_dialog dialog

class wafer_reference_point_dialog : public CDialog
{
// Construction
public:
	wafer_reference_point_dialog(CWnd* pParent = NULL);   // standard constructor
	void Save();

// Dialog Data
	//{{AFX_DATA(wafer_reference_point_dialog)
	enum { IDD = IDD_wafer_reference_point };
	double	m_wafer_delta_x;
	double	m_wafer_delta_y;
	double	m_wafer_x;
	double	m_wafer_y;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(wafer_reference_point_dialog)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(wafer_reference_point_dialog)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(wafer_reference_point_dialog)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAFER_REFERENCE_POINT_DIALOG_H__2408D218_0B70_11D2_8C96_00C04FAD4034__INCLUDED_)
