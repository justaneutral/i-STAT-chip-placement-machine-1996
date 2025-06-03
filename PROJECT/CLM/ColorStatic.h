#if !defined(AFX_COLORSTATIC_H__FDE33603_2E0C_11D2_9190_00C0F02195F5__INCLUDED_)
#define AFX_COLORSTATIC_H__FDE33603_2E0C_11D2_9190_00C0F02195F5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ColorStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CColorStatic window

class CColorStatic : public CStatic
{
// Construction
public:
	CColorStatic();
	COLORREF m_background_color;
	COLORREF m_text_color;
	CString m_text;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorStatic();

	// Generated message map functions
protected:
	//{{AFX_MSG(CColorStatic)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORSTATIC_H__FDE33603_2E0C_11D2_9190_00C0F02195F5__INCLUDED_)
