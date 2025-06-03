#if !defined(AFX_PICK_PLACE_POINT_DIALOG_H__2408D216_0B70_11D2_8C96_00C04FAD4034__INCLUDED_)
#define AFX_PICK_PLACE_POINT_DIALOG_H__2408D216_0B70_11D2_8C96_00C04FAD4034__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// pick_place_point_dialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// pick_place_point_dialog dialog

class pick_place_point_dialog : public CDialog
{
// Construction
public:
	pick_place_point_dialog(double * px,
							double * py,
							double * pz,
							double * pangle,
							CWnd* pParent = NULL);
   // standard constructor
	void Save();

	double *x;
	double *y;
	double *z;
	double *angle;

// Dialog Data
	//{{AFX_DATA(pick_place_point_dialog)
	enum { IDD = IDD_pick_place_point };
	double	m_angle;
	double	m_x;
	double	m_y;
	double	m_z;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(pick_place_point_dialog)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(pick_place_point_dialog)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(pick_place_point_dialog)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICK_PLACE_POINT_DIALOG_H__2408D216_0B70_11D2_8C96_00C04FAD4034__INCLUDED_)
