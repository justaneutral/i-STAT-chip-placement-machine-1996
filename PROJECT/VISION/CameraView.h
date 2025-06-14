#if !defined(AFX_CAMERAVIEW_H__50511462_6D22_11D2_B380_00C04FAD4084__INCLUDED_)
#define AFX_CAMERAVIEW_H__50511462_6D22_11D2_B380_00C04FAD4084__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CameraView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCameraView view

class CCameraView : public CView
{
protected:
	CCameraView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CCameraView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCameraView)
	public:
	virtual void OnFinalRelease();
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CCameraView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CCameraView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CCameraView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CAMERAVIEW_H__50511462_6D22_11D2_B380_00C04FAD4084__INCLUDED_)
