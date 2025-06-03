#if !defined(AFX_EDITCOLOR_H__E96136F1_2BAF_11D2_94BE_000000000000__INCLUDED_)
#define AFX_EDITCOLOR_H__E96136F1_2BAF_11D2_94BE_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// EditColor.h : header file
//
#include <afxwin.h>

/////////////////////////////////////////////////////////////////////////////
// CEditColor window

class CEditColor : public CEdit
{
// Construction
public:
	CEditColor();

// Attributes
public:
    COLORREF m_clrText;
    COLORREF m_clrBkgnd;
    CBrush m_brBkgnd;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditColor)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditColor();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditColor)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITCOLOR_H__E96136F1_2BAF_11D2_94BE_000000000000__INCLUDED_)
