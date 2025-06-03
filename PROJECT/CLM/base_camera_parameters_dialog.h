#if !defined(AFX_BASE_CAMERA_PARAMETERS_DIALOG_H__2408D212_0B70_11D2_8C96_00C04FAD4034__INCLUDED_)
#define AFX_BASE_CAMERA_PARAMETERS_DIALOG_H__2408D212_0B70_11D2_8C96_00C04FAD4034__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// base_camera_parameters_dialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// base_camera_parameters_dialog dialog

class base_camera_parameters_dialog : public CDialog
{
// Construction
public:
	base_camera_parameters_dialog(CWnd* pParent = NULL);   // standard constructor
	void Save();

// Dialog Data
	//{{AFX_DATA(base_camera_parameters_dialog)
	enum { IDD = IDD_base_cmera_parameters };
	double	m_base_camera_angle;
	double	m_base_camera_x;
	double	m_base_camera_y;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(base_camera_parameters_dialog)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(base_camera_parameters_dialog)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(base_camera_parameters_dialog)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASE_CAMERA_PARAMETERS_DIALOG_H__2408D212_0B70_11D2_8C96_00C04FAD4034__INCLUDED_)
