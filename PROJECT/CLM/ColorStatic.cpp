// ColorStatic.cpp : implementation file
//

#include "stdafx.h"
#include "app.h"
#include "ColorStatic.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorStatic

CColorStatic::CColorStatic()
{
	m_background_color = RGB(255,255,255);
	COLORREF m_text_color = RGB(0,0,0);
	CString m_text = _T("");
}

CColorStatic::~CColorStatic()
{
}


BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
	//{{AFX_MSG_MAP(CColorStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorStatic message handlers

BOOL CColorStatic::OnEraseBkgnd(CDC* pDC) 
{
	if(!CStatic::OnEraseBkgnd(pDC)) return FALSE;
	
	CRect rect;
	CBrush brush;
	brush.CreateSolidBrush(m_background_color);
	GetWindowRect(&rect);
	ScreenToClient(&rect);
	pDC->SelectObject(brush);
	pDC->Rectangle(rect);
	pDC->SetBkColor(m_background_color);
	pDC->SetTextColor(m_text_color);
	
	CSize textLen = pDC->GetTextExtent(m_text,m_text.GetLength());
	pDC->TextOut((rect.right-textLen.cx)/2,(rect.bottom-textLen.cy)/2,m_text);

	return TRUE;
}


void CColorStatic::OnPaint() 
{
}

int CColorStatic::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CStatic::OnCreate(lpCreateStruct) == -1)
		return -1;
	
    GetWindowText(m_text);	

	return 0;
}

