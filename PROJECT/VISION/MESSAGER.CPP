#include <windows.h>    /* Required for all Windows applications. */
#include <stdio.h>

#include "commondll.h"


HWND get_hwnd()
{
  HWND hw = (HWND)NULL;
  GetSharedMem((ULONG)512, (LPVOID)&hw, (ULONG)sizeof(HWND));
  return hw;
}


HWND set_hwnd(HWND hwnd)
{
  SetSharedMem((ULONG)512, (LPVOID)&hwnd, (ULONG)sizeof(HWND));
  return get_hwnd();
}


double get_double(ULONG pos)
{
  double x = 0.0;
  GetSharedMem((ULONG)512+(ULONG)sizeof(HWND) + pos*(ULONG)sizeof(double),
                                      (LPVOID)&x,    (ULONG)sizeof(double));
  return x;
}


double set_double(double data, ULONG pos)
{
  double x = 0.0;
  SetSharedMem((ULONG)512+(ULONG)sizeof(HWND) + pos*(ULONG)sizeof(double),
	                                   (LPVOID)&data, (ULONG)sizeof(double));
  return get_double(pos);
}


double get_x()
{
  return get_double((ULONG)0);
}


double get_y()
{
  return get_double((ULONG)1);
}


double get_angle()
{
  return get_double((ULONG)2);
}


double get_checksum()
{
  return get_double((ULONG)3);
}


double set_x(double x)
{
  return set_double(x, (ULONG)0);
}


double set_y(double y)
{
  return set_double(y, (ULONG)1);
}


double set_angle(double angle)
{
  return set_double(angle, (ULONG)2);
}


double set_checksum(double x, double y, double angle)
{
  return set_double(angle+x+y, (ULONG)3);
}




#define IDM_SELECT_CAMERA_1   40001
#define IDM_SELECT_CAMERA_2   40002
#define IDM_GRABIMG              21
#define IDM_PACKET_FINDER     40006



double calculate_base_coordinates(double x, double y, double angle)
{
  if(get_checksum() < 0.0) //command is arrived...
  {
	switch((long)(get_checksum()))
	{
	   case -1 : //get the wafer data and put it to the shared mem.
		   
	   break;
	
	   case -2 : //get the base data and put it to the shared mem.
       
		//set camera # 2.
		
		PostMessage(get_hwnd(),WM_COMMAND,(WPARAM)IDM_SELECT_CAMERA_2,(LPARAM)NULL);
		//grab a picture.
		PostMessage(get_hwnd(),WM_COMMAND,(WPARAM)IDM_GRABIMG,(LPARAM)NULL);
		//make the calculations and set the data to shared mem.
		PostMessage(get_hwnd(),WM_COMMAND,(WPARAM)IDM_PACKET_FINDER,(LPARAM)NULL);

	   break;
	}
  }  

  return 0.0;
}


