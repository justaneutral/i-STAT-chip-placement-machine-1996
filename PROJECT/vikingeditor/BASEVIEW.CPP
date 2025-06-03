// baseview.cpp : implementation file
//

#include "stdafx.h"
#include "vikingeditor.h"
#include "baseview.h"

#include "basedoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseView

IMPLEMENT_DYNCREATE(CBaseView, CEditView)

CBaseView::CBaseView()
{
}

CBaseView::~CBaseView()
{
}


BEGIN_MESSAGE_MAP(CBaseView, CEditView)
	//{{AFX_MSG_MAP(CBaseView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaseView drawing

void CBaseView::OnDraw(CDC* pDC)
{
	CBaseDoc* pDoc = (CBaseDoc*) GetDocument();
	
	for(int i=0;i<30;i++) pDC->TextOut(20*i,20,(pDoc->Data)[i]);
}

/////////////////////////////////////////////////////////////////////////////
// CBaseView diagnostics

#ifdef _DEBUG
void CBaseView::AssertValid() const
{
	CEditView::AssertValid();
}

void CBaseView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBaseView message handlers
