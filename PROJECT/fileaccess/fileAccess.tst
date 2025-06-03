#include <afxwin.h>
#include <iostream.h>
#include <conio.h>
#include <stdio.h>


typedef struct _configuration_data_set 
{
   int ipar;
   double dpar;
   char spar[80];
} configuration_data_set;


bool read_data(char * filename, configuration_data_set * cs)
{
  CFile f;
  CFileException e;

  if(f.Open(filename, CFile::modeRead,  &e))
  {
	DWORD i,l = f.GetLength();
    i = f.Read((void *)cs,l);
    f.Close();
    return (bool)(i==l);
  }
  return false;
}


bool write_data(char * filename, configuration_data_set * cs)
{
  CFile f;
  CFileException e;
  
  if(f.Open( filename, CFile::modeCreate | CFile::modeWrite, &e ))
  {
	f.Write((void *)cs,sizeof(*cs));
    f.Flush();
	f.Close();
	return true;
  }
  return false;
}




void main(int argc, char **argv)
{
   
	
   configuration_data_set cs;
   
   if(argc > 1)
   if(argv[1][0] == 'w')
   {
   	 while(1)
	 for(cs.ipar=0;cs.ipar<1000;cs.ipar++)
	 {
		 cs.dpar = cs.ipar * 2.0;
         itoa(cs.ipar,cs.spar,10);
   
   cout << "integer= " << cs.ipar << endl \
		<< "double = " << cs.dpar << endl \
		<< "string = " << cs.spar << endl;
   
   
       while(!write_data("c:\\ff.txt", &cs)) Sleep(200);

	   Sleep(2000);
	 }
   }
   
   else
   {
	 while(1)
     {
		while(!read_data("c:\\ff.txt", &cs)) Sleep(99);
   
        cout << "integer= " << cs.ipar << endl \
		   << "double = " << cs.dpar << endl \
		   << "string = " << cs.spar << endl;
		Sleep(1000);
     }
   }	 
}
