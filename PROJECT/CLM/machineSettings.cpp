// machineSettings.cpp: implementation of the machineSettings class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "app.h"
#include "machineSettings.h"
#include <assert.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//methods.

void machineSettings::init()
{
	  int i = 0;

	  //initial and worst case.

	  velocity[0] =    500000000.0;
      acceleration[0] =	   100000.0;
      torque[0] =		 8000.0;

	  velocity[1] =    300000000.0;
      acceleration[1] =	   100000.0;
      torque[1] =	     7000.0;

	  velocity[2] =    950000000.0;
      acceleration[2] =	   200000.0;
      torque[2] =		17000.0;
	
	  velocity[3] =    600000000.0;
      acceleration[3] =	   700000.0;
      torque[3] =		28000.0;
	
	  velocity[4] =    200000000.0;
      acceleration[4] =	   100000.0;
      torque[4] =		 8000.0;
	
	  velocity[5] =   2000000000.0;
      acceleration[5] =	   100000.0;
      torque[5] =	     6000.0;
	
	  velocity[6] =    300000000.0; 
      acceleration[6] =	   100000.0;
      torque[6] =		18000.0;
	
	  velocity[7] =      1000000.0;
      acceleration[7] =	  1000000.0;
      torque[7] =		 5000.0;
	
	  velocity[8] =      1000000.0;
      acceleration[8] =	   500000.0;
      torque[8] =		32767.0;
	

	  //homming cycle data.
	  home_velocity[0] = 300000000.0;
      home_acceleration[0] =	   100000.0;
      home_torque[0] =		 7000.0;
	  home_correction_start[0] = 0.0;
	  home_correction_end[0] = 0.0;
	  home_distance[0] = 0.0;
	  home_tolerance[0] = 1.0;

	  
	  home_velocity[1] = 100000000.0;
      home_acceleration[1] =	   100000.0;
      home_torque[1] =	     8000.0;
	  home_correction_start[1] = 0.0;
	  home_correction_end[1] = 0.0;
	  home_distance[1] = 0.0;
	  home_tolerance[1] = 1.0;

	  home_velocity[2] = 100000000.0;
      home_acceleration[2] =	   100000.0;
      home_torque[2] =		12000.0;
	  home_correction_start[2] = 0.0;
	  home_correction_end[2] = 0.0;
	  home_distance[2] = 0.0;
	  home_tolerance[2] = 1.0;
	
	  home_velocity[3] = 100000000.0;
      home_acceleration[3] =	   100000.0;
      home_torque[3] =		6000.0;
	  home_correction_start[3] = 0.0;
	  home_correction_end[3] = 0.0;
	  home_distance[3] = 0.0;
	  home_tolerance[3] = 1.0;
	
	  home_velocity[4] = 100000000.0;
      home_acceleration[4] =	   100000.0;
      home_torque[4] =		 5000.0;
	  home_correction_start[4] = 0.0;
	  home_correction_end[4] = 0.0;
	  home_distance[4] = 0.0;
	  home_tolerance[4] = 1.0;
	
	  home_velocity[5] = 300000000.0;
      home_acceleration[5] =	   100000.0;
      home_torque[5] =	     6000.0;
	  home_correction_start[5] = 0.0;
	  home_correction_end[5] = 0.0;
	  home_distance[5] = 0.0;
	  home_tolerance[5] = 1.0;
	
	  home_velocity[6] = 300000000.0; 
      home_acceleration[6] =	   100000.0;
      home_torque[6] =		12000.0;
	  home_correction_start[6] = 0.0;
	  home_correction_end[6] = 0.0;
	  home_distance[6] = 0.0;
	  home_tolerance[6] = 1.0;
	
	  home_velocity[7] =   1000000.0;
      home_acceleration[7] =	  1000000.0;
      home_torque[7] =		 5000.0;
	  home_correction_start[7] = 0.0;
	  home_correction_end[7] = 0.0;
	  home_distance[7] = 0.0;
	  home_tolerance[7] = 1.0;
	
	  home_velocity[8] =    1000000.0;
      home_acceleration[8] =	   500000.0;
      home_torque[8] =		32767.0;
	  home_correction_start[8] = 0.0;
	  home_correction_end[8] = 0.0;
	  home_distance[8] = 0.0;
	  home_tolerance[8] = 1.0;


	  
	  //gantry pick point.
  	  gantry_x	 = 0.1046;
      gantry_y	 = 0.30;
	  gantry_angle= -8.0;
	  gantry_z	 = 0.37;

	  
	  //gantry place point.
	  base_x     = 0.27;
	  base_y     = 8.15;
	  base_angle = -3.4;
	  base_z     = 0.35;


	  //wafer reference point for { Row=0,Column=0 }.
	  wafer_x	 = 3.31395;
      wafer_y	 = 2.41465;
	  
	  wafer_delta_x	 = 0.0;
      wafer_delta_y	 = 0.0;
	  
	  
	  //base camera correction parameters.
	  base_camera_x = 1.30275;
	  base_camera_y = 0.6215;
	  base_camera_angle = - 0.009375;
	  
	  
	  //wafer camera correction parameters.	  
	  wafer_camera_x = 0.341;
      wafer_camera_y = 0.40425;
   	  wafer_camera_angle = 0.0;
      wafer_camera_back_y = 0.5;
      wafer_camera_back_x = 0.5;
      wafer_camera_forward_y = 0.5;
      wafer_camera_forward_x = 0.5;
	  
	  //pick and place process parameters.
	  
	  smac_z_clearance = 0.01;
	  smac_delay = 600;
	  grip_air_delay = 200;
	  grip_close_delay = 200;
	  needle_timer = 200;

	  
	  //fetcher sequence parameters.
	  stage_wafer_x = 0.0;
      stage_wafer_y = 0.0;
      fetcher_stretcher = 0.0;
      wafer_x_pick = 0.0;
      wafer_y_pick = 0.0;
      fetcher_magazine = 0.0;
      fetcher_clear = 0.0;
      magazine_increment = 0.0;
      magazine_origin = 0.0;
      magazine_capacity = 0.0;
      fetcher_justify = 0.0;
      wafer_x_justify = 0.0;
      wafer_y_justify = 0.0;


	  //Bay parameters.
	  bay_number = 1;
	  bay_active = false;


	  //Base light levels.
	  dBaseBlack = 0.17;
	  dBaseWhite = 0.34;

}



bool machineSettings::recreate()
{
	  init();
	  return save();
}


bool machineSettings::save()
{
	  int i;
	
	  if(settingsfile) 
	  {
		  fclose(settingsfile);
	      settingsfile = (FILE *)NULL;
	  }

	  settingsfile = fopen("../machine.set","w");
	  
	  if(settingsfile == (FILE *)NULL)
	  {
	    return false;
	  }

	  assert(settingsfile);

	  
	  for(i=0;i<9;i++)
	  {
          fprintf(settingsfile,"\nMOTOR_%c\n",(char)i+'1');
		  fprintf(settingsfile,"%f\n",velocity[i]);
		  fprintf(settingsfile,"%f\n",acceleration[i]);
		  fprintf(settingsfile,"%f\n",torque[i]);
          fprintf(settingsfile,"HOME_%c\n",(char)i+'1');
		  fprintf(settingsfile,"%f\n",home_velocity[i]);
		  fprintf(settingsfile,"%f\n",home_acceleration[i]);
		  fprintf(settingsfile,"%f\n",home_torque[i]);
		  fprintf(settingsfile,"%f\n",home_correction_start[i]);
	      fprintf(settingsfile,"%f\n",home_correction_end[i]);
	      fprintf(settingsfile,"%f\n",home_distance[i]);
		  fprintf(settingsfile,"%f\n",home_tolerance[i]);
	  }
	  

	  
  	  fprintf(settingsfile,"\n\n* Pick Point Parameters *\n");
	  fprintf(settingsfile,"\n01:GANTRY_X\n");
	  fprintf(settingsfile,"%f\n",gantry_x);
	  fprintf(settingsfile,"\n02:GANTRY_Y\n");
	  fprintf(settingsfile,"%f\n",gantry_y);
  	  fprintf(settingsfile,"\n03:GANTRY_ANGLE\n");
	  fprintf(settingsfile,"%f\n",gantry_angle);
	  fprintf(settingsfile,"\n04:GANTRY_Z\n");
	  fprintf(settingsfile,"%f\n",gantry_z);

	  
	  
	  fprintf(settingsfile,"\n\n* Place Point Parameters *\n");
	  fprintf(settingsfile,"\n05:BASE_X\n");
	  fprintf(settingsfile,"%f\n",base_x);
	  fprintf(settingsfile,"\n06:BASE_Y\n");
	  fprintf(settingsfile,"%f\n",base_y);
  	  fprintf(settingsfile,"\n07:BASE_ANGLE\n");
	  fprintf(settingsfile,"%f\n",base_angle);
	  fprintf(settingsfile,"\n08:BASE_Z\n");
	  fprintf(settingsfile,"%f\n",base_z);
	  

	  fprintf(settingsfile,"\n\n* Wafer (Row/x/=0,Column/y/=0) coordinates *\n");
	  fprintf(settingsfile,"\n09:WAFER_X\n");
	  fprintf(settingsfile,"%f\n",wafer_x/*-wafer_delta_x-(5.38935-3.31395)*/);
	  fprintf(settingsfile,"\n10:WAFER_Y\n");
	  fprintf(settingsfile,"%f\n",wafer_y/*-wafer_delta_y-(4.06355-2.41465)*/);

	  
	  fprintf(settingsfile,"\n\n* Wafer Row(delta X) and Column(delta Y) additional coordinate shifts*\n");
	  fprintf(settingsfile,"\n11:WAFER_DELTA_X\n");
	  fprintf(settingsfile,"%f\n",wafer_delta_x);
	  fprintf(settingsfile,"\n12:WAFER_DELTA_Y\n");
	  fprintf(settingsfile,"%f\n",wafer_delta_y);

	  
	  fprintf(settingsfile,"\n\n* Base Camera Correction Parameters *\n");
	  fprintf(settingsfile,"\n13:BASE_CAMERA_X\n");
	  fprintf(settingsfile,"%f\n",base_camera_x);
	  fprintf(settingsfile,"\n14:BASE_CAMERA_Y\n");
	  fprintf(settingsfile,"%f\n",base_camera_y);
	  fprintf(settingsfile,"\n15:BASE_CAMERA_ANGLE\n");
	  fprintf(settingsfile,"%f\n",base_camera_angle);


	  
	  fprintf(settingsfile,"\n\n* Wafer Camera Correction Parameters *\n");
	  fprintf(settingsfile,"\n16:WAFER_CAMERA_X\n");
	  fprintf(settingsfile,"%f\n",wafer_camera_x);
	  fprintf(settingsfile,"\n17:WAFER_CAMERA_Y\n");
	  fprintf(settingsfile,"%f\n",wafer_camera_y);
	  fprintf(settingsfile,"\n18:WAFER_CAMERA_ANGLE\n");
	  fprintf(settingsfile,"%f\n",wafer_camera_angle);
	  fprintf(settingsfile,"\n19:WAFER_CAMERA_BACK_X\n");
	  fprintf(settingsfile,"%f\n",wafer_camera_back_x);
	  fprintf(settingsfile,"\n20:WAFER_CAMERA_BACK_Y\n");
	  fprintf(settingsfile,"%f\n",wafer_camera_back_y);
	  fprintf(settingsfile,"\n21:WAFER_CAMERA_FORWARD_X\n");
	  fprintf(settingsfile,"%f\n",wafer_camera_forward_x);
	  fprintf(settingsfile,"\n22:WAFER_CAMERA_FORWARD_Y\n");
	  fprintf(settingsfile,"%f\n",wafer_camera_forward_y);
	  

	  fprintf(settingsfile,"\n\n* Pick and Place Process Parameters *\n");
	  fprintf(settingsfile,"\n23:SMAC_Z_CLEARANCE\n");
	  fprintf(settingsfile,"%f\n",smac_z_clearance);
	  fprintf(settingsfile,"\n24:SMAC_DELAY\n");
	  fprintf(settingsfile,"%f\n",smac_delay);
	  fprintf(settingsfile,"\n25:GRIP_AIR_DELAY\n");
	  fprintf(settingsfile,"%f\n",grip_air_delay);
	  fprintf(settingsfile,"\n26:GRIP_CLOSE_DELAY\n");
	  fprintf(settingsfile,"%f\n",grip_close_delay);

	  fprintf(settingsfile,"\n27:NEEDLE_TIMER\n");
	  fprintf(settingsfile,"%f\n",needle_timer);

   
	  fprintf(settingsfile,"\n\n* Fetcher Sequence Parameters *\n");
	  fprintf(settingsfile,"\n28:STAGE_WAFER_X\n");
	  fprintf(settingsfile,"%f\n", stage_wafer_x);
	  fprintf(settingsfile,"\n29:STAGE_WAFER_Y\n");
	  fprintf(settingsfile,"%f\n", stage_wafer_y);
	  fprintf(settingsfile,"\n30:FETCHER_STRETCHER\n");
	  fprintf(settingsfile,"%f\n",fetcher_stretcher);
	  fprintf(settingsfile,"\n31:WAFER_X_PICK\n");
	  fprintf(settingsfile,"%f\n", wafer_x_pick);
	  fprintf(settingsfile,"\n32:WAFER_Y_PICK\n");
	  fprintf(settingsfile,"%f\n", wafer_y_pick);
	  fprintf(settingsfile,"\n33:FETCHER_MAGAZINE\n");
	  fprintf(settingsfile,"%f\n", fetcher_magazine);
	  fprintf(settingsfile,"\n34:FETCHER_CLEAR\n");
	  fprintf(settingsfile,"%f\n", fetcher_clear);
	  fprintf(settingsfile,"\n35:MAGAZINE_INCREMENT\n");
	  fprintf(settingsfile,"%f\n", magazine_increment);
	  fprintf(settingsfile,"\n36:MAGAZINE_ORIGIN\n");
	  fprintf(settingsfile,"%f\n", magazine_origin);
	  fprintf(settingsfile,"\n37:MAGAZINE_CAPACITY\n");
	  fprintf(settingsfile,"%f\n", magazine_capacity);
	  fprintf(settingsfile,"\n38:FETCHER_JUSTIFY\n");
	  fprintf(settingsfile,"%f\n", fetcher_justify);
	  fprintf(settingsfile,"\n39:WAFER_X_JUSTIFY\n");
	  fprintf(settingsfile,"%f\n", wafer_x_justify);
	  fprintf(settingsfile,"\n40:WAFER_Y_JUSTIFY\n");
	  fprintf(settingsfile,"%f\n", wafer_y_justify);
	  fprintf(settingsfile,"\n* Bay parameters *\n");
	  fprintf(settingsfile,"\n41:BAY_NUMBER\n");
	  fprintf(settingsfile,"%d\n", bay_number);
	  fprintf(settingsfile,"\n42:BAY_ACTIVE\n");
	  fprintf(settingsfile,"%d\n", bay_active ? 1 : 0);
	  fprintf(settingsfile,"\n43:BASE_BLACK\n");
	  fprintf(settingsfile,"%f\n", dBaseBlack);
	  fprintf(settingsfile,"\n44:BASE_WHITE\n");
	  fprintf(settingsfile,"%f\n", dBaseWhite);

	  fclose(settingsfile);
	  settingsfile = (FILE *)NULL;

      return true;
}



bool machineSettings::update()
{
    int i = 0;

	init();
	
	if(settingsfile) 
	{
	  fclose(settingsfile);
	  settingsfile = (FILE *)NULL;
	}


	settingsfile = fopen("../machine.set","r+");

	if(settingsfile == (FILE *)NULL)
	{
	   return false;
	}

	assert(settingsfile);
	

	while(!feof(settingsfile))
	{
	  fscanf(settingsfile,"%s", str_buffer);
	  if(strstr(str_buffer,"MOTOR_") && *str_buffer != '*')
	  {
		i = (int)(str_buffer[6] - '1');
		fscanf(settingsfile,"%s\n",str_buffer);
		velocity[i] = atof(str_buffer);
		sprintf(str_buffer,"velocity[%d]=%f\n",i,velocity[i]);
		
		fscanf(settingsfile,"%s\n",str_buffer);
		acceleration[i] = atof(str_buffer);
		sprintf(str_buffer,"acceleration[%d]=%f\n",i,acceleration[i]);

        fscanf(settingsfile,"%s\n",str_buffer);
		torque[i] = atof(str_buffer);
		sprintf(str_buffer,"torque[%d]=%f\n",i,torque[i]);
	  }
	  
	  if(strstr(str_buffer,"HOME_") && *str_buffer != '*')
	  {
		i = (int)(str_buffer[5] - '1');
		fscanf(settingsfile,"%s\n",str_buffer);
		home_velocity[i] = atof(str_buffer);
		fscanf(settingsfile,"%s\n",str_buffer);
		home_acceleration[i] = atof(str_buffer);
        fscanf(settingsfile,"%s\n",str_buffer);
		home_torque[i] = atof(str_buffer);
        fscanf(settingsfile,"%s\n",str_buffer);
		home_correction_start[i] = atof(str_buffer);
        fscanf(settingsfile,"%s\n",str_buffer);
		home_correction_end[i] = atof(str_buffer);
        fscanf(settingsfile,"%s\n",str_buffer);
		home_distance[i] = atof(str_buffer);
        fscanf(settingsfile,"%s\n",str_buffer);
		home_tolerance[i] = atof(str_buffer);
	  }

	  
	  
      extract("01:GANTRY_X", gantry_x);
      extract("02:GANTRY_Y",gantry_y);
      extract("03:GANTRY_ANGLE",gantry_angle);
      extract("04:GANTRY_Z",gantry_z);
      extract("05:BASE_X",base_x);
      extract("06:BASE_Y",base_y);
      extract("07:BASE_ANGLE",base_angle);
      extract("08:BASE_Z",base_z);
      extract("09:WAFER_X",wafer_x);
      extract("10:WAFER_Y",wafer_y);
      extract("11:WAFER_DELTA_X",wafer_delta_x);
      extract("12:WAFER_DELTA_Y",wafer_delta_y);
      extract("13:BASE_CAMERA_X",base_camera_x);
      extract("14:BASE_CAMERA_Y",base_camera_y);
      extract("15:BASE_CAMERA_ANGLE",base_camera_angle);
      extract("16:WAFER_CAMERA_X",wafer_camera_x);
      extract("17:WAFER_CAMERA_Y",wafer_camera_y);
      extract("18:WAFER_CAMERA_ANGLE",wafer_camera_angle);
      extract("19:WAFER_CAMERA_BACK_X",wafer_camera_back_x);
      extract("20:WAFER_CAMERA_BACK_Y",wafer_camera_back_y);
      extract("21:WAFER_CAMERA_FORWARD_X",wafer_camera_forward_x);
      extract("22:WAFER_CAMERA_FORWARD_Y",wafer_camera_forward_y);
      extract("23:SMAC_Z_CLEARANCE",smac_z_clearance);
      extract("24:SMAC_DELAY",smac_delay);
      extract("25:GRIP_AIR_DELAY",grip_air_delay);
      extract("26:GRIP_CLOSE_DELAY",grip_close_delay);
      extract("27:NEEDLE_TIMER",needle_timer);

      extract("28:STAGE_WAFER_X",stage_wafer_x);
      extract("29:STAGE_WAFER_Y",stage_wafer_y);
      extract("30:FETCHER_STRETCHER",fetcher_stretcher);
      extract("31:WAFER_X_PICK",wafer_x_pick);
      extract("32:WAFER_Y_PICK",wafer_y_pick);
      extract("33:FETCHER_MAGAZINE",fetcher_magazine);
      extract("34:FETCHER_CLEAR",fetcher_clear);
      extract("35:MAGAZINE_INCREMENT",magazine_increment);
      extract("36:MAGAZINE_ORIGIN",magazine_origin);
      extract("37:MAGAZINE_CAPACITY",magazine_capacity);
      extract("38:FETCHER_JUSTIFY",fetcher_justify);
      extract("39:WAFER_X_JUSTIFY",wafer_x_justify);
      extract("40:WAFER_Y_JUSTIFY",wafer_y_justify);
	  extract("41:BAY_NUMBER",bay_number);
	  extract("42:BAY_ACTIVE",bay_active);

	  extract("43:BASE_BLACK",dBaseBlack);
	  extract("44:BASE_WHITE",dBaseWhite);
  }
	
	fclose(settingsfile);
	settingsfile = (FILE *)NULL;

	return true;
}



void machineSettings::extract(char * name, double & parameter)
{
	if(strstr(str_buffer, name) && *str_buffer != '*')
	{
	    fscanf(settingsfile,"%s\n",str_buffer);
		parameter = atof(str_buffer);
	}
}


void machineSettings::extract(char * name, int & parameter)
{
	if(strstr(str_buffer, name) && *str_buffer != '*')
	{
	    fscanf(settingsfile,"%s\n",str_buffer);
		parameter = atoi(str_buffer);
	}
}

void machineSettings::extract(char * name, bool & parameter)
{
	if(strstr(str_buffer, name) && *str_buffer != '*')
	{
	    fscanf(settingsfile,"%s\n",str_buffer);
		parameter = atoi(str_buffer) ? true : false;
	}
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

machineSettings::machineSettings()
{
  settingsfile = (FILE *)NULL;
	
  if(!update()) 
  {	
    recreate();
    update();
  }
}
	


machineSettings::~machineSettings()
{
  if(settingsfile) 
  {
    fclose(settingsfile);
	settingsfile = (FILE *)NULL;
  }
}
