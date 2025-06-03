// DO NOT MODIFY - is using for other projects.

#include <windows.h>
#include <memory.h>
#include "commondll.h"

#define SHMEMSIZE 4096


#define IDM_SELECT_CAMERA_1        40001
#define IDM_SELECT_CAMERA_2        40002
#define IDM_GRABIMG                   21
#define IDM_FFT                    40003
#define IDM_PACKET_FINDER          40006
#define IDM_top_edge		       40011
#define IDM_bottom_edge		       40010
#define IDM_left_edge		       40012
#define IDM_right_edge		       40013
#define IDM_BASE_DETECTOR          40014
#define IDM_PACKET_FINDER2         40015
#define IDM_HALL_COORDINATES       40016
#define IDM_HALL_COORDINATES_2     40017
#define IDM_INITIAL_FIDUCIAL_POINT 40024






static LPVOID lpMem = NULL;


int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
  
  HANDLE hMap = NULL;
  BOOL init,ignore;
	
  switch(dwReason)
  {
  case DLL_PROCESS_ATTACH:
  //create common memory buffer.	  
	  hMap = CreateFileMapping((HANDLE)0xffffffff,
		     NULL,PAGE_READWRITE,0,SHMEMSIZE,"clmcommonmem");
	  if(hMap == NULL) return 0;
	  init = (GetLastError() != ERROR_ALREADY_EXISTS);
	  lpMem = MapViewOfFile(hMap,FILE_MAP_WRITE,0,0,0);
	  if(init) memset(lpMem,'\0',SHMEMSIZE);

  break;


  case DLL_PROCESS_DETACH:
      
	  ignore = UnmapViewOfFile(lpMem);
	  ignore = CloseHandle(hMap);
	  
  break;


  default:
  break;
  }
 
  return 1;
}


__declspec(dllexport) VOID SetSharedMem(ULONG pos, LPVOID buf, ULONG len)
{
  LPBYTE target = (LPBYTE)((ULONG)lpMem+pos); //portion of the shared mem.
  LPBYTE source = (LPBYTE)buf;				  //my data.
  
  pos = min(pos,SHMEMSIZE);
  len = min(len + pos, SHMEMSIZE) - pos; 
  while(len--)
  {
	 *target++ = *source++;
  }
}

__declspec(dllexport) VOID GetSharedMem(ULONG pos, LPVOID buf, ULONG len)
{
  LPBYTE source = (LPBYTE)((ULONG)lpMem+pos); //portion of the shared mem.
  LPBYTE target = (LPBYTE)buf;				  //my data.

  pos = min(pos,SHMEMSIZE);
  len = min(len + pos, SHMEMSIZE) - pos; 
  while(len--)
  {
	 *target++ = *source++;
  }
}




////////  camera   ///////

__declspec(dllexport) void get_hwnd(HWND *hw)
{
  GetSharedMem((ULONG)512, (LPVOID)hw, (ULONG)sizeof(HWND));
}


__declspec(dllexport) void set_hwnd(HWND hwnd)
{
  SetSharedMem((ULONG)512, (LPVOID)&hwnd, (ULONG)sizeof(HWND));
}


__declspec(dllexport) void get_double(double *x, ULONG pos)
{
  GetSharedMem((ULONG)512+(ULONG)sizeof(HWND) + pos*(ULONG)sizeof(double),
                                      (LPVOID)x,    (ULONG)sizeof(double));
}


__declspec(dllexport) void set_double(double data, ULONG pos)
{
  SetSharedMem((ULONG)512+(ULONG)sizeof(HWND) + pos*(ULONG)sizeof(double),
	                                   (LPVOID)&data, (ULONG)sizeof(double));
}


__declspec(dllexport) void get_x(double *x)
{
  get_double(x,(ULONG)0);
}


__declspec(dllexport) void get_y(double *y)
{
  get_double(y,(ULONG)1);
}


__declspec(dllexport) void get_angle(double *angle)
{
  get_double(angle,(ULONG)2);
}


__declspec(dllexport) void get_x_angle(double *x_angle)
{
  get_double(x_angle,(ULONG)4);
}


__declspec(dllexport) void get_y_angle(double *y_angle)
{
  get_double(y_angle,(ULONG)5);
}


__declspec(dllexport) void get_checksum(double *chs)
{
  get_double(chs,(ULONG)3);
}


__declspec(dllexport) void set_x(double x)
{
  set_double(x, (ULONG)0);
}


__declspec(dllexport) void set_y(double y)
{
  set_double(y, (ULONG)1);
}


__declspec(dllexport) void set_angle(double angle)
{
  set_double(angle, (ULONG)2);
}


__declspec(dllexport) void set_x_angle(double x_angle)
{
  set_double(x_angle, (ULONG)4);
}


__declspec(dllexport) void set_y_angle(double y_angle)
{
  set_double(y_angle, (ULONG)5);
}


__declspec(dllexport) void set_checksum(double x, double y, double angle)
{
  set_double(angle+x+y, (ULONG)3);
}


__declspec(dllexport) void set_int_array(int len, int *x)
{
  SetSharedMem((ULONG)512+
	           (ULONG)sizeof(HWND) + 
			   6*(ULONG)sizeof(double),
	           (LPVOID)&x, (ULONG)sizeof(int)*(ULONG)len);

}
__declspec(dllexport) void get_int_array(int len, int *x)
{
  GetSharedMem((ULONG)512+
	           (ULONG)sizeof(HWND) + 
			   6*(ULONG)sizeof(double),
	           (LPVOID)&x, (ULONG)sizeof(int)*(ULONG)len);


}


__declspec(dllexport) double calculate_base_coordinates(double x, double y, double angle)
{
  HWND hw;
  double chs;
  double level = 0.0;

  //check vision system.
  while(!VisionSystemIsStarted()) //wision system is not ready.
  {
	  MessageBox(NULL, "Start vision system and press OK", "CLM diagnostic",MB_OK); 
  }

  get_hwnd(&hw);
  get_checksum(&chs);

  if(chs < 0.0) //command is arrived...
  {
	switch((long)(chs))
	{
	   case -1 : //get the wafer data and put it to the shared mem.
		   
   		//set camera # 1.
		SendMessage(hw,WM_COMMAND,(WPARAM)IDM_SELECT_CAMERA_1,(LPARAM)NULL);
		//grab a picture.
		SendMessage(hw,WM_COMMAND,(WPARAM)IDM_GRABIMG,(LPARAM)NULL);
		//make the calculations and set the data to shared mem.
		SendMessage(hw,WM_COMMAND,(WPARAM)IDM_FFT,(LPARAM)NULL);
		//set camera # 2.
		SendMessage(hw,WM_COMMAND,(WPARAM)IDM_SELECT_CAMERA_2,(LPARAM)NULL);
   
	   break;
	
	   case -2 : //get base data and put it to the shared mem.

		//grab a picture.
		SendMessage(hw,WM_COMMAND,(WPARAM)IDM_GRABIMG,(LPARAM)NULL);
		//make the calculations and set the data to shared mem.
		SendMessage(hw,WM_COMMAND,(WPARAM)IDM_PACKET_FINDER,(LPARAM)NULL);
		get_x_angle(&level); 
	   break;

	   
	   case -3 : //get base data for base with a chip on previouse place 
		         //and put it to the shared mem.

		//grab a picture.
		SendMessage(hw,WM_COMMAND,(WPARAM)IDM_GRABIMG,(LPARAM)NULL);
		//make the calculations and set the data to shared mem.
		SendMessage(hw,WM_COMMAND,(WPARAM)IDM_PACKET_FINDER2,(LPARAM)NULL);
		get_x_angle(&level); 
	   break;
	}
  }  

  return level;
}


__declspec(dllexport) void get_hall_coordinates(double *x, double *y, double *angle)
{
  HWND hw;

  //check vision system.
  while(!VisionSystemIsStarted()) //wision system is not ready.
  {
	  MessageBox(NULL, "Start vision system and press OK", "CLM diagnostic",MB_OK); 
  }

  get_hwnd(&hw);
  
  //set camera # 1.
  SendMessage(hw,WM_COMMAND,(WPARAM)IDM_SELECT_CAMERA_1,(LPARAM)NULL);
  //grab a picture.
  SendMessage(hw,WM_COMMAND,(WPARAM)IDM_GRABIMG,(LPARAM)NULL);
  //make the calculations and set the data to shared mem.
  SendMessage(hw,WM_COMMAND,(WPARAM)IDM_HALL_COORDINATES,(LPARAM)NULL);
  //set camera # 2.
  SendMessage(hw,WM_COMMAND,(WPARAM)IDM_SELECT_CAMERA_2,(LPARAM)NULL);
   
  //x,y,angle.
  get_x(x);
  get_y(y);
  get_angle(angle);
}



__declspec(dllexport) void get_hall_coordinates_2(double *x, double *y, double *angle)
{
  HWND hw;

  //check vision system.
  while(!VisionSystemIsStarted()) //wision system is not ready.
  {
	  MessageBox(NULL, "Start vision system and press OK", "CLM diagnostic",MB_OK); 
  }

  get_hwnd(&hw);

  //set camera # 2.
  SendMessage(hw,WM_COMMAND,(WPARAM)IDM_SELECT_CAMERA_2,(LPARAM)NULL);
  //grab a picture.
  SendMessage(hw,WM_COMMAND,(WPARAM)IDM_GRABIMG,(LPARAM)NULL);
  //make the calculations and set the data to shared mem.
  SendMessage(hw,WM_COMMAND,(WPARAM)IDM_HALL_COORDINATES_2,(LPARAM)NULL);
   
  //x,y,angle.
  get_x(x);
  get_y(y);
  get_angle(angle);
}




__declspec(dllexport) void put_base_coordinates(double x, double y, double angle)
{
   set_x(x);
   set_y(y);
   set_angle(angle);
   set_checksum(x,y,angle);
}


__declspec(dllexport) void get_base_coordinates(double *x, double *y, double *angle)
{
   //double chs;

   get_x(x);
   get_y(y);
   get_angle(angle);
   //get_checksum(&chs);
}



__declspec(dllexport) void get_wafer_coordinates(double *x, double *y, double *angle, double *x_angle, double *y_angle)
{
   //double chs;

   get_x(x);
   get_y(y);
   get_angle(angle);
   get_x_angle(x_angle);
   get_y_angle(y_angle);
   //get_checksum(&chs);
}



__declspec(dllexport) void calculate_top_edge()
{
  HWND hw;
  get_hwnd(&hw);

   		//set camera # 1.
		SendMessage(hw,WM_COMMAND,(WPARAM)IDM_SELECT_CAMERA_1,(LPARAM)NULL);
		//grab a picture.
		SendMessage(hw,WM_COMMAND,(WPARAM)IDM_GRABIMG,(LPARAM)NULL);
		//make the calculations and set the data to shared mem.
		SendMessage(hw,WM_COMMAND,(WPARAM)IDM_top_edge,(LPARAM)NULL);
		//set camera # 2.
		SendMessage(hw,WM_COMMAND,(WPARAM)IDM_SELECT_CAMERA_2,(LPARAM)NULL);
}


__declspec(dllexport) void calculate_bottom_edge()
{
  HWND hw;
  get_hwnd(&hw);

   		//set camera # 1.
		SendMessage(hw,WM_COMMAND,(WPARAM)IDM_SELECT_CAMERA_1,(LPARAM)NULL);
		//grab a picture.
		SendMessage(hw,WM_COMMAND,(WPARAM)IDM_GRABIMG,(LPARAM)NULL);
		//make the calculations and set the data to shared mem.
		SendMessage(hw,WM_COMMAND,(WPARAM)IDM_bottom_edge,(LPARAM)NULL);
		//set camera # 2.
		SendMessage(hw,WM_COMMAND,(WPARAM)IDM_SELECT_CAMERA_2,(LPARAM)NULL);
}


__declspec(dllexport) void calculate_left_edge()
{
  HWND hw;
  get_hwnd(&hw);

   		//set camera # 1.
		SendMessage(hw,WM_COMMAND,(WPARAM)IDM_SELECT_CAMERA_1,(LPARAM)NULL);
		//grab a picture.
		SendMessage(hw,WM_COMMAND,(WPARAM)IDM_GRABIMG,(LPARAM)NULL);
		//make the calculations and set the data to shared mem.
		SendMessage(hw,WM_COMMAND,(WPARAM)IDM_left_edge,(LPARAM)NULL);
		//set camera # 2.
		SendMessage(hw,WM_COMMAND,(WPARAM)IDM_SELECT_CAMERA_2,(LPARAM)NULL);
}



__declspec(dllexport) void calculate_right_edge()
{
  HWND hw;
  get_hwnd(&hw);

   		//set camera # 1.
		SendMessage(hw,WM_COMMAND,(WPARAM)IDM_SELECT_CAMERA_1,(LPARAM)NULL);
		//grab a picture.
		SendMessage(hw,WM_COMMAND,(WPARAM)IDM_GRABIMG,(LPARAM)NULL);
		//make the calculations and set the data to shared mem.
		SendMessage(hw,WM_COMMAND,(WPARAM)IDM_right_edge,(LPARAM)NULL);
		//set camera # 2.
		SendMessage(hw,WM_COMMAND,(WPARAM)IDM_SELECT_CAMERA_2,(LPARAM)NULL);
}




__declspec(dllexport) double get_base_light_level()
{
  HWND hw;
  double dLevel;
  
  //check vision system.
  while(!VisionSystemIsStarted()) //wision system is not ready.
  {
	  MessageBox(NULL, "Start vision system and press OK", "CLM diagnostic",MB_OK); 
  }
  
  get_hwnd(&hw);
  
  //grab a picture.
  SendMessage(hw,WM_COMMAND,(WPARAM)IDM_GRABIMG,(LPARAM)NULL);
  //make the calculations and set the data to shared mem.
  SendMessage(hw,WM_COMMAND,(WPARAM)IDM_BASE_DETECTOR,(LPARAM)NULL);

  get_x(&dLevel);

  return dLevel;
}



__declspec(dllexport) BOOL VisionSystemIsStarted()
{
   HWND hW;
   get_hwnd(&hW);
   return (hW != NULL);
}



__declspec(dllexport) VOID set_camera(int camera)
{
	
   HWND hW;
   get_hwnd(&hW);
	
	
	switch(camera)
	{
	case 1:
		//set camera # 1.
		SendMessage(hW,WM_COMMAND,(WPARAM)IDM_SELECT_CAMERA_1,(LPARAM)NULL);
		break;
	case 2:	
		//set camera # 2.
		SendMessage(hW,WM_COMMAND,(WPARAM)IDM_SELECT_CAMERA_2,(LPARAM)NULL);
		break;
	}
}



__declspec(dllexport) void chip_position_matching(char *mode)
{
   
	HWND hW;
   	get_hwnd(&hW);
	
	//switch to camera # 1.	
	if(!strstr(mode,"Finish"))
	{ 
	  SendMessage(hW,WM_COMMAND,(WPARAM)IDM_SELECT_CAMERA_1,(LPARAM)NULL);
    
	
	//get the position of the chip in the centre of the screen.
    SendMessage(hW,WM_COMMAND,(WPARAM)IDM_INITIAL_FIDUCIAL_POINT,(LPARAM)NULL);	
	}
	//switch to camera # 2.		
	if(!strstr(mode,"Start"))
	  SendMessage(hW,WM_COMMAND,(WPARAM)IDM_SELECT_CAMERA_2,(LPARAM)NULL);
}