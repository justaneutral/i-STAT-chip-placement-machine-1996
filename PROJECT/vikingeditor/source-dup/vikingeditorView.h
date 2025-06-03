// VikingEditorView.h : interface of the CVikingEditorView class
//
/////////////////////////////////////////////////////////////////////////////
#define WM_MAPMOVE WM_USER+110
#define FIRSTACTIVEDIE 1
#define NEXTACTIVEDIE 2
#define FIRSTDIE 3
#define NEXTDIE 4

class CVikingEditorDoc;

class CVikingEditorView : public CView
{
protected: // create from serialization only
	CVikingEditorView();
	DECLARE_DYNCREATE(CVikingEditorView)

// Attributes
public:
	CBrush* GetBincodeBrush(int Bincode, BOOL Printing=FALSE );
	CVikingEditorDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CVikingEditorView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CVikingEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CGdiObject* m_brushes[256];
	CGdiObject* m_brushRed;
	CGdiObject* m_brushGray;
	CGdiObject* m_brushWhite;
	CGdiObject* m_brushYellow;
	CGdiObject* m_brushGreen;
// Generated message map functions
protected:
	afx_msg LONG OnMapMoveNotify(UINT wParam, LONG lParam);
	//{{AFX_MSG(CVikingEditorView)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in VikingEditorView.cpp
inline CVikingEditorDoc* CVikingEditorView::GetDocument()
   { return (CVikingEditorDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////
