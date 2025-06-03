// ProductListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "app.h"
#include "ProductListDlg.h"
#include "CreateProduct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProductListDlg dialog


CProductListDlg::CProductListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProductListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProductListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}



void CProductListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProductListDlg)
	DDX_Control(pDX, IDC_Product_list_list, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProductListDlg, CDialog)
	//{{AFX_MSG_MAP(CProductListDlg)
	ON_BN_CLICKED(IDC_BUTTON_add_Product, OnBUTTONaddProduct)
	ON_BN_CLICKED(IDC_BUTTON_delete_Product, OnBUTTONdeleteProduct)
	ON_BN_CLICKED(IDC_BUTTON_edit_Product, OnBUTTONeditProduct)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CProductListDlg message handlers

BOOL CProductListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_list.DeleteAllItems();
	
	for(int i=0; i < ProductID.GetSize(); i++)
	{
	  	m_list.InsertItem( i,(LPCTSTR)(ProductID[i]+_T(" --> "+ChipName[i])));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}




void CProductListDlg::SaveProduct()
{
	CFile f;
	BOOL b;
	
	b = f.Open("\\\\bay1\\c$\\product.clm", CFile::modeCreate | CFile::modeWrite);
	
	if(b)
	{
	    CArchive a(&f,CArchive::store);
		
		ProductID.Serialize(a);
		ChipName.Serialize(a);
	}
}

void CProductListDlg::ReadProduct()
{
	CFile f;
	BOOL b = FALSE;
	
	b = f.Open("\\\\bay1\\c$\\product.clm",CFile::modeRead);	
	if(b)
	{
		CArchive a(&f,CArchive::load);
		ProductID.Serialize(a);
		ChipName.Serialize(a);
	}
}



void CProductListDlg::OnBUTTONaddProduct() 
{
	CCreateProduct cpdlg;
	cpdlg.m_id               = _T("");
	cpdlg.m_name             = _T("");
	
	if(cpdlg.DoModal() == IDOK)
	  if(cpdlg.m_id != _T(""))
	  {
		  
  		  ProductID.Add(cpdlg.m_id);
		  ChipName.Add(cpdlg.m_name);
		  
		  m_list.InsertItem(m_list.GetItemCount(),
		      (LPCTSTR)(cpdlg.m_id+_T(" --> ")+cpdlg.m_name));
	  }

	
}

void CProductListDlg::OnBUTTONdeleteProduct() 
{
	for(int i=0; i < ProductID.GetSize(); i++)
	 if(m_list.GetItemState(i,LVIS_SELECTED))
	 {
		m_list.DeleteItem(i);
		
		ProductID.RemoveAt(i,1);
		ChipName.RemoveAt(i,1);
		
		return;
	 }
}

void CProductListDlg::OnBUTTONeditProduct() 
{
	for(int i=0; i < ProductID.GetSize(); i++)
	 if(m_list.GetItemState(i,LVIS_SELECTED))
	 {
		//get user data from the users set.
		CCreateProduct cpdlg;
		cpdlg.m_id      = ProductID[i];
		cpdlg.m_name    = ChipName[i];

        if(cpdlg.DoModal() == IDOK && cpdlg.m_id != _T(""))
		{
		  //update product data.
		  ProductID[i]  = cpdlg.m_id;
		  ChipName[i]   = cpdlg.m_name;
          m_list.SetItemText(i,0,(LPCTSTR)(ProductID[i]+_T(" --> ")+ChipName[i]));
		}
		return;
	 }
	
}
