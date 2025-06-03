
#define _WINNT


#define _CHIP_MASCHTAB_ 0.00275
#define _PLUNGE_CAM_X_  293
#define _PLUNGE_CAM_Y_  232

#define _CONVEYOR_

//#define _TEST_WAFER_ORIENTATION_		

//globals representing the devices of the machine.
#define __main_module__
#include "../globals.h"


#include <conio.h>
#include <stdio.h>
#include <math.h>
//#include "../stdafx.h"
#include "../../vikingeditor/vikingeditordoc.h"
#include <process.h>

#include "../manipulator.h"

#include "../get_value_from_dialog.h"


bool FileExists(char *FileName)
{
   HANDLE f;
   f = CreateFile(FileName,GENERIC_READ,FILE_SHARE_READ,0,OPEN_EXISTING,0,0);
   if(f==INVALID_HANDLE_VALUE) return false;
   CloseHandle(f);
   return true; 
}





//pusk.
BOOL start_machina()
{
  char *retstr;

  conout("Initializing I/O board.");
  if(!g_io.Init()) 
  {
	  MessageBox(NULL,"I/O subsystem initialization error. Check power and cables. Restart the program","I/O initialization", MB_OK);
	  //__Error("I/O initialization error.",0);
	  return FALSE;
  }
  conout("I/O initialization success.");
  
  conout("Initializing gantry head motor.");
  if(g_smac.Init()) conout("Gantry head motor initialized successfuly.");
  else 	
  {
	  conout("Gantry head motor not initialized");
	  return FALSE;
  }
  
  conout("Initializing axis motors.");
  retstr = g_Axes.Init();
  if(*retstr)   
  {
	 MessageBox(NULL,"Axis motors initialization error. Check power and cables. Restart the program","Axis motors initialization", MB_OK);
	 conout("Axis motors initialization error. Check power and cables. Restart the program");
	 //__Error("Axis motors initialization error. Check power and cables. Restart the program",0);
	 return FALSE;
  }

  g_Axes.Homming(false);
  
  return TRUE;
}




void errout(char *error)
{
   FILE *f = fopen("../error.log","r+");
   if(f)
   {
      fprintf(f,"%s\n",error);
      fclose(f);
   }
}




void correct(double &value, char *name)
{
   	char str[80];
	sprintf(str,"%s = %f, enter new value",name,(float)value);
	conout(str);
	conin(str);
	value = atof(str);
}



void doevents()
{	
   MSG qMsg;
   if(::GetMessage (&qMsg,NULL,0,0))
   {
	  ::TranslateMessage (&qMsg);
	  ::DispatchMessage (&qMsg);
   }
}


double camera_x, camera_y, camera_angle;


void tie_base_coordinates(double *camera_x, double *camera_y, double *camera_angle)
{
	double Yg,Xg,Qg;
				   
	Yg = g_machine->base_y     - g_machine->base_camera_x  - *camera_x;
	Xg = g_machine->base_x     - g_machine->base_camera_y  - *camera_y;
	Qg = g_machine->base_angle + g_machine->base_camera_angle /*+ *camera_angle*/;

	*camera_x     = Xg;
	*camera_y     = Yg;
	*camera_angle = Qg;
}




void adjust()
{
	char str[80];
	g_Axes.ZeroTorque(1);
	g_Axes.ZeroTorque(2);

	
	conout("Set the 1,2 axes positions manualy and press the button");
	getch();
    //Close the gripers.
	g_io.Grip_Close();
	//rotate and pop smac head before pick up the chip.
	g_smac.DegreeRotateInchesMove(-10.0, 0.01);	
	g_smac.delay(1000);
	//Open the gripers.
	g_io.Grip_Open();

	g_Axes.GetInchPosition(g_machine->wafer_y,1); //Y
	g_Axes.GetInchPosition(g_machine->wafer_x,2); //X
	

	sprintf(str,"Ax1(Y)=%f, Ax2(X)=%f",g_machine->wafer_y,g_machine->wafer_x);
	
	g_Axes.restore_torque();

	conout(str);
	getch();
}





double approach_the_bottom()
{

   	double pos[2];
	
	//Moving head toward the surface.
	pos[0] = pos[1] = 0.0;
	do
	{
	  pos[0] += 0.01;
	  g_smac.InchesMove(pos[0]);
	  Sleep(2*long(g_machine->smac_delay));
	  pos[1] = g_smac.GetLinearPosition();

	  if(fabs(pos[1]-pos[0])>0.06) 
	  {
		  pos[1] = pos[0]+0.002;
	  }
	} 
	while( pos[0] < 0.8 && fabs(pos[0]-pos[1]) < 0.03);
	
	//decrease the tension.
	g_smac.InchesMove(pos[1]);

	return pos[1];
}




void glue_chip()
{
	approach_the_bottom();
	//open gripper.
	g_io.Grip_Open();
	while(g_io.Grip_is_Closed());
	conout("Push the chip to the base.");
	//Grep. Vac. off.
	g_io.Grip_Vac_Off();
	//Gr. Air on and off.
	//g_io.Grip_Air_On();
	//Sleep((DWORD)g_machine->grip_air_delay);
	//g_io.Grip_Air_Off();
	//remove head.
	g_smac.InchesMove(0.0);
	Sleep(long(g_machine->smac_delay));
	//close the gripper.
	g_io.Grip_Close();
}




//very important -- to calibrate all parts.
bool adjust_wafer()
{
	
	int i;
	//char str[80];
	double pos[2], angle;
	
	CManipulator manipulator;

	if(!g_Axes.is_hommed) return false;


	conout("Clearing camera # 1 FOV");	  
	g_io.Grip_Close();
	g_smac.InchesMove(0.0);
	g_Axes.InchPosition(1.0, pos[0],5);
	g_Axes.InchPosition(9.0, pos[0], 4);
	g_Axes.InchPosition(0.0, pos[0],5);
	if(AfxMessageBox("Please make sure there is no wafer loaded in the stretcher",MB_OKCANCEL)==IDCANCEL) return false;
	conout("Closing stretcher");
	g_Axes.InchPosition(1.0, pos[0],5);

	////Invalidate Z coordinates.
	g_machine->base_z = g_machine->gantry_z = 0.0;


	////WAFER POSITION.
	i=AfxMessageBox("Please put CALIBRATION TOOL onto the stretcher",MB_OKCANCEL);
	if(i==IDCANCEL) return false;
	
	
	conout("Lowering the plunge up mechanism position.");
	g_io.Plunge_Dwn();
	Sleep(200);
	while(g_io.Plunge_is_Up());
	

	
	conout("Position axeses # 1 and  # 2 with hand manipulator, so that the hall be in FOV.");
	
	g_Axes.InchPosition(g_machine->wafer_y,pos[0],1);
	g_Axes.InchPosition(g_machine->wafer_x,pos[1],2);
	
	i=AfxMessageBox("Is there the hall in the FOV ?",MB_YESNOCANCEL);
	if(i==IDCANCEL) return false;
	else if(i==IDNO)
	{
		manipulator.DoModal();
		g_machine->wafer_y = manipulator.position[1];
	    g_machine->wafer_x = manipulator.position[2];
	}
	
	pos[0] = pos[1] = 0.0;

	do
	{
	  conout("Setting the 1,2 axes positions");
	
	  conout("Taking a visual shot and calculating camera coordinates");
	  get_hall_coordinates(/*x*/&(pos[1]), /*y*/&(pos[0]), &angle); //bias.

	  if(fabs(pos[1]) > 0.002)
	  {
		  g_machine->wafer_y += pos[1];
	      g_Axes.InchPosition(g_machine->wafer_y, pos[1], 1); //Y
	  }

	  if(fabs(pos[0]) > 0.002)
	  {	  
		  g_machine->wafer_x += pos[0];
	      g_Axes.InchPosition(g_machine->wafer_x, pos[0], 2); //X
	  }

	}
	while(fabs(pos[1]) > 0.002 || fabs(pos[0]) > 0.002);
	

	//// WAFER POSITION READY.




	//// PLUNGE UP.

	conout("Raising the plunge up mechanism position.");
    g_io.Plunge_Up();
	Sleep(300);
	while(!g_io.Plunge_is_Up());

	if(AfxMessageBox("Please adjust plunge up mechanism position.",MB_OKCANCEL)==IDCANCEL) return false;
	conout("Plunge up mechanism position is adjusted.");

	conout("Lower the plunge up mechanism position.");
    g_io.Plunge_Dwn();
	Sleep(300);
	while(g_io.Plunge_is_Up());
	
	///// PLUNGE UP END.
	
	
	
	///// GANTRY.
	
	//remove the hall from FOV.
	g_Axes.InchPosition(g_machine->wafer_y-1.0, pos[1], 1); //Y
	//preparing the chip placement.
	if(AfxMessageBox("Please pull off the tool.",MB_OKCANCEL)==IDCANCEL) return false;
	g_Axes.InchPosition(0.0,pos[0],4);
	
	
	//feed the head with the (better totally black) chip.
	g_io.Grip_Vac_On();
	if(AfxMessageBox("Please put a chip into the Gripper.",MB_OKCANCEL)==IDCANCEL) return false;
	
	g_Axes.InchPosition(7.7,pos[0],4);
	if(AfxMessageBox("Please install the calibration tool back.",MB_OKCANCEL)==IDCANCEL) return false;
	
	
	//position pick up head above previous pick position.

	g_Axes.InchPosition(g_machine->gantry_x,pos[0],3);
    g_Axes.InchPosition(g_machine->gantry_y,pos[1],4);
	g_smac.DegreeRotate(g_machine->gantry_angle);

	
	conout("Use axeses # 3,4 to position chip.");
	
	if(AfxMessageBox("Open manipulator to position chip on the tool ?.",MB_YESNO)==IDYES)
	{
	  //bring chip.
	  manipulator.DoModal();
	}
	//preliminary positions of gantry pick.
	g_Axes.GetInchPosition(g_machine->gantry_x,3); //Gantry X
	g_Axes.GetInchPosition(g_machine->gantry_y,4); //Gantry Y
	
	
	// Put the chip onto the surface.
	glue_chip();
	
	//clear FOV.
	conout("Clearing camera # 1 FOV");
	g_Axes.InchPosition(7.0, pos[0], 4);


	conout("Taking a visual shot and calculating chip position bias");
	
	get_hall_coordinates(/*x*/&(pos[1]), /*y*/&(pos[0]), &angle); //bias.


	//get data (not corrected yet) and correct it.

	g_machine->gantry_x += pos[0];  //Gantry X
	g_machine->gantry_y += pos[1];  //Gantry Y
	g_machine->gantry_angle = g_smac.GetAnglePosition() + angle; //Pick Angle of the head.

	
	if(AfxMessageBox("Please remove upper gantry calibration tool.",MB_OKCANCEL)==IDCANCEL) return false;
	
	
	
	/////// GANTRY END.
	
	
	
	/////// CAMERA #2
	
	
	//last thing - camera # 2.


	//move head toward operator.
	g_Axes.InchPosition(0.0,pos[0],4);
	
	//feed the head with the chip.
	g_io.Grip_Vac_On();
	if(AfxMessageBox("Please put a chip into the Gripper.",MB_OKCANCEL)==IDCANCEL) return false;
	
	//prepare the conveyor and camera # 2 calibration pad.
	if(AfxMessageBox("Please install calibration pad for camera #2 .",MB_OKCANCEL)==IDCANCEL) return false;
	
	//move the chip above the FOV (#2)
	g_Axes.InchPosition(g_machine->base_x,pos[0],3);
	g_Axes.InchPosition(g_machine->base_y,pos[1],4);
	g_smac.DegreeRotate(g_machine->base_angle);
	
	
	conout("Use axeses # 3,4 to position chip.");
	if(AfxMessageBox("Open manipulator to position chip above the tool ?.",MB_YESNO)==IDYES)
	{
	  //bring chip.
	  manipulator.DoModal();
	}
    //preliminary positions of gantry place.
	g_Axes.GetInchPosition(g_machine->base_x,3);
	g_Axes.GetInchPosition(g_machine->base_y,4);
	
	// Put the chip onto the surface.
	glue_chip();

	
	//clear FOV.
	conout("Clearing camera # 2 FOV");
	g_Axes.InchPosition(3.0, pos[0], 4);


	conout("Taking a visual shot and calculating chip position bias");
	
	get_hall_coordinates_2(/*x*/&(pos[1]), /*y*/&(pos[0]), &angle); //bias.


	//get data (not corrected yet) and correct it.
	//let base_x, base_y be the gantry coordinate for centre of camera #2
	g_machine->base_x += pos[0];  //Gantry X
	g_machine->base_y += pos[1];  //Gantry Y
	g_machine->base_angle = g_smac.GetAnglePosition() + angle; //Angle of the head.
	
	if(AfxMessageBox("Please remove camera #2 calibration tool.",MB_OKCANCEL)==IDCANCEL) return false;
	
	//////// CAMERA # 2 END.

	//////// BLACK LEVEL.
	conout("Adjust black level.");
	g_machine->dBaseBlack = get_base_light_level(); //start camera # 2 process.
	///////  END BLACK LEVEL.

	//////// WHITE LEVEL.
	if(AfxMessageBox("Please put the base on the conveyor as it will appear during the process.",MB_OKCANCEL)==IDCANCEL) return false;
	conout("Adjust white level, FOV, & Packet detection.");
	
	g_machine->dBaseWhite = get_base_light_level(); //start camera # 2 process.
	///////  END WHITE LEVEL.

	
	/////// CHIP EDGE.
	
	//coordinates of the packet corner (reference point).
	if(AfxMessageBox("Please adjust the field of view and packet recognition process on the video system.",MB_OKCANCEL)==IDCANCEL) return false;
	
	conout("Get coordinates of the packet corner.");
	get_base_coordinates(&(g_machine->base_camera_x), &(g_machine->base_camera_y), &pos[0]); //better don't trust angle here.
	
	if(AfxMessageBox("Please remove everithing from the base and put the chip into the packet where the system will load it.",MB_OKCANCEL)==IDCANCEL) return false;
	conout("Find where the chip must be loaded.");
	//Chip centre.
	get_hall_coordinates_2(/*x*/&(pos[0]), /*y*/&(pos[1]), &angle); //bias.

	//Chip centre - corner centre = difference from chip to reference corner.
	g_machine->base_camera_x = pos[0] - g_machine->base_camera_x;
	g_machine->base_camera_y = pos[1] - g_machine->base_camera_y;
	g_machine->base_camera_angle = angle;


	///////  END CHIP EDGE.
	g_Axes.InchPosition(7.0, pos[0], 4);
	conout("Opening stretcher");
	g_Axes.InchPosition(0.0,pos[0],5);
	AfxMessageBox("Machine Calibration Finished. Please don't forget to save the settings file.",MB_OK);
	conout("SUCCESS - Machine Calibration Finished");
    return true;
}









int compare( const void *x, const void *y )
{
    if((*(int*)x) > (*(int*)y)) return 1;
	else if((*(int*)x) < (*(int*)y)) return -1;
	return 0;
}


bool proceed_left_edge(double camX, double camY)
{
      //Rows = RowCount/2, Cols = ColumnCount;
   	  int i;
	  int data[480];
	  double tar[9],pos[9];
      int RowCount,ColumnCount,x,y;
	  theApp.map->GetRCCount(RowCount,ColumnCount);

	  tar[1]=g_machine->wafer_y+camX-(double)((long)(ColumnCount/2))*(theApp.map->XDieSize());  
      tar[2]=g_machine->wafer_x+camY-(double)((long)( 0           ))*(theApp.map->YDieSize());
	  
	  g_io.Plunge_Dwn();
	  g_Axes.InchPosition(tar[1], 1);
      g_Axes.InchPosition(tar[2], 2);
      do {g_Axes.GetInchPosition(pos[1],1);} while(fabs(tar[1]-pos[1])>0.01);
      do {g_Axes.GetInchPosition(pos[2],2);} while(fabs(tar[2]-pos[2])>0.01);
	  g_io.Plunge_Up();
	  while(!g_io.Plunge_is_Up()) Sleep(30);

	  //find edge.
	  calculate_left_edge();
	  
	  g_io.Plunge_Dwn();

	  get_int_array(480,data);
	  
	  //check edge location.
	  y=(int)(theApp.map->YDieSize()/2.0/_CHIP_MASCHTAB_);
	  x=(int)(theApp.map->XDieSize()/2.0/_CHIP_MASCHTAB_);

	  
	  qsort( (void *)(&(data[240-y])), (size_t)(2*y), sizeof(int *), compare );
	  
	  i=240; //for(i=(240-y);i<(240+y);i++)
	  {	 
	  	 //chip is in the middle of the VOF.
		 //see if the edge passes it on the left.
		 if(data[i])
	  	  if(data[i] > ( _PLUNGE_CAM_X_ - 0.0*x))
	  	   return false;
	  }
	
	  return true;
}


bool proceed_right_edge(double camX, double camY)
{   
      // Rows = RowCount/2, Cols = 1
	  int i;
	  int data[480];
	
	  double tar[9],pos[9];
      int RowCount,ColumnCount,x,y;
	  theApp.map->GetRCCount(RowCount,ColumnCount);

	  tar[1]=g_machine->wafer_y+camX+(double)((long)((ColumnCount-1)/2))*(theApp.map->XDieSize());
	  tar[2]=g_machine->wafer_x+camY-(double)((long)(0))*(theApp.map->YDieSize()); 
	  
	  g_io.Plunge_Dwn();
	  g_Axes.InchPosition(tar[1], 1);
      g_Axes.InchPosition(tar[2], 2);
      do {g_Axes.GetInchPosition(pos[1],1);} while(fabs(tar[1]-pos[1])>0.01);
      do {g_Axes.GetInchPosition(pos[2],2);} while(fabs(tar[2]-pos[2])>0.01);
      g_io.Plunge_Up();
	  while(!g_io.Plunge_is_Up()) Sleep(30);

	  //find edge.
	  calculate_right_edge();
	  
	  g_io.Plunge_Dwn();

	  get_int_array(480,data);
	  
	  //check edge location.
  	  y=(int)(theApp.map->YDieSize()/2.0/_CHIP_MASCHTAB_);
	  x=(int)(theApp.map->XDieSize()/2.0/_CHIP_MASCHTAB_);
	  
	  qsort( (void *)(&(data[240-y])), (size_t)(2*y), sizeof(int *), compare );
	  
	  i=240;//for(i=(240-y);i<(240+y);i++)
	  {	 
	  	 //chip is in the middle of the VOF.
		 //see if the edge passes it on the right.

	  	 if(data[i])
		  if(data[i] < ( _PLUNGE_CAM_X_ + 0.0*x))
  	       return false;
	  }
	
	  return true;
}   



bool proceed_top_edge(double camX, double camY)
{      
	  //Rows = RowCount, Cols = ColumnCounts/2
   	  int i;
	  int data[640];

	  double tar[9],pos[9];
      int RowCount,ColumnCount,x,y;
	  theApp.map->GetRCCount(RowCount,ColumnCount);
      
	  tar[1]=g_machine->wafer_y+camX-(double)((long)(0))*(theApp.map->XDieSize());
	  tar[2]=tar[2]=g_machine->wafer_x+camY-(double)((long)(RowCount/2))*(theApp.map->YDieSize());
	  
	  g_io.Plunge_Dwn();
	  g_Axes.InchPosition(tar[1], 1);
      g_Axes.InchPosition(tar[2], 2);
      do {g_Axes.GetInchPosition(pos[1],1);} while(fabs(tar[1]-pos[1])>0.01);
      do {g_Axes.GetInchPosition(pos[2],2);} while(fabs(tar[2]-pos[2])>0.01);
      g_io.Plunge_Up();
	  while(!g_io.Plunge_is_Up()) Sleep(30);

	  //find edge.
	  calculate_top_edge();
	  
	  g_io.Plunge_Dwn();
	  
	  get_int_array(640,data);
	  
	  //check edge location.
  	  y=(int)(theApp.map->YDieSize()/2.0/_CHIP_MASCHTAB_);
	  x=(int)(theApp.map->XDieSize()/2.0/_CHIP_MASCHTAB_);
	  
	  qsort( (void *)(&(data[320-x])), (size_t)(2*x), sizeof(int *), compare );
	  
	  i=320; //for(i=(320-x);i<(320+x);i++)
	  {	 
	  	 //chip is in the middle of the VOF.
		 //see if the edge passes it on the top.

	  	 if(data[i])
		  if(data[i] > ( _PLUNGE_CAM_Y_ - 0.0*y))
	  	   return false;
	  }
	
	  return true;
}   


bool proceed_bottom_edge(double camX, double camY)
{
	  //Rows = 1, Cols = ColumnCounts/2
   	  int i;
	  int data[640];
	
	  double tar[9],pos[9];
      int RowCount,ColumnCount,x,y;
	  theApp.map->GetRCCount(RowCount,ColumnCount);
      
	  tar[1]=g_machine->wafer_y+camX-(double)((long)(0))*(theApp.map->XDieSize());
      tar[2]=g_machine->wafer_x+camY+(double)((long)((RowCount-1)/2))*(theApp.map->YDieSize());
	  
	  g_io.Plunge_Dwn();
	  g_Axes.InchPosition(tar[1], 1);
      g_Axes.InchPosition(tar[2], 2);
      do {g_Axes.GetInchPosition(pos[1],1);} while(fabs(tar[1]-pos[1])>0.01);
      do {g_Axes.GetInchPosition(pos[2],2);} while(fabs(tar[2]-pos[2])>0.01);
      g_io.Plunge_Up();
      while(!g_io.Plunge_is_Up()) Sleep(30);

	  //find edge.
	  calculate_bottom_edge();

	  g_io.Plunge_Dwn();

	  get_int_array(640,data);
	  
	  //check edge location.
	  y=(int)(theApp.map->YDieSize()/2.0/_CHIP_MASCHTAB_);
	  x=(int)(theApp.map->XDieSize()/2.0/_CHIP_MASCHTAB_);

	  qsort( (void *)(&(data[320-x])), (size_t)(2*x), sizeof(int *), compare );
	  
	  i=320; //for(i=(320-x);i<(320+x);i++)
	  {	 
	  	 //chip is in the middle of the VOF.
		 //see if the edge passes it on the bottom.

	  	 if(data[i])
		  if(data[i] < ( _PLUNGE_CAM_Y_ + 0.0*y))
	  	   return false;
	  }
	
	  return true;
}   


int PreviousRo = 0;
int PreviousCo = 0;
bool chip_position_correction(int Ro, int Co, double wafer_x, double wafer_y, char *mode)
{
	static bool has_corrected_manually;
	int RoCoDistance = 1;
	
	int i;
	double camX, camY, tar[3], pos[3], corrcoefficient;

	
	
	if(strstr(mode,"Auto"))
	{
	   if(has_corrected_manually)
	   {
	     has_corrected_manually = false;
		 return true;
	   }
	}
	else
	{
	  has_corrected_manually = true;
	}
	
	
	if( strstr(mode,"Auto") && 
		(abs(Ro - PreviousRo) + abs(Co - PreviousCo)) < RoCoDistance) 
	{
	   return true;
	}


	if(!strstr(mode,"Finish"))
	{
	  //clear fov.
	  g_io.Grip_Close();
	  g_smac.InchesMove(g_machine->smac_z_clearance);		
	  g_Axes.GetInchPosition(pos[0],4);
	  if(pos[0] >= 6.0) g_Axes.InchPosition(6.0,4);

	
	  // positioning the chip.
      g_Axes.InchPosition(tar[1]=wafer_y - Co*theApp.map->XDieSize()/*Y*/, 1);
      g_Axes.InchPosition(tar[2]=wafer_x - Ro*theApp.map->YDieSize()/*X*/, 2);

      //wait for positioning complete.
	  for(i=1;i<=2;i++)
	  {
	    do{ 
	        g_Axes.GetInchPosition(pos[i],i);
		    __Warning("Waiting for lower gantry motion completion",1);
		} while(fabs(tar[i]-pos[i])>0.01);
	  }

	
	  //plunge up.
	  while(!g_io.Plunge_is_Up()) 
	  {
		  g_io.Plunge_Up();
		  __Warning("Waiting for the plunge mechanism is up",200);
	  }
	}
	
	//camera correction procedure starting here.
	chip_position_matching(mode); //////tut to do error
	
	if(strstr(mode,"Start")) return true;
	
	



	//get the correlation coefficient (here it must be [0;1])
	get_angle(&corrcoefficient);

	//check if there is a chip.
	if( strstr(mode,"Auto") && corrcoefficient < 0.8) //suspect smthng wrng.
	  return false;
	
	
	//get the point displacement.
	// corner.
	get_x(&camX);
	get_y(&camY);
		
	
	
	
	//chip center.
 	camX += (theApp.map->XDieSize()/2.0);
	camY += (theApp.map->YDieSize()/2.0);

	
	//for future using.
	set_x(camX);
	set_y(camY);

	
	if(strstr(mode,"Finish"))
	{
	
	  
	  //correct camX, camY.
	  g_machine_state.m_camX += camX;
	  g_machine_state.m_camY += camY;
	
		
	  // positioning the chip.
      g_Axes.InchPosition(tar[1]=wafer_y + camX - Co*theApp.map->XDieSize()/*Y*/, 1);
      g_Axes.InchPosition(tar[2]=wafer_x + camY - Ro*theApp.map->YDieSize()/*X*/, 2);

      //wait for positioning complete.
	  for(i=1;i<=2;i++)
	  {
	    do{ 
	        g_Axes.GetInchPosition(pos[i],i);
		    __Warning("Waiting for lower gantry motion completion",1);
		} while(fabs(tar[i]-pos[i])>0.01);
	  }
	  
	  //camera correction procedure starting here.
	  chip_position_matching("Start"); //////tut to do error
	  chip_position_matching("Finish");

	  //get the new point displacement.
	  // corner.
	  get_x(&camX);
	  get_y(&camY);
		
	  //chip center.
 	  camX += (theApp.map->XDieSize()/2.0);
	  camY += (theApp.map->YDieSize()/2.0);

	  //for future using.
	  set_x(camX);
	  set_y(camY);
	}
	
	//else
	{
	  //check if the displacement is not too far.
	  if((fabs(camX)/(theApp.map->XDieSize()) > 0.1) ||
	     (fabs(camY)/(theApp.map->YDieSize()) > 0.1)) //too far => no chip is here.
	  return false;
	
	  //correct camX, camY.
	  g_machine_state.m_camX += camX;
	  g_machine_state.m_camY += camY;
	}
    
	//get new tested position.
	PreviousRo = Ro;
	PreviousCo = Co;

	return true;
} //end chip_position_correction.



void Get_Fiducial_Point(double wafer_zero_x, double wafer_zero_y)
{
       	char str[80];

		int RowCount,ColumnCount, Ro,Co, i;
	    double X = 0.0, Y = 0.0, camX, camY, tar[3], pos[3];
		
		theApp.map->GetRCCount(RowCount,ColumnCount);
	    
		//prepare wafer cycle.   
        
		theApp.map->GetPresentLocation(Ro,Co);
	    //theApp.map->GetPresentCoordinates(X,Y);

		// 1---positioning the chip.
        //g_Axes.InchPosition(tar[1]=wafer_zero_y - Co*theApp.map->XDieSize()/*Y*/, 1);
        //g_Axes.InchPosition(tar[2]=wafer_zero_x - Ro*theApp.map->YDieSize()/*X*/, 2);

		//for(i=1;i<=2;i++)
		//{
		//	do{ 
		//		g_Axes.GetInchPosition(pos[i],i);
		//	} 
		//	while(fabs(tar[i]-pos[i])>0.01);
		//}
		
		//g_io.Plunge_Up();
		//while(!g_io.Plunge_is_Up());
		
		
		chip_position_correction(Ro,Co,wafer_zero_x,wafer_zero_y,"Start");
				
		//g_io.Needls_Up();

		__Warning("Current position is :",0);
		sprintf(str,"  Row # %d;  Column # %d", Ro,Co);
		__Warning(str,0);
		__Error("Please confirm the current chip position",0);
        
		
		chip_position_correction(Ro,Co,wafer_zero_x,wafer_zero_y,"Finish");
		//init the calculation on camera site.

		//g_io.Plunge_Dwn();
		g_io.Needls_Dwn();
		
		
		//get the point displacement.

		
	    // corner.
		//get_x(&camX);
		//get_y(&camY);
		
		/**
		//chip centre.
 		camX += (theApp.map->XDieSize()/2.0);
		camY += (theApp.map->YDieSize()/2.0);

		//correct camX, camY.
		g_machine_state.m_camX += camX;
	    g_machine_state.m_camY += camY;
		**/
		//wafer_zero_y += camX;
		//wafer_zero_x += camY;
		
		
		//go there.
		//g_Axes.InchPosition(tar[1]=wafer_zero_y  - Co*theApp.map->XDieSize()/*Y*/, 1);
        //g_Axes.InchPosition(tar[2]=wafer_zero_x  - Ro*theApp.map->YDieSize()/*X*/, 2);

		
		//check the position.
		//for(i=1;i<=2;i++)
		//{
		//	do{ 
		//		g_Axes.GetInchPosition(pos[i],i);
		//	} 
		//	while(fabs(tar[i]-pos[i])>0.01);
		//}
		
		g_io.Plunge_Up();
		while(!g_io.Plunge_is_Up());

		//AfxMessageBox("!!!!!");
		
		//g_io.Plunge_Dwn();
		
		//prepare the fiducial automaticaly.
		//chip_position_correction(Ro,Co,wafer_zero_x,wafer_zero_y,"Auto");

		
		return;

} //end of fiducial locator.



bool Wafer_Orientation()
{

    char *st = NULL;
	char str[80];
	double hinch;
	double tar[9],pos[9];
	int RowCount,ColumnCount;
	DWORD to;
	bool left,right,top,bottom;

	int i,j,ierror;
	int shift_r, shift_c;

    double camIntAngle, camXAngle, camYAngle, camX, camY, tX,tY;
	
	//clear camera # 1 fov.
	   ierror = 1022;
       st = g_Axes.InchPosition(7.0f,hinch,4);
       if(*st) goto err_out;


	   if(theApp.map == (CVikingEditorDoc *)NULL) return false;


	   theApp.map->GetRCCount(RowCount,ColumnCount);
	   sprintf(str,"Wafer: RowCount = %d, ColumnCount = %d", RowCount,ColumnCount);
	   conout(str);
	

	   /*
	   for(j=1;j<=RowCount;j++)
	   {   
		   char s[64];
		   for(int c=0;c<ColumnCount;c++)
		   {
		     switch(theApp.map->GetBincode(j,c))
			 {
		       case -1:  s[c] = '-';
			     break;
		       case 128: s[c] = '+';
			     break;
		       default:  s[c] = ' ';
			 }
		   }
	   	   s[ColumnCount] = '\0'; 
		   conout(s);
		}   
		*/

      //set mastab.
	
      theApp.map->SetConversionFactor(0.00001 * 1.023270440252/*012742282843*/, 0.00001 * 1.023270440252);
	  
	  g_io.Plunge_Up();
      
	  i=0;
	  do
	  { 
        //position wafer before exposure.

	    tar[1] = g_machine->wafer_y-(double)((long)(0)-i+2)*(theApp.map->XDieSize());
	    tar[2] = g_machine->wafer_x-(double)((long)(0)-i+2)*(theApp.map->YDieSize());
	   
	    ierror = 101;
        st = g_Axes.InchPosition(tar[1], hinch,1);
        if(*st) goto err_out;
	    ierror = 102;
        st = g_Axes.InchPosition(tar[2], hinch,2);
        if(*st) goto err_out;

	    do {g_Axes.GetInchPosition(pos[1],1);} while(fabs(tar[1]-pos[1])>0.001);
        do {g_Axes.GetInchPosition(pos[2],2);} while(fabs(tar[2]-pos[2])>0.001);
	 
	    while(!g_io.Plunge_is_Up()) conout("waiting for plunge up");


	 
	    //Get cam. # 1 (wafer) data.
		while(!VisionSystemIsStarted()) 
			__Error("There is no vision system started. Start or restart it now",0);
        set_checksum(-1.0,0.0,0.0);	//set command = -1 for camera process.
		calculate_base_coordinates(0.0, 0.0, 0.0); //start camera # 1 process.
        //Sleep(4000); ???????????????????
        /////// here we need the coordinates from the camera #1.
        get_checksum(&hinch);
        to = GetTickCount();
        while((GetTickCount()-to)<27000 && hinch < -0.2f); //we are waiting for camera rezults.
		{
     	   Sleep(100);
		   get_checksum(&hinch);		
		}
        //if(hinch > -0.2f); //general vision errror.

		//get camera coordinates.
        get_wafer_coordinates(&camX, &camY, &camIntAngle, &camXAngle, &camYAngle);
	    i++;
	  }
      while(i<=7 && fabs(camIntAngle - 90.0) > 0.4f 
	             && fabs(camXAngle -90.0) < 0.9f 
		   	     && fabs(camYAngle) < 0.9f  );
   
	  if( fabs(camIntAngle - 90.0) > 0.4f 
	      && fabs(camXAngle -90.0) < 0.9f 
		  && fabs(camYAngle) < 0.9f)
	  {
		  __Error("VISUAL SYSTEM: WAFER COORDINATES ARE INVALID",20000);
	  	  return false;
	  }
	  
      //normalization forward and back.
	  
	  //// to redo ??????????????????????????? this..
      //camX -= g_machine->wafer_camera_x; //0.341;
      //camY -= g_machine->wafer_camera_y; //0.40425;
   
	  
	  //ballanceing up.
      while(camY < (theApp.map->YDieSize()/2.0)) camY += theApp.map->YDieSize();
      while(camX < (theApp.map->XDieSize()/2.0)) camX += theApp.map->XDieSize();
   
      //ballanceing down.
      while(camY >= (theApp.map->YDieSize()/2.0)) camY -= theApp.map->YDieSize();
      while(camX >= (theApp.map->XDieSize()/2.0)) camX -= theApp.map->XDieSize();
	  

	  // centre of chip reference.
      camY += (theApp.map->YDieSize()/2.0);
      camX += (theApp.map->XDieSize()/2.0);


	  
	  /*
	  while(camY < -(theApp.map->YDieSize() * g_machine->wafer_camera_back_y)) camY += theApp.map->YDieSize();
      while(camX < -(theApp.map->XDieSize() * g_machine->wafer_camera_back_x)) camX += theApp.map->XDieSize();
   
      //ballanceing down.
      while(camY > (theApp.map->YDieSize() * g_machine->wafer_camera_forward_y)) camY -= theApp.map->YDieSize();
      while(camX > (theApp.map->XDieSize() * g_machine->wafer_camera_forward_x)) camX -= theApp.map->XDieSize();
   	  */

      /* Coordinates relation.
	          Wafer[1](X,Ax#2);
			  Viking X.
			    A
	            |
	            | 
	  CamX<-----+---->Wafer[0](Y,Ax#1);viking Y.
	            |
		        |
		        V
		       CamY
       */




	  
   
      //very important: test position wafer after exposure....

      
	  /*
	  //before.
	  
      ierror = 2001;
      st = g_Axes.InchPosition(tar[1]=g_machine->wafer_y-(double)((long)(0))*(theApp.map->XDieSize()), hinch,1);
      if(*st) goto err_out;
      ierror = 2002;
      st = g_Axes.InchPosition(tar[2]=g_machine->wafer_x-(double)((long)(0))*(theApp.map->YDieSize()), hinch,2);
      if(*st) goto err_out;
      do {g_Axes.GetInchPosition(pos[1],1);} while(fabs(tar[1]-pos[1])>0.001);
      do {g_Axes.GetInchPosition(pos[2],2);} while(fabs(tar[2]-pos[2])>0.001);
      
	  g_io.Plunge_Up();   
	  AfxMessageBox("test centre.");
	  g_io.Plunge_Dwn();

      //after - (Rows/2, Cols/2)
      ierror = 1001;
      st = g_Axes.InchPosition(tar[1]=g_machine->wafer_y+camX-(double)((long)(0 * ColumnCount/2))*(theApp.map->XDieSize()), hinch,1);
      if(*st) goto err_out;
      ierror = 1002;
      st = g_Axes.InchPosition(tar[2]=g_machine->wafer_x+camY-(double)((long)(0 * RowCount/2))*(theApp.map->YDieSize()), hinch,2);
      if(*st) goto err_out;
      do {g_Axes.GetInchPosition(pos[1],1);} while(fabs(tar[1]-pos[1])>0.001);
      do {g_Axes.GetInchPosition(pos[2],2);} while(fabs(tar[2]-pos[2])>0.001);
      
	  g_io.Plunge_Up();
	  AfxMessageBox("corrected");
	  g_io.Plunge_Dwn();

	  //end of test 
   	  */
	  if(FALSE)
	  {
   		i=7;
	    shift_c = 0;
		do
		{ i--;
	      left = proceed_left_edge(camX,camY);
		  if(!left) conout("bad left");
	      right  = proceed_right_edge(camX,camY);
		  if(!right) conout("bad right");

		  if(left && right)            j = 0;
		  else if(!left && !right)     j = 2;
		  else if(!left && right)      j= +1;
		  else if(left && !right)	   j= -1;
		  else j = 0;
			
		  if(j==2) 
		  {
			  conout("Bad left or right edge");
		  	  camX -= shift_c * theApp.map->XDieSize();
		  }
		  else
		  {
			  camX += j * theApp.map->XDieSize();
	  	      shift_c += j;
		  }
		}
		while(i && !(left&&right) && j<2 && abs(shift_c)<3);

		shift_r = 0;
		i=7;
		do
		{ i--;
	      top     = proceed_top_edge(camX,camY);
		  if(!top) conout("bad top");
	      bottom  = proceed_bottom_edge(camX,camY);
		  if(!bottom) conout("bad bottom");

		  if(top && bottom)            j = 0;
		  else if(!top && !bottom)     j = 2;
		  else if(!top && bottom)      j= -1;
		  else if(top && !bottom)	   j= +1;
		  else j = 0;
			
		  if(j==2)
		  {
			  conout("Bad top or bottom edge");
			  camY -= shift_r * theApp.map->YDieSize();
		  } 
		  else
		  {
			  camY += j * theApp.map->YDieSize();
	  	      shift_r += j;
		  }
		}
		while(i && !(top&&bottom) && j<2 && abs(shift_r)<3);
	  }

        
	  if(FALSE)
	  {
		for(i=0;i<=2;i++)
		{
		  tX = camX + ((i==2)? -1:i) * theApp.map->XDieSize();
		  for(j = 0;j<=2;j++)
		  {
			  tY = camY + ((j==2)? -1:j) * theApp.map->YDieSize(); 
		 
			 if(!(left   = proceed_left_edge(tX,tY)))   continue;
	         if(!(right  = proceed_right_edge(tX,tY)))  continue;
		     if(!(top    = proceed_top_edge(tX,tY)))    continue;
 	         bottom = proceed_bottom_edge(tX,tY);

			 if(left&&right&&top&&bottom) 		
			 {	
		       sprintf(str,"Positioning: i=%d; j=%d",i,j);
		       conout(str);
		       camX = tX;
		       camY = tY;
			   goto llabel;
			 }
		  }
		}
llabel:;
	  
	    if(!(left&&right&&top&&bottom)) 
		{	
			conout("Can not ignore this error. Wafer must be unloaded.");
			__Error("Wafer's edges do not fit (bad borders ...)",0);
			
	  		return false;
		}
	  }
	  
	  //save the rezults.
	  g_machine_state.m_camX += camX;
	  g_machine_state.m_camY += camY;
	  
	  return true;
err_out: 
	  conout("General wafer orientation error");
	  return false;
}






bool LoadWafer()	
{		
	double hinch;
	double MagazinePosition;
    
	
	//Check wafer number.
	if(g_machine_state.m_wafer_position < 1 && 
	   g_machine_state.m_wafer_position > g_machine->magazine_capacity)
       return false;
	

	//Ajust magazine position
	MagazinePosition = g_machine->magazine_origin + 
	   (g_machine_state.m_wafer_position - 1) * g_machine->magazine_increment;
	g_Axes.InchPosition(MagazinePosition,hinch,7);
	
	
	

	//Plunge up mechanism set to down.
	g_io.Plunge_Dwn();	

	//Open fetcher griper.
	g_io.Waf_Open();
	
	

	// 1) Move fetcher to magazine.
    g_Axes.InchPosition(g_machine->fetcher_magazine , hinch, 6);
		
	
	// 2) Close  fetcher gripper.
    g_io.Waf_Close();
	

	// 2**) Open the stretcher, but not as much - to keep ability to move it.
	g_Axes.InchPosition(0.1,hinch,5);
	
	// 3) Move fetcher to place.
	g_Axes.InchPosition(g_machine->fetcher_stretcher,hinch,6);
	
	

	// 4) Read bar code and Open the map file.
	if((theApp.map) != (CVikingEditorDoc *)NULL)
	{
	   delete (CVikingEditorDoc *)(theApp.map);
	}
	
	theApp.map = new CVikingEditorDoc();
	
	//save current state.

	g_machine_state.m_file_name = CString(g_barcode.Get_File_Name());
	g_machine_state.m_wafer_is_loaded = true;
	g_machine_state.m_chip_position = 1;
	g_machine_state.Save();


	if
	( g_machine_state.m_file_name == CString("<cancel>") ||
	  g_machine_state.m_file_name == CString("")         ||
	  g_machine_state.m_file_name.GetLength() < 8        ||
	  g_machine_state.m_file_name.Right(4) != CString(".VIK")
	)
	{
	    //reject the wafer.
		conout("Rejecting the wafer");

	    // ~1) Move fetcher back to magazine.
        g_Axes.InchPosition(g_machine->fetcher_magazine , hinch, 6);

		//Open fetcher griper.
	    g_io.Waf_Open();
	
		//Move fetcher to rest.
        g_Axes.InchPosition(g_machine->fetcher_clear , hinch, 6);

	 	g_machine_state.m_wafer_is_loaded = false;
	    
		if(g_machine_state.m_wafer_position >= g_machine->magazine_capacity)
		  g_machine_state.m_magazine_is_ready = true;
	    else
	      g_machine_state.m_wafer_position +=1;

	    g_machine_state.Save();

		return true;
	}



	theApp.map->OnOpenDocument(g_machine_state.m_file_name);
		
 	
	// 5) Mowe wafer gantry to pick.
	//X
	g_Axes.InchPosition(g_machine->wafer_x_pick, 2);
	//Y
	g_Axes.InchPosition(g_machine->wafer_y_pick, hinch, 1);
	

	
	// 6) Open fetcher gripper.
	g_io.Waf_Open();


	// 7) Move wafer gantry to stage.
	//Y
	g_Axes.InchPosition(g_machine->stage_wafer_y,1);
	//X
	g_Axes.InchPosition(g_machine->stage_wafer_x,hinch,2);
    

	// 8) Move fetcher to push position.		
	g_Axes.InchPosition(g_machine->fetcher_justify , hinch, 6);
	
	
	// 8') Move wafer gantry to P12+3/4,P13+3/4 position.
	//X
	g_Axes.InchPosition(g_machine->wafer_x_justify+0.75, 2);
	//Y
	g_Axes.InchPosition(g_machine->wafer_y_justify+0.75, hinch, 1);
	

	
	// 9) Move wafer gantry to justify position.
	//Y
	g_Axes.InchPosition(g_machine->wafer_y_justify, 1);
	//X
	g_Axes.InchPosition(g_machine->wafer_x_justify, hinch, 2);
	

	// 10) Close the stretcher.
	g_Axes.InchPosition(1.0,hinch,5);
	

	//Move fetcher to rest.
    g_Axes.InchPosition(g_machine->fetcher_clear , hinch, 6);
	


	g_machine_state.m_wafer_is_loaded = true;
	g_machine_state.m_chip_position = 1;
	
	g_machine_state.Save();

	
	
	return true;
}





bool UnloadWafer()	
{		
	double hinch;
	double MagazinePosition;
    
	//Check wafer number.
	if(g_machine_state.m_wafer_position < 1 && 
	   g_machine_state.m_wafer_position > g_machine->magazine_capacity)
	  return false;
	

	//Ajust magazine position
	MagazinePosition = g_machine->magazine_origin + 
	   (g_machine_state.m_wafer_position - 1) * g_machine->magazine_increment;
    g_Axes.InchPosition(MagazinePosition,7);
	
	
	// 1) Plung up mechanism set to down.
	g_io.Plunge_Dwn();	

	// 2) Open fetcher griper.
	g_io.Waf_Open();
 
	// 3) Move wafer gantry to stage.
	//X
    g_Axes.InchPosition(g_machine->stage_wafer_x,2);
	//Y
    g_Axes.InchPosition(g_machine->stage_wafer_y,hinch,1);
	
	// 4) Move fetcher to place.
	g_Axes.InchPosition(g_machine->fetcher_stretcher,hinch,6);
		
		
	// 5) Open stretcher.
	g_Axes.InchPosition(0.1,hinch,5);
	
	// 6) Mowe wafer gantry to pick.
	//X
	g_Axes.InchPosition(g_machine->wafer_x_pick, 2);
	//Y
	g_Axes.InchPosition(g_machine->wafer_y_pick, hinch, 1);
	
	
	// 7) Close  fetcher gripper.
    g_io.Waf_Close();


	// 8) Move wafer gantry to stage.
	//X
	g_Axes.InchPosition(g_machine->stage_wafer_x,2);
	//Y
	g_Axes.InchPosition(g_machine->stage_wafer_y,hinch,1);
	
	
	// 9) Move fetcher to magazine.
	g_Axes.InchPosition(g_machine->fetcher_magazine , hinch, 6);
	
	
	// 10) Open fetcher gripper.
	g_io.Waf_Open();

	// 11) Move fetcher to rest.
	g_Axes.InchPosition(g_machine->fetcher_clear , hinch, 6);
	
	
	//destroy the map information.
	if(theApp.map) 
	{
	   delete (theApp.map);
       (theApp.map) = NULL;
	}

	
	
	
	g_machine_state.m_wafer_is_loaded = false;
	
	//was it the last wafer in magazine?
	if(g_machine_state.m_wafer_position >= g_machine->magazine_capacity)
	{
		//Yes.
		g_machine_state.m_magazine_is_ready = true;
	}
	else
	{	//No.
	    g_machine_state.m_wafer_position +=1;
	}
	

	g_machine_state.Save();
	

	return true;
}






double gy_prediction = 7.0;
int pick_missing_counter = 0;


bool chip_pick_and_place(int Ro, int Co, double wafer_x,double wafer_y)
{       
		
		double tar[7],pos[7];
		char str[80];
        int i;

		BOOL base_detected = FALSE;
		int  base_counter = 0;
		double dBaseLightLevel = 0.0;
		DWORD time;




#ifndef _TEST_WAFER_ORIENTATION_		
		// move the head above the chip.
		g_Axes.InchPosition(tar[4]=g_machine->gantry_y, 4);
		g_Axes.InchPosition(tar[3]=g_machine->gantry_x, 3);
#endif			
		// positioning the chip.
        g_Axes.InchPosition(tar[1]=wafer_y - Co*theApp.map->XDieSize()/*Y*/, 1);
        g_Axes.InchPosition(tar[2]=wafer_x - Ro*theApp.map->YDieSize()/*X*/, 2);



#ifdef _TEST_WAFER_ORIENTATION_
       
		//g_io.Plunge_Up();
		//while(!g_io.Plunge_is_Up());
		//g_io.Needls_Up();
		//AfxMessageBox("See the rezults.");
		g_io.Plunge_Dwn();
		Sleep(300);
		//g_io.Needls_Dwn();
		return true;

#endif
		
		if(!g_machine_state.m_chip_is_picked_up)
		{
		  //Close the gripers.
	      g_io.Grip_Close();

	      //rotate smac head before pick up the chip.
	      g_smac.DegreeRotate(g_machine->gantry_angle);	
	
	      //Open the gripers.
	      g_io.Grip_Open();
		}
		
		
		do{ g_Axes.GetInchPosition(pos[4],4); } while(pos[4]>7.0);
		do{ g_Axes.GetInchPosition(pos[1],1); } while(fabs(tar[1]-pos[1])>0.01);
        
		//conveyor signal release.
	    g_io.Cycle_Cmp_Off();

		
		do{ g_Axes.GetInchPosition(pos[2],2); } while(fabs(tar[2]-pos[2])>0.01);		

	    
		
		if(!g_machine_state.m_chip_is_picked_up)
		{

		    //rais plunge up stage.
	        g_io.Plunge_Up();
			while(!g_io.Plunge_is_Up());

	        //Plunge up wac. on.
	        g_io.Plunge_Vac_On();
	
	        //timer.
	        Sleep(200);


	        //Needles up.
	        g_io.Needls_Up();


	        //timer.
	        Sleep((DWORD)g_machine->needle_timer);

	
	        //Griper wac.on.
	        g_io.Grip_Vac_On();
	
	        
			//final check of the upper gantry position.
			for(i=3;i<=4;i++)
			 do{ g_Axes.GetInchPosition(pos[i],i); } 
			 while(fabs(tar[i]-pos[i])>0.01);
			
			
			//push smac head.
			if(g_machine->gantry_z > 0.0) g_smac.InchesMove(g_machine->gantry_z);
			//learn the Z pozition.
			else g_machine->gantry_z = approach_the_bottom();

		    sprintf(str,"g_machine->gantry_z = %f",g_machine->gantry_z);
		    conout(str);

		    Sleep(DWORD(g_machine->smac_delay));  //par#1

		   
		   // the presence of the chip.
	       
		   time = GetTickCount();
		   while(GetTickCount()-time < 2000 && !g_io.Chip_Pres()) 
		     conout("Waiting for vacuum");	
		   
		   if(!g_io.Chip_Pres())
		   {
			   __Error("Chip is not present.",0);
		   }
		   
		   g_machine_state.m_chip_is_picked_up = true;
		   g_machine_state.Save();
		   //Close the gripers.
	       g_io.Grip_Close();
		   Sleep(DWORD(g_machine->grip_close_delay));
		}
		   
		//Pop smac head before puting chip to the base.
	    g_smac.InchesMove(g_machine->smac_z_clearance);
	    

        //Needls down.
	    g_io.Needls_Dwn();

	    //P.u. Vac. off.
	    g_io.Plunge_Vac_Off();


		/////// here we need the coordinates from the camera #2.
	    //is Conveyor ready ?

place_on_next_base:

#ifdef _CONVEYOR_		
	
		while(!g_io.Start_Cycle()) conout("Waiting for conveyor ready");
#endif		
		//Conveyor ready.
		//predicted motion toward base...
		
		
		//I've commented this line due to it spoils the shadow distribution.
		//if(base_counter == 0) g_Axes.InchPosition(gy_prediction,4);
		
		/////try to calculate base coord here.
		i = 0;
		do
		{
		  while(!VisionSystemIsStarted()) 
			 __Error("There is no vision system started. Start or restart it now",0);

		  set_checksum(-2.0,0.0,0.0);	//set command = -2 for camera process.
		  dBaseLightLevel = calculate_base_coordinates(0.0, 0.0, 0.0); //start camera # 2 process.
		
		  //check if there is a base in the FOV.
		  base_detected = (BOOL)
			  (dBaseLightLevel >= (g_machine->dBaseBlack + g_machine->dBaseWhite)/2.0);
		  i++;
		}
		while(!base_detected && i<1); 
		
		if(base_detected)
		{
		   //clear base detecting errors.
		   base_counter = 0;
		   //get camera coordinates.
		   get_base_coordinates(&camera_x, &camera_y, &camera_angle);
		   //turn them into gantry coordinates.
		   tie_base_coordinates(&camera_x, &camera_y, &camera_angle);
		
		   gy_prediction = camera_y;
	   
				
		
     	   //move to base.
		   // #4 to 0.0
		   g_Axes.InchPosition(tar[4]=camera_y, 4);
 	       // #3 to 0.3
           g_Axes.InchPosition(tar[3]=camera_x, 3);
	    

	       //Rerotate smac head before puting chip to the base.
	       g_smac.DegreeRotate(camera_angle);
		
		   //Open the gripers.
	       g_io.Grip_Open();
		
		   conout("\nApproaching base position:");
		   do{ g_Axes.GetInchPosition(pos[4],4);} while(fabs(tar[4]-pos[4])>0.1);
		   conout("Y - ready");
		   do{ g_Axes.GetInchPosition(pos[3],3);} while(fabs(tar[3]-pos[3])>0.1);
		   conout("X - ready.\n");
	
	       // push the chip to the base.
	       if(g_machine->base_z > 0.0) g_smac.InchesMove(g_machine->base_z);
		   //learn the Z pozition.
		   else g_machine->base_z = approach_the_bottom();

		   Sleep((DWORD)g_machine->smac_delay); //par#1 also.

	       //Grep. Vac. off.
	       g_io.Grip_Vac_Off();
	       //Gr. Air on and off.
	       g_io.Grip_Air_On();
	    
		   Sleep((DWORD)g_machine->grip_air_delay);	//par#2
	    
		   g_io.Grip_Air_Off();
		
		   g_smac.InchesMove(g_machine->smac_z_clearance);  //par#3 (smac_z_clearance)
		   do
		   {
		     Sleep((DWORD)g_machine->smac_delay);
			 pos[0] = g_smac.GetLinearPosition();
		   }
		   while(fabs(pos[0] - g_machine->smac_z_clearance) > 0.01);
		   
		   //Conveyor -> go!
		   g_io.Cycle_Cmp_On();
	    
		   g_machine_state.m_chip_is_picked_up = false;
		   g_machine_state.Save();

		
		}
		else  // no base detected.
		{
		   	//isn't necessare anymore (shadow distribution.)
			//g_Axes.InchPosition(7.0,pos[4],4);
			base_counter++;
			conout("There is no base on the conveyor");
			if(base_counter > 3) 
			{
				__Error("There is no base on the conveyor for 3-d time consequently.",0);
			}
		
		
		    //Conveyor -> go!
			g_io.Cycle_Cmp_Off();
			Sleep(400);
			while(!g_io.Start_Cycle()) 
			{
			   conout("Waiting for conveyor."); 
			   Sleep(400);
			}
		    Sleep(400);
			g_io.Cycle_Cmp_On();
	    
		    g_machine_state.m_chip_is_picked_up = true;
	 	    g_machine_state.Save();

			
			Sleep(400);
			g_io.Cycle_Cmp_Off();
			Sleep(400);

			goto place_on_next_base;
		}
		
		return true;
}



void Init_Cycle()
{
	double hinch;

	//Pop up the smac head.
	g_smac.InchesMove(g_machine->smac_z_clearance);
	Sleep((DWORD)g_machine->smac_delay);
	
	//Close the gripers.
	g_io.Grip_Close();
	
	//close stretcher.
	g_Axes.InchPosition(0.98, hinch,5);
}



void Reload_Magazine()
{
	  Init_Cycle();

	  //conout("LOAD NEW MAGAZINE AND PRESS RETRY OR ENTER");
	  
	  //kbHit=false;
	  //while(!(g_io.Retry_is_Pressed() || kbHit));
	  //conout("NEW MAGAZINE IS LOADED");
}





bool motor_process()
{
	int rejection_counter = 0;
	int no_next_chip_counter = 0;
	
	bool a=false,b=false;
	char str[80];
	int i;

	int ierror = 0;	  

	int Row,Col,Ro,Co;
	int RowCount = 0 ,ColumnCount = 0;

	double X,Y;
    
	double correlation_coeffitient, displacementX, displacementY;
	
	g_io.Grip_Close();
	Sleep(300);
	g_smac.Homming();
	Sleep(300);
	g_smac.InchesMove(0.0);
	Sleep(200);
	while(g_smac.GetLinearPosition() > 0.1) 
	{
		conout("Waiting for the head.");
		Sleep(200);
	}


	while(!VisionSystemIsStarted()) 
		__Error("There is no vision system started. Start or restart it now",0);



	while(1)
	{
	  
	  if(g_machine_state.m_magazine_is_ready) 
	  {	  
		  Reload_Magazine();
		  __Error("Magazine is empty",0);
		 		  
          g_machine_state.m_magazine_is_ready = false;
	      g_machine_state.m_wafer_is_loaded = false;
	      g_machine_state.m_wafer_position = 1;
	      g_machine_state.m_chip_position = 1;
	      g_machine_state.Save();
	  }	
	  
	

	  if(g_machine_state.m_wafer_is_loaded == TRUE)
	  {
	    if((theApp.map) != (CVikingEditorDoc *)NULL)
		{
	      delete (CVikingEditorDoc *)(theApp.map);
		}
	    theApp.map = new CVikingEditorDoc();				  
		
		if(FileExists((char*)(LPCTSTR)g_machine_state.m_file_name))
		{
		  theApp.map->OnOpenDocument(g_machine_state.m_file_name);
		  conout("WAFER HAS BEEN LOADED");
		  a = true;
		}
		else
		{
		   a = false;
		   conout("WAFER HAS BEEN LOADED, BUT NOT IDENTIFIED");
		   conout("THE AMBIGUITY IS FOUND.");
		   conout("PROCESS IS STOPED AUTOMATICLY.");
		   __AbsoluteError("Wafer has been loaded, but there is no map file information stored for it.");
		}
	  }
	  else if(g_machine_state.m_wafer_is_loaded ==false)
	  {	
		  conout("LOADING NEW WAFER");
		  LoadWafer();
		  if(g_machine_state.m_wafer_is_loaded == TRUE)
		  {
			  conout("NEW WAFER IS LOADED");
			  rejection_counter = 0;
			  a = true;
		  }
		  else
		  {	 
			 conout("New wafer is rejected"); 
			 if((++rejection_counter)>2)
			 {
			   __Error("All last 3 wafers are rejected",0);
			   rejection_counter = 0;
			 }
			 
			 a = false;
		  }
	  }	  

	
	  if(a)
	  {
	    conout("STARTING WAFER ORIENTING");
	  	
	    if(b = Wafer_Orientation()) conout("WAFER IS ORIENTED");
	    else
		{
	  	  conout("WAFER IS NOT ORIENTED");
		}  

		
	  }

      
	  if(a && b && g_machine_state.m_wafer_is_loaded)
	  {				 
		
		theApp.map->GetRCCount(RowCount,ColumnCount);
	    
		conout("GO TO FIRST CHIP LOCATION");
		//prepare wafer cycle.   
        theApp.map->FirstLocation();
        


		//skip some...
		for(i=1;i<(int)g_machine_state.m_chip_position;i++)
		{
			theApp.map->NextLocation();
			conout("Skiping already loaded chips");
		}
		

		
		
	  
	    Get_Fiducial_Point(
	       g_machine->wafer_x + 
		   (double)((long)(RowCount/2))*(theApp.map->YDieSize()) + 
		   g_machine_state.m_camY /*x*/,

		   g_machine->wafer_y + 
		   (double)((long)(ColumnCount/2))*(theApp.map->XDieSize()) +
		   g_machine_state.m_camX /*y*/
		 
		  );

		
		
		
		conout("START PICK-PLACE CYCLE");
		do
		{
		  theApp.map->GetPresentLocation(Ro,Co);

		  if(true || theApp.map->GetBincode(Ro,Co) == 128)
		  {
		    theApp.map->GetPresentCoordinates(X,Y);
		    //see coordinates relations.....
		    
#ifdef _PRINCETON_	
			
		
		//provide that chip is on the of the pick position.

		//get the point displacement.
		

		
	    if(	
			chip_position_correction(Ro, Co, 
			
			g_machine->wafer_x + 
			(double)((long)(RowCount/2))*(theApp.map->YDieSize()) + 
		    g_machine_state.m_camY, //x


		    g_machine->wafer_y + 
			(double)((long)(ColumnCount/2))*(theApp.map->XDieSize()) +
		    g_machine_state.m_camX, //y
		
			"Auto")	
		)
		
		
		{
		  
		  //pick up the chip.		
		  chip_pick_and_place( Ro, Co,
			    
			g_machine->wafer_x + 
			(double)((long)(RowCount/2))*(theApp.map->YDieSize()) + 
		    g_machine_state.m_camY /*x*/,


		    g_machine->wafer_y + 
			(double)((long)(ColumnCount/2))*(theApp.map->XDieSize()) +
		    g_machine_state.m_camX /*y*/
			
			);

		    no_next_chip_counter = 0;
		}
		// looks like there is no chip here ...
		else 
		{
			get_angle(&correlation_coeffitient);
			get_x(&displacementX);
			get_y(&displacementY);
			
			__Warning("No chip in the position.",0);
			sprintf(str,"Correlation = %f",correlation_coeffitient);
            __Warning(str,0);
			sprintf(str,"Displacement: X = %f, Y = %f",displacementX,displacementY);
            __Warning(str,0);

			no_next_chip_counter++;

			if(no_next_chip_counter >= 3)
			{
			   chip_position_correction(Ro, Co, 
			
			     g_machine->wafer_x + 
			     (double)((long)(RowCount/2))*(theApp.map->YDieSize()) + 
		         g_machine_state.m_camY /*x*/,

				 g_machine->wafer_y + 
			     (double)((long)(ColumnCount/2))*(theApp.map->XDieSize()) +
		         g_machine_state.m_camX /*y*/,
				 
				 "Start");
				
			   __Error("There is no chip for 3 times",0);

			   chip_position_correction(Ro, Co, 
			     
			     g_machine->wafer_x + 
			     (double)((long)(RowCount/2))*(theApp.map->YDieSize()) + 
		         g_machine_state.m_camY /*x*/,

				 g_machine->wafer_y + 
			     (double)((long)(ColumnCount/2))*(theApp.map->XDieSize()) +
		         g_machine_state.m_camX /*y*/,
				 
				 "Finish");
			}
		}


#endif
            sprintf(str,"position=%d -> Row=%d, Col=%d",g_machine_state.m_chip_position,Ro,Co);
            conout(str);
		  }
	
	      g_machine_state.m_chip_position += 1;
	      g_machine_state.Save();

	      //check if it was the last position
	      theApp.map->NextLocation();
          theApp.map->GetPresentLocation(Row,Col);

		} while(!(Ro == Row && Co == Col)); //Last position reached.
	    
		if(a && b) conout("LAST CHIP HAS BEEN PROCEED");
		
		if(a)
		{
		  conout("WAFER UNLOADING");
		  UnloadWafer();
		  conout("WAFER IS UNLOADED");
		}
	  }
	}

    return true;
}	
	
		


//void cpres()
//{
//  while(true)
//  {
//	if(io.Chip_Pres()) printf("+");
//	else printf("-");
//  }
//}


bool AskHome()
{
  return AfxMessageBox("Home motors?",MB_YESNO) == IDYES;
}


bool AskOpenStretcher()
{
  return AfxMessageBox("Open Stretcher?",MB_YESNO) == IDYES;
}


void test_all_axes()
{   
	int i,j;
	double tar[2][9],pos[9];

	g_Axes.Homming(AskHome());
	
	for(i=1;i<=7;i++) g_Axes.ZeroTorque(i);
	AfxMessageBox("Bring axes to the initial position and press OK");
	
	for(i=1;i<=7;i++) g_Axes.GetInchPosition(tar[0][i],i);
    
	AfxMessageBox("Bring axes to the final position and press OK");
    
	for(i=1;i<=7;i++) g_Axes.GetInchPosition(tar[1][i],i);

	while(1)
	for(i=0;i<2;i++)
	  for(j=1;j<=7;j++) 
		g_Axes.InchPosition(tar[i][j], pos[j],j);
}


void test_axeses()
{   
	char *st;
	char s[80];

	int i;
	double tar,/*sour,*/hinch;
	DWORD time;


	tar = 0.0;
	for(i=0;i<50;i++)
	{
	  if(tar > 0.0) tar = 0.0;
	  else tar = 7.0;

	  
	  //g_Cmnds.Set_command_mode();

	  //conout("cycle\n");
	  time = GetTickCount();
	  st = g_Axes.InchPosition(tar, hinch,4);
	  time = GetTickCount() - time;
	  sprintf(s,"Command sent in %d ms\n",time);
	  conout(s);
	  
	  //Sleep(350);
	  
	  //g_Cmnds.Set_query_mode();
	  /*
	  do
	  { 
		  //time = GetTickCount();
		  g_Axes.GetInchPosition(sour,4); 
		  //time = GetTickCount() - time;
		  //sprintf(s,"Command sent in %d ms\n",(int)time);
	      //conout(s);
	  } 
	  while(fabs(tar-sour)>0.001); */
	}
}





void test_quick_axeses()
{   
	char s[80];

	int i,j;
	double tar[2][7],pos;
	DWORD time;
    
	
	conout("====MOTOR===TEST=============");
	conout("=============================");



	for(i=1;i<=7;i++) 
	{
		g_Axes.GetInchPosition(tar[0][i],i); 
		g_Axes.ZeroTorque(i);
	}
	
	conout("The positions of the motors are stored");
	conout("PLEASE CHANGE POSITIONS OF THE MOTORS AND PRESS ENTER");
	kbHit=false;
	while(!kbHit);

	for(i=1;i<=7;i++) 
	{
		g_Axes.GetInchPosition(tar[1][i],i); 
	}
	
	g_Axes.restore_torque();

	while(1)
	{
	  conout("====MOTORS===STARTING============");
	  conout("=================================");

	  for(j=0;j<=1;j++)
	  {
		  for(i=7;i>=1;i--) 
		  {
	         time = GetTickCount();
	         g_Axes.InchPosition(tar[j][i], i);
	         time = GetTickCount() - time;
	         sprintf(s,"Command sent in %d ms\n",time);
	         conout(s);
		  }  
	      
	  	  for(i=7;i>=1;i--)
		  {
			  do 
			  { 
				  g_Axes.GetInchPosition(pos,i);
			  } 
			  while(fabs(tar[j][i] - pos) > 0.1);
		  }
	  }	  
	  conout("====Finish===================");
	  conout("PRESS THE BUTTON");
  
	  while(!kbHit);
	}
}





void test_smac()
{   
	int i;
	double degree, inches;
	
	for(i=0;i<50;i++)
	{
	  //forward.
	  degree = 180.0;
	  inches = 0.25;
	  //g_smac.DegreeRotateInchesMove(degree,inches);
	  g_smac.InchesMove(inches);
	  Sleep(50);
	  
	  //back.
	  degree = 0.0;
	  inches = 0.0;
	  //g_smac.DegreeRotateInchesMove(degree,inches);
	  g_smac.InchesMove(inches);
	  Sleep(50);
	}
}

void test_gripper()
{   
	int i;
	
	DWORD time;
	char s[80];  
	
	for(i=0;i<50;i++)
	{
	  //open
	  time = GetTickCount();
	  g_io.Grip_Open();
	  while(g_io.Grip_is_Closed() && !g_io.Grip_is_Open());
	  time = GetTickCount() - time;
	  sprintf(s,"opened in %d ms\n",time);
	  conout(s);

	  Sleep(200);
	  
	  
	  //close
	  time = GetTickCount();
	  g_io.Grip_Close();
	  while(g_io.Grip_is_Open() && !g_io.Grip_is_Closed());
  	  time = GetTickCount() - time;
	  sprintf(s,"closed in %d ms\n",time);
	  conout(s);

	  Sleep(200);
	}
}


void test_all_together()
{   
	char *st;
	char s[80];

	int i;
	double tar,/*sour,*/hinch;
	double degree, inches;
	DWORD time;

	tar = 0.0;
	for(i=0;i<5000;i++)
	{
	  if(tar > 0.0) tar = 0.0;
	  else tar = 7.0;

	  
	  //g_Cmnds.Set_command_mode();

	  conout("cycle\n");
	  time = GetTickCount();
	  st = g_Axes.InchPosition(tar, hinch,4);
	  time = GetTickCount() - time;
	  sprintf(s,"Command sent in %d ms\n",time);
	  conout(s);
	  
	  Sleep(350);
	  
	  //g_Cmnds.Set_query_mode();
	  /*
	  do
	  { 
		  //time = GetTickCount();
		  g_Axes.GetInchPosition(sour,4); 
		  //time = GetTickCount() - time;
		  //sprintf(s,"Command sent in %d ms\n",(int)time);
	      //conout(s);
	  } 
	  while(fabs(tar-sour)>0.001); */

	
	//head forward.
	  degree = 180.0;
	  inches = 0.25;
	  //g_smac.DegreeRotateInchesMove(degree,inches);
	  g_smac.InchesMove(inches);

	  Sleep(10);

  	  g_io.Grip_Open();
	  g_io.Grip_Vac_On();
	  while(g_io.Grip_is_Closed() && !g_io.Grip_is_Open());

	  Sleep(40);

   	  g_io.Grip_Close();
	  while(!g_io.Grip_is_Closed() && g_io.Grip_is_Open());
	  g_io.Grip_Vac_Off();

	  //head back.
	  degree = 0.0;
	  inches = 0.0;
	  //g_smac.DegreeRotateInchesMove(degree,inches);
	  g_smac.InchesMove(inches);
	  Sleep(50);

	}
}


void test_grip_vac()
{   
	int i;
	
	DWORD time;
	char s[80];  
	
	for(i=0;i<50;i++)
	{
	  //vac. on.
	  time = GetTickCount();
	  g_io.Grip_Vac_On();
	  while(!g_io.Chip_Pres());
	  time = GetTickCount() - time;
	  sprintf(s,"vac on in %d ms\n",time);
	  conout(s);
	  Sleep(200);
	  
	  
	  //close
	  time = GetTickCount();
	  g_io.Grip_Vac_Off();
	  while(g_io.Chip_Pres());
  	  time = GetTickCount() - time;
	  sprintf(s,"vac off in %d ms\n",time);
	  conout(s);

	  Sleep(200);
	}
}


void test_conveyor()
{
    int i;	
	
	for(i=0;i<50;i++)
	{
	   while(!g_io.Start_Cycle());
	        
	   g_io.Cycle_Cmp_On();

	   Sleep(600);

	   g_io.Cycle_Cmp_Off();

	   Sleep(10);
	}

	i = 0;
}

//prototype.
//void test_camera_process_hard();

void proc_supervisor()
{
  double h;
  if(!g_io.Init()) 
  {	  
	 __AbsoluteError("IO devices are not initialized.");
  }
  

  //test_camera_process_hard();


  //g_io.Homming();
  //g_smac.Homming();
  
  //pick up the parameters.
  g_Axes.Homming(FALSE);
  
  //g_Axes.Homming(AskHome());

  
  //if(AskOpenStretcher()) 
  //{	  g_Axes.InchPosition(0.0,h,5);
  //    AfxMessageBox("Enter the wafer");
  //}
  
  //close stretcher.
  g_Axes.InchPosition(1.0,h,5);

  
  g_io.Plunge_Dwn();
  //adjust();
  
  
  
  
  ////test blocks.
  //test_axeses();
  //test_quick_axeses();
  //test_smac();
  //test_gripper();
  //test_grip_vac();
  //test_all_together();
  //test_conveyor();
  //test_all_axes();
  ////end test blocks.

  conout("Machine is started succesfully");

  motor_process();
  //_endthread();
}



/*
void test_camera_process_hard()
{
     
	 double hinch;
	 DWORD to;
	 double camX,camY,camIntAngle,camXAngle,camYAngle;
	 char str[256];

	 while(!_kbhit())
	 {
	   //Get cam. # 1 (wafer) data.
       set_checksum(-1.0,0.0,0.0);	//set command = -1 for camera process.
       calculate_base_coordinates(0.0, 0.0, 0.0); //start camera # 1 process.
       Sleep(8000);
       /////// here we need the coordinates from the camera #1.
       get_checksum(&hinch);
       to = GetTickCount();
       while((GetTickCount()-to)<27000 && hinch < -0.2f); //we are waiting for camera rezults.
	   {
          Sleep(100);
		  get_checksum(&hinch);		
	   }
       //get camera coordinates.
       get_wafer_coordinates(&camX, &camY, &camIntAngle, &camXAngle, &camYAngle);
	   //show them on the console.
	   sprintf(str,"camera#1:x=%f y=%f a=%f, ax=%f, ay=%f\n",(float)camX,(float)camY,(float)camIntAngle,(float)camXAngle,(float)camYAngle);
	   conout(str);



	   //camera # 2.
	   set_checksum(-2.0,0.0,0.0);	//set command = -2 for camera process.
	   calculate_base_coordinates(0.0, 0.0, 0.0); //start camera # 2 process.

	   Sleep(2000);
	 
	   get_checksum(&hinch);
	   to = GetTickCount();
	   while(abs(GetTickCount()-to)<7000 && hinch < -0.2f); //we are waiting for camera rezults.
	   {
	   	  Sleep(300);
		  get_checksum(&hinch);		
	   }
  	   //get camera coordinates.
	   get_base_coordinates(&camX, &camY, &camIntAngle);
       //show them on the console.
	   sprintf(str,"camera#2:x=%f y=%f a=%f\n",(float)camX,(float)camY,(float)camIntAngle);
	   conout(str);
	 }
}
*/