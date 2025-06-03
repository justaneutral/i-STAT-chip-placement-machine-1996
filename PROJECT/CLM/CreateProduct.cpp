// CreateProduct.cpp : implementation file
//

#include "stdafx.h"
#include "app.h"
#include "CreateProduct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCreateProduct dialog


CCreateProduct::CCreateProduct(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateProduct::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCreateProduct)
	m_id = _T("");
	m_name = _T("");
	//}}AFX_DATA_INIT
}


void CCreateProduct::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCreateProduct)
	DDX_Text(pDX, IDC_PRODUCT_ID, m_id);
	DDX_Text(pDX, IDC_PRODUCT_NAME, m_name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCreateProduct, CDialog)
	//{{AFX_MSG_MAP(CCreateProduct)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCreateProduct message handlers
