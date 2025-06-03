// basedoc.cpp : implementation file
//

#include "stdafx.h"
#include "vikingeditor.h"
#include "basedoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBaseDoc

IMPLEMENT_DYNCREATE(CBaseDoc, CDocument)

CBaseDoc::CBaseDoc()
{
	for(int i=0;i<30;i++)
	strcpy(Data[i],"123456789");
}

BOOL CBaseDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CBaseDoc::~CBaseDoc()
{
}


BEGIN_MESSAGE_MAP(CBaseDoc, CDocument)
	//{{AFX_MSG_MAP(CBaseDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBaseDoc diagnostics

#ifdef _DEBUG
void CBaseDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CBaseDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBaseDoc serialization

void CBaseDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
		
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CBaseDoc commands
