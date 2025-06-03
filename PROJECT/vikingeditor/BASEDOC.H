#if !defined(AFX_BASEDOC_H__FE5B0ED1_D9EF_11D1_8C51_00C04FAD4034__INCLUDED_)
#define AFX_BASEDOC_H__FE5B0ED1_D9EF_11D1_8C51_00C04FAD4034__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// basedoc.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBaseDoc document

class CBaseDoc : public CDocument
{
protected:
	CBaseDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CBaseDoc)

// Attributes
public:
   char Data[30][80];
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBaseDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBaseDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CBaseDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BASEDOC_H__FE5B0ED1_D9EF_11D1_8C51_00C04FAD4034__INCLUDED_)
