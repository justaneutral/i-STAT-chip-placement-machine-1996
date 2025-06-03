// UsersListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "app.h"
#include "UsersListDlg.h"
#include "createUserDlg.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUsersListDlg dialog


CUsersListDlg::CUsersListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUsersListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CUsersListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CUsersListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CUsersListDlg)
	DDX_Control(pDX, IDC_user_list_list, m_list);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CUsersListDlg, CDialog)
	//{{AFX_MSG_MAP(CUsersListDlg)
	ON_BN_CLICKED(IDC_BUTTON_delete_user, OnBUTTONdeleteuser)
	ON_BN_CLICKED(IDC_BUTTON_edit_user, OnBUTTONedituser)
	ON_BN_CLICKED(IDC_BUTTON_add_user, OnBUTTONadduser)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CUsersListDlg message handlers

BOOL CUsersListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_list.DeleteAllItems();
	
	for(int i=0; i < UserID.GetSize(); i++)
	{
	  	//sprintf(s,"%s\t%s",(LPCTSTR)((m_Users[i]).FullName),
		//	               (LPCTSTR)((m_Users[i]).UserID));
		m_list.InsertItem( i,(LPCTSTR)(FullName[i]+_T(" ( ")+UserID[i]+_T(" )")));
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CUsersListDlg::SaveUsers()
{
	CFile f;
	BOOL b;
	
	b = f.Open("\\\\bay1\\c$\\users.clm", CFile::modeCreate | CFile::modeWrite);
	
	if(b)
	{
	    CArchive a(&f,CArchive::store);
		
		UserID.Serialize(a);
		FullName.Serialize(a);
		Password.Serialize(a);
		Restrictions.Serialize(a);
	}
}

void CUsersListDlg::ReadUsers()
{
	CFile f;
	BOOL b = FALSE;
	
	b = f.Open("\\\\bay1\\c$\\users.clm",CFile::modeRead);	
	if(b)
	{
		CArchive a(&f,CArchive::load);
		UserID.Serialize(a);
		FullName.Serialize(a);
		Password.Serialize(a);
		Restrictions.Serialize(a);
	}
}

void CUsersListDlg::OnBUTTONdeleteuser() 
{	
	for(int i=0; i < UserID.GetSize(); i++)
	 if(m_list.GetItemState(i,LVIS_SELECTED))
	 {
		m_list.DeleteItem(i);
		
		UserID.RemoveAt(i,1);
		FullName.RemoveAt(i,1);
		Password.RemoveAt(i,1);
		Restrictions.RemoveAt(i,1);

		return;
	 }
}





void CUsersListDlg::OnBUTTONedituser() 
{
	for(int i=0; i < UserID.GetSize(); i++)
	 if(m_list.GetItemState(i,LVIS_SELECTED))
	 {
		//get user data from the users set.
		createUserDlg cudlg;
		cudlg.m_id               = UserID[i];
		cudlg.m_name             = FullName[i];
		cudlg.m_password         = Password[i];
		cudlg.m_password_confirm = Password[i];
		cudlg.m_permissions      = Restrictions[i];

		do
		{ 
			if(cudlg.DoModal() == IDCANCEL) return;
		} 
		while( cudlg.m_id == _T("") || cudlg.m_password != cudlg.m_password_confirm);
		
		//update user data.
		UserID[i]       = cudlg.m_id;
		FullName[i]     = cudlg.m_name;
		Password[i]     = cudlg.m_password;
		Restrictions[i] = cudlg.m_permissions;
		
		
		m_list.SetItemText(i,0,(LPCTSTR)(FullName[i]+_T(" ( ")+UserID[i]+_T(" )")));
		
		return;
	 }
	
}

void CUsersListDlg::OnBUTTONadduser() 
{
	createUserDlg cudlg;
	cudlg.m_id               = _T("");
	cudlg.m_name             = _T("");
	cudlg.m_password         = _T("");
	cudlg.m_password_confirm = _T("");
	cudlg.m_permissions      = false;
	
	if(cudlg.DoModal() == IDOK)
	  if(cudlg.m_id != _T("") && cudlg.m_password == cudlg.m_password_confirm)
	  {
		  
  		  UserID.Add(cudlg.m_id);
		  FullName.Add(cudlg.m_name);
		  Password.Add(cudlg.m_password);
		  Restrictions.Add(cudlg.m_permissions);


		  m_list.InsertItem(m_list.GetItemCount(),
			      (LPCTSTR)(cudlg.m_name+_T("  ")+cudlg.m_id));
	  }
}
