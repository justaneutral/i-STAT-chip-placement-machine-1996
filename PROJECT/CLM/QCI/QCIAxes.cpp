// QCIAxes.cpp: implementation of the CQCIAxes class.
//
//////////////////////////////////////////////////////////////////////
#include <assert.h>
#include "..\stdafx.h"

#include "..\globals.h"

#include <math.h>

//globals.



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQCIAxes::CQCIAxes()
{
   Init();
}

CQCIAxes::~CQCIAxes()
{

}

//methods.

char * CQCIAxes::Init()
{
    assert(&g_Cmnds);
	// Initialize Comm Device
	
	is_hommed = false;
	return g_Cmnds.Init();
}


//update home strokes.
void CQCIAxes::CollectHomeDistances()
{
	int i;
	for(i=1;i<=7;i++)
	{
	  g_machine->home_distance[i-1] = Counts2inch(i,g_home_absolute[i]);
	}
}


//check if hommed properly.
bool CQCIAxes::HommedWell()
{
	int i;
	bool b;

	b = true;
	for(i=1;i<=7;i++)
	{
	  m_hommed[i] = true;	
	  if(fabs(fabs(Counts2inch(i,g_home_absolute[i])) - fabs(g_machine->home_distance[i-1])) > fabs(g_machine->home_tolerance[i-1]))
	  {
	     m_hommed[i]=false;
		 b = false;
	  }
	}

	return b;
}



//Escape and Homming procedure.
char * CQCIAxes::Homming(bool refresh, int axis_number)
{

	if(!refresh) if(Retreve_Home_Data()) 
	{
		if(!restore_torque()) return &"torque is not restored";
		is_hommed = true;
		return &"";
	}

	int i;
	
	unsigned long home_acceleration[8];
	unsigned long home_velocity[8];
	unsigned short home_torque[8];
	
	for(i=1;i<8;i++)
	{
	   home_acceleration[i] = long(fabs(g_machine->home_acceleration[i-1]));
	   home_velocity[i] = long(fabs(g_machine->home_velocity[i-1]));
	   home_torque[i] = short(fabs(g_machine->home_torque[i-1]));
	}
	
	//rehome and resave the homminh data.
	char * st;
	long hposition;
	//clear home positions.
	for(i = 0; i < 8;i++) g_home_absolute[i] = 0;

	// 7 -> down.
	if(	axis_number == 0 || axis_number == 7)
	{
	  st = g_Cmnds.Distance(7,hposition,
		    20000000,               //d
			home_acceleration[7],   //a
			home_velocity[7],       //v
			17000,  //timeout
			home_torque[7], //torques
			home_torque[7],
			home_torque[7],
			home_torque[7],
			140,140,800); //limits.

	  if(*st) 
	  {	
		 __Error("Axis  # 7 failed to home",0);
		 return st;
	  }
	
	  st = g_Cmnds.Distance(7,hposition,
	 	    -2000,				//d
			home_acceleration[7],   //a
			home_velocity[7],       //v
			17000,  //timeout
			home_torque[7], //torques
			home_torque[7],
			home_torque[7],
			home_torque[7],
			140,140,800); //limits.

	  if(*st) 
	  {	
		__Error("Axis # 7 failed to home",0);
		return st;
	  }
	} // 7 end.

	// 5 -> toward zero degree angle.
	if(	axis_number == 0 || axis_number == 5)
	{
	  st = g_Cmnds.Distance(5,hposition,
	        20000000,				//d
			home_acceleration[5]/2,   //a
			home_velocity[5],       //v
			17000,  //timeout
			home_torque[5], //torques
			home_torque[5],
			home_torque[5],
			home_torque[5],
			40,40,800); //limits.

	  if(*st) 
	  {	
		  __Error("Axis # 5 failed to home",0);
		  return st;
	  }

	  st = g_Cmnds.Distance(5,hposition,
		    -2000,				    //d
			home_acceleration[5]*2,   //a
			home_velocity[5],       //v
			17000,  //timeout
			home_torque[5], //torques
			home_torque[5],
			home_torque[5],
			home_torque[5],
			40,40,800); //limits.

	  if(*st) 
	  {	
		__Error(" Axis # 5 failed to home",0);
		return st;
	  }
	} // 5 end.
	
	// 2 -> right.
	if(	axis_number == 0 || axis_number == 2)
	{ 
	   st = g_Cmnds.Distance(2,hposition,
		    -20000000,				//d
			home_acceleration[2],   //a
			home_velocity[2],       //v
			17000,  //timeout
			home_torque[2], //torques
			home_torque[2],
			home_torque[2],
			home_torque[2],
			140,140,800); //limits.

	   if(*st) 
	   {	
		__Error("Axis # 2 failed to home",0);
		return st;
	   }


	   st = g_Cmnds.Distance(2,hposition,
		    2000,   				//d
			home_acceleration[2],   //a
			home_velocity[2],       //v
			17000,  //timeout
			home_torque[2], //torques
			home_torque[2],
			home_torque[2],
			home_torque[2],
			140,140,800); //limits.

	   if(*st) 
	   {	
		__Error("Axis # 2 failed to home",0);
		return st;
	   }
	} // 2 end.
	
	// 6 -> Homing and goto rest = 22%
	if(	axis_number == 0 || axis_number == 6)
	{
	  st = g_Cmnds.Home(6,hposition,//2000,//gap.
		    20000000,				//d
			home_acceleration[6],   //a
			home_velocity[6],       //v
			17000,  //timeout
			home_torque[6], //torques
			home_torque[6],
			home_torque[6],
			home_torque[6],
			140,140,800); //limits.

	  if(*st) 
	  {	
		__Error(" Axis # 6 failed to home",0);
		return st;
	  }
	
	  g_home_absolute[6] = hposition;
	
	  st = g_Cmnds.Location(6,hposition,
		    (long)(0.78 * (double)hposition), //L
			home_acceleration[6],   //a
			home_velocity[6],       //v
			17000,  //timeout
			home_torque[6], //torques
			home_torque[6],
			home_torque[6],
			home_torque[6],
			140,140,800); //limits.
	  if(*st) 
	  {	
		__Error("Axis # 6 failed to home",0);
		return st;
	  }
	} // 6 end.

	// 7 -> Homing and leave in down position / rest = 0%
	if(	axis_number == 0 || axis_number == 7)
	{
	   st = g_Cmnds.Home(7,hposition,//2000,//gap.
		    20000000,				//d
			home_acceleration[7],   //a
			home_velocity[7],       //v
			17000,  //timeout
			home_torque[7], //torques
			home_torque[7],
			home_torque[7],
			home_torque[7],
			140,140,800); //limits.
	   if(*st) 
	   {	
		__Error(" Axis # 7 failed to home",0);
		return st;
	   }
	
	   g_home_absolute[7] = hposition;
	

       st = g_Cmnds.Location(7,hposition,
		    (long)(1.00 * (double)hposition), //L
			home_acceleration[7],   //a
			home_velocity[7],       //v
			17000,  //timeout
			home_torque[7], //torques
			home_torque[7],
			home_torque[7],
			home_torque[7],
			140,140,800); //limits.

	   if(*st) 
	   {	
		__Error(" Axis # 7 failed to home",0);
		return st;
	   }
	} // 7 end.
	
	// 1 -> Homing and rest = 50%
	if(	axis_number == 0 || axis_number == 1)
	{
	  st = g_Cmnds.Home(1,hposition,//2000,//gap.
		    20000000,				//d
			home_acceleration[1],   //a
			home_velocity[1],       //v
			17000,  //timeout
			home_torque[1], //torques
			home_torque[1],
			home_torque[1],
			home_torque[1],
			140,140,800); //limits.
	  if(*st) 
	  {	
		__Error("1 Axis at Bay ~ failed to home",0);
		return st;
	  }
	
	  g_home_absolute[1] = hposition;
	

      st = g_Cmnds.Location(1,hposition,
		    (long)(0.50 * (double)hposition), //L
			home_acceleration[1],   //a
			home_velocity[1],       //v
			17000,  //timeout
			home_torque[1], //torques
			home_torque[1],
			home_torque[1],
			home_torque[1],
			140,140,800); //limits.
	  if(*st) 
	  {	
		__Error("Axis # 1 failed to home",0);
		return st;
	  }
	} // 1 end.

	// 3 -> Homing and rest = 0%
	if(	axis_number == 0 || axis_number == 3)
	{
	   st = g_Cmnds.Home(3,hposition,//-800,//gap.
		    -20000000,				//d
			home_acceleration[3],   //a
			home_velocity[3],       //v
			17000,  //timeout
			home_torque[3], //torques
			home_torque[3],
			home_torque[3],
			home_torque[3],
			140,140,800); //limits.
	   if(*st) 
	   {	
		__Error("Axis # 3 failed to home",0);
		return st;
	   }
	
	   g_home_absolute[3] = hposition;
	

	   st = g_Cmnds.Location(3,hposition,
		    (long)(1.00 * (double)hposition), //L
			home_acceleration[3],   //a
			home_velocity[3],       //v
			17000,  //timeout
			home_torque[3], //torques
			home_torque[3],
			home_torque[3],
			home_torque[3],
			140,140,800); //limits.
	   if(*st) 
	   {	
		__Error("Axis # 3 failed to home",0);
		return st;
	   }
	} // 3 end.
	
	// 2 -> Homing and rest = 75%
	if(	axis_number == 0 || axis_number == 2)
	{
	   st = g_Cmnds.Home(2,hposition,//2000,//gap.
		    20000000,				//d
			home_acceleration[2],   //a
			home_velocity[2],       //v
			17000,  //timeout
			home_torque[2], //torques
			home_torque[2],
			home_torque[2],
			home_torque[2],
			140,140,800); //limits.
	   if(*st) 
	   {	
		__Error("Axis # 2 failed to home",0);
		return st;
	   }
	
	   g_home_absolute[2] = hposition;
	

       st = g_Cmnds.Location(2,hposition,
		    (long)(0.25 * (double)hposition), //L
			home_acceleration[2],   //a
			home_velocity[2],       //v
			17000,  //timeout
			home_torque[2], //torques
			home_torque[2],
			home_torque[2],
			home_torque[2],
			140,140,800); //limits.
	   if(*st) 
	   {	
		__Error("Axis # 2 failed to home",0);
		return st;
	   }
	} // 2 end.

	// 4 -> Homing and rest = 90%
	if(	axis_number == 0 || axis_number == 4)
	{
	   st = g_Cmnds.Home(4,hposition,//-1000,//gap.
		    -20000000,				//d
			home_acceleration[4],   //a
			home_velocity[4],       //v
			17000,  //timeout
			home_torque[4], //torques
			home_torque[4],
			home_torque[4],
			home_torque[4],
			140,140,800); //limits.
	   if(*st) 
	   {	
		__Error("Axis # 4 failed to home",0);
		return st;
	   }
	
	   g_home_absolute[4] = hposition;
	

       st = g_Cmnds.Location(4,hposition,
		    (long)(0.10 * (double)hposition), //d
			home_acceleration[4],   //a
			home_velocity[4],       //v
			17000,  //timeout
			home_torque[4], //torques
			home_torque[4],
			home_torque[4],
			home_torque[4],
			140,140,800); //limits.
	   if(*st) 
	   {	
		__Error("Axis # 4 failed to home",0);
		return st;
	   }
	} // 4 end.

	// 5 -> home and leave up.
	if(	axis_number == 0 || axis_number == 5)
	{
	   st = g_Cmnds.Home5(hposition,//-4000,//gap.
		    -20000000,				//d
			home_acceleration[5],   //a
			home_velocity[5],       //v
			17000,  //timeout
			home_torque[5], //torques
			home_torque[5],
			home_torque[5],
			home_torque[5],
			40,40,800); //limits.
	   if(*st) 
	   {	
		__Error("Axis 5 failed to home",0);
		return st;
	   }
	
	   g_home_absolute[5] = hposition;
	} // 5 end.
	
	
	is_hommed = true;
	if(!restore_torque() || 
	   !Save_Home_Data() || 
	   !HommedWell()) is_hommed = false;

	return &"";
}




double CQCIAxes::gear_ratio(int address)
{
   switch(address)
   {
   case 1: return 20000;
   case 2: return 20000;   
   case 3: return 40000;   
   case 4: return  5000;
   case 5: return (double)(abs(g_home_absolute[5]));
   case 6: return 20000;
   case 7: return 20000;   
   }

   return 400000.0;
}


long CQCIAxes::Inch2counts(int address, double inch)		
{
  
  long count = 0;
	
  if(g_home_absolute[address] >= 0)
  {
    count = max(0,min(g_home_absolute[address], long(inch * gear_ratio(address))));
  }
  else
  {
    count = min(0,max(g_home_absolute[address], long(-inch * gear_ratio(address))));
  }

  return count;
}	  



double CQCIAxes::Counts2inch(int axis, long count)
{

  double result = 0;

  if(g_home_absolute[axis] >= 0)
  {
    result = ((double)count) / gear_ratio(axis);
  }
  else
  {
    result = ((double)(-count)) / gear_ratio(axis);
  }

  return result;
}


char * CQCIAxes::ZeroTorque(int axis)
{
   return g_Cmnds.Motor_Torque(axis,0,0,0,0);
}

char * CQCIAxes::SetTorque(int axis)
{
   return g_Cmnds.Motor_Torque(axis,(unsigned short)g_machine->torque[axis],
	                                (unsigned short)g_machine->torque[axis],
									(unsigned short)g_machine->torque[axis],
									(unsigned short)g_machine->torque[axis]);
}


bool CQCIAxes::restore_torque(void)
{
  int i;
  char *st;

  for(i=1;i<=7;i++)
  {
     st = SetTorque(i);
	 if(*st)  return false;
  }
  return true;
}

char * CQCIAxes::GetInchPosition(double & hinch,int axis)
{

	
	char * st;
	long position;
	
	//Get the initial position position.
    st = g_Cmnds.Read_Symbolic(axis, 1, position);
    if(*st) return st;


	switch(axis)
	{
	   case 2:
	   case 3:
	   case 4:
	   case 5:
	   case 6:
	   case 7:
	   position = g_home_absolute[axis]-position;
	   break;
	}
	
	//everything is OK.
	hinch = Counts2inch(axis,position);
	return st;
}



char * CQCIAxes::InchPosition(double inch, int axis)
{
   long location;
   unsigned long acceleration, velocity;

   location = Inch2counts(axis,inch);
   if(axis!=1) location = g_home_absolute[axis] - location;
   
   acceleration = (unsigned long)(g_machine->acceleration[axis-1]);

   velocity = (unsigned long)(g_machine->velocity[axis-1]);
   
   return g_Cmnds.LAV(axis,location,acceleration,velocity);
}




char * CQCIAxes::InchPosition(double inch, double & hinch,int axis)
{
   return InchPositionEx(inch,hinch,axis, 
    			  g_machine->velocity[axis-1],
				  g_machine->acceleration[axis-1],
				  g_machine->torque[axis-1]);
}


char * CQCIAxes::InchPositionEx(double inch, double & hinch,int axis, 
				  double velocity, double acceleration, double torque)
{

	
	static char * st;
	long hposition;


	while(!is_hommed)
	{
	   __Error("Motors are not hommed. Please home.",0);
	  //st = (char *)NULL;
	  // return st;
	}

	
	long count = Inch2counts(axis,inch);
	
	//use later gear_ratio(axis);
	unsigned long c_velocity = long(fabs(velocity));
	unsigned long c_acceleration = long(fabs(acceleration));
	unsigned short c_torque = short(fabs(torque));

	switch(axis)
	{
	case 1:
	st = g_Cmnds.Location(axis,hposition,
		    count,c_acceleration,c_velocity, //d,a,v
			17000, //timeout
			c_torque,c_torque,c_torque,c_torque, //torque
			140,140,800); //limits.
	if(*st) return st;
	break;
	
	case  2:
	case  3:
	case  4:
    case  5:
	case  6:
	case  7:
    st = g_Cmnds.Location(axis,hposition,
		    g_home_absolute[axis]-count,c_acceleration,c_velocity, //d,a,v
			17000, //timeout
			c_torque,c_torque,c_torque,c_torque, //torque
			140,140,800); //limits.
	if(*st) return st;
	hposition = g_home_absolute[axis]-hposition;
	break;
	}
	
	//everything is OK.
	hinch = Counts2inch(axis,hposition);
	return st;
}



bool CQCIAxes::Save_Home_Data()
{
   FILE *f;
   /* Open for write */
   if( (f = fopen( "../qcihome.txt", "w" )) == NULL )
   {
	  conout( "The file 'qcihome.txt' was not open\n" );
	  return false;
   }

   for(int i = 1;i < 8;i++)
   {
     fprintf(f,"%d\n",g_home_absolute[i]);
   }
   
   /* Close stream */
   if( fclose( f ) ) return false;
   return true;
}



bool CQCIAxes::Retreve_Home_Data()
{
   FILE *f;
   /* Open for read */
   if( (f = fopen( "../qcihome.txt", "r" )) == NULL )
   {
	  conout( "The file 'qcihome.txt' was not open\n" );
	  return false;
   }
   
   for(int i = 1;i < 8;i++)
   {
     fscanf(f,"%d",&(g_home_absolute[i]));
   }

   
   /* Close stream */
   if( fclose( f ) ) return false;
   return true;
}
