// ButtonColor.cpp : implementation file
//

#include "stdafx.h"
#include "app.h"
#include "ButtonColor.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButtonColor

CButtonColor::CButtonColor()
{
   m_timer = 0;

   SetColorSelected(RGB(192,192,192));
   SetColorFocus(RGB(191,191,191));
   SetColor(RGB(190,190,190));
}

CButtonColor::~CButtonColor()
{	
}	


BEGIN_MESSAGE_MAP(CButtonColor, CButton)
	//{{AFX_MSG_MAP(CButtonColor)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButtonColor message handlers


void CButtonColor::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{

	register int state =  lpDrawItemStruct->itemState;
	
	CBrush brush;

	if(m_timer)
	{
	  if(state & ODS_SELECTED) brush.CreateSolidBrush(m_color_selected);
	  else if(*m_source)       brush.CreateSolidBrush(m_color1);
	  else					   brush.CreateSolidBrush(m_color2);
	}
	else
	{
	  if(state & ODS_SELECTED)   brush.CreateSolidBrush(m_color_selected);
	  else if(state & ODS_FOCUS) brush.CreateSolidBrush(m_color_focus);
	  else					   brush.CreateSolidBrush(m_color);
	}


	CRect rect;
	GetClientRect(&rect);
	CDC *dc = CDC::FromHandle(lpDrawItemStruct->hDC);
	dc->SelectObject(brush);
	dc->Rectangle(&rect);
	dc->SetBkMode(TRANSPARENT);

	CString text;
	if(m_timer)
	{
	   if(*m_source) text = m_text1;
	   else          text = m_text2;
	}
	else text = m_text;
	
	CSize textLen = dc->GetTextExtent(text,text.GetLength());
	dc->TextOut((rect.right-textLen.cx)/2,(rect.bottom-textLen.cy)/2,text);
}




void CButtonColor::OnTimer(UINT nIDEvent) 
{											 
	Invalidate();
	CButton::OnTimer(nIDEvent);
}

BOOL CButtonColor::Create( LPCTSTR lpszCaption, DWORD dwStyle, const RECT& rect,
						   CWnd* pParentWnd, UINT nID ) 
{
	m_text = lpszCaption;
	
	if(CButton::Create( lpszCaption, dwStyle, rect, pParentWnd, nID ))
	{
      SetTrigger(RGB(0,255,0),_T("TRUE"),RGB(255,0,0),_T("FALSE"),0,NULL);
	  return TRUE;
	}
	return FALSE;
}

BOOL CButtonColor::DestroyWindow() 
{
    if(m_timer) KillTimer(m_timer);	
	return CButton::DestroyWindow();
}


COLORREF CButtonColor::SetColorSelected(COLORREF clr)
{
  COLORREF c = m_color_selected;
  m_color_selected = clr;
  return c;
}

COLORREF CButtonColor::SetColorFocus(COLORREF clr)
{
  COLORREF c = m_color_focus;
  m_color_focus = clr;
  return c;
}

COLORREF CButtonColor::SetColor(COLORREF clr)
{
  COLORREF c = m_color;
  m_color = clr;
  return c;
}


BOOL CButtonColor::SetTrigger(COLORREF color1, CString text1, 
	                          COLORREF color2, CString text2, 
				              int period, BOOL *source)
{
  	if(m_timer != NULL) KillTimer(m_timer);
	m_timer = NULL;

	if(period == 0) return TRUE;
	if(source == NULL) return FALSE;
	
	m_color1 = color1;
	m_text1 = text1;

	m_color2 = color2;
	m_text2 = text2;
	
	m_source = source;
	
	m_timer = SetTimer(0,period,NULL);
	if(m_timer == NULL) return FALSE;
	return TRUE;
}
