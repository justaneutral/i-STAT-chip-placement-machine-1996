#include "stdafx.h"

#if !defined(AFX_EDITSTRING_H__7982E466_D53B_11D1_8C4D_00C04FAD4034__INCLUDED_)
#define AFX_EDITSTRING_H__7982E466_D53B_11D1_8C4D_00C04FAD4034__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// EditString.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditString window

class CEditString : public CEdit
{
// Construction
public:
	CEditString();

// Attributes
public:
	bool ready_f;
	char string[80];
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditString)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CEditString();

	// Generated message map functions
protected:
	//{{AFX_MSG(CEditString)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITSTRING_H__7982E466_D53B_11D1_8C4D_00C04FAD4034__INCLUDED_)
