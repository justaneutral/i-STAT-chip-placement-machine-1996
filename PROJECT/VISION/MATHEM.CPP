#include <windows.h>	//to define type "POINT"
#include <math.h>
#include "mathem.h"


POINT diffpoint(POINT A, POINT B)
{
   POINT rez;

   rez.x = A.x - B.x;
   rez.y = A.y - B.y;

   return rez;
}


POINT diffvect( VECTOR A)
{
  	POINT rez;
	
	rez	= diffpoint(A.end, A.start);

	return rez;
}


double PointScalProd(POINT A, POINT B)
{
	double rez;
	rez = A.x * B.x + A.y * B.y;
    return rez;
}


double VectScalProd(VECTOR A, VECTOR B)
{
	POINT rezA, rezB;
	double rez;
 	rezA = diffvect(A);
    rezB = diffvect(B);
 	rez = PointScalProd(rezA, rezB);
    return rez;
}



double PointModule(POINT A)
{

  double rez;
  rez = sqrt(A.x * A.x + A.y * A.y);
  return rez;

}


double VectModule(VECTOR A)
{
  POINT rezA;
  double rez;
  rezA = diffvect(A);
  rez = PointModule(rezA);
  return rez;

}


double PointAngle(POINT A, POINT B)
{
	double angle;
	angle = PointScalProd(A,B) / ( PointModule(A) * PointModule(B) );
	angle = acos( angle );
	return angle;
}


double VectAngle(VECTOR A, VECTOR B)
{
	double angle;
	POINT rezA, rezB;
	rezA = diffvect(A);
	rezB = diffvect(B);
	angle = PointAngle(rezA,rezB);
	return angle;
}

/*POINT VectCross(VECTOR X, VECTOR Y)
{
  double x,y,kx,ky,bx,by,t;
  POINT rez;
  
  if(X.end.x == X.start.x)
	if(Y.end.x == Y.start.x) //  lines || to e.a.
	{ 
	  x = 0;
	  y = 0;
	}
    else  //lines |\ or |/ to e.a.
	{
	  x = X.start.x;
	  t = Y.end.x - Y.start.x; //temp.
	  ky = (Y.end.y - Y.start.y) / t;
	  by = Y.start.y - ky;
	  y = x * ky + by;
	}
  else
	if(Y.end.x == Y.start.x) //  lines |/ or |\ to e.a.
	{ 
	  x = Y.start.x;
	  t = X.end.x - X.start.x; //temp.
	  kx = (X.end.y - X.start.y) / t;
	  bx = X.start.y - kx;
	  y = x*kx + bx;
	}
    else
	{
	  kx = (double)(X.end.y - X.start.y) / (double)(X.end.x - X.start.x);
	  ky = (double)(Y.end.y - Y.start.y) / (double)(Y.end.x - Y.start.x);
	  
	  if (kx == ky) //lines = to e.a.
	  {
		 x = 0.0;
		 y = 0.0;
	  }
	  else   //lines > or /- or = or to e.a.
	  {
	    bx = X.start.y - kx;
	    by = Y.start.y - ky;
		
		t = (ky - kx);
		x = (bx - by) / t;		
		y = (ky * bx - kx * by) / t;
	  }
	}
  rez.x = (LONG)x;
  rez.y = (LONG)y;

  return rez;
}*/


int order(int index,int a, int b,int c,int d)
{
  int i,j,t,par[4];

  par[0] = a;
  par[1] = b;
  par[2] = c;
  par[3] = d;

  for(i=0;i<4;i++)
   for(j=1;j<4;j++)
	   if(par[j-1]>par[j])
	   { t = par[j];
	     par[j] = par[j-1];
		 par[j-1] = t;
	   } 

  return par[index-1];
}


POINT VectCross(VECTOR X, VECTOR Y)
{
  int x,y;
  POINT rez;
  double Xdist,Ydist,Xgrad,Ygrad,Min;
  
  int ix0,ix1,iy0,iy1;

  Xdist = sqrt((X.start.x-X.end.x)*(X.start.x-X.end.x)+(X.start.y-X.end.y)*(X.start.y-X.end.y));
  Ydist = sqrt((Y.start.x-Y.end.x)*(Y.start.x-Y.end.x)+(Y.start.y-Y.end.y)*(Y.start.y-Y.end.y));

  ix0 = order(2,X.start.x,Y.start.x,X.end.x,Y.end.x);
  ix1 = order(3,X.start.x,Y.start.x,X.end.x,Y.end.x);
  iy0 = order(2,X.start.y,Y.start.y,X.end.y,Y.end.y);
  iy1 = order(3,X.start.y,Y.start.y,X.end.y,Y.end.y);  

  Min = 7777777.0;
  for(x=ix0;x<=ix1;x++)
   for(y=iy0;y<=iy1;y++)
   {
   	 Xgrad = sqrt((X.start.x-x)*(X.start.x-x)+(X.start.y-y)*(X.start.y-y)) +
   	         sqrt((x-X.end.x)*(x-X.end.x)+(y-X.end.y)*(y-X.end.y)) - Xdist;

   	 Ygrad = sqrt((Y.start.x-x)*(Y.start.x-x)+(Y.start.y-y)*(Y.start.y-y)) +
   	         sqrt((x-Y.end.x)*(x-Y.end.x)+(y-Y.end.y)*(y-Y.end.y)) - Ydist;

   	 if((Xgrad+Ygrad)<Min) 
	 {
		 Min=Xgrad+Ygrad;
	     rez.x = (LONG)x;
		 rez.y = (LONG)y;
	 }
   }

   return rez;
}





int DrowVector(char *screen, POINT Sizes, VECTOR X, VECTOR Y, char color)
{
   long i,point;
	
   	   // put x - line to the picture.
	   for(i = 0; i < 640; i+=1)	
	   {	
	      point = (long)((double)X.start.y * (1.0 - (double)i/Sizes.x) + (double)X.end.y * (double)i/Sizes.x);
	      point= point * Sizes.x + i;
				   
	      screen[point] = (char)color;
	   }

	   // put y - line to the picture.
	   for(i = 0; i < 480; i+=1)	
	   {	
	      point = (long)((double)Y.start.x * (1.0 - (double)i/Sizes.y) + (double)Y.end.x * (double)i/Sizes.y);
	      point = point * Sizes.x + i;
				   
	      screen[point] = (char)color;
	   }

   return 0;
}
