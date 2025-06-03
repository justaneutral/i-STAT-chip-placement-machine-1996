#include <stdio.h>

class Settings  
{
public:
	Settings();
	~Settings();

	bool update();
	bool save();
	bool recreate();

public:

        WORD    _STEP_;   
        WORD    _LINE_0_;
        WORD	_LINE_1_;
        WORD	_PIXEL_0_;
        WORD	_PIXEL_1_;


	FILE    *settingsfile;
};
