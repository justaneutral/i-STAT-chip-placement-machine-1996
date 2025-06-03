#if !defined(AFX_FILETIMEOUTDIALOG_H__9E857961_1FE0_11D2_8CA5_00C04FAD4034__INCLUDED_)
#define AFX_FILETIMEOUTDIALOG_H__9E857961_1FE0_11D2_8CA5_00C04FAD4034__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// FileTimeOutDialog.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileTimeOutDialog dialog

class CFileTimeOutDialog : public CFileDialog
{
	DECLARE_DYNAMIC(CFileTimeOutDialog)

public:
	CFileTimeOutDialog(BOOL bOpenFileDialog, // TRUE for FileOpen, FALSE for FileSaveAs
		LPCTSTR lpszDefExt = NULL,
		LPCTSTR lpszFileName = NULL,
		DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		LPCTSTR lpszFilter = NULL,
		CWnd* pParentWnd = NULL);

	DWORD m_timer;

protected:
	//{{AFX_MSG(CFileTimeOutDialog)
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILETIMEOUTDIALOG_H__9E857961_1FE0_11D2_8CA5_00C04FAD4034__INCLUDED_)
