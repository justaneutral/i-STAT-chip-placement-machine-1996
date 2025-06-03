// VikingEditorView.cpp : implementation of the CVikingEditorView class
//

#include "stdafx.h"

#include "VikingEditorDoc.h"
#include "VikingEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVikingEditorView

IMPLEMENT_DYNCREATE(CVikingEditorView, CView)

BEGIN_MESSAGE_MAP(CVikingEditorView, CView)
	ON_MESSAGE(WM_MAPMOVE, OnMapMoveNotify)
	//{{AFX_MSG_MAP(CVikingEditorView)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVikingEditorView construction/destruction

CVikingEditorView::CVikingEditorView()
{
	// TODO: add construction code here
	int i;
	m_brushGray =new CBrush( RGB(100,100,100) );
	m_brushWhite=new CBrush( RGB(255,255,255) );
	m_brushGreen= new CBrush( RGB(0,255,0) );
	m_brushYellow=new CBrush( RGB(255,255,0) );
	m_brushRed= new CBrush( RGB(255,0,0) );

	m_brushes[0]=m_brushWhite;
	for(i=1; i<255; i++){
		m_brushes[i]=m_brushRed;
	}
	m_brushes[255]=m_brushGreen;
}

CVikingEditorView::~CVikingEditorView()
{
	delete m_brushRed;
	delete m_brushGray;
	delete m_brushWhite;
	delete m_brushYellow;
	delete m_brushGreen;

}

BOOL CVikingEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CVikingEditorView drawing

void CVikingEditorView::OnDraw(CDC* pDCon)
{
	// TODO: add draw code for native data here
	int GridHeight, GridWidth, BoxHeight, BoxWidth;
	int x, y;
	int Bincode;
	int RowCount,ColumnCount;
	BOOL res;
	RECT lpRect;
	CBitmap *pBitMap=new CBitmap;
	CBitmap *pOldBitMap;

	
	CVikingEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	GetClientRect(&lpRect);
	
    CDC* pDC= new CDC;
	ASSERT_VALID(pDC);

	res=pDC->CreateCompatibleDC(pDCon);
	ASSERT(res);
	
	pBitMap->CreateCompatibleBitmap( pDCon, lpRect.right, lpRect.bottom);
	
	pOldBitMap=(CBitmap *)pDC->SelectObject( pBitMap );
	
	CBrush* pOldBrush=pDC->GetCurrentBrush();
	
	pDC->Rectangle(&lpRect);
		
	pDoc->GetRCCount(RowCount,ColumnCount);

	GridHeight= (lpRect.bottom) / RowCount;
	GridWidth=lpRect.right/ ColumnCount;
	BoxHeight=GridHeight-1;
	BoxWidth=GridWidth-1;

	
	for(x=0; x< ColumnCount; x++){
		for(y=0; y< RowCount; y++){
			Bincode=pDoc->GetBincode(x+1,y+1);
			if(Bincode!=-1){
				pDC->SelectObject( GetBincodeBrush( Bincode));
				pDC->Rectangle( x*GridWidth, y*GridHeight, (x*GridWidth)+BoxWidth, (y*GridHeight)+BoxHeight );
			}
		}
	}

	//Set Present die location in yellow
	pDC->SelectObject( m_brushYellow );
	pDC->Rectangle( pDoc->PresentColumn*GridWidth, pDoc->PresentRow*GridHeight, (pDoc->PresentColumn*GridWidth)+BoxWidth, (pDoc->PresentRow*GridHeight)+BoxHeight );

	//BitBlt the memory DC
	pDCon->BitBlt( lpRect.left, lpRect.top, lpRect.right, lpRect.bottom, pDC, 0, 0, SRCCOPY  );

	pDC->SelectObject( pOldBrush );
	pDC->SelectObject( pOldBitMap );

	delete pDC;
	delete pBitMap;

}

/////////////////////////////////////////////////////////////////////////////
// CVikingEditorView printing

BOOL CVikingEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CVikingEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CVikingEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CVikingEditorView diagnostics

#ifdef _DEBUG
void CVikingEditorView::AssertValid() const
{
	CView::AssertValid();
}

void CVikingEditorView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVikingEditorDoc* CVikingEditorView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVikingEditorDoc)));
	return (CVikingEditorDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CVikingEditorView message handlers



void CVikingEditorView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class

	if((viewdelta)lHint==PRESENTDIE){
		CVikingEditorDoc* pDoc = GetDocument();
		RECT lpRect;
		CGdiObject* brushOldDie;
		int GridHeight, GridWidth, BoxHeight, BoxWidth;
		int Bincode;
		int RowCount,ColumnCount;


		CDC* pDC=GetDC( );
		CBrush* pOldBrush=pDC->GetCurrentBrush();
		
		GetClientRect(&lpRect);
	
		pDoc->GetRCCount(RowCount,ColumnCount);
		GridHeight= (lpRect.bottom) / RowCount;
		GridWidth=lpRect.right/ ColumnCount;
		BoxHeight=GridHeight-1;
		BoxWidth=GridWidth-1;

		pDC->SelectObject( m_brushYellow );
		pDC->Rectangle( pDoc->PresentColumn*GridWidth, pDoc->PresentRow*GridHeight, (pDoc->PresentColumn*GridWidth)+BoxWidth, (pDoc->PresentRow*GridHeight)+BoxHeight );

		Bincode=pDoc->GetBincode(pDoc->OldColumn+1,pDoc->OldRow+1);
		if(Bincode!=-1){
			brushOldDie= GetBincodeBrush( Bincode);
		}else{
			TRACE0("Bincode = -1\n");
			brushOldDie= m_brushWhite;
		}

		pDC->SelectObject( brushOldDie );
		pDC->Rectangle( pDoc->OldColumn*GridWidth, pDoc->OldRow*GridHeight, (pDoc->OldColumn*GridWidth)+BoxWidth, (pDoc->OldRow*GridHeight)+BoxHeight );

		pDC->SelectObject( pOldBrush );
		ReleaseDC( pDC );
		::GdiFlush();
	}else{
		CView::OnUpdate( pSender,  lHint,  pHint );
	}
	
}

afx_msg LONG CVikingEditorView::OnMapMoveNotify(UINT wParam, LONG)
{
	CVikingEditorDoc* pDoc = GetDocument();
	switch(wParam){
	case FIRSTACTIVEDIE:
		return pDoc->FirstActiveLocation();
	case NEXTACTIVEDIE:
		return pDoc->NextActiveLocation();
	case FIRSTDIE:
		return pDoc->FirstLocation();
	case NEXTDIE:
		return pDoc->NextLocation();
	default:
		ASSERT(FALSE);
		return 0;
	}
}

CBrush* CVikingEditorView::GetBincodeBrush(int Bincode, BOOL )
{
	if(((CVikingEditorDoc*)m_pDocument)->IsActiveBincode( Bincode)){
		return (CBrush *)m_brushes[Bincode];
	}else{
		return (CBrush *)m_brushGray;
	}
}
