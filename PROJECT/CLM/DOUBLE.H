#if !defined(AFX_DOUBLE_H__7982E463_D53B_11D1_8C4D_00C04FAD4034__INCLUDED_)
#define AFX_DOUBLE_H__7982E463_D53B_11D1_8C4D_00C04FAD4034__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DOUBLE.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// DOUBLE window

class DOUBLE : public CEdit
{
	DECLARE_DYNCREATE(DOUBLE)

// Construction
public:
	DOUBLE();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(DOUBLE)
	public:
	virtual void OnFinalRelease();
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~DOUBLE();

	// Generated message map functions
protected:
	//{{AFX_MSG(DOUBLE)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(DOUBLE)

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(DOUBLE)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOUBLE_H__7982E463_D53B_11D1_8C4D_00C04FAD4034__INCLUDED_)
