// processThread.cpp : implementation file
//

#include "stdafx.h"
#include "app.h"
#include "processThread.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



//prototipes.
bool motor_process();

/////////////////////////////////////////////////////////////////////////////
// processThread

IMPLEMENT_DYNCREATE(processThread, CWinThread)

processThread::processThread()
{
   m_stop = false;
}

processThread::~processThread()
{
}

BOOL processThread::InitInstance()
{
	
	//while(!m_stop);
	////////and here we have a process of doing our process.

	motor_process();

    ///////das zite ist genug.
	ExitInstance();
	return TRUE;
}

int processThread::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(processThread, CWinThread)
	//{{AFX_MSG_MAP(processThread)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// processThread message handlers
