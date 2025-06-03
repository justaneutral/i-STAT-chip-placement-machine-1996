#include <commdlg.h>
#include "dbdefs.h"
#include "camerarsc.h"
#include "mathem.h"
#include "resource.h"

#define P_DEFAULT_CAMERA_X_SIZE     640
#define P_DEFAULT_CAMERA_Y_SIZE     480

#define P_STRING_SIZE         256
#define P_XSCREEN_POS         0
#define P_YSCREEN_POS         0
#define P_ERRMSG_SIZE         P_STRING_SIZE
#define P_MAX_BOARDS          1

#define  APPNAME         "CLM Vision Subsystem"  /* Application name. */

#define MAXFILTERLEN P_STRING_SIZE

#define BOUND(x,min,max) ((x) < (min) ? (min) : ((x) > (max) ? (max) : (x)))

/*---------------------------------*/
/* Enable or disable menu options. */
/*---------------------------------*/

#define P_DISABLE_ITEM  (MF_DISABLED | MF_GRAYED | MF_BYPOSITION)
#define P_ENABLE_ITEM   (MF_ENABLED | MF_BYPOSITION)

#define P_DISABLE_SUB_ITEM  (MF_DISABLED | MF_GRAYED | MF_BYCOMMAND)
#define P_ENABLE_SUB_ITEM   (MF_ENABLED | MF_BYCOMMAND)

/**========================================================================**/
/*
* Common dialog structures and constants.
*/

typedef struct tagFOCHUNK
{
        OPENFILENAME of;
        CHAR szFile[P_STRING_SIZE];
        CHAR szFileTitle[P_STRING_SIZE];
} FOCHUNK;

typedef FOCHUNK FAR *LPFOCHUNK;
typedef FOCHUNK FAR *LPFSCHUNK;
typedef WORD (CALLBACK* FARHOOK) (HWND, UINT, WPARAM, LPARAM);
typedef WORD FAR *LPPARAMETERS;

/**========================================================================**/
/*
* Global variables for main module.
*/

#ifdef MAIN

HANDLE  G_hInst;
HWND    G_hWndMain;

CHAR    G_pszOpenName[P_STRING_SIZE];
CHAR    G_pszSaveName[P_STRING_SIZE];

CHAR   *G_pszMBTitleError = "XVL system error";/* Error message box title. */
CHAR   *G_pszMBTitleInfo = "XVL system info";  /* Info mesage box title. */
CHAR    G_pszErrMsg[P_ERRMSG_SIZE];      /* Error message text. */

INT     G_iBoardNumber = -1;        /* Which board to use. */
INT     G_iNumBoards = 0;           /* Number of board in the system. */

INT     G_iStartLine;          /* Start line of rectangle to display. */
INT     G_iStartPixel;         /* Start pixel of rectangle to display. */
INT     G_iEndLine;            /* End line of rectangle to display. */
INT     G_iEndPixel;           /* End pixel of rectangle to display. */

UINT    G_uiFilterTypeId;      /* Current Windows item number for filter type. */
UINT    G_uiKernelSizeId;      /* Current Windows item number for kernel ID. */


LONG    G_lCPSNumberSet[2];     /* Stored CPS indexes    */
LONG    G_lCPSNumber;          /* Index into CPS table. */
LONG    G_lPPSNumber;          /* Index into PPS table. */
LONG    G_lQGSNumber;          /* Index into PPS table. */
LONG    G_lPPMActiveFlag = FALSE;  /* PPM active flag. It is used for */
                                        /* PPM detection algorythm and can be */
                                        /* set to P_ERROR_NOT_PPM_MODULE. */

LONG    G_lCurrentImage = -1;  /* Currently displayed image. */
LONG    G_lFilterType;         /* Filter type for processing. */
LONG    G_lGrabImage;          /* Image to grab to. */
LONG    G_lKernelSize;         /* Kernel size for processing. */
LONG    G_lOutputImage;        /* Output for image processing. */

LONG    G_lBoardType;          /* XPG or FPG. */
LONG    G_lBusType;            /* PCI or ISA. */
LONG    G_lDisplayType;        /* Type of display board. */
LONG    G_lDisplayMode;        /* Type of display board. */
LONG    G_lBusMasteringFlag = FALSE;   /* TRUE or FALSE. */
LONG    G_lPIOModeFlag      = FALSE;   /* PIO Mode */
LONG    G_lNoSyncModeFlag   = FALSE;   /* Com Port DMA Sync Mode */
LONG    G_lNumComPort       = 1;       /* Number of ComPorts to use  */



BUFHEADER    G_FftBufHeader;       /* Fft buffer header information. */
LONG         G_lFftBufferNumber;   /* Buffer number for Fft data. */


#else

extern  HANDLE  G_hInst;
extern  HWND    G_hWndMain;

extern  CHAR    G_pszOpenName[P_STRING_SIZE];
extern  CHAR    G_pszSaveName[P_STRING_SIZE];

extern  CHAR   *G_pszMBTitleError;
extern  CHAR   *G_pszMBTitleInfo;
extern  CHAR    G_pszErrMsg[P_ERRMSG_SIZE];

extern  INT     G_iBoardNumber;
extern  INT     G_iNumBoards;
extern  INT     G_iStartLine;
extern  INT     G_iStartPixel;
extern  INT     G_iEndLine;
extern  INT     G_iEndPixel;

extern  UINT    G_uiFilterTypeId;
extern  UINT    G_uiKernelSizeId;

extern  LONG    G_lCPSNumberSet[2];
extern  LONG    G_lCPSNumber;
extern  LONG    G_lPPSNumber;
extern  LONG    G_lQGSNumber;
extern  LONG    G_lPPMActiveFlag;

extern  LONG    G_lCurrentImage;
extern  LONG    G_lFilterType;
extern  LONG    G_lGrabImage;
extern  LONG    G_lKernelSize;
extern  LONG    G_lOutputImage;

extern  LONG    G_lBoardType;
extern  LONG    G_lBusType;
extern  LONG    G_lDisplayType;
extern  LONG    G_lDisplayMode;
extern  LONG    G_lBusMasteringFlag;
extern  LONG    G_lPIOModeFlag;
extern  LONG    G_lNoSyncModeFlag;
extern  LONG    G_lNumComPort;

extern  BUFHEADER    G_FftBufHeader;
extern  LONG         G_lFftBufferNumber;

#endif

/**========================================================================**/
/*
* Function prototypes.
*/

/*---------------------------------*/
/* Function prototypes for camera.c */
/*---------------------------------*/

INT     PASCAL WinMain (HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpCmdLine,
                INT iCmdShow);
BOOL    InitApplication (HANDLE hInstance);
BOOL    InitInstance (HANDLE hInstance, INT iCmdShow);
VOID    WaitCursor (BOOL bWait);
LRESULT WINAPI MainWndProc (HWND hWnd, UINT uiMessage, WPARAM wParam,
                LPARAM lParam);
VOID    WMCreate (HWND hWnd);
VOID    CheckItem (HWND hWnd, UINT *puiCurrentItem, UINT uiNewSelection);

/*-----------------------------------*/
/* Function prototypes for csetup.c  */
/*-----------------------------------*/

VOID    ConfigInfo (HWND hWnd);
LONG    IDMLoadCPF (HWND hWnd, LPSTR lpszCameraName, LONG *plGrabStartPixel,
            LONG *plGrabNumPixels, LONG *plGrabStartLine, LONG *plGrabNumLines);
LONG    IDMLoadPPF (HWND hWnd);
VOID    IDMSelectBoard (HWND hWnd);
VOID    FillBoardSelectionStrings (INT iNumBoards);
LONG    SelectBoardProcess (HWND hWnd, INT iBoardNumber);
VOID    SetupItems (HWND hWnd);

#ifdef WIN32
    LRESULT APIENTRY About (HWND hDlg, unsigned uMessage, WORD wParam,
                LONG lParam);
    LRESULT APIENTRY SelectBoardDialog (HWND hDlg, unsigned uMessage,
                WORD wParam, LONG lParam);
#else
    BOOL FAR PASCAL __export About (HWND hDlg, unsigned uMessage,
                WORD wParam, LONG lParam);
    BOOL FAR PASCAL __export SelectBoardDialog (HWND hDlg,
                unsigned uMessage, WORD wParam, LONG lParam);
#endif

/*-----------------------------------*/
/* Function prototypes for WDIMAGE.C */
/*-----------------------------------*/

VOID    DisplayImage (HWND hWnd, LONG lImageNumber);
VOID    IDMAbortGrab (BOOL bSingleGrabFlag, LONG lQGSNumber);
VOID    IDMDiskToWindows (HWND hWnd);
VOID    IDMGrabImg (HWND hWnd, BOOL *pbSingleGrabFlag);
VOID    IDMHistogram (HWND hWnd);
VOID    IDMLoadImage (HWND hWnd);
VOID    IDMSaveImage (HWND hWnd);
VOID    IDMSMulImgGrab (HWND hWnd, BOOL *pbSingleGrabFlag);
VOID    IDMTestImage (HWND hWnd);
VOID    IDMTiffToWindows (HWND hWnd);
VOID    IDMWindowsToTiff (HWND hWnd);
VOID    IDMDspImgToTiff (HWND hWnd);
VOID    IDMTiffToDspImg (HWND hWnd);
VOID    SizeImageDisplay (HWND hWnd, LONG lImageNumber, INT *piStartLine,
            INT *piStartPixel, INT *piEndLine, INT *piEndPixel);

#ifdef WIN32
LRESULT APIENTRY GetNumFrames (HWND hDlg, unsigned uMessage, WORD wParam,
            LONG lParam);
#else
BOOL FAR PASCAL __export GetNumFrames (HWND hDlg, unsigned uMessage,
            WORD wParam, LONG lParam);
#endif

/*----------------------------------*/
/* Function prototypes for WDPROC.C */
/*----------------------------------*/

VOID IDMFft(HWND hWnd);
VOID IDMHallCoordinates(HWND hWnd);
VOID IDMHallCoordinates2(HWND hWnd);
VOID IDMPacketFinder(HWND hWnd);
VOID IDMPacketFinder2(HWND hWnd);
VOID IDMPacketFinder3(HWND hWnd);
VOID IDMSectionVision(HWND hWnd, VECTOR X, VECTOR Y);
VOID IDMFiltering (HWND hWnd);
VOID IDMProcess (HWND hWnd);
VOID IDMArith (HWND hWnd);

#ifdef WIN32
LRESULT APIENTRY DoArith (HWND hDlg, unsigned uMessage, WORD wParam,
            LONG lParam);
#else
BOOL FAR PASCAL __export DoArith (HWND hDlg, unsigned uMessage,
            WORD wParam, LONG lParam);
#endif

/*----------------------------------*/
/* Function prototypes for CMNDLG.C */
/*----------------------------------*/

VOID    InitializeStruct (WORD wCommDlgType, LPSTR lpStruct);
LONG    OpenFileDialog (WORD wCommDlgType, LPSTR lpszOpenName);
LONG    SaveFileDialog (WORD wCommDlgType, LPSTR lpszSaveName);


/*-----------------------------------*/
int fMessageBox( HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType);

/**=========================================================================**/
