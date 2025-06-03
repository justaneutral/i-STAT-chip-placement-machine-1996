// EditString.cpp : implementation file
//

#include "stdafx.h"
#include "app.h"
#include "EditString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditString

CEditString::CEditString()
{
   ready_f = false;
}

CEditString::~CEditString()
{
}


BEGIN_MESSAGE_MAP(CEditString, CEdit)
	//{{AFX_MSG_MAP(CEditString)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditString message handlers

void CEditString::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar != (char)0xd && nChar != (char)0xa)
	{
	  CEdit::OnChar(nChar, nRepCnt, nFlags);
	}
	else 
	{
		ready_f = true;
		GetWindowText(string,80);
	}
}
