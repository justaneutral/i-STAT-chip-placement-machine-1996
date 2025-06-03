// CameraView.cpp : implementation file
//

#include "stdafx.h"
#include "camera.h"
#include "CameraView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCameraView

IMPLEMENT_DYNCREATE(CCameraView, CView)

CCameraView::CCameraView()
{
	EnableAutomation();
}

CCameraView::~CCameraView()
{
}

void CCameraView::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CView::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CCameraView, CView)
	//{{AFX_MSG_MAP(CCameraView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CCameraView, CView)
	//{{AFX_DISPATCH_MAP(CCameraView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ICameraView to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {50511461-6D22-11D2-B380-00C04FAD4084}
static const IID IID_ICameraView =
{ 0x50511461, 0x6d22, 0x11d2, { 0xb3, 0x80, 0x0, 0xc0, 0x4f, 0xad, 0x40, 0x84 } };

BEGIN_INTERFACE_MAP(CCameraView, CView)
	INTERFACE_PART(CCameraView, IID_ICameraView, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCameraView drawing

void CCameraView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CCameraView diagnostics

#ifdef _DEBUG
void CCameraView::AssertValid() const
{
	CView::AssertValid();
}

void CCameraView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCameraView message handlers

BOOL CCameraView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CView::PreCreateWindow(cs);
}
