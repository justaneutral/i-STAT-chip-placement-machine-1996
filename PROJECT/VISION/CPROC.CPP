#include <windows.h>    /* Required for all Windows applications. */
#include <stdlib.h>     /* malloc, free */
#include <string.h>     /* strcmp */
#include <math.h>       /* atof */


#include "settings.h"
extern Settings g_settings;

void delete_g_y();
void delete_g_x();
BOOL init_g_x();
BOOL init_g_y();
void copy(double (*x)[480],double (*y)[480]);


extern "C"
{
#include "dpdefs.h"     /* XPG parameters. */
#include "dpptypes.h"   /* XPG function prototypes. */
#include "dberrors.h"   /* XPG error codes and warnings definitions. */

#include "../commondll/commondll.h"

}

#include "camera.h"      /* Specific to this program. */

//#include "mathem.h"

/**=========================================================================**/
/*
* #define constants
*/



#define _BASE_MASCHTAB_  0.001002012072435	  
                                  /* measured: 497pixels == 0.498" */

#define _CHIP_MASCHTAB_  0.00275  /*  0.00275 == 1.32 inches / 480 pixels  */
		        			      /*  0.00275 == 1.76 inches / 640 pixels  */






#define P_ARITH_ONEIMGOPS       9
#define P_ARITH_OPTYPES         2





#define ximg(x,y) bfr[479-(y)][(x)]
#define img(x,y,r,a) ximg((x)+(int)((r)*cos((a)*0.01745329251994)),(y)+(int)((r)*sin((a)*0.01745329251994)))
#define coimg(x,y,r,a,d) img((x)+(int)((d)*sin((a)*0.01745329251994)),(y)+(int)((d)*cos((a)*0.01745329251994)),(r),(a))


#define Pf(i,j) (i<480&&i>=0&&j<640&&j>=0)?((double)(BYTE)cpPC_Buffer[(i)*640+(j)]):0.0
#define Pfl(i,j) cpPC_Buffer[(max(min(479,(i)),0))*640+max(min(639,(j)),0)]
//#define Pfl(i,j) cpPC_Buffer[(i)*640+(j)]
#define Paint_Corner(i,j) for(k=(i)-3;k<=(i)+3;k++) for(l=(j)-3;l<=(j)+3;l++) if((i)>=0&&(i)<480&&(j)>=0&&(j)<640) if(k==(i)||l==(j)) Pfl(k,l)=255-Pfl(k,l);else Pfl(k,l)=128;
#define PI 3.14159265359






/**=========================================================================**/
/*
* Module global variables.
*/

static FARPROC M_lpArithProc;

static BOOL     M_bArithCancel;             /* TRUE if function cancelled. */

static CHAR    *M_pszArithImageTypes[] = {"One Image", "Two Image", ""};
static CHAR    *M_pszDigits = "0123456789."; /* Valid characters in string. */
static CHAR    *M_pszOneImgOps[] = {
                    "NEGATE", "NOT", "Arithmetic Shift", "Logical Shift",
                    "Scale & Offset", "AND with Constant", "OR with constant",
                    "XOR with constant", "NOT & XOR with constant", ""};

static FLOAT    M_fArithScale = 1.0f;   /* Scale factor for arith. */

static INT      M_iArithOperation = 0;  /* Operation to perform. */
static INT      M_iArithImageType = 0;  /* Type of image operation, single or two. */

static LONG     M_lArithOffset = 0;     /* Offset value for arith. */
static LONG     M_lArithConstant = 0;   /* Constant value for arith. */

/**============================== CODE ====================================**/


extern double (*g_x)[480];
extern double (*g_y)[480];
extern double (*g_pattern)[45];







VOID IDMSectionVision(HWND hWnd, VECTOR X, VECTOR Y)
{
  LONG lStatus;
  POINT Sizes = {640,480};
  char screen[640*480];

  lStatus = DPK_XDCGetImage(G_lGrabImage,(HVOID *)screen);
  if(lStatus < P_ERROR) return;

  DrowVector(screen, Sizes, X, Y, (char)255);
  
  lStatus = DPK_XDCPutImage(G_lGrabImage,(HVOID *)screen);
  if (lStatus == P_SUCCESS) DisplayImage (hWnd, G_lGrabImage);
}










/*============================================================================
*
*  Name        : AxesCoord
*  Author      : Paul Dourbal
*  Description : To peform coord lines calculation upon image of the wafer.
*
*  Parameters  :
*       hWnd            Handle to window.
*
*  Returns     : none
*
*===========================================================================*/


VOID AxesCoord(HWND hWnd, long* LL0,long* LL1,long* RL0,long* RL1,long* TP0,long* TP1,long* BP0,long* BP1)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    LONG    lStatus;        /* Error status. */
    INT     i;              /* Loop indice. */
    INT     j;              /* Loop indice. */
	

	
	
	//statistic parameters.
	double x,y;
	double dAccumulator;
	double dMinimumX, dMinimumY;
	double dWrk;
    //long lRightLineNumberMaximum, lLeftLineNumberMaximum;	
	long lRightLineNumberMinimum, lLeftLineNumberMinimum;
    //long lTopPixelNumberMaximum, lBottomPixelNumberMaximum;
	long lTopPixelNumberMinimum, lBottomPixelNumberMinimum;	
	
	//long Hist[256];
		
	int step = 3;

	// pixel parameters.

	long lPixelNumber, lLineNumber, lRightLineNumber, lLeftLineNumber;
	long lTopPixelNumber, lBottomPixelNumber;
	long lPixelValue;
	long lPointNumber;
	
	// get the pixels from image.
	
	HCHAR *cpPC_Buffer;
	static HGLOBAL hPC_Buffer;
	unsigned long ulImageSize = 640 * 480;

	
	WaitCursor(TRUE);
	
	hPC_Buffer = GlobalAlloc(GMEM_FIXED,ulImageSize);
	cpPC_Buffer = (HCHAR *) GlobalLock(hPC_Buffer);
	lStatus = DPK_XDCGetImage(G_lGrabImage,(HVOID *)cpPC_Buffer);

	if(lStatus >= P_ERROR)
	{
	   //save the picture in g_y();
	   for(i=0;i<640;i++)
		for(j=0;j<480;j++)
		  g_x[i][j] = g_y[i][j] = (double)(BYTE)(cpPC_Buffer[i+j*640]);
		  
		
	   //look for x - cord.

   	   dMinimumX = 255.0 * 255.0 * 640.0;
	   lRightLineNumberMinimum = 0;
	   lLeftLineNumberMinimum = 0;

	   for (lLeftLineNumber = *LL0; lLeftLineNumber < *LL1; lLeftLineNumber+=step)
       for (lRightLineNumber = *RL0; lRightLineNumber < *RL1; lRightLineNumber+=step)
	   { 
		   dAccumulator = 0.0;
		   
		   lPointNumber = lLeftLineNumber  * 640;
		   lPixelValue = cpPC_Buffer[lPointNumber];
		   
		   for( lPixelNumber = max(*BP0,*TP0); 
		        lPixelNumber < min(*BP1,*TP1); 
				lPixelNumber+=step)	
		   {	
             lLineNumber = (long)((double)lLeftLineNumber * (1.0 - (double)lPixelNumber/639.0) + 
			            (double)lRightLineNumber * (double)lPixelNumber/639.0);
             lPointNumber = lLineNumber * 640 + lPixelNumber;
               
			 dWrk = (cpPC_Buffer[lPointNumber] - lPixelValue);
		     dAccumulator += (dWrk*dWrk);
			   
	         lPixelValue = cpPC_Buffer[lPointNumber];
		   }

		   if ( dAccumulator < dMinimumX)
		   {   
			   dMinimumX = dAccumulator;
			   lRightLineNumberMinimum = lRightLineNumber;
               lLeftLineNumberMinimum = lLeftLineNumber;
		   }
	   }

	
	   

	   //look for y - coord.

	   dMinimumY = 255.0* 255.0 * 480.0;
	   lTopPixelNumberMinimum = 0;
	   lBottomPixelNumberMinimum = 0;

	   for (lTopPixelNumber = *TP0; lTopPixelNumber < *TP1; lTopPixelNumber+=step)
       for (lBottomPixelNumber = *BP0; lBottomPixelNumber < *BP1; lBottomPixelNumber+=step)
	   { 
		   dAccumulator = 0.0;
		   
		   lPointNumber = lTopPixelNumber;
		   lPixelValue = cpPC_Buffer[lPointNumber];
		   
		   for(lLineNumber = max(*LL0,*RL0);
		       lLineNumber < min(*LL1,*RL1);
			   lLineNumber+=step)	
		   {	
	           lPixelNumber = (long)((double)lTopPixelNumber * (1.0 - (double)lLineNumber/479.0) + 
			            (double)lBottomPixelNumber * (double)lLineNumber/479.0);
		       lPointNumber = lLineNumber * 640 + lPixelNumber;
               
			   dWrk = (cpPC_Buffer[lPointNumber] - lPixelValue);	           
			   dAccumulator += (dWrk*dWrk);
			   lPixelValue = cpPC_Buffer[lPointNumber];

		   

		   }

		   if ( dAccumulator < dMinimumY)
		   {   
			   dMinimumY = dAccumulator;
			   lTopPixelNumberMinimum = lTopPixelNumber;
               lBottomPixelNumberMinimum = lBottomPixelNumber;
		   }
	   }





	   // put x - line to the picture.
	   for(lPixelNumber = max(*TP0,*BP0); 
	       lPixelNumber < min(*TP1,*BP1);
		   lPixelNumber+=1 )	
	   {	
	      lLineNumber = (long)((double)lLeftLineNumberMinimum * (1.0 - (double)lPixelNumber/639.0) + 
			            (double)lRightLineNumberMinimum * (double)lPixelNumber/639.0);
	      lPointNumber = lLineNumber * 640 + lPixelNumber;
				   
	      cpPC_Buffer[lPointNumber] = (char)255;
	   }

	   // put y - line to the picture.
	   for(lLineNumber = max(*LL0,*RL0);
	       lLineNumber < min(*LL1,*RL1);
		   lLineNumber+=1)	
	   {	
	      lPixelNumber = (long)((double)lTopPixelNumberMinimum * (1.0 - (double)lLineNumber/479.0) + 
			            (double)lBottomPixelNumberMinimum * (double)lLineNumber/479.0);
	      lPointNumber = lLineNumber * 640 + lPixelNumber;
				   
	      cpPC_Buffer[lPointNumber] = (char)255;
	   }



	   lStatus = DPK_XDCPutImage(G_lGrabImage,(HVOID *)cpPC_Buffer);   
	}



	GlobalUnlock(hPC_Buffer);
	GlobalFree(hPC_Buffer);


    /*----------------------*/
    /* Display the results. */
    /*----------------------*/

    if (lStatus == P_SUCCESS)
    {
        DisplayImage (hWnd, G_lGrabImage);
    }

	// display internal angle between axes.
	{
	   char report[80];
	   double InternalAngle, ExternalAngleX, ExternalAngleY;
	   VECTOR X,Y,A;
	   POINT Cross;

	   X.start.x = *BP0;
	   X.end.x   = *BP1;
	   X.start.y = lLeftLineNumberMinimum;
	   X.end.y   = lRightLineNumberMinimum;

       Y.start.x = lTopPixelNumberMinimum;
       Y.end.x   = lBottomPixelNumberMinimum;
	   Y.start.y = *LL0;
	   Y.end.y   = *LL1;


	   A.start.x = *TP0;
	   A.start.y = *LL0;
	   A.end.x   = *BP0;
	   A.end.y   = *LL1;



       InternalAngle = 180.0 / 3.14152 * VectAngle(X,Y);
	   ExternalAngleX = 180.0 / 3.14152 * VectAngle(X,A);
	   ExternalAngleY = 180.0 / 3.14152 * VectAngle(Y,A);

	   
	   set_angle(InternalAngle);
	   set_x_angle(ExternalAngleX);
	   set_y_angle(ExternalAngleY);

	   sprintf(report,"Internal angle = %f\nExt.X = %f\nExt.Y = %f",
		   InternalAngle, ExternalAngleX, ExternalAngleY);
       

	   //if (fabs(90.0 - InternalAngle) > 0.5)
	   //{
	   //   strcat(report,"\nMeasurement is invalid. Repeate.");
	   //}

	   {
		  
		  //char s[30];
	      Cross = VectCross(X,Y);

		  //eliminate shift.
		  x = ((double)Cross.x);
		  y = ((double)Cross.y);
		  
		  


		  
		  // having x,y it is possible to do correlation for finding other chips.
		  //let save the pattern for that.
		  for(i=0;i<45;i++)
		   for(j=0;j<45;j++)
		   { 
			 g_pattern[i][j] = g_y[i+Cross.x][j+Cross.y];
		   }
		  
		  
		  
		  //transform ref. point from upper left corner to the centre.
		  x = (x - 320.0)*_CHIP_MASCHTAB_;
		  y = (y - 240.0)*_CHIP_MASCHTAB_;
		  
		  set_x(x);
		  set_y(y);

		  set_checksum(0.0f,0.0f,0.0f);
		  
	      //sprintf(s,"\nCross point = %f,%f", x,y);
		  //strcat(report,s);
	   
	   
	   }

	   double dWrk;
	   //MessageBox (hWnd, report, "Vision Data", MB_OK | MB_ICONINFORMATION);

	   	DBG_WriteText (G_lGrabImage, 20, 20,255,40,2, "*** Results ***");
		
	    get_x(&dWrk);
    	sprintf (report, "Cross point X = %f", (float)dWrk);
        DBG_WriteText (G_lGrabImage,20,40,255,40,1,report);

		get_y(&dWrk);
		sprintf (report, "Cross point Y = %f", (float)dWrk);
        DBG_WriteText (G_lGrabImage,20,60,255,40,1,report);

		get_angle(&dWrk);
	    sprintf (report, "Internal Angle = %f", (float)dWrk);
        DBG_WriteText (G_lGrabImage, 20, 80,255,40,1,report);

		if(fabs(dWrk-90.0)>0.5)
		{
          DBG_WriteText (G_lGrabImage, 20, 140,255,40,1,"Measurement is invalid.");
		}

		get_x_angle(&dWrk);
	    sprintf (report, "External X Angle = %f", (float)dWrk);
        DBG_WriteText (G_lGrabImage, 20, 100,255,40,1,report);
		
		get_y_angle(&dWrk);
	    sprintf (report, "External X Angle = %f", (float)dWrk);
        DBG_WriteText (G_lGrabImage, 20, 120,255,40,1,report);
		
		//redraw main window.
		PostMessage(hWnd,(UINT)WM_PAINT,(WPARAM)NULL,(LPARAM)NULL);
	}
	
	
	
	
	//correct field of view.
	*LL0 = lLeftLineNumberMinimum;
	*RL0 = lRightLineNumberMinimum;

	*TP0 = lTopPixelNumberMinimum;
	*BP0 = lBottomPixelNumberMinimum;

	
	WaitCursor(FALSE);

	return;
}




void copy(double (*x)[480],double (*y)[480])
{
   int i,j;
   
   for ( i=0; i<640;i++ )
	for ( j=0; j<480;j++ )
	 y[i][j] = x[i][j];
}



void normalize(double (*x)[480])
{

	int i,j;
	double dmin,dmax, amp = 255.0;
    
    dmin = 255.0;
	dmax = 0.0;

	for ( i=0; i<640;i++ )
	 for ( j=0; j<480;j++ )
	 {
		dmax = dmax>=x[i][j] ? dmax : x[i][j];
	   	dmin = dmin<=x[i][j] ? dmin : x[i][j]; 
	 }
	amp = amp/(dmax-dmin);

	for ( i=0; i<640;i++ )
	 for ( j=0; j<480;j++ )
	 {
		x[i][j] = amp*(x[i][j] - dmin);
	 }
}


void feel(double (*x)[480])
{
    LONG    lStatus;
	int i,j;
	HCHAR *cpPC_Buffer;
	static HGLOBAL hPC_Buffer;
	unsigned long ulImageSize = 640*480;

	hPC_Buffer = GlobalAlloc(GMEM_FIXED,ulImageSize);
	cpPC_Buffer = (HCHAR *) GlobalLock(hPC_Buffer);
	lStatus = DPK_XDCGetImage(G_lGrabImage,(HVOID *)cpPC_Buffer);

	if(lStatus >= P_ERROR)
	{
	   lStatus = DPK_XDCPutImage(G_lGrabImage,(HVOID *)cpPC_Buffer);   
	
	   for(i=0;i<640;i++) 
		for(j=0;j<480;j++) 
		  x[i][j] = (unsigned char)cpPC_Buffer[i+640*j];
	}
	
	GlobalUnlock(hPC_Buffer);
	GlobalFree(hPC_Buffer);
}


void write_text_double(int x,int y, int forground, int background, int scale, char *text, double parametre)
{
   	char s[80];
	sprintf(s,"%s%f",text,(float)parametre);
	
	DBG_WriteText (G_lGrabImage,x,y,forground,background,scale,s);
}


void show(double (*x)[480])
{
    LONG    lStatus;
	int i,j;
	UCHAR *cpPC_Buffer;
	static HGLOBAL hPC_Buffer;
	unsigned long ulImageSize = 640 * 480;
	hPC_Buffer = GlobalAlloc(GMEM_FIXED,ulImageSize);
	cpPC_Buffer = (UCHAR *) GlobalLock(hPC_Buffer);
	
	for(i=0;i<640;i++)
	  for(j=0;j<480;j++)
	    cpPC_Buffer[i+640*j] = (unsigned char) max(0.0,min(x[i][j],255.0));

	lStatus = DPK_XDCPutImage(G_lGrabImage,(HVOID *)cpPC_Buffer);   
	GlobalUnlock(hPC_Buffer);
	GlobalFree(hPC_Buffer);
    if (lStatus == P_SUCCESS)
    {
        DisplayImage (G_hWndMain, G_lGrabImage);
		//redraw main window.
		PostMessage(G_hWndMain,(UINT)WM_PAINT,(WPARAM)NULL,(LPARAM)NULL);
	}
}

int imedian4(int a,int b,int c,int d)
{
   	int tmax = max(max(a,b),max(c,d));
    int tmin = min(min(a,b),min(c,d));
	return (a + b + c + d -tmax -tmin)/2;
}		   


double dmedian4(double a,double b,double c,double d)
{
   	double tmax = max(max(a,b),max(c,d));
    double tmin = min(min(a,b),min(c,d));
	return (a + b + c + d -tmax -tmin)/2.0;
}		   



void mf(double (*x)[480])
{
	int i,j;
	double (*y)[480];
	y = new double[640][480];

	for ( i=0; i<640;i++ )
	 for ( j=0; j<480;j++ )
	 {
	   if(i==639 && j==479) y[i][j] = x[i][j];
	   else if(i==639) y[i][j] = dmedian4(x[i][j],x[i][j],  x[i][j+1],x[i][j+1]);
	   else if(j==479) y[i][j] = dmedian4(x[i][j],x[i+1][j],x[i][j],  x[i+1][j]);
	   else y[i][j] =            dmedian4(x[i][j],x[i+1][j],x[i][j+1],x[i+1][j+1]);
	 }
	
	for ( i=0; i<640;i++ )
	 for ( j=0; j<480;j++ )
	 {
	   if(i==0 && j==0) x[i][j] = y[i][j];
	   else if(i==0) x[i][j] = dmedian4(y[i][j],y[i][j],y[i][j-1],y[i][j-1]);
	   else if(j==0) x[i][j] = dmedian4(y[i][j],y[i-1][j],y[i][j],y[i-1][j]);
	   else x[i][j] = dmedian4(y[i][j],y[i-1][j],y[i][j-1],y[i-1][j-1]);
	 }
	 
	 
	 
	 delete [] y;
}

void mfo(double (*x)[480])
{
	int i,j;
	double (*y)[480];
	y = new double[640][480];

	for ( i=0; i<640;i++ )
	 for ( j=0; j<480;j++ )
	 {
	   if(i==639 && j==479) y[i][j] = x[i][j];
	   else if(i==639) y[i][j] = dmedian4(x[i][j],x[i][j],  x[i][j+1],x[i][j+1]);
	   else if(j==479) y[i][j] = dmedian4(x[i][j],x[i+1][j],x[i][j],  x[i+1][j]);
	   else y[i][j] =            dmedian4(x[i][j],x[i+1][j],x[i][j+1],x[i+1][j+1]);
	 }
	
	for ( i=0; i<640;i++ )
	 for ( j=0; j<480;j++ )
	 {
	   if(i==0 && j==0) x[i][j] = y[i][j];
	   else if(i==0) x[i][j] = dmedian4(y[i][j],y[i][j],y[i][j-1],y[i][j-1]);
	   else if(j==0) x[i][j] = dmedian4(y[i][j],y[i-1][j],y[i][j],y[i-1][j]);
	   else x[i][j] = dmedian4(y[i][j],y[i-1][j],y[i][j-1],y[i-1][j-1]);
	 }
	 

	for ( i=0; i<640;i++ )
	 for ( j=0; j<480;j++ )
	 {
	   if(i==639 && j==479) y[i][j] = x[i][j];
	   else if(i==639) y[i][j] = dmedian4(x[i][j],x[i][j],  x[i][j-1],x[i][j-1]);
	   else if(j==0) y[i][j] = dmedian4(x[i][j],x[i+1][j],x[i][j],  x[i+1][j]);
	   else y[i][j] =            dmedian4(x[i][j],x[i+1][j],x[i][j-1],x[i+1][j-1]);
	 }
	
	for ( i=0; i<640;i++ )
	 for ( j=0; j<480;j++ )
	 {
	   if(i==0 && j==0) x[i][j] = y[i][j];
	   else if(i==0) x[i][j] = dmedian4(y[i][j],y[i][j],y[i][j+1],y[i][j+1]);
	   else if(j==479) x[i][j] = dmedian4(y[i][j],y[i-1][j],y[i][j],y[i-1][j]);
	   else x[i][j] = dmedian4(y[i][j],y[i-1][j],y[i][j+1],y[i-1][j+1]);
	 }
	 
	 
	 delete [] y;
}

void mfx(double (*x)[480])
{
	int i,j;

	for ( i=0; i<640;i++ )
	 for ( j=0; j<480;j++ )
	 {
	   if(i==639 && j==479);
	   else if(i==639) x[i][j] = dmedian4(x[i][j],x[i][j],  x[i][j+1],x[i][j+1]);
	   else if(j==479) x[i][j] = dmedian4(x[i][j],x[i+1][j],x[i][j],  x[i+1][j]);
	   else x[i][j] =            dmedian4(x[i][j],x[i+1][j],x[i][j+1],x[i+1][j+1]);
	 }
	
	for ( i=0; i<640;i++ )
	 for ( j=0; j<480;j++ )
	 {
	   if(i==0 && j==0);
	   else if(i==0) x[i][j] = dmedian4(x[i][j],x[i][j],x[i][j-1],x[i][j-1]);
	   else if(j==0) x[i][j] = dmedian4(x[i][j],x[i-1][j],x[i][j],x[i-1][j]);
	   else x[i][j] = dmedian4(x[i][j],x[i-1][j],x[i][j-1],x[i-1][j-1]);
	 }
}




void lpf(double (*x)[480])
{
	int i,j,k,l,m;
	double d,f;
	double (*y)[480];
	y = new double[640][480];
	for(i=0;i<640;i++) 
	  for(j=0;j<480;j++) 
	  {
		  y[i][j]=x[i][j];
		  x[i][j]=0.0;
	  }
	
	m=5;
	for ( i=m; i<640-m;i++ )
	 for ( j=m; j<480-m;j++ )
	 {
	   d = 0.0;
	   for(k=-m;k<=m;k++)
	     for(l=-m;l<=m;l++)
		 {	 
			 f = 1.0/(1.0 + sqrt(l*l+k*k));
			 d+=y[i+k][j+l]*f;
		 }
	   x[i][j] = d;
	 }
	 delete [] y;
}







void top_edge(double (*x)[480])
{
  	int horizontal_begin = 200,
		horizontal_end   = 400,
		vertical_begin   = 100,
		vertical_end     = 300;
	
	int i,j,k,/*m,*/jmax[640]/*, jm[2][640]*/;
	double max,s/*,sin1,sin2,sin*/;
	int lng;
	
	for(i=0;i<640;i++) jmax[i]=0;
	
	lng = 70;
	for(i=horizontal_begin;i<horizontal_end;i++)
	{ 
	  jmax[i]=0;
	  max = 0.0;
	  for(j=(vertical_end-(lng/2));j>=(vertical_begin+(lng/2));j--)
	  {
	  	 s = 0.0;
		 for(k=1;k<=(lng/2);k++) 
		 {
			 s += (-x[i][j+k]+x[i][j-k]);
		 }
	     
		 if( max < s) 
		 {
		   jmax[i] = j;
		   max  = s;
		 }
	  }

    }

    
	for(i=horizontal_begin;i<horizontal_end && jmax[i]==0;i++)
	
	for(j=i-1;j>=horizontal_begin;j--)
	{ 
		jmax[j] = jmax[i];
	}
	

	
    /*// correcting inmpossible readings.
	k=1;
	for(i=horizontal_begin+1;i<horizontal_end;i++) 
		if((jmax[i]-jmax[i-1])<-k) jmax[i]=jmax[i-1]-k;
		else if((jmax[i]-jmax[i-1])>k) jmax[i]=jmax[i-1]+k;
	*/
	for(i=horizontal_begin;i<horizontal_end;i++) 
	  for(j=vertical_begin;j<vertical_end;j++)
		x[i][j] = 0.0;
	
	for(i=horizontal_begin;i<horizontal_end;i++) x[i][jmax[i]] = 255.0;


	//deliver data to other process.
	//use there _CHIP_MASCHTAB_ =  0.00275 to transform from pixels to inches.
	set_int_array(640,jmax);

}


void bottom_edge(double (*x)[480])
{
  	int horizontal_begin = 200,
		horizontal_end   = 400,
		vertical_begin   = 200,
		vertical_end     = 479;
	
	int i,j,k,/*m,*/jmax[640]/*, jm[2][640]*/;
	double max,s/*,sin1,sin2,sin*/;
	int lng;
	
	for(i=0;i<640;i++) jmax[i]=0;
	
	lng = 70;
	for(i=horizontal_begin;i<horizontal_end;i++)
	{ 
	  jmax[i]=0;
	  max = 0.0;
	  for(j=(vertical_end-(lng/2));j>=(vertical_begin+(lng/2));j--)
	  {
	  	 s = 0.0;
		 for(k=1;k<=(lng/2);k++) 
		 {
			 s += (x[i][j+k]-x[i][j-k]);
		 }
	     
		 if( max < s) 
		 {
		   jmax[i] = j;
		   max  = s;
		 }
	  }

    }

    
	for(i=horizontal_begin;i<horizontal_end && jmax[i]==0;i++)
	
	for(j=i-1;j>=horizontal_begin;j--)
	{ 
		jmax[j] = jmax[i];
	}
	

	
    /*// correcting inmpossible readings.
	k=1;
	for(i=horizontal_begin+1;i<horizontal_end;i++) 
		if((jmax[i]-jmax[i-1])<-k) jmax[i]=jmax[i-1]-k;
		else if((jmax[i]-jmax[i-1])>k) jmax[i]=jmax[i-1]+k;
	*/
	for(i=horizontal_begin;i<horizontal_end;i++) 
	  for(j=vertical_begin;j<vertical_end;j++)
		x[i][j] = 0.0;
	
	for(i=horizontal_begin;i<horizontal_end;i++) x[i][jmax[i]] = 255.0;

	//deliver data to other process.
	//use there _CHIP_MASCHTAB_ =  0.00275 to transform from pixels to inches.
	set_int_array(640,jmax);
}



void left_edge(double (*x)[480])
{
  	int horizontal_begin = 150,
		horizontal_end   = 450,
		vertical_begin   = 150,
		vertical_end     = 350;
	
	int i,j,k,/*m,*/jmax[480];
	double max,s/*,sin1,sin2,sin*/;
	int lng;
	
	for(i=0;i<480;i++) jmax[i]=0;

	lng = 77;
	for(i=vertical_begin;i<vertical_end;i++)
	{ 
	  jmax[i]=0;
	  max = 0.0;
	  for(j=(horizontal_end-(lng/2));j>=(horizontal_begin+(lng/2));j--)
	  {
	  	 s = 0.0;
		 for(k=1;k<=(lng/2);k++) 
		 {
			 s += (-x[j+k][i]+x[j-k][i]);
		 }
	     
		 if( max < s) 
		 {
		   jmax[i] = j;
		   max  = s;
		 }
	  }

    }

    
	for(i=vertical_begin;i<vertical_end && jmax[i]==0;i++)
	
	for(j=i-1;j>=vertical_begin;j--)
	{ 
		jmax[j] = jmax[i];
	}
	

	
    /*// correcting inmpossible readings.
	k=1;
	for(i=vertical_begin+1;i<vertical_end;i++) 
		if((jmax[i]-jmax[i-1])<-k) jmax[i]=jmax[i-1]-k;
		else if((jmax[i]-jmax[i-1])>k) jmax[i]=jmax[i-1]+k;
	*/
	//output 
	for(i=horizontal_begin;i<horizontal_end;i++) 
	  for(j=vertical_begin;j<vertical_end;j++)
		x[i][j] = 0.0;
	
	for(i=vertical_begin;i<vertical_end;i++) x[jmax[i]][i] = 255.0;

	//deliver data to other process.
	//use there _CHIP_MASCHTAB_ =  0.00275 to transform from pixels to inches.
	set_int_array(480,jmax);
}





void right_edge(double (*x)[480])
{
  	int horizontal_begin = 150,
		horizontal_end   = 450,
		vertical_begin   = 150,
		vertical_end     = 350;
	
	int i,j,k/*,m*/,jmax[480];
	double max,s/*,sin1,sin2,sin*/;
	int lng;
	
	for(i=0;i<480;i++) jmax[i]=0;

	lng = 77;
	for(i=vertical_begin;i<vertical_end;i++)
	{ 
	  jmax[i]=0;
	  max = 0.0;
	  for(j=(horizontal_end-(lng/2));j>=(horizontal_begin+(lng/2));j--)
	  {
	  	 s = 0.0;
		 for(k=1;k<=(lng/2);k++) 
		 {
			 s += (x[j+k][i]-x[j-k][i]);
		 }
	     
		 if( max < s) 
		 {
		   jmax[i] = j;
		   max  = s;
		 }
	  }

    }

    
	for(i=vertical_begin;i<vertical_end && jmax[i]==0;i++)
	
	for(j=i-1;j>=vertical_begin;j--)
	{ 
		jmax[j] = jmax[i];
	}
	

	
    /*// correcting inmpossible readings.
	k=1;
	for(i=vertical_begin+1;i<vertical_end;i++) 
		if((jmax[i]-jmax[i-1])<-k) jmax[i]=jmax[i-1]-k;
		else if((jmax[i]-jmax[i-1])>k) jmax[i]=jmax[i-1]+k;
	*/
	//output 
	for(i=horizontal_begin;i<horizontal_end;i++) 
	  for(j=vertical_begin;j<vertical_end;j++)
		x[i][j] = 0.0;
	
	for(i=vertical_begin;i<vertical_end;i++) x[jmax[i]][i] = 255.0;

	//deliver data to other process.
	//use there _CHIP_MASCHTAB_ =  0.00275 to transform from pixels to inches.
	set_int_array(480,jmax);
}









#define _KOEF_ 2

VOID TopEdge(HWND hWnd)
{
	int i;
	
	WaitCursor(TRUE);

	feel(g_x);
	normalize(g_x);
	
	//mfx(g_x);
	for(i=0;i<_KOEF_;i++) mfo(g_x);
	top_edge(g_x);
	//lpf(g_x);
	normalize(g_x);
	show(g_x);
	
	WaitCursor(FALSE);
}



VOID BottomEdge(HWND hWnd)
{
	int i;
	
	WaitCursor(TRUE);

	feel(g_x);
	normalize(g_x);
	
	//mfx(g_x);
	for(i=0;i<_KOEF_;i++) mfo(g_x);
	bottom_edge(g_x);
	//lpf(g_x);
	normalize(g_x);
	show(g_x);

	WaitCursor(FALSE);
}



VOID LeftEdge(HWND hWnd)
{
	int i;
	
	WaitCursor(TRUE);

	feel(g_x);
	normalize(g_x);
	
	//mfx(g_x);
	for(i=0;i<_KOEF_;i++) mfo(g_x);
	left_edge(g_x);
	//lpf(g_x);
	normalize(g_x);
	show(g_x);

	WaitCursor(FALSE);
}



VOID RightEdge(HWND hWnd)
{
	int i;

	WaitCursor(TRUE);

	feel(g_x);
	normalize(g_x);
	
	//mfx(g_x);
	for(i=0;i<_KOEF_;i++) mfo(g_x);
	right_edge(g_x);
	//lpf(g_x);
	normalize(g_x);
	show(g_x);

	WaitCursor(FALSE);
}


unsigned char median4(unsigned char a,unsigned char b,unsigned char c,unsigned char d)
{
   	unsigned char tmax = max(max(a,b),max(c,d));
    unsigned char tmin = min(min(a,b),min(c,d));
	
	return (unsigned char)(((long)a + (long)b + (long)c + (long)d -(long)tmax -(long)tmin)/2);
}





/*============================================================================
*
*  Name        : HallCoordinates
*  Author      : Paul Dourbal
*  Description : To peform wafer camera justification.
*
*  Parameters  :
*       hWnd            Handle to window.
*
*  Returns     : none
*
*===========================================================================*/

VOID HallCoordinates(HWND hWnd, double* x,double* y)
{

    LONG    lStatus;        /* Error status. */
	int indentation,i,j,k;
	double potential, maximum;
	int lpx,lpy;

	char report[80];
	
	HCHAR *cpPC_Buffer;
	static HGLOBAL hPC_Buffer;
	unsigned long ulImageSize = 640 * 480;


	WaitCursor(TRUE);

	hPC_Buffer = GlobalAlloc(GMEM_FIXED,ulImageSize);
	cpPC_Buffer = (HCHAR *) GlobalLock(hPC_Buffer);
	lStatus = DPK_XDCGetImage(G_lGrabImage,(HVOID *)cpPC_Buffer);

	if(lStatus >= P_ERROR)
	{
	   //Get potentials.
	   maximum = -9999999999999999990.0;
	   indentation = 70;
	   for(i=indentation;i<480-indentation;i++) for(j=indentation;j<640-indentation;j++)
	   {
		  potential = 0.0;
		  
		  for(k=-indentation;k<indentation;k++)
		  {	  
		    potential += (-(double)(BYTE)cpPC_Buffer[(i+k)*640+(j)]);
	   	    potential += (-(double)(BYTE)cpPC_Buffer[(i)*640+(j+k)]);
			potential += (-(double)(BYTE)cpPC_Buffer[(i+k)*640+(j+k)]);
	   	    potential += (-(double)(BYTE)cpPC_Buffer[(i+k)*640+(j-k)]);

		  }

		  if(potential > maximum)
		  {
			maximum = potential;  
		  	lpy = i;
			lpx = j;
		  }
	   }

	   for(k=-indentation;k<indentation;k++)
	   {	  
		 cpPC_Buffer[(lpy+k)*640+(lpx)]	  = (char)255;
	   	 cpPC_Buffer[(lpy)*640+(lpx+k)]   = (char)255;
		 cpPC_Buffer[(lpy+k)*640+(lpx+k)] = (char)255;
	   	 cpPC_Buffer[(lpy+k)*640+(lpx-k)] = (char)255;
	   }


	   lStatus = DPK_XDCPutImage(G_lGrabImage,(HVOID *)cpPC_Buffer);   
	}

	GlobalUnlock(hPC_Buffer);
	GlobalFree(hPC_Buffer);

	// Display the results. */
    if(lStatus==P_SUCCESS)
    {
       DisplayImage (hWnd, G_lGrabImage);
	   *x = (double)lpx * _CHIP_MASCHTAB_;
	   *y = (double)lpy * _CHIP_MASCHTAB_;
	   set_x(*x);
	   set_y(*y);
	   set_checksum(0.0f,0.0f,0.0f);
		  
	   DBG_WriteText (G_lGrabImage, 20, 20,255,40,2, "*** Results ***");
		
	   get_x(x);
	   get_y(y);
       sprintf (report,"Central Point X = %f,  Y = %f",(float)(*x),(float)(*y));
       DBG_WriteText (G_lGrabImage,20,40,255,40,1,report);
		
	   //redraw main window.
	   PostMessage(hWnd,(UINT)WM_PAINT,(WPARAM)NULL,(LPARAM)NULL);
	}
	
	WaitCursor(FALSE);
	return;
}






//helper for sorting.

int compare( const void *x, const void *y )
{
    if((*(int*)x) > (*(int*)y)) return 1;
	else if((*(int*)x) < (*(int*)y)) return -1;
	return 0;
}



/*============================================================================
*
*  Name        : HallCoordinates
*  Author      : Paul Dourbal
*  Description : To peform wafer camera justification (gives bias from centre).
*
*  Parameters  :
*       hWnd            Handle to window.
*
*  Returns     : none
*
*===========================================================================*/
/*
int  ltop,lbottom,lleft,lright;


VOID HallCoordinateBias(HWND hWnd, double* x,double* y)
{
	

	int edge[640];
	
	int i;

	
	WaitCursor(TRUE);
	
	
	feel(g_x);

	normalize(g_x);

	for(i=0;i<_KOEF_;i++) mfo(g_x);
	copy(g_x,g_y);
	
	
	copy(g_y,g_x);
	right_edge(g_x);
	get_int_array(480,edge);
	qsort((void *)edge, (size_t)(480), sizeof(int *), compare );
	for(i=0; i<479 && edge[i]==0 ;i++);
	lright = edge[239+i/2];
	

	copy(g_y,g_x);
	bottom_edge(g_x);
    get_int_array(640,edge);
	qsort((void *)edge, (size_t)(640), sizeof(int *), compare );
	for(i=0; i<639 && edge[i]==0 ;i++);
	lbottom = edge[319+i/2];
	
		
	copy(g_y,g_x);
	left_edge(g_x);
	get_int_array(480,edge);
	qsort((void *)edge, (size_t)(480), sizeof(int *), compare );
	for(i=0; i<479 && edge[i]==0 ;i++);
	lleft = edge[239+i/2];
    

	copy(g_y,g_x);
	top_edge(g_x);
	get_int_array(640,edge);
	qsort((void *)edge, (size_t)(640), sizeof(int *), compare );
	for(i=0; i<639 && edge[i]==0 ;i++);
	ltop = edge[319+i/2];

	normalize(g_y);

	for(i=lleft; i<=lright; i++)
	{
	  g_y[i][ltop] = g_y[i][lbottom] = 255.0;
	}

	for(i=ltop; i<=lbottom; i++)
	{
	  g_y[lleft][i] = g_y[lright][i] = 255.0;
	}

	show(g_y);
	
	delete_g_y();

	WaitCursor(FALSE);
}
*/




//corner 90 deg. --> sum->minimum.
double round_sum_ratio(HCHAR *cpPC_Buffer,int y, int x, int radius, double& maximum, double& minimum)
{
   int i1,j1,i0,j0;
   double sq2,sum, ang, sumplus, summinus, rad, angstep, S,C;
   double angle[2];

   angle[0] = angle[1] = 0.0;
      
   angstep = 4.0 * PI / 180.0;
   sq2 = 3; //sqrt(2);
   
   for(ang = 0; ang < 2*PI; ang += angstep)
   {
	 
	 S = sin(ang);
	 C = cos(ang);
	 sumplus = 0.0;
	 summinus = 0.0;

	 for(rad = 0; rad <= radius; rad += 1.0)
	 {
	    i0 = i1 = y + int(radius*S);
	    j0 = j1 = x + int(radius*C);

		i0 += int(sq2*C);
		j0 += int(sq2*S);

		i1 -= int(sq2*C);
		j1 -= int(sq2*S);
	 
	  
	    if(UINT(Pf(i1,j1)) > UINT(Pf(i0,j0))) sumplus  += 1.0;
		else if(UINT(Pf(i1,j1)) < UINT(Pf(i0,j0)))	summinus += 1.0;

	}
   
	if(sumplus > summinus)
	{
	   if(sumplus > maximum) 
	   {  
		 maximum = sumplus;
		 angle[0] = ang;
	   }
	}
   	else if(sumplus < summinus)
	{
	   if(summinus > minimum) 
	   {  
		 minimum = summinus;
		 angle[1] = ang;
	   }
	}
   
  }

  sum = fabs(angle[0] - angle[1]);
  sum = fabs(PI/2.0 - sum);
  return sum;
}


void DrowAngle(HCHAR *cpPC_Buffer,int x,int y,double a, int l)
{
    double x0,x1,dx,y0,y1,dy,S,C;
	int i;
	int ix,iy;
	
	S=sin(a);
	C=cos(a);
	
	x0 = x - l*C;
	y0 = y + l*S;
	x1 = x + l*C;
	y1 = y - l*S;

	dx = (x1-x0)/l;
	dy = (y1-y0)/l;

	for(i=0;i<=l;i++)
	{
	  ix = int(x0);
	  iy = int(y0);

	  Pfl(iy,ix) = char(255);

	  x0 += dx;
	  y0 += dy;
	}
}


double Hough(HCHAR *cpPC_Buffer,int x,int y,double a, int l)
{
    double sum ,x0,x1,dx,y0,y1,dy,S,C;
	int i;
	int ix,iy;
	
	sum = 0.0;
	S=sin(a);
	C=cos(a);
	
	x0 = x - l*C;
	y0 = y + l*S;
	x1 = x + l*C;
	y1 = y - l*S;

	dx = (x1-x0)/l/2.0;
	dy = (y1-y0)/l/2.0;

	for(i=0;i<=l;i+=1)
	{
	  ix = int(x0);
	  iy = int(y0);

	  //for(m= -2;m<=2;m++)
	  //  for(k= -2;k<=2;k++)
		  sum += Pf(iy/*+m*/,ix/*+k*/);

	  x0 += dx;
	  y0 += dy;
	}
	return sum;
}


double HoughTrans(HCHAR *cpPC_Buffer, int x,int y,double a, int l)
{
   int i,j;
   double t,max = 0.0;

   for(j=x-l;j<=x+l;j+= 1)
   	for(i=y-l;i<=y+l;i+= 1)
	{ 
	  if(Pf(i,j)>0.0)
	  {
   	    t = Hough(cpPC_Buffer,j,i,a,l);
        if(t > max) max = t;
	  }
    }
   return max;
}


double Angle(HCHAR *cpPC_Buffer, int x,int y,int l)
{
   double a, amax=0.0 ,t,max = 0.0;
   double step = PI/360.0;
   
   
   for(a = -PI/2.0; a < -PI/4.0; a += step)
   {
   	  t = HoughTrans(cpPC_Buffer,x,y,a,l);
      if(t > max) 
	  { 
		  max = t;
		  amax = a+PI/2;
	  }  
   }
   
   for(a = PI/4.0; a < PI/2.0; a += step)
   {
   	  t = HoughTrans(cpPC_Buffer,x,y,a,l);
      if(t > max) 
	  { 
		  max = t;
		  amax = a-PI/2;
	  }  
   }


   return amax;
}




double quadrant_summ(HCHAR *cpPC_Buffer,int y,int x,int dy,int dx)
{  
   double sum = 0.0;
   int ip,jp;
   int blc = 1;

   if(Pf(y,x)<128) return sum;
   
   for(ip=y-dy;ip<=y+dy;ip++) 
    for(jp=x-dx;jp<=x+dx;jp++) 
     sum += Pf(ip++,jp++);
   return sum;
}



VOID HallCoordinateBias(HWND hWnd, double* x,double* y, double maschtab, int indentation, double threshold)
{

    LONG    lStatus;
	int i,j,k;
	long mc,mpx,mpy;
	double potential, potentialminus[4], potentialplus[4],
		   maximum, minimum, dc, dplus, dminus;
	int lpx,lpy;

	char report[80];
		
	HCHAR *cpPC_Buffer;
	static HGLOBAL hPC_Buffer;
	unsigned long ulImageSize = 640 * 480;


	WaitCursor(TRUE);

	hPC_Buffer = GlobalAlloc(GMEM_FIXED,ulImageSize);
	cpPC_Buffer = (HCHAR *) GlobalLock(hPC_Buffer);
	lStatus = DPK_XDCGetImage(G_lGrabImage,(HVOID *)cpPC_Buffer);

	if(lStatus >= P_ERROR)
	{

	  dc=0.0;
	  maximum = 0.0;
	  //for(i=indentation;i<480-indentation;i++) 
	  // for(j=indentation;j<640-indentation;j++)
 	  for(i=0;i<480;i++) 
	   for(j=0;j<640;j++)
	   {
	     dc += (double)(BYTE)cpPC_Buffer[(i)*640+(j)];
	     maximum += 1.0;
	   }		
	
	  dc /= maximum;

	  
	  //for(i=indentation;i<480-indentation;i++) for(j=indentation;j<640-indentation;j++)

	  dc *= threshold; //add 20 % to the threshold.

	  for(i=1;i<480;i++) 
		for(j=0;j<640;j++)
		{
	      if(dc <= (double)(BYTE)cpPC_Buffer[(i)*640+(j)]) 
			  cpPC_Buffer[(i)*640+(j)] = char(0);
		  else
		    cpPC_Buffer[(i)*640+(j)] = char(255);
	    }		
	  
	  
	  	
		
	  /*// Enchancement.
	  for(i=0;i<480;i++) for(j=0;j<640;j++)
	  {
		 if(i<indentation || i>=480-indentation || j<indentation || j>=640-indentation)
		 {
		    cpPC_Buffer[(i)*640+(j)] = 255;
		 }
	  }
	  */


	  //Get potentials.
	   maximum = -9999999999999999990.0;
	   minimum = 9999999999999999990.0;

	   
	   //for(i=indentation;i<480-indentation;i++) for(j=indentation;j<640-indentation;j++)
	   for(i=0;i<480;i+=4) for(j=0;j<640;j+=4)
	   {
		  
		  for(k=0;k<4;k++) potentialminus[k] = potentialplus[k] = 0.0;
		  
		  for(k=-indentation;k<indentation;k++)
		  {	  
		    dplus = dminus = 0.0;
			potential = (indentation-abs(k))*(indentation-abs(k));  
			
			if(k>0)
			{ 
			  if(i+k<0 || i+k>=480) dplus = 0.0;
			  else dplus = (double)(BYTE)cpPC_Buffer[(i+k)*640+(j)];
			  potentialplus[0] += dplus * potential;
			}
			else if(k<0)
			{
			  if(i+k<0 || i+k>=480) dminus = 0.0;
			  else dminus = (double)(BYTE)cpPC_Buffer[(i+k)*640+(j)];
			  potentialminus[0]+= dminus * potential;
			}
			
			
			
			
			
			if(k>0)
			{
			  if(j+k<0 || j+k>=640) dplus = 0.0;
			  else dplus = (double)(BYTE)cpPC_Buffer[(i)*640+(j+k)]; 
			  potentialplus[1] += dplus * potential;
			}
			else if(k<0)
			{
			  if(j+k<0 || j+k>=640) dminus = 0.0;
			  else dminus = (double)(BYTE)cpPC_Buffer[(i)*640+(j+k)]; 
			  potentialminus[1]+= dminus* potential;
			}
			
			

			
			
			
			if(k>0)
			{
			  if(i+k<0 || i+k>=480 || j+k<0 || j+k>=640) dplus = 0.0;
   		      else dplus = (double)(BYTE)cpPC_Buffer[(i+k)*640+(j+k)];	
			  potentialplus[2] += dplus * potential;
			}
			else if(k<0)
			{
			  if(i+k<0 || i+k>=480 || j+k<0 || j+k>=640) dminus = 0.0;
   		      else dminus = (double)(BYTE)cpPC_Buffer[(i+k)*640+(j+k)]; 
			  potentialminus[2]+= dminus* potential;
			}
			
			

			
			
			if(k>0)
			{
			  if(i+k<0 || i+k>=480 || j-k<0 || j-k>=640) dplus = 0.0;
		      else dplus = (double)(BYTE)cpPC_Buffer[(i+k)*640+(j-k)];
			  potentialplus[3] += dplus * potential;

			}
			else if(k<0)
			{
			  if(i+k<0 || i+k>=480 || j-k<0 || j-k>=640) dminus = 0.0;
		      else dminus = (double)(BYTE)cpPC_Buffer[(i+k)*640+(j-k)];
			  potentialminus[3]+= dminus* potential;
			}
		  }

		  potential = 0.0;
		  for(k=0;k<4;k++) 
			  potential += 
			  (potentialplus[k]+potentialminus[k]-fabs(potentialplus[k]-potentialminus[k]));
		  
		  if(potential > maximum)
		  {
			//if(different <= minimum)
			{
			  
			  maximum = potential;  
		  	  lpy = i;
			  lpx = j;
			}
		  }
	   }


	  
	  mc = mpx = mpy = 0;
	  
	  for(i=lpy-indentation;i<=lpy+indentation;i++)
	    for(j=lpx-indentation;j<=lpx+indentation;j++)
		{
	  	  if(Pf(i,j)> 255) 
		  {  mpx += j;
		     mpy += i;
		     mc  += 1;
		  }
		}

	  if(mc>0)
	  {
	  	  mpx /= mc;
		  mpy /= mc;
	  }


	  lpx = mpx;
	  lpy = mpy;

	  //countur
	  for(i=lpy-indentation; i<= lpy+indentation; i++)
	   for(j=lpx-indentation; j<= lpx; j++)
	   {
	   	if(Pf(i,j+1) != Pf(i,j)) 
		{
			//Pfl(i,j) = char(255);
			Pfl(i-1,j) = char(255);
			Pfl(i-2,j) = char(255);
			while(j < lpx)
			{
			   j++;
			   Pfl(i,j) = char(0);
			}
		    break;
		}
	   }
		 

	  for(i=lpy+indentation; i>= lpy-indentation; i--)
	   for(j=lpx+indentation; j>= lpx; j--)
	   {
	   	if(Pf(i,j-1) != Pf(i,j)) 
		{
			//Pfl(i,j+2) = char(255);
			Pfl(i,j+1) = char(255);
			Pfl(i,j) = char(255);
			while(j > lpx)
			{
			   j--;
			   Pfl(i,j) = char(0);
			}
		    break;
		}
	   }

	  
	  
	   
	   
	   
	   
	  dc = Angle(cpPC_Buffer,lpx,lpy,indentation); 
	   
	  DrowAngle(cpPC_Buffer,lpx,lpy,dc,indentation); 
      DrowAngle(cpPC_Buffer,lpx,lpy,dc+PI/2,indentation);
      DrowAngle(cpPC_Buffer,lpx,lpy,dc+PI/4,indentation/2);
      DrowAngle(cpPC_Buffer,lpx,lpy,dc+3*PI/4,indentation/2);

	  

      lStatus = DPK_XDCPutImage(G_lGrabImage,(HVOID *)cpPC_Buffer);   
	}

	GlobalUnlock(hPC_Buffer);
	GlobalFree(hPC_Buffer);

	// Display the results.
    if(lStatus==P_SUCCESS)
    {
       DisplayImage (hWnd, G_lGrabImage);
	   *x = (double)(lpx-320) * maschtab;
	   *y = (double)(lpy-240) * maschtab;
	   set_x(*x);
	   set_y(*y);

	   //turn into degree.
	   dc *= (180.0/PI);
	   set_angle(dc);
	   set_checksum(0.0f,0.0f,0.0f);
		  
	   DBG_WriteText (G_lGrabImage, 20, 20,255,40,2, "*** Results ***");
		
	   get_x(x);
	   get_y(y);
	   get_angle(&dc);
       sprintf (report,"Central Point Bias X = %f, Y = %f",(float)(*x),(float)(*y));
       DBG_WriteText (G_lGrabImage,20,60,255,40,1,report);
       sprintf (report,"Angle = %f degree.",dc);
       DBG_WriteText (G_lGrabImage,20,90,255,40,1,report);
		
	   //redraw main window.
	   PostMessage(hWnd,(UINT)WM_PAINT,(WPARAM)NULL,(LPARAM)NULL);
	}
	
	WaitCursor(FALSE);
	return;
}





VOID IDMHallCoordinates (HWND hWnd)
{
	double x,y;
	//HallCoordinates(hWnd,&x,&y);
	HallCoordinateBias(hWnd,&x,&y,(double)_CHIP_MASCHTAB_, 70, 0.9);
}




/*============================================================================
*
*  Name        : HallCoordinateBias2
*  Author      : Paul Dourbal
*  Description : To peform base camera justification (gives bias from centre).
*
*  Parameters  :
*       hWnd     Handle to window.
*
*  Returns     : none
*
*===========================================================================*/

VOID HallCoordinateBias2(HWND hWnd, double* x,double* y, double* angle)
{

    LONG    lStatus;        /* Error status. */
	int indentation,i,j,k;
	double potential, maximum;
	int lpx,lpy;

	char report[80];
	
	HCHAR *cpPC_Buffer;
	static HGLOBAL hPC_Buffer;
	unsigned long ulImageSize = 640 * 480;


	WaitCursor(TRUE);

	hPC_Buffer = GlobalAlloc(GMEM_FIXED,ulImageSize);
	cpPC_Buffer = (HCHAR *) GlobalLock(hPC_Buffer);
	lStatus = DPK_XDCGetImage(G_lGrabImage,(HVOID *)cpPC_Buffer);

	if(lStatus >= P_ERROR)
	{
	   //Get potentials.
	   maximum = -9999999999999999990.0;
	   indentation = 100;
	   for(i=indentation;i<480-indentation;i++) for(j=indentation;j<640-indentation;j++)
	   {
		  potential = 0.0;
		  
		  for(k=-indentation;k<indentation;k++)
		  {	  
		    potential += (-(double)(BYTE)cpPC_Buffer[(i+k)*640+(j)]);
	   	    potential += (-(double)(BYTE)cpPC_Buffer[(i)*640+(j+k)]);
			potential += (-(double)(BYTE)cpPC_Buffer[(i+k)*640+(j+k)]);
	   	    potential += (-(double)(BYTE)cpPC_Buffer[(i+k)*640+(j-k)]);

		  }

		  if(potential > maximum)
		  {
			maximum = potential;  
		  	lpy = i;
			lpx = j;
		  }
	   }

	   for(k=-indentation;k<indentation;k++)
	   {	  
		 cpPC_Buffer[(lpy+k)*640+(lpx)]	  = (char)255;
	   	 cpPC_Buffer[(lpy)*640+(lpx+k)]   = (char)255;
		 cpPC_Buffer[(lpy+k)*640+(lpx+k)] = (char)255;
	   	 cpPC_Buffer[(lpy+k)*640+(lpx-k)] = (char)255;
	   }


	   lStatus = DPK_XDCPutImage(G_lGrabImage,(HVOID *)cpPC_Buffer);   
	}

	GlobalUnlock(hPC_Buffer);
	GlobalFree(hPC_Buffer);

	// Display the results. */
    if(lStatus==P_SUCCESS)
    {
       DisplayImage (hWnd, G_lGrabImage);
	   *x = (double)(lpx-320) * _BASE_MASCHTAB_;
	   *y = (double)(lpy-240) * _BASE_MASCHTAB_;
	   set_x(*x);
	   set_y(*y);
	   set_angle(*angle);
	   set_checksum(0.0f,0.0f,0.0f);
		  
	   DBG_WriteText (G_lGrabImage, 20, 20,255,40,2, "*** Results ***");
		
	   get_x(x);
	   get_y(y);
       sprintf (report,"Central Point Bias X = %f, Y = %f",(float)(*x),(float)(*y));
       DBG_WriteText (G_lGrabImage,20,60,255,40,1,report);
		
	   //redraw main window.
	   PostMessage(hWnd,(UINT)WM_PAINT,(WPARAM)NULL,(LPARAM)NULL);
	}
	
	WaitCursor(FALSE);
	return;
}




VOID IDMHallCoordinates2 (HWND hWnd)
{
	double x,y;
	//HallCoordinates2(hWnd,&x,&y);
	//HallCoordinateBias2(hWnd,&x,&y);	  
	HallCoordinateBias(hWnd,&x,&y,(double)_BASE_MASCHTAB_, 120, 0.4);
}












VOID AxCo(HWND hWnd,int xmin,int xmax, int ymin, int ymax, int amin, int amax,int xc,int yc,int rc,int *mx,int *my,int *ma)
{
	//some parameters.
	int chip_r,chip_d;
    
	LONG st;
	int ix,iy,ia,ir,id;
	int imx,imy,ima,imr;
	double acc,macc;

	
	// get the pixels from image.
	
	char bfr[480][640];
	st = DPK_XDCGetImage(G_lGrabImage,(HVOID *)bfr);

	
	//matching.
	//for(x=0;x<640;x++)
	// for(y=0;y<480;y++)

	// ximg(x,y) = x<640/2 && y<480/2 ? 
	//    median4(ximg(x*2,y*2),ximg(x*2+1,y*2),ximg(x*2+1,y*2+1),ximg(x*2,y*2+1)) : 0;


	// get horizontal line.
	macc = 640.0 * 255.0;
	ix = xmin;
	for(iy=ymin;iy<ymax;iy+=3)
	 for(ia=amin;ia<amax;ia++)
	 {
	   acc = 0.0;
	   for(ir=0;ir<(xmax-xmin);ir++)
	   {
	   	 acc += abs(img(ix,iy,ir,ia)-img(ix,iy,ir+1,ia));
	   }

	   if(acc<macc && abs(yc-iy)<rc)
	   {
	      macc = acc;
	   	  imy = iy;
		  ima = ia;
	   }
	 }
	
	
	//get vertical line. 
	macc = 640.0 * 255.0;
	for(ir=0;ir<(xmax-xmin);ir++)
	{
	   acc = 0.0;
	   for(id =-80;id<80;id++) acc += abs(coimg(ix,imy,ir,ima,id) - coimg(ix,imy,ir,ima,id+1));
	   if(acc < macc && abs(xc-(ix+ir))<rc)
	   {
	      macc = acc;
		  imr = ir;
	   }
	} 
	 
	
	//privedenie coordinat k central point.
    imx = ix  +(int)(imr*cos(ima*0.01745329251994)); // + (int)(id*sin(ia*0.01745329251994));
    imy = imy +(int)(imr*sin(ima*0.01745329251994)); // + (int)(id*cos(ia*0.01745329251994));
    imr = 0;
	
	//here we have mx,my,ma.
	*mx = imx;
	*my = imy;
	*ma = ima;

	

	//get chip size.
	 
	
	chip_r = 75;
	chip_d = 55;
	//get chip reference.	
	for(ir=0;ir<chip_r;ir++)
	 for(id=0;id<chip_d;id++)
	 {
	    coimg(imx,imy,ir,ima,id) = ~coimg(imx,imy,ir,ima,id); 
	 }
	
	 //the line of chips.

	
	//put horizontal line. 
	for(ir=-100;ir<100;ir++) img(imx,imy,ir,ima) = (char)255;

	//put vert line.
	for(id=-80;id<80;id++) coimg(imx,imy,imr,ima,id) = (char)255;

	st = DPK_XDCPutImage(G_lOutputImage,(HVOID *)bfr);   
	if (st == P_SUCCESS) DisplayImage (hWnd, G_lOutputImage);
	return;
}




double Correlation(double image[640][480], double pattern[45][45], int di,int dj, int step)
{
   int i,j;
   double s=0.0,xx,yy,xy;


   xy = 0.0;
   xx = 0.0;
   yy = 0.0;

   for(i=0;i<45;i+=step)
	 for(j=0;j<45;j+=step)
	 {
	   xx = image[i+di-22][j+dj-22] * image[i+di-22][j+dj-22];
	   yy = pattern[i][j] * pattern[i][j];
	   //xx = sqrt(xx + yy);
	   xy = image[i+di-22][j+dj-22] * pattern[i][j];
	   
	   s += xy/(xx+yy);

	   //s += fabs(image[i+di-22][j+dj-22] - pattern[i][j]);
	 }

   return s;
}


double CorrPosition(double image[640][480], double pattern[45][45], int *di,int *dj)
{
   	int i,j, mi, mj, step, region;
	double s,max;
	
	//parameters.
	step = 5;
	region = 12;

	s=0.0;
	max = -999999999.0; 
	
	//1-st approximation.
	
	for(i=*di-region;i<=*di+region;i++)
	  for(j=*dj-region;j<=*dj+region;j++)
	  {
	    s = Correlation(image, pattern, i,j,step);
		if(s > max)
		{
		   max = s;
		   mi = i;
		   mj = j;
		}
	  }
	*di = mi;
	*dj = mj;

	
	//fine approximation.
	for(i=*di-step;i<=*di+step;i++)
	  for(j=*dj-step;j<=*dj+step;j++)
	  {
	    s = Correlation(image, pattern, i,j,1);
		if(s > max)
		{
		   max = s;
		   mi = i;
		   mj = j;
		}
	  }
	*di = mi-22;
	*dj = mj-22;


	return 2.0*max/(45*45);
}


void LocatePointer(int *dx,int *dy)
{
	int i,j, ix=77,jy=57;
	double corrfactor;

	WaitCursor(TRUE);
	feel(g_x);
	//normalize(g_x);
	
	//i=dx;
	//j=dy;
	
	corrfactor = CorrPosition(g_x, g_pattern, dx,dy);

	*dx = max(0,min(*dx,590));
	*dy = max(0,min(*dy,440));


	set_x(double(*dx-320)*_CHIP_MASCHTAB_);
	set_y(double(*dy-240)*_CHIP_MASCHTAB_);
	set_angle(corrfactor);
	
	for(i=*dx;i<*dx+45;i++)
     for(j=*dy;j<*dy+45;j++)
		g_x[i][j] = min(255,g_pattern[i-*dx][j-*dy] + 100);


	/*
	//pati = 0;
	for(i=dx-ix/2;i<= dx+ix/2;i++)
	{
	  //patj = 0;
	  for(j=dy-jy/2;j<= dy+jy/2;j++)
	  {
		g_x[i][j] = min(255,g_x[i][j]+100);// + g_pattern[i][j];
	    //if(pati < 45 && patj<45) 
		//{
	    //  g_x[i][j] = g_x[i][j] + g_pattern[pati][patj];
		//  patj++;
		//}
	  }
	  //pati++;
	}
	
	*/
	
	for(i=-30;i<30;i++) g_x[320][240+i] = g_x[320+i][240] = 255; 
	 
	show(g_x);
	write_text_double(20,20,255,100,1, "Closeness factor = ", corrfactor);

	WaitCursor(FALSE);
}



/////MMMMMMMMMMMMM//////////
VOID IDMFft (HWND hWnd)
{
  long LL0,LL1,RL0,RL1,TP0,TP1,BP0,BP1;
  
  LL0 = 30;
  LL1 = 350;
  RL0 = 30;
  RL1 = 350;
  TP0 = 30;
  TP1 = 510;
  BP0 = 30;
  BP1 = 510;

  AxesCoord(hWnd,&LL0,&LL1,&RL0,&RL1,&TP0,&TP1,&BP0,&BP1);
}





/*to double the code for any case*/
VOID dupIDMFft (HWND hWnd) //muliaj for using.....
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    LONG    lStatus;        /* Error status. */
    //INT     i;              /* Loop indice. */
    //INT     j;              /* Loop indice. */
	

	
	
	//statistic parameters.
	double dAccumulator;
	double dMinimumX, dMinimumY;
	double dWrk;
    //long lRightLineNumberMaximum, lLeftLineNumberMaximum;	
	long lRightLineNumberMinimum, lLeftLineNumberMinimum;
    //long lTopPixelNumberMaximum, lBottomPixelNumberMaximum;
	long lTopPixelNumberMinimum, lBottomPixelNumberMinimum;	
	
	
	// pixel parameters.

	long lPixelNumber, lLineNumber, lRightLineNumber, lLeftLineNumber;
	long lTopPixelNumber, lBottomPixelNumber;
	long lPixelValue;
	long lPointNumber;
	
	// get the pixels from image.
	
	HCHAR *cpPC_Buffer;
	static HGLOBAL hPC_Buffer;
	unsigned long ulImageSize = 640 * 480;

	hPC_Buffer = GlobalAlloc(GMEM_FIXED,ulImageSize);
	cpPC_Buffer = (HCHAR *) GlobalLock(hPC_Buffer);
	lStatus = DPK_XDCGetImage(G_lGrabImage,(HVOID *)cpPC_Buffer);

	if(lStatus >= P_ERROR)
	{
	

	
	   //look for x - cord.

   	   dMinimumX = 255.0 * 255.0 * 640.0;
	   lRightLineNumberMinimum = 0;
	   lLeftLineNumberMinimum = 0;

	   for (lLeftLineNumber = 30; lLeftLineNumber < 450; lLeftLineNumber+=10)
       for (lRightLineNumber = 30; lRightLineNumber < 450; lRightLineNumber+=10)
	   { 
		   dAccumulator = 0.0;
		   
		   lPointNumber = lLeftLineNumber  * 640;
		   lPixelValue = cpPC_Buffer[lPointNumber];
		   
		   for(lPixelNumber = 1; lPixelNumber < 640; lPixelNumber+=5)	
		   {	
             lLineNumber = (long)((double)lLeftLineNumber * (1.0 - (double)lPixelNumber/639.0) + 
			            (double)lRightLineNumber * (double)lPixelNumber/639.0);
             lPointNumber = lLineNumber * 640 + lPixelNumber;
               
			 dWrk = (cpPC_Buffer[lPointNumber] - lPixelValue);
		     dAccumulator += (dWrk*dWrk);
			   
	         lPixelValue = cpPC_Buffer[lPointNumber];
		   }

		   if ( dAccumulator < dMinimumX)
		   {   
			   dMinimumX = dAccumulator;
			   lRightLineNumberMinimum = lRightLineNumber;
               lLeftLineNumberMinimum = lLeftLineNumber;
		   }
	   }

	
	   

	   //look for y - coord.

	   dMinimumY = 255.0* 255.0 * 480.0;
	   lTopPixelNumberMinimum = 0;
	   lBottomPixelNumberMinimum = 0;

	   for (lTopPixelNumber = 30; lTopPixelNumber < 610; lTopPixelNumber+=5)
       for (lBottomPixelNumber = 30; lBottomPixelNumber < 610; lBottomPixelNumber+=7)
	   { 
		   dAccumulator = 0.0;
		   
		   lPointNumber = lTopPixelNumber;
		   lPixelValue = cpPC_Buffer[lPointNumber];
		   
		   for(lLineNumber = 1; lLineNumber < 480; lLineNumber+=3)	
		   {	
	           lPixelNumber = (long)((double)lTopPixelNumber * (1.0 - (double)lLineNumber/479.0) + 
			            (double)lBottomPixelNumber * (double)lLineNumber/479.0);
		       lPointNumber = lLineNumber * 640 + lPixelNumber;
               
			   dWrk = (cpPC_Buffer[lPointNumber] - lPixelValue);	           
			   dAccumulator += (dWrk*dWrk);
			   lPixelValue = cpPC_Buffer[lPointNumber];

		   

		   }

		   if ( dAccumulator < dMinimumY)
		   {   
			   dMinimumY = dAccumulator;
			   lTopPixelNumberMinimum = lTopPixelNumber;
               lBottomPixelNumberMinimum = lBottomPixelNumber;
		   }
	   }



	   // put x - line to the picture.
	   for(lPixelNumber = 0; lPixelNumber < 640; lPixelNumber+=1)	
	   {	
	      lLineNumber = (long)((double)lLeftLineNumberMinimum * (1.0 - (double)lPixelNumber/639.0) + 
			            (double)lRightLineNumberMinimum * (double)lPixelNumber/639.0);
	      lPointNumber = lLineNumber * 640 + lPixelNumber;
				   
	      cpPC_Buffer[lPointNumber] = (char)255;
	   }

	   // put y - line to the picture.
	   for(lLineNumber = 0; lLineNumber < 480; lLineNumber+=1)	
	   {	
	      lPixelNumber = (long)((double)lTopPixelNumberMinimum * (1.0 - (double)lLineNumber/479.0) + 
			            (double)lBottomPixelNumberMinimum * (double)lLineNumber/479.0);
	      lPointNumber = lLineNumber * 640 + lPixelNumber;
				   
	      cpPC_Buffer[lPointNumber] = (char)255;
	   }



	   lStatus = DPK_XDCPutImage(G_lGrabImage,(HVOID *)cpPC_Buffer);   
	}


	





	GlobalUnlock(hPC_Buffer);
	GlobalFree(hPC_Buffer);


    /*----------------------*/
    /* Display the results. */
    /*----------------------*/

    if (lStatus == P_SUCCESS)
    {
        DisplayImage (hWnd, G_lGrabImage);
    }

	// display internal angle between axes.
	{
	   char report[80];
	   double InternalAngle, ExternalAngleX, ExternalAngleY;
	   VECTOR X,Y,A;
	   POINT Cross;

	   X.start.x = 0;
	   X.end.x   = 640;
	   X.start.y = lLeftLineNumberMinimum;
	   X.end.y   = lRightLineNumberMinimum;

       Y.start.x = lTopPixelNumberMinimum;
       Y.end.x   = lBottomPixelNumberMinimum;
	   Y.start.y = 0;
	   Y.end.y   = 480;


	   A.start.x = 0;
	   A.start.y = 0;
	   A.end.x   = 640;
	   A.end.y   = 0;



       InternalAngle = 180.0 / 3.14152 * VectAngle(X,Y);
	   ExternalAngleX = 180.0 / 3.14152 * VectAngle(X,A);
	   ExternalAngleY = 180.0 / 3.14152 * VectAngle(Y,A);
	   sprintf(report,"Internal angle = %f\nExt.X = %f\nExt.Y = %f",
		   InternalAngle, ExternalAngleX, ExternalAngleY);
       

	   if (fabs(90.0 - InternalAngle) > 0.5)
	   {
	      strcat(report,"\nMeasurement is invalid. Repeate.");
	   }

	   {
		  char s[30];
	      Cross = VectCross(X,Y);
	      sprintf(s,"\nCross point = %d,%d", Cross.x,Cross.y);
		  strcat(report,s);
	   }

	   fMessageBox (hWnd, report, "Vision Data", MB_OK | MB_ICONINFORMATION);

	}
	
	return;
}


/*============================================================================
*
*  Name        : IDMHistogram
*  Author      : Pierre Landriau
*  Description : To do an image histogram.   The histogram is then drawn to
*                an image and the statistics written to the image.  The results
*                are dispalyed.
*
*  Parameters  :
*       hWnd            Handle to window.
*
*  Returns     : none
*
*===========================================================================*/

BOOL Histogram (HWND hWnd, ULONG *pulBuffer)
{
	int i;
	int max_mode = 0,min_mode = 0;
	ULONG min[2] = {0,0};
	

    LONG    lHistogramBufferNumber; /* Histogram buffer number. */
    LONG    lStatus;                /* Error status. */

    if ((lStatus = DBI_CreateHistogramBuffer (P_DATA_SIZE_BYTE,
        &lHistogramBufferNumber)) != P_SUCCESS)
    return FALSE;

    if ((lStatus = DBI_Histogram (G_lGrabImage, lHistogramBufferNumber,
        pulBuffer)) < P_ERROR)
    return FALSE;

	
	for(i=1;i<256;i++)
	{
	  pulBuffer[i] = 0;
	}

    return TRUE;
} /* End of My Histogram function. */



  
  
  
  
  
/*============================================================================
*
*  Name        : IDMBaseDetector
*  Author      : Paul Dourbal
*  Description : To peform base light level acquiring
*
*  Parameters  :
*       hWnd            Handle to window.
*
*  Returns     : double level of the base light (for cam #2) or 0.0 for cam #1
*
*===========================================================================*/


double IDMBaseDetector (HWND hWnd)
{

    LONG    lStatus;        /* Error status. */
		
	//statistic parameters.
	double dAccumulator;
	double dWrk;
	double level = 0.0;

	
	// pixel parameters.
	long lPixelValue;
	long lPointNumber;
	
	// get the pixels from image.
	
	HCHAR *cpPC_Buffer;
	static HGLOBAL hPC_Buffer;
	unsigned long ulImageSize = 640 * 480;

	WaitCursor(TRUE);
	
	hPC_Buffer = GlobalAlloc(GMEM_FIXED,ulImageSize);
	cpPC_Buffer = (HCHAR *) GlobalLock(hPC_Buffer);
	lStatus = DPK_XDCGetImage(G_lGrabImage,(HVOID *)cpPC_Buffer);

	if(lStatus >= P_ERROR)
	{
	
	   //check if the base is situated on the cariage of conveyor.
       
	   dAccumulator = 0.0;
	   for(lPointNumber=0; lPointNumber < (long)ulImageSize; lPointNumber++)
	   {
		 lPixelValue = (BYTE)cpPC_Buffer[lPointNumber];
	   	 dWrk = (double)lPixelValue;
	     dAccumulator += dWrk;
	   }
	   
	   dAccumulator /= (640.0 * 480.0 * 255.0);
		
	   //deliver the rezults.
	   set_x(dAccumulator);
	   
	   //trigger main application in case it has called this function asynchronousely.
	   set_checksum(0.0,0.0,0.0);

	   //Write out the base image light level the image.
	   get_x(&level);
	   
	}

	GlobalUnlock(hPC_Buffer);
	GlobalFree(hPC_Buffer);

    /*----------------------*/
    /* Display the results. */
    /*----------------------*/

    if (lStatus == P_SUCCESS)
    {
        DisplayImage (hWnd, G_lGrabImage);
    }

	WaitCursor(FALSE);
	
	return level;
}




  
  
/*============================================================================
*
*  Name        : IDMpacketfinder
*  Author      : Paul Dourbal
*  Description : To peform coord lines calculation upon image of the base packets.
*
*  Parameters  :
*       hWnd            Handle to window.
*
*  Returns     : none
*
*===========================================================================*/

VOID IDMPacketFinder (HWND hWnd)
{

    LONG    lStatus;        /* Error status. */
	CHAR rezult_string[80];
	
	
	//statistic parameters.
	double dAccumulator;
	double dMinimumX, dMinimumY,dMaximumY;
	double dLevel;
	double dWrk;
    long lRightLineNumberMinimum, lLeftLineNumberMinimum;
    long lTopPixelNumberMinimum, lBottomPixelNumberMinimum;	
	long lTopPointNumberMinimum, lBottomPointNumberMinimum;
	long lTopPointNumberMaximum, lBottomPointNumberMaximum;

	int i;
		
	// pixel parameters.

	long lPixelNumber, lLineNumber, lRightLineNumber, lLeftLineNumber;
	long lTopPointNumber;
	long lPixelValue;
	long lPointNumber;
	
	// get the pixels from image.
	
	HCHAR *cpPC_Buffer;
	static HGLOBAL hPC_Buffer;
	unsigned long ulImageSize = 640 * 480;

	WaitCursor(TRUE);

	hPC_Buffer = GlobalAlloc(GMEM_FIXED,ulImageSize);
	cpPC_Buffer = (HCHAR *) GlobalLock(hPC_Buffer);
	lStatus = DPK_XDCGetImage(G_lGrabImage,(HVOID *)cpPC_Buffer);

	if(lStatus >= P_ERROR)
	{
	
	   //check if the base is situated on the cariage of conveyor.
       
	   dAccumulator = 0.0;
	   for(lPointNumber=0; lPointNumber < (long)ulImageSize; lPointNumber++)
	   {
		 lPixelValue = (BYTE)cpPC_Buffer[lPointNumber];
	   	 dWrk = (double)lPixelValue;
	     dAccumulator += dWrk;
	   }
	   
	   dLevel = dAccumulator / (640.0 * 480.0 * 255.0);
	   set_x_angle(dLevel);
	   
		
	   //look for x bottom - cord.
	   
	   dMinimumX = 0.0;
	   lRightLineNumberMinimum = 0;
	   lLeftLineNumberMinimum = 0;

	   for (lLeftLineNumber = g_settings._LINE_0_; lLeftLineNumber < g_settings._LINE_1_; lLeftLineNumber+=g_settings._STEP_)
         for (lRightLineNumber = g_settings._LINE_0_; lRightLineNumber < g_settings._LINE_1_; lRightLineNumber+=g_settings._STEP_)
		 { 
		   dAccumulator = 0.0;
		   for(lPixelNumber = 0; lPixelNumber < 640; lPixelNumber+=g_settings._STEP_)	
		   {	
             lLineNumber = (long)((double)lLeftLineNumber * (1.0 - (double)lPixelNumber/639.0) + 
			                      (double)lRightLineNumber * (double)lPixelNumber/639.0);
             lPointNumber = lLineNumber * 640 + lPixelNumber;
             lPixelValue = (BYTE)cpPC_Buffer[lPointNumber];
			 dWrk = (double)lPixelValue;
			 
			 dAccumulator+=dWrk;
		   }
		   
		   if(dAccumulator > (double)dMinimumX)
		   {   
			     dMinimumX = (double)dAccumulator;
			     lRightLineNumberMinimum = lRightLineNumber;
                 lLeftLineNumberMinimum = lLeftLineNumber;
		   }
		 }

	
	   

	   //look for y - coord.

	   dMinimumY = 0.0;
	   lTopPixelNumberMinimum = 0;
	   lBottomPixelNumberMinimum = 0;
	   dMaximumY = 0.0;
	   lTopPointNumberMaximum = 0;
	   lBottomPointNumberMaximum = 0;

	   for(lPixelNumber = g_settings._PIXEL_0_; lPixelNumber < g_settings._PIXEL_1_; lPixelNumber+=1)	
	   {	
         lLineNumber = (long)((double)lLeftLineNumberMinimum * (1.0 - (double)lPixelNumber/639.0) + 
		                      (double)lRightLineNumberMinimum * (double)lPixelNumber/639.0);
         
		 
		 dAccumulator = 0.0;
		 lPointNumber = lLineNumber * 640 + lPixelNumber;
		 lTopPointNumber = lPointNumber;
		 lTopPointNumber = lPointNumber;

		 
		 lPointNumber += 7*640;
		 for(i=0; i<60 && lPointNumber<(479*640) ;i++)
		 {
			 lPixelValue = (BYTE)cpPC_Buffer[lPointNumber - 1];
			 
			 //maximum gradient measure.
			 //dWrk = (double)cpPC_Buffer[lPointNumber + 1] - (double)lPixelValue;
             
			 //minimum briteness measure.
			 dWrk = (double)cpPC_Buffer[lPointNumber];

			 dAccumulator+=dWrk;
			 lPointNumber += 640; 
         }
		 
		 if(dAccumulator < (double)dMinimumY)
		 {   
			 dMinimumY = (double)dAccumulator;
	         lBottomPointNumberMinimum = lPointNumber;
	         lTopPointNumberMinimum = lTopPointNumber;
		 }


	   	 if(dAccumulator > (double)dMaximumY)
		 {   
			 dMaximumY = (double)dAccumulator;
	         lBottomPointNumberMaximum = lPointNumber;
	         lTopPointNumberMaximum = lTopPointNumber;
		 }

	   
	   }
	   
	   
	   //deliver the rezults.
	   dWrk =(double)((lTopPointNumberMinimum % 640)-320)*_BASE_MASCHTAB_; 
	   set_x(dWrk);

	   dWrk = (double)(((lTopPointNumberMinimum / 640l) % 480)-240)*_BASE_MASCHTAB_;
       set_y(dWrk);
       
	   dWrk = (double)asin((double)(lLeftLineNumberMinimum - lRightLineNumberMinimum)/640.0);
	   set_angle(dWrk);
       
	   
	   set_checksum(0.0,0.0,0.0);


	   // put x - line to the picture.
	   for(lPixelNumber = g_settings._PIXEL_0_; lPixelNumber < g_settings._PIXEL_1_; lPixelNumber+=1)	
	   {	
	      lLineNumber = (long)((double)lLeftLineNumberMinimum * (1.0 - (double)lPixelNumber/639.0) + 
			            (double)lRightLineNumberMinimum * (double)lPixelNumber/639.0);
	      lPointNumber = lLineNumber * 640 + lPixelNumber;
				   
	      cpPC_Buffer[lPointNumber] = (char)255;
	   }

	   // put y - lines to the picture.
	   for( lPointNumber = lTopPointNumberMinimum;
	        lPointNumber < lBottomPointNumberMinimum; 
			lPointNumber += 640)	
	   {	
		  if(ulImageSize <= (ULONG)(lPointNumber+1)) break;
	      cpPC_Buffer[lPointNumber] = (char)0;
		  cpPC_Buffer[lPointNumber+1] = (char)255;

	   }


	/*		
	   if((lTopPointNumberMinimum % 640) > (lTopPointNumberMaximum % 640))		
	   for( lPointNumber = lTopPointNumberMaximum;
	        lPointNumber < lBottomPointNumberMaximum; 
			lPointNumber += 640 )	
	   {	
				   
	      cpPC_Buffer[lPointNumber] = (char)255;
		  cpPC_Buffer[lPointNumber+1] = (char)0;

	   }
	*/
	   
	   lStatus = DPK_XDCPutImage(G_lGrabImage,(HVOID *)cpPC_Buffer);

	   
	   //Write out the field of interest.
	   DBG_DrawLine(G_lGrabImage, g_settings._PIXEL_0_,g_settings._LINE_0_,g_settings._PIXEL_0_,g_settings._LINE_1_,255);
	   DBG_DrawLine(G_lGrabImage, g_settings._PIXEL_0_,g_settings._LINE_0_,g_settings._PIXEL_1_,g_settings._LINE_0_,255);
	   DBG_DrawLine(G_lGrabImage, g_settings._PIXEL_0_,g_settings._LINE_1_,g_settings._PIXEL_1_,g_settings._LINE_1_,255);
	   DBG_DrawLine(G_lGrabImage, g_settings._PIXEL_1_,g_settings._LINE_1_,g_settings._PIXEL_1_,g_settings._LINE_0_,255);
	   
	   
	   //Write out the base coordinats to the image.
		get_x(&dWrk);
	    sprintf (rezult_string, "Base X = %f", (float)dWrk);
        DBG_WriteText (G_lGrabImage, 20, 340,255,40,1,rezult_string);

	    get_y(&dWrk);
		sprintf (rezult_string, "Base Y = %f", (float)dWrk);
        DBG_WriteText (G_lGrabImage, 20, 352,255,40,1,rezult_string);

		get_angle(&dWrk);
	    sprintf (rezult_string, "Base Angle = %f", (float)dWrk);
        DBG_WriteText (G_lGrabImage, 20, 364,255,40,1,rezult_string);

		get_x_angle(&dWrk);
		sprintf(rezult_string,"Average Light Level = %f",dWrk);
	    DBG_WriteText (G_lGrabImage, 20, 376,255,40,1,rezult_string);

	}



	GlobalUnlock(hPC_Buffer);
	GlobalFree(hPC_Buffer);


    /*----------------------*/
    /* Display the results. */
    /*----------------------*/

    if (lStatus == P_SUCCESS)
    {
        DisplayImage (hWnd, G_lGrabImage);
    }

	WaitCursor(FALSE);

	return;
} /*end pocket location finder */



// second variant.



VOID IDMPacketFinder2 (HWND hWnd)
{

    LONG    lStatus;        /* Error status. */
	CHAR rezult_string[80];
	
	
	//statistic parameters.
	double dAccumulator;
	double dMinimumX, dMinimumY,dMaximumY;
	double dLevel;
	double dWrk;
    
	long lRightLineNumberMinimum, lLeftLineNumberMinimum;
    long lTopPixelNumberMinimum,  lBottomPixelNumberMinimum;	
	long lTopPointNumberMinimum,  lBottomPointNumberMinimum;
	long lTopPointNumberMaximum,  lBottomPointNumberMaximum;
	int i;
	
	// pixel parameters.

	long lPixelNumber, lLineNumber, lRightLineNumber, lLeftLineNumber;
    long lTopPointNumber;
	long lPixelValue;
	long lPointNumber;
	
	// get the pixels from image.
	
	HCHAR *cpPC_Buffer;
	static HGLOBAL hPC_Buffer;
	unsigned long ulImageSize = 640 * 480;

	
	WaitCursor(TRUE);
	
	hPC_Buffer = GlobalAlloc(GMEM_FIXED,ulImageSize);
	cpPC_Buffer = (HCHAR *) GlobalLock(hPC_Buffer);
	lStatus = DPK_XDCGetImage(G_lGrabImage,(HVOID *)cpPC_Buffer);

	if(lStatus >= P_ERROR)
	{
	
	   //check if the base is situated on the cariage of conveyor.
       
	   dAccumulator = 0.0;
	   for(lPointNumber=0; lPointNumber < (long)ulImageSize; lPointNumber++)
	   {
		 lPixelValue = (BYTE)cpPC_Buffer[lPointNumber];
	   	 dWrk = (double)lPixelValue;
	     dAccumulator += dWrk;
	   }
	   
	   dLevel = dAccumulator / (640.0 * 480.0 * 255.0);
	   set_x_angle(dLevel);
	   
		
	   //look for x bottom - cord.
	   
	   dMinimumX = 0.0;
	   lRightLineNumberMinimum = 0;
	   lLeftLineNumberMinimum = 0;

	   for (lLeftLineNumber = g_settings._LINE_0_; lLeftLineNumber < g_settings._LINE_1_; lLeftLineNumber+=g_settings._STEP_)
         for (lRightLineNumber = g_settings._LINE_0_; lRightLineNumber < g_settings._LINE_1_; lRightLineNumber+=g_settings._STEP_)
		 { 
		   dAccumulator = 0.0;
		   for(lPixelNumber = 0; lPixelNumber < 640; lPixelNumber+=g_settings._STEP_)	
		   {	
             lLineNumber = (long)((double)lLeftLineNumber * (1.0 - (double)lPixelNumber/639.0) + 
			                      (double)lRightLineNumber * (double)lPixelNumber/639.0);
             lPointNumber = lLineNumber * 640 + lPixelNumber;
             
			 //maximum light.
			 lPixelValue = (BYTE)cpPC_Buffer[lPointNumber];
			 dWrk = double(lPixelValue);

			 //gradient.
			 for(i=1;i<7;i++)
			 {
			     lPixelValue = (BYTE)(cpPC_Buffer[lPointNumber+640*i]-cpPC_Buffer[lPointNumber-640*i]);
				 dWrk += fabs(lPixelValue);
			 }
			 
			 //add potential.
			 dAccumulator+=dWrk;
		   }
		   
		   if(dAccumulator > (double)dMinimumX)
		   {   
			     dMinimumX = (double)dAccumulator;
			     lRightLineNumberMinimum = lRightLineNumber;
                 lLeftLineNumberMinimum = lLeftLineNumber;
		   }
		 }

	
	   

	   //look for y - coord.

	   dMinimumY = 99999999999999999999999999999999.0;
	   lTopPixelNumberMinimum = 0;
	   lBottomPixelNumberMinimum = 0;
	   dMaximumY = 0.0;
	   lTopPointNumberMaximum = 0;
	   lBottomPointNumberMaximum = 0;

	   for(lPixelNumber = g_settings._PIXEL_0_; lPixelNumber < g_settings._PIXEL_1_; lPixelNumber+=1)	
	   {	
         lLineNumber = (long)((double)lLeftLineNumberMinimum * (1.0 - (double)lPixelNumber/639.0) + 
		                      (double)lRightLineNumberMinimum * (double)lPixelNumber/639.0);
         
		 
		 dAccumulator = 0.0;
		 lPointNumber = lLineNumber * 640 + lPixelNumber;
		 lTopPointNumber = lPointNumber;
		 lTopPointNumber = lPointNumber;

		 
		 lPointNumber += 7*640;
		 for(i=0; i<60 && lPointNumber<(479*640) ;i++)
		 {
			 lPixelValue = (BYTE)cpPC_Buffer[lPointNumber];
			 
			 //maximum gradient measure.
			 dWrk = ((double)cpPC_Buffer[lPointNumber + 7]+1.0) / ((double)cpPC_Buffer[lPointNumber - 7] + 1.0);
             
			 //minimum briteness measure.
			 //dWrk = (double)cpPC_Buffer[lPointNumber];

			 dAccumulator+=dWrk;
			 lPointNumber += 640; 
         }
		 
		 if(dAccumulator < (double)dMinimumY)
		 {   
			 dMinimumY = (double)dAccumulator;
	         lBottomPointNumberMinimum = lPointNumber;
	         lTopPointNumberMinimum = lTopPointNumber;
		 }


	   	 if(dAccumulator > (double)dMaximumY)
		 {   
			 dMaximumY = (double)dAccumulator;
	         lBottomPointNumberMaximum = lPointNumber;
	         lTopPointNumberMaximum = lTopPointNumber;
		 }

	   
	   }
	   
	   
	   //deliver the rezults.
	   dWrk =(double)((lTopPointNumberMinimum % 640)-320)*_BASE_MASCHTAB_; 
	   set_x(dWrk);

	   dWrk = (double)(((lTopPointNumberMinimum / 640l) % 480)-240)*_BASE_MASCHTAB_;
       set_y(dWrk);
       
	   dWrk = (double)asin((double)(lLeftLineNumberMinimum - lRightLineNumberMinimum)/640.0);
	   set_angle(dWrk);
       
	   
	   set_checksum(0.0,0.0,0.0);


	   // put x - line to the picture.
	   for(lPixelNumber = g_settings._PIXEL_0_; lPixelNumber < g_settings._PIXEL_1_; lPixelNumber+=1)	
	   {	
	      lLineNumber = (long)((double)lLeftLineNumberMinimum * (1.0 - (double)lPixelNumber/639.0) + 
			            (double)lRightLineNumberMinimum * (double)lPixelNumber/639.0);
	      lPointNumber = lLineNumber * 640 + lPixelNumber;
				   
	      cpPC_Buffer[lPointNumber] = (char)255;
	   }

	   // put y - lines to the picture.
	  
	   //minimim.
	   
	   for( lPointNumber = lTopPointNumberMinimum;
	        lPointNumber < lBottomPointNumberMinimum; 
			lPointNumber += 640)	
	   {	
		  if(ulImageSize <= (ULONG)(lPointNumber+1)) break;
	      cpPC_Buffer[lPointNumber] = (char)0;
		  //cpPC_Buffer[lPointNumber+1] = (char)255;

	   }

	   
	
	   //maximum.
	   
	   /*		
	   for( lPointNumber = lTopPointNumberMaximum;
	        lPointNumber < lBottomPointNumberMaximum; 
			lPointNumber += 640 )	
	   {	
				   
	      cpPC_Buffer[lPointNumber] = (char)255;
		  //cpPC_Buffer[lPointNumber+1] = (char)0;

	   }
	   */
	   
	   lStatus = DPK_XDCPutImage(G_lGrabImage,(HVOID *)cpPC_Buffer);

	   
	   //Write out the field of interest.
	   DBG_DrawLine(G_lGrabImage, g_settings._PIXEL_0_,g_settings._LINE_0_,g_settings._PIXEL_0_,g_settings._LINE_1_,255);
	   DBG_DrawLine(G_lGrabImage, g_settings._PIXEL_0_,g_settings._LINE_0_,g_settings._PIXEL_1_,g_settings._LINE_0_,255);
	   DBG_DrawLine(G_lGrabImage, g_settings._PIXEL_0_,g_settings._LINE_1_,g_settings._PIXEL_1_,g_settings._LINE_1_,255);
	   DBG_DrawLine(G_lGrabImage, g_settings._PIXEL_1_,g_settings._LINE_1_,g_settings._PIXEL_1_,g_settings._LINE_0_,255);
	   
	   
	   //Write out the base coordinats to the image.
		get_x(&dWrk);
	    sprintf (rezult_string, "Base X = %f", (float)dWrk);
        DBG_WriteText (G_lGrabImage, 20, 340,255,40,1,rezult_string);

	    get_y(&dWrk);
		sprintf (rezult_string, "Base Y = %f", (float)dWrk);
        DBG_WriteText (G_lGrabImage, 20, 352,255,40,1,rezult_string);

		get_angle(&dWrk);
	    sprintf (rezult_string, "Base Angle = %f", (float)dWrk);
        DBG_WriteText (G_lGrabImage, 20, 364,255,40,1,rezult_string);

		get_x_angle(&dWrk);
		sprintf(rezult_string,"Average Light Level = %f",dWrk);
	    DBG_WriteText (G_lGrabImage, 20, 376,255,40,1,rezult_string);

	}



	GlobalUnlock(hPC_Buffer);
	GlobalFree(hPC_Buffer);


    /*----------------------*/
    /* Display the results. */
    /*----------------------*/

    if (lStatus == P_SUCCESS)
    {
        DisplayImage (hWnd, G_lGrabImage);
    }

	
	WaitCursor(FALSE);
	
	return;
} /*end pocket location finder (second type) */






VOID IDMPacketFinder3 (HWND hWnd)
{

    LONG    lStatus;        /* Error status. */
	CHAR rezult_string[80];
	
	
	//statistic parameters.
	double dAccumulator;
	double dMinimumX, dMinimumY,dMaximumY;
	double dLevel;
	double dWrk;
    long lRightLineNumberMaximum, lLeftLineNumberMaximum;	
	long lRightLineNumberMinimum, lLeftLineNumberMinimum;
	long lTopPixelNumberMinimum, lBottomPixelNumberMinimum;	
	long lTopPointNumberMinimum, lBottomPointNumberMinimum;
	long lTopPointNumberMaximum, lBottomPointNumberMaximum;

	int i;
	// pixel parameters.

	long lPixelNumber, lLineNumber, lRightLineNumber, lLeftLineNumber;
	long lTopPointNumber;
	long lPixelValue;
	long lPointNumber;
	
	// get the pixels from image.
	
	HCHAR *cpPC_Buffer;
	static HGLOBAL hPC_Buffer;
	unsigned long ulImageSize = 640 * 480;

	
	WaitCursor(TRUE);
	
	hPC_Buffer = GlobalAlloc(GMEM_FIXED,ulImageSize);
	cpPC_Buffer = (HCHAR *) GlobalLock(hPC_Buffer);
	lStatus = DPK_XDCGetImage(G_lGrabImage,(HVOID *)cpPC_Buffer);

	if(lStatus >= P_ERROR)
	{
	
	   //check if the base is situated on the cariage of conveyor.
       
	   dAccumulator = 0.0;
	   for(lPointNumber=0; lPointNumber < (long)ulImageSize; lPointNumber++)
	   {
		 lPixelValue = (BYTE)cpPC_Buffer[lPointNumber];
	   	 dWrk = (double)lPixelValue;
	     dAccumulator += dWrk;
	   }
	   
	   dLevel = dAccumulator / (640.0 * 480.0 * 255.0);
	   set_x_angle(dLevel);
	   
		
	   //look for x bottom - cord.
	   
	   dMinimumX = 0.0;
	   lRightLineNumberMinimum = 0;
	   lLeftLineNumberMinimum = 0;

	   for (lLeftLineNumber = g_settings._LINE_0_; lLeftLineNumber < g_settings._LINE_1_; lLeftLineNumber+=g_settings._STEP_)
         for (lRightLineNumber = g_settings._LINE_0_; lRightLineNumber < g_settings._LINE_1_; lRightLineNumber+=g_settings._STEP_)
		 { 
		   dAccumulator = 0.0;
		   for(lPixelNumber = 0; lPixelNumber < 640; lPixelNumber+=g_settings._STEP_)	
		   {	
             lLineNumber = (long)((double)lLeftLineNumber * (1.0 - (double)lPixelNumber/639.0) + 
			                      (double)lRightLineNumber * (double)lPixelNumber/639.0);
             lPointNumber = lLineNumber * 640 + lPixelNumber;
             lPixelValue = (BYTE)cpPC_Buffer[lPointNumber];
			 dWrk = (double)lPixelValue;
			 
			 dAccumulator+=dWrk;
		   }
		   
		   if(dAccumulator > (double)dMinimumX)
		   {   
			     dMinimumX = (double)dAccumulator;
			     lRightLineNumberMinimum = lRightLineNumber;
                 lLeftLineNumberMinimum = lLeftLineNumber;
		   }
		 }

	
	   

	   //look for y - coord.

	   dMinimumY = 99999999999999999999999999999999.0;
	   lTopPixelNumberMinimum = 0;
	   lBottomPixelNumberMinimum = 0;
	   dMaximumY = 0.0;
	   lTopPointNumberMaximum = 0;
	   lBottomPointNumberMaximum = 0;

	   for(lPixelNumber = g_settings._PIXEL_0_; lPixelNumber < g_settings._PIXEL_1_; lPixelNumber+=1)	
	   {	
         lLineNumber = (long)((double)lLeftLineNumberMinimum * (1.0 - (double)lPixelNumber/639.0) + 
		                      (double)lRightLineNumberMinimum * (double)lPixelNumber/639.0);
         
		 
		 dAccumulator = 0.0;
		 lPointNumber = lLineNumber * 640 + lPixelNumber;
		 lTopPointNumber = lPointNumber;
		 lTopPointNumber = lPointNumber;

		 
		 lPointNumber += 7*640;
		 for(i=0; i<60 && lPointNumber<(479*640) ;i++)
		 {
			 lPixelValue = (BYTE)cpPC_Buffer[lPointNumber];
			 
			 //maximum gradient measure.
			 dWrk = ((double)cpPC_Buffer[lPointNumber + 7]+1.0) / ((double)cpPC_Buffer[lPointNumber - 7] + 1.0);
             
			 //minimum briteness measure.
			 //dWrk = (double)cpPC_Buffer[lPointNumber];

			 dAccumulator+=dWrk;
			 lPointNumber += 640; 
         }
		 
		 if(dAccumulator < (double)dMinimumY)
		 {   
			 dMinimumY = (double)dAccumulator;
	         lBottomPointNumberMinimum = lPointNumber;
	         lTopPointNumberMinimum = lTopPointNumber;
		 }


	   	 if(dAccumulator > (double)dMaximumY)
		 {   
			 dMaximumY = (double)dAccumulator;
	         lBottomPointNumberMaximum = lPointNumber;
	         lTopPointNumberMaximum = lTopPointNumber;
		 }

	   
	   }
	   
	   
	   //deliver the rezults.
	   dWrk =(double)((lTopPointNumberMaximum % 640)-320)*_BASE_MASCHTAB_; 
	   set_x(dWrk);

	   dWrk = (double)(((lTopPointNumberMaximum / 640l) % 480)-240)*_BASE_MASCHTAB_;
       set_y(dWrk);
       
	   dWrk = (double)asin((double)(lLeftLineNumberMaximum - lRightLineNumberMaximum)/640.0);
	   set_angle(dWrk);
       
	   
	   set_checksum(0.0,0.0,0.0);


	   // put x - line to the picture.
	   for(lPixelNumber = g_settings._PIXEL_0_; lPixelNumber < g_settings._PIXEL_1_; lPixelNumber+=1)	
	   {	
	      lLineNumber = (long)((double)lLeftLineNumberMinimum * (1.0 - (double)lPixelNumber/639.0) + 
			            (double)lRightLineNumberMinimum * (double)lPixelNumber/639.0);
	      lPointNumber = lLineNumber * 640 + lPixelNumber;
				   
	      cpPC_Buffer[lPointNumber] = (char)255;
	   }

	   // put y - lines to the picture.
	  
	   //minimim.
	   /*
	   for( lPointNumber = lTopPointNumberMinimum;
	        lPointNumber < lBottomPointNumberMinimum; 
			lPointNumber += 640)	
	   {	
		  if(ulImageSize <= (ULONG)(lPointNumber+1)) break;
	      cpPC_Buffer[lPointNumber] = (char)0;
		  //cpPC_Buffer[lPointNumber+1] = (char)255;

	   }
	   */
	   
	
	   //maximum.
	   
	   for( lPointNumber = lTopPointNumberMaximum;
	        lPointNumber < lBottomPointNumberMaximum; 
			lPointNumber += 640 )	
	   {	
				   
	      cpPC_Buffer[lPointNumber] = (char)255;
		  //cpPC_Buffer[lPointNumber+1] = (char)0;

	   }
	   
	   
	   lStatus = DPK_XDCPutImage(G_lGrabImage,(HVOID *)cpPC_Buffer);

	   
	   //Write out the field of interest.
	   DBG_DrawLine(G_lGrabImage, g_settings._PIXEL_0_,g_settings._LINE_0_,g_settings._PIXEL_0_,g_settings._LINE_1_,255);
	   DBG_DrawLine(G_lGrabImage, g_settings._PIXEL_0_,g_settings._LINE_0_,g_settings._PIXEL_1_,g_settings._LINE_0_,255);
	   DBG_DrawLine(G_lGrabImage, g_settings._PIXEL_0_,g_settings._LINE_1_,g_settings._PIXEL_1_,g_settings._LINE_1_,255);
	   DBG_DrawLine(G_lGrabImage, g_settings._PIXEL_1_,g_settings._LINE_1_,g_settings._PIXEL_1_,g_settings._LINE_0_,255);
	   
	   
	   //Write out the base coordinats to the image.
		get_x(&dWrk);
	    sprintf (rezult_string, "Base X = %f", (float)dWrk);
        DBG_WriteText (G_lGrabImage, 20, 340,255,40,1,rezult_string);

	    get_y(&dWrk);
		sprintf (rezult_string, "Base Y = %f", (float)dWrk);
        DBG_WriteText (G_lGrabImage, 20, 352,255,40,1,rezult_string);

		get_angle(&dWrk);
	    sprintf (rezult_string, "Base Angle = %f", (float)dWrk);
        DBG_WriteText (G_lGrabImage, 20, 364,255,40,1,rezult_string);

		get_x_angle(&dWrk);
		sprintf(rezult_string,"Average Light Level = %f",dWrk);
	    DBG_WriteText (G_lGrabImage, 20, 376,255,40,1,rezult_string);

	}



	GlobalUnlock(hPC_Buffer);
	GlobalFree(hPC_Buffer);


    /*----------------------*/
    /* Display the results. */
    /*----------------------*/

    if (lStatus == P_SUCCESS)
    {
        DisplayImage (hWnd, G_lGrabImage);
    }

	
	WaitCursor(FALSE);
	
	return;
} /*end pocket location finder (3-d type) */





/*============================================================================
*
*  Name        : IDMFiltering
*  Author      : Paul Dourbal
*  Description : To peform contur finding of the base.
*
*  Parameters  :
*       hWnd            Handle to window.
*
*  Returns     : none
*
*===========================================================================*/
VOID IDMFiltering (HWND hWnd) //muliaj for using.....
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    LONG    lStatus;        /* Error status. */
	int i,is,is641,is2;
	//statistic parameters.
	// pixel parameters.

	long lPixelNumber, lLineNumber;
	long lPixelValue;
	double dPixelValue, dWrk, dtmp;
	long lPointNumber[4];
	
	// get the pixels from image.
	
	HCHAR *cpPC_Buffer;
	static HGLOBAL hPC_Buffer;
	unsigned long ulImageSize = 640 * 480;

	hPC_Buffer = GlobalAlloc(GMEM_FIXED,ulImageSize);
	cpPC_Buffer = (HCHAR *) GlobalLock(hPC_Buffer);
	lStatus = DPK_XDCGetImage(G_lGrabImage,(HVOID *)cpPC_Buffer);

	if(lStatus >= P_ERROR)
	{
		is = 1; // filtering amplitude.

		is641 = is * 641;
		is2 = is * 2;
	   
	   for (lLineNumber = is2; lLineNumber < 480; lLineNumber+=1)
	   { 
		   for(lPixelNumber = is; lPixelNumber < (640 - is); lPixelNumber+=1)	
		   {
			   
			   
               lPointNumber[0] = lLineNumber * 640 + lPixelNumber;
			   lPointNumber[1] = lPointNumber[0] - is641; // -1 line, -1 pixel.
			   lPointNumber[2] = lPointNumber[1] + is2;   // +2 pixels
			   lPointNumber[3] = lPointNumber[2] - is641; // - 1 line, -1 pixel.

			   dWrk = 0;
			   for (i=0;i<4;i++) dWrk += cpPC_Buffer[lPointNumber[i]]; // average.
			   
               
			   dWrk /= 4.0 ;
			   dPixelValue = 0.0;
			   
			   for (i=0;i<4;i++)
			   {  
				  dtmp = (cpPC_Buffer[lPointNumber[i]] - dWrk)/dWrk;
			      dPixelValue += fabs(dtmp);
			   }

			   lPixelValue = min(255,(long)dPixelValue);
               cpPC_Buffer[lPointNumber[3]] = (char)lPixelValue;

		   }
	   }

	   lStatus = DPK_XDCPutImage(G_lGrabImage,(HVOID *)cpPC_Buffer);   
	}


	GlobalUnlock(hPC_Buffer);
	GlobalFree(hPC_Buffer);


    /*----------------------*/
    /* Display the results. */
    /*----------------------*/

    if (lStatus == P_SUCCESS)
    {
        DisplayImage (hWnd, G_lGrabImage);
    }


	return;
}


/*============================================================================
*
*  Name        : IDMProcess
*  Author      : Pierre Landriau
*  Description : To filter the current image and display the results.
*
*  Parameters  :
*       hWnd            Handle to window.
*
*  Returns     : none
*
*===========================================================================*/

VOID IDMProcess (HWND hWnd)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    LONG    lStatus;        /* Error status. */

    /*--------------------*/
    /* Process the image. */
    /*--------------------*/

    WaitCursor (TRUE);
    if ((lStatus = DBI_Filter (G_lFilterType, G_lKernelSize,
        G_lGrabImage, G_lOutputImage)) != P_SUCCESS)
    {
        wsprintf (G_pszErrMsg, "Error %ld: Unable to process image.",
            lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
    } /* End if. */
    WaitCursor (FALSE);

    /*----------------------*/
    /* Display the results. */
    /*----------------------*/

    if (lStatus == P_SUCCESS)
    {
        DisplayImage (hWnd, G_lOutputImage);
    } /* End if. */
    return;

} /* End of the IDMProcess function. */

/*============================================================================
*
*  Name        : IDMArith
*  Author      : David McCormack
*  Description : To peform arithmetic operations upon image.
*
*  Parameters  :
*       hWnd            Handle to window.
*
*  Returns     : none
*
*===========================================================================*/

VOID IDMArith (HWND hWnd)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    LONG    lArithOp;           /* Operation for the arith function. */
    LONG    lStatus;            /* Function return status. */

    /*---------------------------------------------------*/
    /* Bring up the dialog box and get arith parameters. */
    /*---------------------------------------------------*/

    M_bArithCancel = FALSE;
    M_lpArithProc = MakeProcInstance ((FARPROC) DoArith, G_hInst);
    DialogBox (G_hInst, "ARITHDIALOG", hWnd, M_lpArithProc);
    FreeProcInstance (M_lpArithProc);
    if (M_bArithCancel)
    {
        return;
    } /* End if. */

    /*---------------------------------------------------------------*/
    /* If it is a one image operand then add the one operand offset. */
    /*---------------------------------------------------------------*/

    lArithOp = (LONG) M_iArithOperation;
    if (M_iArithImageType == 0)
    {
        lArithOp += P_ARITH_NEG;
    } /* End if. */

    /*------------------------------*/
    /* Perform the arith operation. */
    /*------------------------------*/

    if ((lStatus = DBI_Arith (G_lGrabImage, G_lGrabImage,
        G_lOutputImage, lArithOp, M_fArithScale, (FLOAT) M_lArithOffset,
        M_lArithConstant)) != P_SUCCESS)
    {
        wsprintf (G_pszErrMsg,
            "Error %ld: Unable to do arith operation.", lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        return;
    } /* End If. */

    /*--------------------------------*/
    /* Display the new image to user. */
    /*--------------------------------*/

    DisplayImage (hWnd, G_lOutputImage);

} /* End of the IDMArith function. */

/*============================================================================
*
*  Name        : DoArith
*  Author      : Pierre Landriau
*  Description : Processes messages for "Arith" dialog box
*
*  Parameters  :
*       hDlg                Window handle of the dialog box.
*       umessage            Type of message.  WM_INITDIALOG or WM_COMMAND.
*       wParam              Message-specific information.
*       lParam              More message-specific information.
*
*  Returns     : Error status
*
*  Context/Restrictions:
*===========================================================================*/

#ifdef WIN32
LRESULT APIENTRY DoArith (HWND hDlg, unsigned uMessage, WORD wParam,
        LONG lParam)
#else
BOOL FAR PASCAL __export DoArith (HWND hDlg, unsigned uMessage,
            WORD wParam, LONG lParam)
#endif

{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    INT     i;              /* Loop index. */

    BOOL    bStatus;        /* Variable for error flag. */

    CHAR    pszInput[30];   /* Text from user. */
    CHAR    pszScale[30];   /* Text of the scale to be printed. */

    /*------------------------------*/
    /* Process dialog box messages. */
    /*------------------------------*/

    switch (uMessage)
    {
        /*------------------------*/
        /* Initialize dialog box. */
        /*------------------------*/

        case WM_INITDIALOG:
            /*-------------------------------------------------*/
            /* Fill in drop down menu with the image operation */
            /* selection.   Highlight current selection.       */
            /*-------------------------------------------------*/

            for (i = 0; i <= P_ARITH_OPTYPES; i++)
            {
                SendDlgItemMessage (hDlg, IDM_IMAGENUMBEROPS, CB_INSERTSTRING,
                    i, (LPARAM) ((LPCSTR) M_pszArithImageTypes[i]));
            } /* End for. */
            SendDlgItemMessage (hDlg, IDM_IMAGENUMBEROPS, CB_SELECTSTRING,
                0, (LPARAM) ((LPCSTR) M_pszArithImageTypes[M_iArithImageType]));

            /*-------------------------------------------------*/
            /* Label the drop box with apporiate data options. */
            /*-------------------------------------------------*/

            for (i = 0; i <= P_ARITH_ONEIMGOPS; i++)
            {
                SendDlgItemMessage (hDlg, IDM_IMAGEOPERATION, CB_INSERTSTRING,
                    i, (LPARAM) ((LPCSTR) M_pszOneImgOps [i]));
            } /* End for.*/

            /*--------------------------------------*/
            /* Select the default item in the list. */
            /*--------------------------------------*/

            SendDlgItemMessage (hDlg, IDM_IMAGEOPERATION, CB_SELECTSTRING, 0,
                (LPARAM) ((LPCSTR) M_pszOneImgOps[M_iArithOperation]));

            /*--------------------------------------------*/
            /* Set the default user enterable parameters. */
            /*--------------------------------------------*/

            sprintf (pszScale, "%7.3f", M_fArithScale);
            SetDlgItemText (hDlg, IDM_SCALE, pszScale);
            SetDlgItemInt (hDlg, IDM_OFFSET, (INT) M_lArithOffset, FALSE);
            SetDlgItemInt (hDlg, IDM_CONSTANT, (INT) M_lArithConstant, FALSE);
            break;

            /*---------------------*/
            /* Received a command. */
            /*---------------------*/

        case WM_COMMAND:
            /*-------------------------------------------------------------*/
            /* "Ok" or "Cancel" box selected or system menu close command? */
            /*-------------------------------------------------------------*/

            switch (wParam)
            {
                /*-----------------------------------------*/
                /* Get user input.  Close dialog and exit. */
                /*-----------------------------------------*/

                case IDOK:
                    /*-----------------------------------------------------*/
                    /* Get the current type of operations to be performed. */
                    /*-----------------------------------------------------*/

                    GetDlgItemText (hDlg, IDM_IMAGEOPERATION, pszInput,
                        sizeof (pszInput));

                    /*-------------------------------------------*/
                    /* Translate the text to a number operation. */
                    /*-------------------------------------------*/

                    for (i = 0; i < P_ARITH_ONEIMGOPS; i++)
                    {
                        if (strcmp (M_pszOneImgOps[i], pszInput) == 0)
                        {
                            M_iArithOperation = i;
                            break;
                        } /* End if. */
                    } /* End for. */

                    /*----------------------------------------*/
                    /* Get the numeric value from the dialog. */
                    /*----------------------------------------*/

                    M_lArithConstant = GetDlgItemInt (hDlg, IDM_CONSTANT,
                        (BOOL FAR *) &bStatus, FALSE);
                    M_lArithOffset = GetDlgItemInt (hDlg, IDM_OFFSET,
                        (BOOL FAR *) &bStatus, FALSE);

                    /*---------------------------------------------------*/
                    /* Get the floating point value by doing conversion. */
                    /*---------------------------------------------------*/

                    GetDlgItemText (hDlg, IDM_SCALE, pszInput,
                        sizeof (pszInput));
                    M_fArithScale = (FLOAT) atof (pszInput);
                    EndDialog (hDlg, TRUE);
                    break;

                case IDCANCEL:
                    M_bArithCancel = TRUE;
                    EndDialog (hDlg, TRUE);
                    break;
            } /* End switch. */

            return (TRUE);
            break;
    } /* End switch. */

    /*---------------------------*/
    /* Didn't process a message. */
    /*---------------------------*/

    return (FALSE);

} /* End of the DoArith function. */

/**========================================================================**/
