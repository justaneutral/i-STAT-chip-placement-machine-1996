#if !defined(AFX_PROCESSTHREAD_H__C937DB21_C58B_11D1_8C3D_00C04FAD4034__INCLUDED_)
#define AFX_PROCESSTHREAD_H__C937DB21_C58B_11D1_8C3D_00C04FAD4034__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// processThread.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// processThread thread

class processThread : public CWinThread
{
	DECLARE_DYNCREATE(processThread)
protected:
	processThread();           // protected constructor used by dynamic creation

// Attributes
public:
bool m_stop;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(processThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~processThread();

	// Generated message map functions
	//{{AFX_MSG(processThread)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROCESSTHREAD_H__C937DB21_C58B_11D1_8C3D_00C04FAD4034__INCLUDED_)
