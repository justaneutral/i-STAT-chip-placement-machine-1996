#include <windows.h>
#include "settings.h"
#include <assert.h>

//methods.


//global instance.

Settings g_settings;


bool Settings::recreate()
{
  int i = 0;

  //initial and worst case.

	  _STEP_    = 3;
	  _LINE_0_  = 100;
	  _LINE_1_  = 300;
	  _PIXEL_0_ = 20;
	  _PIXEL_1_ = 600;

  return save();
}



bool Settings::save()
{
	  
	  if(settingsfile) 
	  {
		  fclose(settingsfile);
	      settingsfile = (FILE *)NULL;
	  }

	  settingsfile = fopen("../vision.set","w");
	  
	  if(settingsfile == (FILE *)NULL)
	  {
	    return false;
	  }

	  assert(settingsfile);
	  
	  fprintf(settingsfile,"\n\n* FOV settings *\n");

	  fprintf(settingsfile,"\n_STEP_\n");
	  fprintf(settingsfile,"%d\n",_STEP_);
	  fprintf(settingsfile,"\n_LINE_0_\n");
	  fprintf(settingsfile,"%d\n",_LINE_0_);
	  fprintf(settingsfile,"\n_LINE_1_\n");
	  fprintf(settingsfile,"%d\n",_LINE_1_);
	  fprintf(settingsfile,"\n_PIXEL_0_\n");
	  fprintf(settingsfile,"%d\n",_PIXEL_0_);
	  fprintf(settingsfile,"\n_PIXEL_1_\n");
	  fprintf(settingsfile,"%d\n",_PIXEL_1_);

	  fclose(settingsfile);
	  settingsfile = (FILE *)NULL;

      return true;
}



bool Settings::update()
{
    int i = 0;
	char str[80];

	if(settingsfile) 
	{
	  fclose(settingsfile);
	  settingsfile = (FILE *)NULL;
	}


	settingsfile = fopen("../vision.set","r+");

	if(settingsfile == (FILE *)NULL)
	{
	   return false;
	}

	assert(settingsfile);

	while(!feof(settingsfile))
	{
	  fscanf(settingsfile,"%s", str);
	  if(strstr(str,"_STEP_") && *str != '*')
	  {
	    fscanf(settingsfile,"%s\n",str);
	    _STEP_ = atoi(str);
	  }

	  if(strstr(str,"_LINE_0_") && *str != '*')
	  {
	    fscanf(settingsfile,"%s\n",str);
	    _LINE_0_ = atoi(str);
	  }

	  if(strstr(str,"_LINE_1_") && *str != '*')
	  {
	    fscanf(settingsfile,"%s\n",str);
	    _LINE_1_ = atoi(str);
	  }

	  if(strstr(str,"_PIXEL_0_") && *str != '*')
	  {
	    fscanf(settingsfile,"%s\n",str);
	    _PIXEL_0_ = atoi(str);
	  }

	  if(strstr(str,"_PIXEL_1_") && *str != '*')
	  {
	    fscanf(settingsfile,"%s\n",str);
	    _PIXEL_1_ = atoi(str);
	  }
    }
	
	fclose(settingsfile);
	settingsfile = (FILE *)NULL;

	return true;
}




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Settings::Settings()
{
  settingsfile = (FILE *)NULL;
	
  if(!update()) 
  {	
    recreate();
    update();
  }
}
	


Settings::~Settings()
{
  if(settingsfile) 
  {
    fclose(settingsfile);
	settingsfile = (FILE *)NULL;
  }
}
