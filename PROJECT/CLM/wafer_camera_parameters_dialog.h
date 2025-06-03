#if !defined(AFX_WAFER_CAMERA_PARAMETERS_DIALOG_H__2408D21A_0B70_11D2_8C96_00C04FAD4034__INCLUDED_)
#define AFX_WAFER_CAMERA_PARAMETERS_DIALOG_H__2408D21A_0B70_11D2_8C96_00C04FAD4034__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// wafer_camera_parameters_dialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// wafer_camera_parameters_dialog dialog

class wafer_camera_parameters_dialog : public CDialog
{
// Construction
public:
	wafer_camera_parameters_dialog(CWnd* pParent = NULL);   // standard constructor
	void Save();

// Dialog Data
	//{{AFX_DATA(wafer_camera_parameters_dialog)
	enum { IDD = IDD_wafer_camera_parameters };
	double	m_wafer_camera_angle;
	double	m_wafer_camera_back_x;
	double	m_wafer_camera_back_y;
	double	m_wafer_camera_forward_x;
	double	m_wafer_camera_forward_y;
	double	m_wafer_camera_x;
	double	m_wafer_camera_y;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(wafer_camera_parameters_dialog)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(wafer_camera_parameters_dialog)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(wafer_camera_parameters_dialog)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAFER_CAMERA_PARAMETERS_DIALOG_H__2408D21A_0B70_11D2_8C96_00C04FAD4034__INCLUDED_)
