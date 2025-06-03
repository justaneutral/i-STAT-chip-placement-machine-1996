// app.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "mainfrm.h"
#include "globals.h"
#include <iostream.h>

extern void doevents();

//protot.
BOOL start_machina();



BOOL application::InitInstance()
{
	
	
	g_machine = new machineSettings;
	
	map = (CVikingEditorDoc *)NULL;

	m_pMainWnd = new mainFrame;
	g_Wnd = m_pMainWnd;
	
	g_Wnd->SetWindowText("CLM main control process");
	

    return TRUE;
}

