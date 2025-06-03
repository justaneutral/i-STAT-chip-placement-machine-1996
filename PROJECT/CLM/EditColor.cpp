// EditColor.cpp : implementation file
//

#include "globals.h"
#include "EditColor.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditColor

CEditColor::CEditColor()
{
    m_clrText = RGB( 0, 0, 0 );
    m_clrBkgnd = RGB( 255, 255, 0 );
    m_brBkgnd.CreateSolidBrush( m_clrBkgnd );

/*	RECT Rect;
	g_Wnd->GetClientRect(&Rect);
    Rect.left += 20;
    Rect.top  += 30;
	Rect.right /= 2; 
    Rect.bottom /= 2; 
 	
	if(!Create(WS_CHILD|WS_VISIBLE|WS_BORDER|WS_VSCROLL|WS_HSCROLL|
		        ES_MULTILINE|ES_READONLY|ES_AUTOHSCROLL|ES_AUTOVSCROLL,
		Rect, g_Wnd, 70000))
	{
	    AfxAbort();
		return;
	}
				 
    g_text_output = this;
*/
}



CEditColor::~CEditColor()
{
    DestroyWindow();

}


BEGIN_MESSAGE_MAP(CEditColor, CEdit)
	//{{AFX_MSG_MAP(CEditColor)
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditColor message handlers

HBRUSH CEditColor::CtlColor(CDC* pDC, UINT nCtlColor) 
{
    pDC->SetTextColor( m_clrText );    // text
    pDC->SetBkColor( m_clrBkgnd );    // text bkgnd
    return m_brBkgnd;                // ctl bkgnd}
}
