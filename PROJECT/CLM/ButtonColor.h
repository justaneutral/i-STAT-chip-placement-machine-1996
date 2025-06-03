#if !defined(AFX_BUTTONCOLOR_H__FDE33604_2E0C_11D2_9190_00C0F02195F5__INCLUDED_)
#define AFX_BUTTONCOLOR_H__FDE33604_2E0C_11D2_9190_00C0F02195F5__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// ButtonColor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CButtonColor window

class CButtonColor : public CButton
{
// Construction
public:
	CButtonColor();

// Attributes
public:
    COLORREF m_color_selected;
    COLORREF m_color_focus;
	COLORREF m_color;
	CString  m_text;
	UINT     m_timer;

	COLORREF m_color1;
	CString  m_text1;
    COLORREF m_color2;
	CString  m_text2;
    int      m_period;
	BOOL    *m_source;

// Operations
public:
   COLORREF SetColorSelected(COLORREF clr);
   COLORREF SetColorFocus(COLORREF clr);
   COLORREF SetColor(COLORREF clr);

   BOOL SetTrigger(COLORREF color1, CString text1, 
	               COLORREF color2, CString text2, 
				   int period, BOOL *source);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonColor)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual BOOL Create( LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID );
	virtual BOOL DestroyWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CButtonColor();

	// Generated message map functions
protected:
	//{{AFX_MSG(CButtonColor)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUTTONCOLOR_H__FDE33604_2E0C_11D2_9190_00C0F02195F5__INCLUDED_)
