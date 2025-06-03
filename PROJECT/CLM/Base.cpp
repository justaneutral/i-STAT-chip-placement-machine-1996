// Base.cpp: implementation of the Base class.
//
//////////////////////////////////////////////////////////////////////

#include ""
#include "app.h"
#include "Base.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Base::Base()
{
   
}

Base::~Base()
{

}

bool Base::save()
{
}

bool Base::retreive()
{
   char str[80];
   bf = fopen("../base.set","r");
   while(!feof(settingsfile))
   {
	  fscanf(bf,"%s", str);
	  {
		i = (int)(str[6] - '1');
		fscanf(settingsfile,"%s\n",str);
		velocity[i] = atof(str);
		fscanf(settingsfile,"%s\n",str);
		acceleration[i] = atof(str);
        fscanf(settingsfile,"%s\n",str);
		torque[i] = atof(str);
	  }

}

bool Base::restore()
{
	x = 0.27;
	y = 8.16;
	q = -4.0;
}
