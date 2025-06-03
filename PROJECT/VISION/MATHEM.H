#if !defined ___mathem___
#define ___mathem___

typedef struct _CHIP
{
	unsigned short curpos[2];
	unsigned short type;
  
	struct _CHIP *left;
	struct _CHIP *right;
	struct _CHIP *top;
	struct _CHIP *bottom;

} CHIP;


typedef struct _VECTOR
{
	POINT start;
	POINT end;

} VECTOR;



// mathem function prototypes.

POINT diffpoint(POINT A, POINT B);
POINT diffvect( VECTOR A);
double PointScalProd(POINT A, POINT B);
double VectScalProd(VECTOR A, VECTOR B);
double PointModule(POINT A);
double VectModule(VECTOR A);
double PointAngle(POINT A, POINT B);
double VectAngle(VECTOR A, VECTOR B);
POINT VectCross(VECTOR X, VECTOR Y);
int DrowVector(char *screen, POINT Sizes, VECTOR X, VECTOR Y, char color);
#endif
