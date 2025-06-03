// FileTimeOutDialog.cpp : implementation file
//

#include "stdafx.h"
#include "app.h"
#include "FileTimeOutDialog.h"

#ifdef _DEBUG
//#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileTimeOutDialog

IMPLEMENT_DYNAMIC(CFileTimeOutDialog, CFileDialog)

CFileTimeOutDialog::CFileTimeOutDialog(BOOL bOpenFileDialog, LPCTSTR lpszDefExt, LPCTSTR lpszFileName,
		DWORD dwFlags, LPCTSTR lpszFilter, CWnd* pParentWnd) :
		CFileDialog(bOpenFileDialog, lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
    m_timer = 0;   
	AfxMessageBox("Constructed.");
		
}


BEGIN_MESSAGE_MAP(CFileTimeOutDialog, CFileDialog)
	//{{AFX_MSG_MAP(CFileTimeOutDialog)
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CFileTimeOutDialog::OnTimer(UINT nIDEvent) 
{
	EndDialog(IDCANCEL);
	AfxMessageBox("In OnTimer.");
	CFileDialog::OnTimer(nIDEvent);

}

int CFileTimeOutDialog::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFileDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
    m_timer = SetTimer(0,10000,NULL);
	AfxMessageBox("In OnCreate.");
	return 0;
}

void CFileTimeOutDialog::OnClose() 
{
	KillTimer(m_timer);
	AfxMessageBox("In OnClose.");
	CFileDialog::OnClose();
}

void CFileTimeOutDialog::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_timer) KillTimer(m_timer);
	m_timer = SetTimer(0,10000,NULL);

	CFileDialog::OnMouseMove(nFlags, point);
}
