// mainfrm.cpp : implementation of the mainFrame class
//

#include "stdafx.h"

#include "globals.h"
#include "mainfrm.h"

#include "date.h"
#include "motor_parameters_dialog.h"
#include "fetcher_sequence_parameters_dialog.h"
#include "pick_place_parameters_dialog.h"
#include "pick_place_point_dialog.h"
#include "wafer_reference_point_dialog.h"
#include "wafer_camera_parameters_dialog.h"
#include "EditColor.h"
#include "about.h"
#include "ProductTypeDlg.h"
#include "StatusScreenDlg.h"
#include "SetupScreenDlg.h"
#include "UsersListDlg.h"
#include "ProductListDlg.h"
#include "Manipulator.h"

#include "createuserdlg.h"
#include "getuserdlg.h"

#include "machina\machina.h"


#define  _MAX_TIMER_COUNTER_ 10;

/////////////////////////////////////////////////////////////////////////////
// mainFrame

BEGIN_MESSAGE_MAP(mainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(mainFrame)
	ON_COMMAND(IDM_Exit, onExit)
	ON_COMMAND(IDM_Simple, onSimple)
	ON_COMMAND(IDM_Modal, onModal)
	ON_COMMAND(IDM_Modeless, onModeless)
	ON_COMMAND(IDM_MessageBox, onMessageBox)
	ON_COMMAND(IDM_diagnostic, Ondiagnostic)
	ON_COMMAND(IDM_SMAC_TX, OnSmacTx)
	ON_COMMAND(IDM_PROC_START, OnProcStart)
	ON_COMMAND(IDM_GetBaseCoord, OnGetBaseCoord)
	ON_COMMAND(ID_Create_new_user, OnCreatenewuser)
	ON_COMMAND(IDM_PROC_SUSPEND, OnProcSuspend)
	ON_COMMAND(IDM_Settings_update, OnSettingsupdate)
	ON_COMMAND(IDM_Settings_Recreate, OnSettingsRecreate)
	ON_COMMAND(IDM_Read_from_menu, OnReadfrommenu)
	ON_COMMAND(IDM_change_user, Onchangeuser)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_COMMAND(IDM_fetcher_sequence_parameters, Onfetchersequenceparameters)
	ON_COMMAND(IDM_motor_parameters_1, Onmotorparameters1)
	ON_COMMAND(IDM_motor_parameters_2, Onmotorparameters2)
	ON_COMMAND(IDM_motor_parameters_3, Onmotorparameters3)
	ON_COMMAND(IDM_motor_parameters_4, Onmotorparameters4)
	ON_COMMAND(IDM_motor_parameters_5, Onmotorparameters5)
	ON_COMMAND(IDM_motor_parameters_6, Onmotorparameters6)
	ON_COMMAND(IDM_motor_parameters_7, Onmotorparameters7)
	ON_COMMAND(IDM_motor_parameters_8, Onmotorparameters8)
	ON_COMMAND(IDM_motor_parameters_9, Onmotorparameters9)
	ON_COMMAND(IDM_pick_place_parameters, Onpickplaceparameters)
	ON_COMMAND(IDM_pick_point_parameters, Onpickpointparameters)
	ON_COMMAND(IDM_place_piont_pirameters, Onplacepiontpirameters)
	ON_COMMAND(IDM_wafer_reference_point_parameters, Onwaferreferencepointparameters)
	ON_COMMAND(IDM_wafer_camera_parameters, Onwafercameraparameters)
	ON_COMMAND(IDM_save_settings, Onsavesettings)
	ON_COMMAND(IDM_process_current_conditions, Onprocesscurrentconditions)
	ON_WM_PAINT()
	ON_WM_CHAR()
	ON_WM_KEYUP()
	ON_COMMAND(IDM_ABOUT, OnAbout)
	ON_COMMAND(IDM_window, Onwindow)
	ON_COMMAND(IDM_change_product_type, Onchangeproducttype)
	ON_COMMAND(IDM_status_screen, Onstatusscreen)
	ON_COMMAND(IDM_user_login, Onuserlogin)
	ON_COMMAND(IDM_EDIT_USERS, OnEditUsers)
	ON_COMMAND(IDM_BASE_BLACK, OnBaseBlack)
	ON_COMMAND(IDM_BASE_WHITE, OnBaseWhite)
	ON_COMMAND(IDM_COLLECT_HOME_STROKES, OnCollectHomeStrokes)
	ON_COMMAND(IDM_PRODUCT_LIST, OnProductList)
	ON_COMMAND(IDM_MANIPULATOR, OnManipulator)
	ON_COMMAND(IDM_WAFER_ADJUST, OnWaferAdjust)
	ON_COMMAND(IDM_INIT_MACHINE, OnInitMachine)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// mainFrame construction/destruction

mainFrame::mainFrame()
{
	if( !Create(NULL,						// Window class name
		_T("CLM"),						// Window title
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,	// Window style
		rectDefault,						// Origin and extent
		NULL,								// Parent
		MAKEINTRESOURCE(IDR_MainMenu))		// Menu
	) 
	{
	    AfxAbort();
		return;
	}
	
	
	ShowWindow(SW_MINIMIZE);
	
	
	/*
	RECT Rect;
	GetClientRect(&Rect);
    Rect.left += 20;
    Rect.top  += 30;
	Rect.right /= 2; 
    Rect.bottom /= 2; 
 	
	if(!m_text_output->Create(WS_CHILD|WS_VISIBLE|WS_BORDER|WS_VSCROLL|WS_HSCROLL|
		                     ES_MULTILINE|ES_READONLY|ES_AUTOHSCROLL|ES_AUTOVSCROLL,
		Rect, this, 70000))
	{
	    AfxAbort();
		return;
	}
	*/			 
    g_text_output = m_text_output = NULL;

	m_proc_thread = (HANDLE)NULL;
	is_suspended = false;

}

/////////////////////////////////////////////////////////////////////////////
// mainFrame message handlers

void mainFrame::onExit() 
{
	PostMessage(WM_CLOSE);
}

void mainFrame::onMessageBox() 
{
	if( AfxMessageBox(IDS_MessageBox,MB_YESNO|MB_ICONQUESTION)==IDYES )
		onSimple();
}

void mainFrame::onModal() 
{
	 //dateDialog dlg(this);
	 dateDialog *dlg = new dateDialog(this,FALSE);
}

void mainFrame::onModeless() 
{
	dateDialog *dlg = new dateDialog(this,FALSE);
}

void mainFrame::onSimple() 
{
	CDialog dlg(IDD_About);
	dlg.DoModal();
}

void mainFrame::Ondiagnostic() 
{
   	//dateDialog dlg(this);
	dateDialog *dlg = new dateDialog(this,FALSE);
}


void mainFrame::OnSmacTx() 
{




}


void mainFrame::OnProcStart() 
{
  ULONG trid;
  is_suspended = false;


  g_machine_state.status = _T("Running");

  GetMenu()->ModifyMenu(IDM_PROC_SUSPEND,MF_BYCOMMAND,
    IDM_PROC_SUSPEND,_T("&Suspend process"));

  if(m_proc_thread == (HANDLE)NULL)
  {
	 GetMenu()->ModifyMenu(IDM_PROC_START,MF_BYCOMMAND,
		   IDM_PROC_START,_T("&Stop process"));
	    
	 m_proc_thread = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)
	                       proc_supervisor,NULL,0,(ULONG *)&trid);

	 if(m_proc_thread) g_machine_state.is_started = true;
  }
  else
  {
    g_machine_state.status = _T("Idle");
	g_machine_state.m_error = false;

	TerminateThread(m_proc_thread,0);
	m_proc_thread = (HANDLE)NULL;

	GetMenu()->ModifyMenu(IDM_PROC_START,MF_BYCOMMAND,
		   IDM_PROC_START,_T("&Start process"));

    g_io.Plunge_Dwn();

	g_machine_state.is_started = false;

	g_io.Pause_Lt_Off();

  }	 
}


void mainFrame::ProcStop() 
{
  is_suspended = false;

  g_machine_state.status = _T("Idle");
  g_machine_state.m_error = false;

  GetMenu()->ModifyMenu(IDM_PROC_SUSPEND,MF_BYCOMMAND,
    IDM_PROC_SUSPEND,_T("&Suspend process"));

  g_io.Plunge_Dwn();
  
  if(m_proc_thread != (HANDLE)NULL)
  {
    TerminateThread(m_proc_thread,0);
	m_proc_thread = (HANDLE)NULL;

	GetMenu()->ModifyMenu(IDM_PROC_START,MF_BYCOMMAND,
		   IDM_PROC_START,_T("&Start process"));
  }	 
}



void mainFrame::OnProcSuspend() 
{

  if(m_proc_thread == (HANDLE)NULL)
  {
	 is_suspended = false;

	 GetMenu()->ModifyMenu(IDM_PROC_SUSPEND,MF_BYCOMMAND,
	   IDM_PROC_SUSPEND,_T("&Suspend process"));
	   g_io.Pause_Lt_Off(); 

  }
  else
  {
	 if(!is_suspended)
	 { 
	   ::SuspendThread(m_proc_thread);
	   
	   is_suspended = true;

	   g_machine_state.status = _T("Paused");

	   GetMenu()->ModifyMenu(IDM_PROC_SUSPEND,MF_BYCOMMAND,
  	   	   IDM_PROC_SUSPEND,_T("&Resume process"));

	   g_io.Pause_Lt_On(); 
	 }
	 else
	 {
	   ::ResumeThread(m_proc_thread);
	   
	   is_suspended = false;

	   g_machine_state.status = _T("Running");

	   GetMenu()->ModifyMenu(IDM_PROC_SUSPEND,MF_BYCOMMAND,
	      IDM_PROC_SUSPEND,_T("&Suspend process"));

	   g_io.Pause_Lt_Off(); 
	 }
  }	 
	
}



void mainFrame::OnGetBaseCoord() 
{
  double x, y, angle;
  char str[80];

  set_checksum(-2.0,0.0,0.0);	//set command = -2 for camera process.
  calculate_base_coordinates(0.0, 0.0, 0.0); //start camera process.
  AfxMessageBox("See the base coordinates.");
  get_checksum(&x);
  //while(x<0.5); //we are waiting for camera rezults.
  //{
//	  //::doevents();
	  get_checksum(&x);

  //}

  get_base_coordinates(&x, &y, &angle);
  tie_base_coordinates(&x, &y, &angle);

  sprintf(str,"x=%f; y=%f; angle=%f",x,y,angle);
  AfxMessageBox(str);
}


void mainFrame::OnCreatenewuser() 
{
	createUserDlg dlg = new createUserDlg;
	if(dlg.DoModal() == IDOK)
	{
	  if(dlg.m_password != dlg.m_password_confirm) 
	  {
	    AfxMessageBox("Passwords do not mach");
	    delete dlg;
	    return;
	  }
	
	  FILE *f;
	  
	  if((f = fopen(dlg.m_name,"w")) == NULL)
  	  {
        AfxMessageBox("Can not write to disk");
	    delete dlg;
	    return;
	  }

	  fprintf(f,"%d%s",(int)dlg.m_permissions,dlg.m_password);
	  fclose(f);
	}

    delete dlg;
}

void mainFrame::OnSettingsupdate() 
{
   if(g_machine) delete g_machine;
   g_machine = new machineSettings;
}

void mainFrame::OnSettingsRecreate() 
{
   AfxMessageBox("Deleting Old Settings file.");
   if(g_machine) delete g_machine;
   g_machine = new machineSettings;
   g_machine->init();
   g_machine->save();
}

void mainFrame::OnReadfrommenu() 
{
	if((theApp.map) != (CVikingEditorDoc *)NULL)
	{
	   delete (CVikingEditorDoc *)(theApp.map);
	}
	
	theApp.map = new CVikingEditorDoc();
	
	//conout("Get vik file");
	CFileDialog *fd = new CFileDialog(TRUE);
	fd->DoModal();
	
	if(!theApp.map->OnOpenDocument(fd->GetPathName()));

	delete fd;
}

void mainFrame::Onchangeuser() 
{
	int i;
	
	getuserDlg User;
	CUsersListDlg Users;
	
	//Serialize users data.
	Users.ReadUsers();
	
	do
	{
	  User.DoModal();

	  for(i=0; i < Users.FullName.GetSize();i++)
	    if(Users.UserID[i]==User.m_name && Users.Password[i]==User.m_password)
		{
		   //Entered.
		   g_current_user.Restrictions = Users.Restrictions[i];
		   g_current_user.FullName     = Users.FullName[i];
		   g_current_user.UserID       = Users.UserID[i];
		}
	}
	while (g_current_user.UserID.IsEmpty());
	

	if(g_current_user.Restrictions == FALSE) //Operator.
	{
	   GetMenu()->EnableMenuItem(2,MF_GRAYED|MF_BYPOSITION);
	   GetMenu()->EnableMenuItem(3,MF_GRAYED|MF_BYPOSITION);
       GetMenu()->EnableMenuItem(ID_Delete_user,MF_GRAYED);
	}
	else									 //Supervisor.
	{
  		      
	   GetMenu()->EnableMenuItem(2,MF_ENABLED|MF_BYPOSITION);
	   GetMenu()->EnableMenuItem(3,MF_ENABLED|MF_BYPOSITION);
       GetMenu()->EnableMenuItem(ID_Delete_user,MF_ENABLED);
	}

	DrawMenuBar();
}


int mainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	is_in_critical_section = false;
    
	SetTimer(0,50,NULL);
	conout("Timer is created.");
	
	//Init User.
	g_current_user.UserID = _T("");
	g_current_user.Restrictions = FALSE;
	g_current_user.FullName = _T("");
	
	timer_counter = _MAX_TIMER_COUNTER_;
	

	

	////starting machine subsystems.
	PostMessage(WM_COMMAND,IDM_INIT_MACHINE,0);	  
	
	//initial screens.
	//PostMessage(WM_COMMAND,IDM_user_login,0);	  
	
	return 0;
}

void mainFrame::OnDestroy() 
{
	KillTimer(0);
	if(g_text_output) delete g_text_output;
	g_text_output = NULL;	
	CFrameWnd::OnDestroy();
}

void mainFrame::OnTimer(UINT nIDEvent) 
{
	
	DWORD locCounter;

	if(!(timer_counter--))
	{
	  Invalidate();
	  timer_counter = _MAX_TIMER_COUNTER_;
	}
	

	if(!machine_is_initialized) return;

	//Process Pause/Resume button and light.
	
	if(g_io.Pause_is_Pressed())
	{	
	   OnProcSuspend();

	   do
	   {
	     Sleep(200);
	   }
	   while(g_io.Pause_is_Pressed());
	}
	else
	
	//Process Start/Stop button.
	
	if(g_io.Start_is_Pressed())
	{	
	   OnProcStart();

	   do
	   {
	     Sleep(200);
	   }
	   while(g_io.Start_is_Pressed());
	}
	else
	
	//Home button is pressed.
	
	if(g_io.Home_is_Pressed()) 
	{	
		
		ProcStop();
		g_io.Homming();
		g_Axes.Homming(FALSE);
		g_smac.Homming();
	}
	else
	
	//clear error for retry.
	if(g_io.Ignore_is_Pressed())
	{
	   g_machine_state.m_error = false;

	   while(g_io.Ignore_is_Pressed());
	}
	
	
	
	
	
	if(is_in_critical_section) Sleep(200);
	
}



void mainFrame::Onfetchersequenceparameters() 
{
	fetcher_sequence_parameters_dialog dlg;
	dlg.DoModal();
}

void mainFrame::Onmotorparameters1() 
{
   motor_parameters_dialog 	dlg(1);
   dlg.DoModal();
}

void mainFrame::Onmotorparameters2() 
{
   motor_parameters_dialog 	dlg(2);
   dlg.DoModal();
}

void mainFrame::Onmotorparameters3() 
{
   motor_parameters_dialog 	dlg(3);
   dlg.DoModal();
}

void mainFrame::Onmotorparameters4() 
{
   motor_parameters_dialog 	dlg(4);
   dlg.DoModal();
}

void mainFrame::Onmotorparameters5() 
{
   motor_parameters_dialog 	dlg(5);
   dlg.DoModal();
}

void mainFrame::Onmotorparameters6() 
{
   motor_parameters_dialog 	dlg(6);
   dlg.DoModal();
}

void mainFrame::Onmotorparameters7() 
{
   motor_parameters_dialog 	dlg(7);
   dlg.DoModal();
}

void mainFrame::Onmotorparameters8() 
{
   motor_parameters_dialog 	dlg(8);
   dlg.DoModal();
}

void mainFrame::Onmotorparameters9() 
{
   motor_parameters_dialog 	dlg(9);
   dlg.DoModal();
}

void mainFrame::Onpickplaceparameters() 
{
   pick_place_parameters_dialog dlg;
   dlg.DoModal();	
}

void mainFrame::Onpickpointparameters() 
{
	if(!g_machine) 
	{
	   AfxMessageBox("Settings are corrupted.");
	   return;
	}

	pick_place_point_dialog dlg(&(g_machine->gantry_x),
								&(g_machine->gantry_y),
								&(g_machine->gantry_z),
								&(g_machine->gantry_angle));
	dlg.DoModal();
}

void mainFrame::Onplacepiontpirameters() 
{
	if(!g_machine) 
	{
	   AfxMessageBox("Settings are corrupted.");
	   return;
	}

	pick_place_point_dialog dlg(&(g_machine->base_x),
								&(g_machine->base_y),
								&(g_machine->base_z),
								&(g_machine->base_angle));
	
    dlg.DoModal();
}

void mainFrame::Onwaferreferencepointparameters() 
{
    wafer_reference_point_dialog dlg;
	dlg.DoModal();
}

void mainFrame::Onwafercameraparameters() 
{
    wafer_camera_parameters_dialog dlg;
	dlg.DoModal();
}

void mainFrame::Onsavesettings() 
{
    if(!g_machine)
	{
	  AfxMessageBox("Settings structure does not exist");
	  return;
	}
    g_machine->save();
}

void mainFrame::Onprocesscurrentconditions() 
{
   g_machine_state.DoModal();
}

void mainFrame::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	static bool migalka;
	char inf[64];
	char mig = (migalka = !migalka) ? '(' : ')';



	sprintf(inf,"W#%d (%s) D#%d %c",
		g_machine_state.m_wafer_position,
		g_machine_state.m_file_name,
		g_machine_state.m_chip_position,
		mig );

	CRect r;
	this->GetClientRect(&r);
	CPoint p = r.BottomRight();

	dc.TextOut(0,p.y-20,inf,strlen(inf));

	dc.MoveTo(0,16*20); dc.LineTo(p.x,16*20);


	int i;
	for(i=0;i<12;i++)
	{
	  dc.TextOut(0,i*20,(g_wcons.outp)[i],strlen((g_wcons.outp)[i]));
	  if(i<3) dc.TextOut(0,(i+17)*20, (g_wcons.inp)[i],strlen((g_wcons.inp)[i]));
	}
	
	// Do not call CFrameWnd::OnPaint() for painting messages
}

void mainFrame::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int i;
	i = min(45,strlen(g_wcons.inp[2]));

	switch(nChar)
	{
	  case 13:
		 g_wcons.inp[2][i] = (char)'\0';
	  break;
	  default:
	     g_wcons.inp[2][i] = (char)nChar;
         g_wcons.inp[2][i+1] = (char)'\0';
	}

	
	CFrameWnd::OnChar(nChar, nRepCnt, nFlags);
	Invalidate();
}

void mainFrame::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	int i;
	
	//functional keys.
	switch(nChar)
	{
	  case 112: //"<F1>"
	  	   strcpy(g_wcons.inp[2],"<F1>"); 
         break;
	}
	
	//common.
	switch(nChar)
	{
	  case 13: //<Entre>
	  case 112: //"<F1>"
		   for(i=0;i<2;i++)  strcpy(g_wcons.inp[i],g_wcons.inp[i+1]);
		   strcpy(g_wcons.inp[2],">");
 	       kbHit = true;
		 break;
	}
	
	
	CFrameWnd::OnKeyUp(nChar, nRepCnt, nFlags);
	Invalidate();
}

void mainFrame::OnAbout() 
{
	CAbout abdlg;
	abdlg.DoModal();
	
}

void mainFrame::Onwindow() 
{
    if(g_text_output == NULL) g_text_output = new CEditColor();
	else 
	{
		delete  g_text_output;
		g_text_output = NULL;
	}
}

void mainFrame::Onchangeproducttype() 
{
  if(indicator) return;
  CProductTypeDlg *dlg = new CProductTypeDlg(this, &indicator);
}

void mainFrame::Onstatusscreen() 
{
   if(indicator) return;
   CStatusScreenDlg *dlg = new CStatusScreenDlg(this,&indicator);
   //dlg.DoModal();
}

void mainFrame::Onuserlogin() 
{
    if(indicator) return;

	Onchangeuser();
    Onchangeproducttype();
	while(indicator) doevents();

	Onstatusscreen();
}

/*

void mainFrame::OnParametersChange() 
{
	CSetupScreenDlg setup_scr_dlg;
	setup_scr_dlg.DoModal();

}
*/


void mainFrame::OnEditUsers() 
{
   CUsersListDlg users_list_dlg;
   users_list_dlg.ReadUsers();
   users_list_dlg.DoModal();

   users_list_dlg.SaveUsers();
}

void mainFrame::OnBaseBlack() 
{
	sprintf(s,"The current value for black level is %f. To renew - OK, to keep it - CANCEL",
		      g_machine->dBaseBlack);
		
	if(MessageBox(s,"Vision system calibration",MB_OKCANCEL)==IDCANCEL) return;
	
	while(!VisionSystemIsStarted())
		MessageBox(
		"Vision System is not started. Please start or restart it and press OK button",
		"Vision system calibration",MB_OK);

	
	set_camera(2);
	g_machine->dBaseBlack = get_base_light_level();

	sprintf(s,"The new value for black level is %f.",
		      g_machine->dBaseBlack);
		
	MessageBox(s,"Vision system calibration",MB_OK);
}

void mainFrame::OnBaseWhite() 
{
	sprintf(s,"The current value for white level is %f. To renew - OK, to keep it - CANCEL",
		      g_machine->dBaseWhite);
		
	if(MessageBox(s,"Vision system calibration",MB_OKCANCEL)==IDCANCEL) return;
	
	while(!VisionSystemIsStarted())
		MessageBox(
		"Vision System is not started. Please start or restart it and press OK button",
		"Vision system calibration",MB_OK);

	
	set_camera(2);
	g_machine->dBaseWhite = get_base_light_level();

	sprintf(s,"The new value for white level is %f.",
		      g_machine->dBaseWhite);
		
	MessageBox(s,"Vision system calibration",MB_OK);
}

void mainFrame::OnCollectHomeStrokes() 
{
   g_Axes.CollectHomeDistances();	
}

void mainFrame::OnProductList() 
{
   CProductListDlg product_list_dlg;
   product_list_dlg.ReadProduct();
   product_list_dlg.DoModal();

   product_list_dlg.SaveProduct();

}

void mainFrame::OnManipulator() 
{
   CManipulator dlg;

   dlg.DoModal();
}

void mainFrame::OnWaferAdjust() 
{ 
	adjust_wafer();
}




void mainFrame::OnInitMachine() 
{
	
    if(machine_is_initialized)
    {
       conout("Machine subsystems are initialized allready");
	   return;
	}

#ifdef _CONSOLE_

	AllocConsole();
	g_in = GetStdHandle(STD_INPUT_HANDLE);
    g_out = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTitle("CLM Machine Console");

#endif
	
	conout("Main Module of the chiploader started");
	conout("Starting machine subsystems.");
	
	machine_is_initialized = start_machina(); 
	
}
