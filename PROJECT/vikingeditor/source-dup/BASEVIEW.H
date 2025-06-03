#if !defined(AFX_BASEVIEW_H__FE5B0ED2_D9EF_11D1_8C51_00C04FAD4034__INCLUDED_)
#define AFX_BASEVIEW_H__FE5B0ED2_D9EF_11D1_8C51_00C04FAD4034__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// baseview.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBaseView view

class CBaseView : public CEditView
{
protected:
	CBaseView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CBaseView)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CBaseView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CBaseView)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASEVIEW_H__FE5B0ED2_D9EF_11D1_8C51_00C04FAD4034__INCLUDED_)
