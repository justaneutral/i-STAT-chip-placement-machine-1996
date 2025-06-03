#if !defined(AFX_CREATEPRODUCT_H__AC8CC301_5176_11D2_B36B_00C04FAD4084__INCLUDED_)
#define AFX_CREATEPRODUCT_H__AC8CC301_5176_11D2_B36B_00C04FAD4084__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// CreateProduct.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCreateProduct dialog

class CCreateProduct : public CDialog
{
// Construction
public:
	CCreateProduct(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCreateProduct)
	enum { IDD = IDD_CREATE_PRODUCT };
	CString	m_id;
	CString	m_name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCreateProduct)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCreateProduct)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CREATEPRODUCT_H__AC8CC301_5176_11D2_B36B_00C04FAD4084__INCLUDED_)
