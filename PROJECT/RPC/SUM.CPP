// This code implements the RPC server function.
// sum.cpp

#include <windows.h>
#include "sum.h"

//the real function in the camera.cpp file.
extern double camera_connector(short manipulator);

double SumUp(short sumVal)
{
  double data;
  Beep(300, 30);
  
  data = camera_connector(sumVal);

  Beep(300, 30);
  return -1.0f;
}
								