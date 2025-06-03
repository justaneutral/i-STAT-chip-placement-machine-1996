// machineSettings.h: interface for the machineSettings class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MACHINESETTINGS_H__7CFEAF22_BE70_11D1_8C36_00C04FAD4034__INCLUDED_)
#define AFX_MACHINESETTINGS_H__7CFEAF22_BE70_11D1_8C36_00C04FAD4034__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class machineSettings  
{
public:
	machineSettings();
	~machineSettings();

	bool update();
	bool recreate();
	bool save();
	void init();
	void extract(char * name, double & parameter);
    void extract(char * name, int & parameter);
    void extract(char * name, bool & parameter);

	char str_buffer[80];

public:

	double	velocity[9];
	double	torque[9];
	double	acceleration[9];

	double home_velocity[9];
	double home_acceleration[9];
	double home_torque[9];

	double home_distance[9];
	double home_tolerance[9];
	
	double home_correction_start[9];
	double home_correction_end[9];

	double gantry_x,gantry_y,gantry_angle,gantry_z;
	double base_x,base_y,base_angle, base_z;
	
	double wafer_x,wafer_y;
	double wafer_delta_x,wafer_delta_y;

	double base_camera_x,base_camera_y,base_camera_angle;
	double wafer_camera_x,wafer_camera_y,wafer_camera_angle;
    double wafer_camera_back_y,wafer_camera_back_x;
    double wafer_camera_forward_y,wafer_camera_forward_x;

	double smac_z_clearance;
	double smac_delay;
	double grip_air_delay;
	double grip_close_delay;

	double needle_timer;
    
	double stage_wafer_x;
    double stage_wafer_y;
    double fetcher_stretcher;
    double wafer_x_pick;
    double wafer_y_pick;
    double fetcher_magazine;
    double fetcher_clear;
    double magazine_increment;
    double magazine_origin;
    double magazine_capacity;
    double fetcher_justify;
    double wafer_x_justify;
    double wafer_y_justify;

    int bay_number;
	BOOL bay_active;


	double dBaseBlack;
	double dBaseWhite;

	FILE    *settingsfile;
};

#endif // !defined(AFX_MACHINESETTINGS_H__7CFEAF22_BE70_11D1_8C36_00C04FAD4034__INCLUDED_)
