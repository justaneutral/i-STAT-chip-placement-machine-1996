// Wafer.cpp: implementation of the CWafer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <fstream.h>
#include "app.h"
#include "Wafer.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
CWafer::CWafer()
{
	centre[0] = 3.3552;  //X centre.
	centre[1] = 2.18265; //Y centre.
	
	dimension[0] = ; //X chip size.
	dimension[1] = ; //Y chip size.

	maxcnt[0] = ; //X coord max number of chips.
	maxcnt[1] = ; //Y coord max number of chips.

	currindex[0] = ; //X current logical   pos.
	currindex[1] = ; //Y current logical	pos.

	currcoord[0] = ; //X current phisical	pos.
	currcoord[1] = ; //Y current phisical	pos.
}


CWafer::~CWafer()
{

}

void CWafer::setwaferstruct()
{
   ifstream waferfile;
   waferfile.open("../wafer.txt");
   if(!waferfile)
   {
   	 ofstream waferfile;
   	 if(!waferfile) 
	 {
		 AfxMessageBox("Can not create file 'wafer.txt'");
	     return;
	 }
	 waferfile << "Centre X" << endl << centre[0] << endl;
	 waferfile << "Centre Y" << endl << centre[1] << endl;
	 

	 waferfile.close();
   }



   waferfile.close();
}

/*	void CWafer::correctwafeerinfo() //tie video system information.
	void CWafer::setwaferinfo()
	void CWafer::firstpos()
	void CWafer::nextpos()
    bool CWafer::islastpos()


	double centre[2];
	double dimension[2];
	double maxcnt[2];

	int currindex[2];
	double currcoord[2];

*/