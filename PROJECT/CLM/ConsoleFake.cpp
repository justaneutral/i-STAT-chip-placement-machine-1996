// ConsoleFake.cpp: implementation of the CConsoleFake class.
//
//////////////////////////////////////////////////////////////////////

#include "globals.h"
#include "ConsoleFake.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConsoleFake::CConsoleFake()
{
	int i;
	
	for(i=0;i<11;i++) 
	{
		strcpy(outp[i],"........................................");
	}

	for(i=0;i<3;i++)
	{
		strcpy(inp[i],">");
	}
	
}

CConsoleFake::~CConsoleFake()
{

}
