#define MAIN

#include <windows.h>

//parameters. 
#include "settings.h"
extern Settings g_settings;


extern "C"
{
#include "dpdefs.h"     /* XPG parameters. */
#include "dpptypes.h"   /* XPG function prototypes. */
#include "dberrors.h"   /* XPG error codes and warnings definitions. */
}

#ifdef WIN32
    #include    "dpgioctl.h"
#endif

#include "camera.h"      /* Specific to this program. */


extern "C"
{
#include "../commondll/commondll.h"
}

//RPC server start process prototype.
extern BOOL start_rpc_server(VOID);
//RPC connect. func.
double camera_connector(short manipulator)
{
  return -1.0f;
}


VOID TopEdge(HWND hWnd);
VOID BottomEdge(HWND hWnd);
VOID LeftEdge(HWND hWnd);
VOID RightEdge(HWND hWnd);
double IDMBaseDetector(HWND hWnd);
void LocatePointer(int *dx,int *dy);



HWND g_hWnd;
double (*g_x)[480];
double (*g_y)[480];
double (*g_pattern)[45];

int g_fiducial_dx = 320;
int g_fiducial_dy = 240;


BOOL init_g_x()
{
  g_x= new double[640][480];
  if(g_x == NULL) return FALSE;
  int i,j;
  for(i=0;i<640;i++) 
   for(j=0;j<480;j++)
     g_x[i][j]=0.0;
  return TRUE;
}


BOOL init_g_y()
{
  g_y= new double[640][480];
  if(g_y == NULL) return FALSE;
  int i,j;
  for(i=0;i<640;i++) 
   for(j=0;j<480;j++)
     g_y[i][j]=0.0;
  return TRUE;
}


BOOL init_g_pattern()
{
  g_pattern= new double[45][45];
  if(g_pattern == NULL) return FALSE;
  int i,j;
  for(i=0;i<45;i++) 
   for(j=0;j<45;j++)
     g_y[i][j]=0.0;
  return TRUE;
}


void delete_g_x()
{
  	if(g_y) delete [] g_x;
	g_x = NULL;
}


void delete_g_y()
{
  	if(g_y) delete [] g_y;
	g_y = NULL;
}


void delete_g_pattern()
{
  	if(g_pattern) delete [] g_pattern;
	g_pattern = NULL;
}



VECTOR X = {0,239,639,239},Y = {320,0,320,479};

INT PASCAL WinMain (HANDLE hInstance, HANDLE hPrevInstance, LPSTR lpCmdLine,
                INT iCmdShow)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    MSG     qMsg;       /* Message. */

    /*---------------------------------*/
    /* Other instances of app running? */
    /*---------------------------------*/

   if(!init_g_x() || !init_g_y() || !init_g_pattern()) return FALSE;

    if (!hPrevInstance)
    {
        /*---------------------------*/
        /* Initialize shared things. */
        /*---------------------------*/

        if (!InitApplication (hInstance))
        {
            /*--------------------------------*/
            /* Exits if unable to initialize. */
            /*--------------------------------*/

            return (FALSE);
        } /* End if. */
    } /* End if. */

    /*------------------------------------------------------------*/
    /* Perform initializations that apply to a specific instance. */
    /*------------------------------------------------------------*/

    if (!InitInstance (hInstance, iCmdShow))
    {
        return (FALSE);
    } /* End if. */

    /*--------------------------------------------------------------------*/
    /* Acquire and dispatch messages until a WM_QUIT message is received. */
    /*--------------------------------------------------------------------*/


	while (GetMessage (&qMsg,  /* message structure                      */
        NULL,                  /* handle of window receiving the message */
        0,                     /* lowest message to examine              */
        0))                    /* highest message to examine             */
    {
        TranslateMessage (&qMsg);   /* Translates virtual key codes. */
        DispatchMessage (&qMsg);    /* Dispatches message to window. */
    } /* End while. */

    /*-----------------------------------------*/
    /* Returns the value from PostQuitMessage. */
    /*-----------------------------------------*/

	if(g_pattern) delete_g_pattern();
	if(g_x) delete_g_x();
	if(g_y) delete_g_y();

    return (qMsg.wParam);

} /* End of the WinMain function. */

BOOL InitApplication (HANDLE hInstance)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    WNDCLASS  qWindowClass;

    /*------------------------------------------------*/
    /* Fill in window class structure with parameters */
    /* that describe the main window.                 */
    /*------------------------------------------------*/

    qWindowClass.style = CS_HREDRAW | CS_VREDRAW | CS_BYTEALIGNCLIENT; /* Class style(s). */
    qWindowClass.lpfnWndProc = MainWndProc;      /* Function to retrieve messages for */
                                                 /*   windows of this class. */
    qWindowClass.cbClsExtra = 0;                 /* No per-class extra data. */
    qWindowClass.cbWndExtra = sizeof (COLORREF); /* Sets number of extra bytes */
    qWindowClass.hInstance = hInstance;          /* Application that owns the class. */
    qWindowClass.hIcon = LoadIcon (hInstance, "XVLICON");
    qWindowClass.hCursor = LoadCursor (NULL, IDC_ARROW);
    qWindowClass.hbrBackground = GetStockObject (WHITE_BRUSH);     /* Brush. */
    qWindowClass.lpszMenuName =  "XVLMenu";      /* Name of menu resource in .RC file. */
    qWindowClass.lpszClassName = "XVLWndClass";  /* Name used in call to CreateWindow. */

    /*------------------------------------------------------------*/
    /* Register the window class and return success/failure code. */
    /*------------------------------------------------------------*/

    return (RegisterClass (&qWindowClass));

} /* End of the InitApplication function. */



BOOL InitInstance (HANDLE hInstance, INT iCmdShow)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    HDC         hDC;            /* Handle to device context. */
    HWND        hWnd;           /* Main window handle. */

    INT     iVgaMaxColours;     /* Maximum number of colours for vga. */

    LONG    lStatus;            /* Error status. */
    ULONG   ulValue;            /* For getting info back from DPU_GetHostInfo. */

    /*----------------------------------------------*/
    /* Save the instance handle in static variable, */
    /* which will be used in many subsequence calls */
    /* from this application to Windows.            */
    /*----------------------------------------------*/

    G_hInst = hInstance;

    /*-----------------------------------------------------*/
    /* Create a main window for this application instance. */
    /*-----------------------------------------------------*/

    hWnd = CreateWindow (
        "XVLWndClass",          /* See RegisterClass() call.          */
        APPNAME,                /* Text for window title bar.         */
        WS_OVERLAPPEDWINDOW | WS_VSCROLL | WS_HSCROLL |
             WS_MAXIMIZEBOX | WS_MINIMIZEBOX, /* Window style.        */
        //CW_USEDEFAULT
		1023-P_DEFAULT_CAMERA_X_SIZE,/* Horizontal position.       */
        //CW_USEDEFAULT,
		767-P_DEFAULT_CAMERA_Y_SIZE,/* Vertical position.         */
        P_DEFAULT_CAMERA_X_SIZE,/* Default width.                     */
        P_DEFAULT_CAMERA_Y_SIZE,/* Default height.                    */
        NULL,                   /* Overlapped windows have no parent. */
        NULL,                   /* Use the window class menu.         */
        hInstance,              /* This instance owns this window.    */
        NULL);                  /* Pointer not needed.                */

    /*---------------------------------------------------*/
    /* If window could not be created, return "failure". */
    /*---------------------------------------------------*/

    if (!hWnd)
    {
        return (FALSE);
    } /* End if. */
    G_hWndMain = hWnd;

    /*--------------------------------------------------*/
    /* Make the window visible; update its client area. */
    /*--------------------------------------------------*/

    ShowWindow (hWnd, iCmdShow);
    UpdateWindow (hWnd);


   	set_hwnd(G_hWndMain);

	
	
	
	
	/*--------------------------------------*/
    /* Initialize the communication kernel. */
    /* Ignore warnings.                     */
    /*--------------------------------------*/

    if ((lStatus = DPK_InitPCK (P_MAX_BOARDS)) < P_ERROR)
    {
        wsprintf (G_pszErrMsg,
            "Error %ld: Unable to initialize PCK.", lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        return FALSE;
    } /* End if. */

    DPU_GetHostInfo (P_HOST_INFO_NUM_BOARDS, &ulValue);
    G_iNumBoards = (INT) ulValue;

    if (G_iNumBoards != 0)
    {
        FillBoardSelectionStrings (G_iNumBoards);
    }
    else
    {
        wsprintf (G_pszErrMsg, "Error: No boards found");
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        return FALSE;
    } /* End if. */

    /*----------------------------------------------------------------*/
    /* Setup environment variable and load the out file to the board. */
    /* Exit on error, report warning and continue.                    */
    /*----------------------------------------------------------------*/

    DPK_XCCSetWaitMode (P_WAIT_COMPLETE);
    if ((lStatus = DPU_GetEnvironmentVar ()) != P_SUCCESS)
    {
        wsprintf (G_pszErrMsg,
            "Error %ld: Unable to get environment info.", lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        return FALSE;
    } /* End if. */

    /*------------------------------------------------------*/
    /* If there is only one board available then select it. */
    /* Disable the board selection item.                    */
    /*------------------------------------------------------*/

    if (G_iNumBoards == 1)
    {
        G_iBoardNumber = 0;
        if ((lStatus = SelectBoardProcess (hWnd, G_iBoardNumber))
            != P_SUCCESS)
        {
            return FALSE;
        } /* End if. */
        //EnableMenuItem (GetMenu (hWnd), IDM_SELECT_BOARD, P_DISABLE_SUB_ITEM);
    }
    else
    {
        SetupItems (hWnd);
    } /* End if. */

    /*-------------------*/
    /* Load vga palette. */
    /*-------------------*/

    if ((hDC = GetDC (hWnd)) == (HDC) NULL)
    {
        wsprintf (G_pszErrMsg, "Error %ld: Unable to get HDC.", lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        return P_ERROR_WINDOWS;
    } /* End if. */

    iVgaMaxColours = GetDeviceCaps (hDC, BITSPIXEL);

    if ((lStatus = DPW_LoadWindowsPalette (hDC, "palette.0"))
        != P_SUCCESS)
    {
        wsprintf (G_pszErrMsg,
            "Error %ld: Unable to get palette from disk.", lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        return P_ERROR_WINDOWS;
    } /* End if. */
    ReleaseDC (hWnd, hDC);
    return (start_rpc_server()); //call RPC server process to start.

} /* End of the InitInstance function. */



LRESULT WINAPI MainWndProc (HWND hWnd, UINT uiMessage, WPARAM wParam,
            LPARAM lParam)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    FARPROC         lpProcAbout;/* About box procedure. */
    HDC             hDC;        /* Handle to display context. */
    HGLOBAL         hGlobalMem; /* Handle to global memory object. */
    PAINTSTRUCT     qPaintInfo;
    RECT            qRect;      /* RECT structure for resize (etc...) */

    BOOL    bSingleGrabFlag;    /* TRUE if only one image to be grabbed. */
    BOOL    bUpdate;            /* TRUE if event requires screen update */

    FCHAR   *fpcBuffer;         /* Buffer to hold a full image. */

    INT     iScrollMin;         /* Scroll bar range minimum value. */
    INT     iScrollMax;         /* Scroll bar range maximum value. */

    LONG    lGrabStartPixel;    /* First pixel to grab. */
    LONG    lGrabNumPixels;     /* Number of pixels to grab. */
    LONG    lGrabStartLine;     /* First line to grab. */
    LONG    lGrabNumLines;      /* Number of lines to grab. */
    LONG    lNumPortOption;
    LONG    lStatus;            /* Error status. */

    
	/*----------------------*/
    /* Process the message. */
    /*----------------------*/
	
    switch (uiMessage)
    {
        /*---------------------------------------------------*/
        /* Initialize the state of the top level menu items. */
        /* Only the CPF item is active.   The others are     */
        /* disabled until a .cpf file has been loaded.       */
        /*---------------------------------------------------*/

        case WM_CREATE:
            WMCreate (hWnd);
            break;

        /*-----------------------------------------*/
        /* Message: command from application menu. */
        /*-----------------------------------------*/

        case WM_COMMAND:
            switch (wParam)
            {
                /*-----------------*/
                /* Select a board. */
                /*-----------------*/

                case IDM_SELECT_BOARD:
                    IDMSelectBoard (hWnd);
                    break;

                /*------------------------*/
                /* File exit menu option. */
                /*------------------------*/

                case IDM_EXIT:
                    DPK_EndPCK ();
                    PostQuitMessage (0);
                    break;

                /*--------------------------------*/
                /* Construct a message box with   */
                /* the configuration information. */
                /*--------------------------------*/

                case IDM_CONFIGURATION:
                    ConfigInfo (hWnd);
                    break;

                /*------------*/
                /* About box. */
                /*------------*/

                case IDM_ABOUT:
                    lpProcAbout = MakeProcInstance ((FARPROC) About,
                        G_hInst);
                    DialogBox (G_hInst, "AboutBox", hWnd, lpProcAbout);
                    FreeProcInstance (lpProcAbout);
                    break;

                /*----------------------------*/
                /* Select a CPF file to load. */
                /*----------------------------*/

                case IDM_LOAD_CPF:
                    IDMLoadCPF (hWnd, (LPSTR)"sony1.cpf",&lGrabStartPixel, &lGrabNumPixels,
                        &lGrabStartLine, &lGrabNumLines);
                    break;

				/*----------------------------*/
				/* Select camera # 1          */
				/*----------------------------*/

				case IDM_SELECT_CAMERA_1:
                    IDMLoadCPF (hWnd, (LPSTR)"sony1.cpf",&lGrabStartPixel, &lGrabNumPixels,
                        &lGrabStartLine, &lGrabNumLines);
					break;

				/*----------------------------*/
				/* Select camera # 2          */
				/*----------------------------*/

				case IDM_SELECT_CAMERA_2:
                    IDMLoadCPF (hWnd, (LPSTR)"sony2.cpf",&lGrabStartPixel, &lGrabNumPixels,
                        &lGrabStartLine, &lGrabNumLines);
					break;

				
				/*----------------------------*/
                /* Select a PPF file to load. */
                /*----------------------------*/

                case IDM_LOAD_PPF:
                    IDMLoadPPF (hWnd);
                    break;

                /*-------------------------------------------------------*/
                /* Turn bus mastering on or off.                         */
                /* Note that this item is only valid on a PCI bus board. */
                /*-------------------------------------------------------*/

                case IDM_TOGGLE_BUS_MASTERING:
                    if (G_lBusMasteringFlag)
                    {
                        CheckMenuItem (GetMenu (hWnd), IDM_TOGGLE_BUS_MASTERING,
                            MF_UNCHECKED);
                        G_lBusMasteringFlag = FALSE;
                    }
                    else
                    {
                        CheckMenuItem (GetMenu (hWnd), IDM_TOGGLE_BUS_MASTERING,
                            MF_CHECKED);
                        G_lBusMasteringFlag = TRUE;
                    } /* End if, else. */
                    DPK_UseBusMasteringMode (G_lBusMasteringFlag);
                    break;

                /*-------------------------------------------------------*/
                /* Turn Synchronous PIO transfer mode on or off.         */
                /* Note that this item is only valid on a PCI bus board. */
                /*-------------------------------------------------------*/

                case IDM_TOGGLE_PIO_MODE:
                    if (G_lPIOModeFlag)
                    {
                        CheckMenuItem (GetMenu (hWnd), IDM_TOGGLE_PIO_MODE,
                            MF_UNCHECKED);
                        G_lPIOModeFlag = FALSE;
                        lNumPortOption = G_lNumComPort;
                    }
                    else
                    {
                        CheckMenuItem (GetMenu (hWnd), IDM_TOGGLE_PIO_MODE,
                            MF_CHECKED);
                        G_lPIOModeFlag = TRUE;
                        lNumPortOption = G_lNumComPort | P_XDC_DSPPIO_MODE;
                    } /* End if, else. */

                    DBK_XDCSetComPortNumber (lNumPortOption);
                    break;

                /*-------------------------------------------------------*/
                /* Turn Synchronous PIO transfer mode on or off.         */
                /* Note that this item is only valid on a PCI bus board. */
                /*-------------------------------------------------------*/

                case IDM_TOGGLE_NOSYNC_MODE:
                    if (G_lNoSyncModeFlag)
                    {
                        CheckMenuItem (GetMenu (hWnd), IDM_TOGGLE_NOSYNC_MODE,
                            MF_UNCHECKED);
                        G_lNoSyncModeFlag = FALSE;
                        lNumPortOption = G_lNumComPort;
                    }
                    else
                    {
                        CheckMenuItem (GetMenu (hWnd), IDM_TOGGLE_NOSYNC_MODE,
                            MF_CHECKED);
                        G_lNoSyncModeFlag = TRUE;
                        lNumPortOption = G_lNumComPort | P_XDC_NOSYNC_DMA_MODE;
                    } /* End if, else. */

                    DBK_XDCSetComPortNumber (lNumPortOption);
                    break;

                /*------------------------------------------------------*/
                /* Use 2 Com ports.                                     */
                /* Note that this item is only valid on a PCI bus board */
                /* and that once the number of com ports is set to 2 it */
                /* cannot be changed back.                              */
                /*------------------------------------------------------*/

                case IDM_USE_2COMPORTS:
                    lNumPortOption = 2;
                    if (G_lPIOModeFlag)
                    {
                        lNumPortOption |= P_XDC_DSPPIO_MODE;
                    } /* End if. */
                    if (G_lNoSyncModeFlag)
                    {
                        lNumPortOption |= P_XDC_NOSYNC_DMA_MODE;
                    } /* End if. */
                    G_lNumComPort = 2;
                    DBK_XDCSetComPortNumber (lNumPortOption);
                    CheckMenuItem (GetMenu (hWnd), IDM_USE_2COMPORTS, MF_CHECKED);
                    break;

                /*------------------------------------------------*/
                /* Draw the test image in the current grab image. */
                /*------------------------------------------------*/

                case IDM_TEST_IMAGE:
                    IDMTestImage (hWnd);
                    break;

                /*----------------------------------*/
                /* Save the grab image to the disk. */
                /*----------------------------------*/

                case IDM_SAVE_IMAGE:
                    IDMSaveImage (hWnd);
                    break;

                /*------------------------------------------*/
                /* Load an image to the dsp and display it. */
                /*------------------------------------------*/

                case IDM_LOAD_IMAGE:
                    IDMLoadImage (hWnd);
                    break;


				case IDM_FFT:
				    IDMFft (hWnd);
					break;

				
				//hall on the tool for camera #1.
				case IDM_HALL_COORDINATES:
					IDMHallCoordinates (hWnd);
					break;

					
				//chip centre on the tool for camera #2.
				case IDM_HALL_COORDINATES_2:
					IDMHallCoordinates2 (hWnd);
					break;
					
					
				//top edge.
				case IDM_top_edge:
					TopEdge(hWnd);
					break;
                
				 //bottom edge.
				case IDM_bottom_edge:
					BottomEdge(hWnd);
					break;

				//left edge.
				case IDM_left_edge:
					LeftEdge(hWnd);
					break;

				//right edge.
				case IDM_right_edge:
                    RightEdge(hWnd);
					break;

                
				case IDM_FOV_UPDATE:
					g_settings.update();
					break;
				
				case IDM_FOV_RECREATE:
					g_settings.recreate();
					break;

				case IDM_FOV_SAVE:
					g_settings.save();
					break;
				
				
				case IDM_PACKET_FINDER:
					IDMPacketFinder(hWnd);
					break;

				case IDM_PACKET_FINDER2:
					IDMPacketFinder2(hWnd);
					break;


				case IDM_PACKET_FINDER3:
					IDMPacketFinder3(hWnd);
					break;
					

				case IDM_BASE_DETECTOR:
					IDMBaseDetector (hWnd);	
					break;

                
				
				case IDM_FID_X_INC:
				    g_fiducial_dx = min(547,(g_fiducial_dx+=30));
					IDMGrabImg (hWnd, &bSingleGrabFlag);
 				    LocatePointer(&g_fiducial_dx, &g_fiducial_dy);
					break;
				
				case IDM_FID_X_DEC:
				    g_fiducial_dx = max(68,(g_fiducial_dx-=30));
					IDMGrabImg (hWnd, &bSingleGrabFlag);
 				    LocatePointer(&g_fiducial_dx, &g_fiducial_dy);
					break;
				
				case IDM_FID_Y_INC:
					g_fiducial_dy = min(430,(g_fiducial_dy+=30));
					IDMGrabImg (hWnd, &bSingleGrabFlag);
 				    LocatePointer(&g_fiducial_dx, &g_fiducial_dy);
					break;
				
				case IDM_FID_Y_DEC:
				    g_fiducial_dy = max(68,(g_fiducial_dy-=30));
					IDMGrabImg (hWnd, &bSingleGrabFlag);
 				    LocatePointer(&g_fiducial_dx, &g_fiducial_dy);
					break;
				
				
				case IDM_INITIAL_FIDUCIAL_POINT:
                    g_fiducial_dx = 320;
					g_fiducial_dy = 240;
				
				case IDM_FIDUCIAL:
					IDMGrabImg (hWnd, &bSingleGrabFlag);
					LocatePointer(&g_fiducial_dx, &g_fiducial_dy);
					break;
				
					
					
				/*----------------------*/
                /* Grab a single image. */
                /*----------------------*/

                case IDM_GRABIMG:
                    IDMGrabImg (hWnd, &bSingleGrabFlag);
                    break;


                case IDM_GET_IMAGE:
                    /*---------------------*/
                    /* Get the image size. */
                    /*---------------------*/

                    if ((lStatus = DBF_GetGrabWindow (P_DEFAULT_QGS,
                        &lGrabStartPixel, &lGrabNumPixels, &lGrabStartLine,
                        &lGrabNumLines)) != P_SUCCESS)
                    {
                        wsprintf (G_pszErrMsg,
                            "Error %ld: Unable to get grab window.", lStatus);
                        fMessageBox (hWnd, G_pszErrMsg,
                            G_pszMBTitleError, MB_OK | MB_ICONINFORMATION);
                        return P_ERROR_WINDOWS;
                    } /* End if. */

                    /*-------------------------------------------*/
                    /* Allocate space for the image and lock it. */
                    /*-------------------------------------------*/

                    if ((hGlobalMem = GlobalAlloc (GMEM_FIXED,
                        (DWORD) (lGrabNumPixels * lGrabNumLines))) == NULL)
                    {
                        wsprintf (G_pszErrMsg,
                            "Error %ld: Unable to allocate far buffer.",
                            lStatus);
                        fMessageBox (hWnd, G_pszErrMsg,
                            G_pszMBTitleError, MB_OK | MB_ICONINFORMATION);
                        return P_ERROR_WINDOWS;
                    } /* End if. */

                    fpcBuffer = (FCHAR *) GlobalLock (hGlobalMem);

                    /*---------------------*/
                    /* Get the image data. */
                    /*---------------------*/

                    if ((lStatus = DPK_XDCGetImage (G_lGrabImage,
                        (HVOID *) fpcBuffer)) != P_SUCCESS)
                    {
                        wsprintf (G_pszErrMsg,
                            "Error %ld: Unable get image data.", lStatus);
                        fMessageBox (hWnd, G_pszErrMsg,
                            G_pszMBTitleError, MB_OK | MB_ICONINFORMATION);
                        GlobalUnlock (hGlobalMem);
                        GlobalFree (hGlobalMem);
                        return P_ERROR_WINDOWS;
                    } /* End if. */

                    /*---------------------------------------*/
                    /* Put the image data back to the board. */
                    /*---------------------------------------*/

                    if ((lStatus = DPK_XDCPutImage (G_lOutputImage,
                        (HVOID *) fpcBuffer)) != P_SUCCESS)
                    {
                        wsprintf (G_pszErrMsg,
                            "Error %ld: Unable put image data.", lStatus);
                        fMessageBox (hWnd, G_pszErrMsg,
                            G_pszMBTitleError, MB_OK | MB_ICONINFORMATION);
                        GlobalUnlock (hGlobalMem);
                        GlobalFree (hGlobalMem);
                        return P_ERROR_WINDOWS;
                    } /* End if. */

                    /*--------------------------------*/
                    /* Put the image onto the screen. */
                    /*--------------------------------*/

                    DisplayImage (hWnd, G_lOutputImage);
                    GlobalUnlock (hGlobalMem);
                    GlobalFree (hGlobalMem);
                    break;

                /*----------------------------------*/
                /* Let Windows process the message. */
                /*----------------------------------*/

                default:
                    return (DefWindowProc (hWnd, uiMessage, wParam, lParam));
            } /* End switch. */
            break;

        /*---------------------*/
        /* Repaint the window. */
        /*---------------------*/

        case WM_PAINT:
            if (G_lCurrentImage != -1)
            {
                hDC = BeginPaint (hWnd, &qPaintInfo);
                DisplayImage (hWnd, G_lCurrentImage);
                EndPaint (hWnd, &qPaintInfo);
            }
            else
            {
                return (DefWindowProc (hWnd, uiMessage, wParam, lParam));
            } /* End if, else. */
            break;

        /*--------------------------------*/
        /* Message: window being resized. */
        /*--------------------------------*/

        case WM_SIZE:
            GetClientRect (hWnd, &qRect);
            SizeImageDisplay (hWnd, G_lCurrentImage,
                &G_iStartLine, &G_iStartPixel, &G_iEndLine,
                &G_iEndPixel);
            break;

        /*----------------------------------------------*/
        /* Message: window being scrolled horizontally. */
        /*----------------------------------------------*/
		case WM_HSCROLL:
            bUpdate = TRUE;
            switch (LOWORD (wParam))
            {
                case SB_LINEDOWN:
                    G_iStartPixel += 4;
                    break;

                case SB_LINEUP:
                    G_iStartPixel -= 4;
                    break;

                case SB_PAGEUP:
                    G_iStartPixel -= 128;
                    break;

                case SB_PAGEDOWN:
                    G_iStartPixel += 128;
                    break;

                case SB_THUMBPOSITION:
                case SB_THUMBTRACK:
                    G_iStartPixel = HIWORD (wParam);
                    break;

                default:
                    bUpdate = FALSE;
                    break;
            } /* End switch. */

            if (bUpdate)
            {
                GetScrollRange (hWnd, SB_HORZ, &iScrollMin, &iScrollMax);
                G_iStartPixel = BOUND (G_iStartPixel, iScrollMin,
                    iScrollMax);
                G_iStartPixel &= ~0x0003;  /* Longword alignment. */
                SizeImageDisplay (hWnd, G_lCurrentImage,
                    &G_iStartLine, &G_iStartPixel, &G_iEndLine,
                    &G_iEndPixel);
                SetScrollPos (hWnd, SB_HORZ, G_iStartPixel, TRUE);
                InvalidateRect (hWnd, NULL, TRUE);
                UpdateWindow (hWnd);
            } /* End if. */
            break;

        /*--------------------------------------------*/
        /* Message: window being scrolled vertically. */
        /*--------------------------------------------*/

        case WM_VSCROLL:
            bUpdate = TRUE;
            switch (LOWORD (wParam))
            {
                case SB_LINEDOWN:
                    G_iStartLine += 4;
                    break;

                case SB_LINEUP:
                    G_iStartLine -= 4;
                    break;

                case SB_PAGEUP:
                    G_iStartLine -= 128;
                    break;

                case SB_PAGEDOWN:
                    G_iStartLine += 128;
                    break;

                case SB_THUMBPOSITION:
                case SB_THUMBTRACK:
                    G_iStartLine = HIWORD (wParam);
                    break;
                default:
                    bUpdate = FALSE;
            } /* End switch. */

            if (bUpdate)
            {
                GetScrollRange (hWnd, SB_VERT, &iScrollMin, &iScrollMax);
                G_iStartLine = BOUND (G_iStartLine, iScrollMin,
                    iScrollMax);
                SizeImageDisplay (hWnd, G_lCurrentImage,
                    &G_iStartLine, &G_iStartPixel, &G_iEndLine,
                    &G_iEndPixel);
                SetScrollPos (hWnd, SB_VERT, G_iStartLine, TRUE);
                InvalidateRect (hWnd, NULL, TRUE);
                UpdateWindow (hWnd);
            } /* End if. */		  
            break;


			case WM_KEYUP:
			switch ((char) wParam)
			{
			  case '1': Y.start.x--;
			            IDMSectionVision(hWnd,X,Y);
				        break;
			  case '2':	Y.start.x++;
			            IDMSectionVision(hWnd,X,Y);
				        break;
			  case 'z':	Y.end.x--;
			            IDMSectionVision(hWnd,X,Y);
				        break;
			  case 'x':	Y.end.x++;
			            IDMSectionVision(hWnd,X,Y);
				        break;
			  case 'q':	X.start.y--;
			            IDMSectionVision(hWnd,X,Y);
				        break;
			  case 'a':	X.start.y++;
			            IDMSectionVision(hWnd,X,Y);
				        break;
			  case 'w':	X.end.y--;
			            IDMSectionVision(hWnd,X,Y);
				        break;
			  case 's':	X.end.y++;
			            IDMSectionVision(hWnd,X,Y);
			}
		
			break;

			
			case WM_LBUTTONUP:
			            if( (wParam & MK_CONTROL) && (wParam & MK_SHIFT))
						{
                           g_settings._PIXEL_0_ = LOWORD(lParam);
					       g_settings._LINE_0_  = HIWORD(lParam);
						}
						break;

			case WM_RBUTTONUP:
				        if( (wParam & MK_CONTROL) && (wParam & MK_SHIFT))
						{
						   g_settings._PIXEL_1_ = LOWORD(lParam);
					       g_settings._LINE_1_  = HIWORD(lParam);
						}
						else if( (wParam & MK_CONTROL) && (!(wParam & MK_SHIFT)))
						{
			  	           char posstr[80];
				           int ix,iy;
				           ix = LOWORD(lParam);
				           iy = HIWORD(lParam);
				           sprintf(posstr,"p(%d,%d)=%f",ix,iy,g_x[ix][iy]);
				  		   TextOut(GetDC(hWnd),ix,iy,posstr,strlen(posstr));
						}
						else if(!(wParam & MK_CONTROL) && (!(wParam & MK_SHIFT)))
						{
			  	           g_fiducial_dx = LOWORD(lParam);// - 320;
					       g_fiducial_dy = HIWORD(lParam);// - 240;

			  	           g_fiducial_dx = max(90,min(560,g_fiducial_dx));
					       g_fiducial_dy = max(90,min(370,g_fiducial_dy));


					       IDMGrabImg (hWnd, &bSingleGrabFlag);
 				           LocatePointer(&g_fiducial_dx, &g_fiducial_dy);
						}
						break;

			
		/*	
			
			case WM_MOUSEMOVE:
			  if( !(wParam & MK_CONTROL) && !(wParam & MK_SHIFT))
			  {
			     if((wParam & MK_LBUTTON) && !(wParam & MK_RBUTTON) )
				 {
                    g_fiducial_dx = LOWORD(lParam);// - 320;
					g_fiducial_dy = HIWORD(lParam);// - 240;
					IDMGrabImg (hWnd, &bSingleGrabFlag);
 				    LocatePointer(&g_fiducial_dx, &g_fiducial_dy);
				 }
			  }
			break;
		 */
			
			

        /*----------------------------------*/
        /* Message: window being destroyed. */
        /*----------------------------------*/

        case WM_DESTROY:
            PostQuitMessage (0);
            break;

        /*-------------------------------*/
        /* Passes it on if unproccessed. */
        /*-------------------------------*/

        default:
            return (DefWindowProc (hWnd, uiMessage, wParam, lParam));
    } /* End switch. */

    return ((LRESULT) P_SUCCESS);

} /* End of the MainWndProc function. */


VOID WMCreate (HWND hWnd)
{
    SetupItems (hWnd);

    /*---------------------------------------*/
    /* Setup the default processing options. */
    /*---------------------------------------*/

    G_lFilterType = P_FILTER_SOBEL;
    G_lKernelSize = P_FILTER_KERNEL_SIZE_3;
    G_uiFilterTypeId = IDM_SOBEL;
    G_uiKernelSizeId = IDM_KERNEL_3;
    G_lCurrentImage = -1;

    /*----------------------------------*/
    /*  Initialize the line/pixel info. */
    /*----------------------------------*/

    G_iStartLine = G_iStartPixel = 0;
    G_iEndLine   = G_iEndPixel   = 4;

    //CheckMenuItem (GetMenu (hWnd), G_uiKernelSizeId, MF_CHECKED);
    //CheckMenuItem (GetMenu (hWnd), G_uiFilterTypeId, MF_CHECKED);

} /* End of the WMCreate function. */


VOID CheckItem (HWND hWnd, UINT *puiCurrentItem, UINT uiNewSelection)
{
    CheckMenuItem (GetMenu (hWnd), *puiCurrentItem, MF_UNCHECKED);
    *puiCurrentItem = uiNewSelection;
    CheckMenuItem (GetMenu (hWnd), *puiCurrentItem, MF_CHECKED);
} /* End of the CheckItem function. */


VOID WaitCursor (BOOL bWait)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    HCURSOR         hCursor;        /* Hourglass cursor handle. */
    static HCURSOR  hOldCursor;     /* Old cursor handle. */

    /*------------------------------------*/
    /* If hourglass cursor is to be used, */
    /* load it and save old cursor.       */
    /*------------------------------------*/

    if (bWait)
    {
        hCursor = LoadCursor (NULL, IDC_WAIT);
        hOldCursor = SetCursor (hCursor);
    }

    /*---------------------*/
    /* Restore old cursor. */
    /*---------------------*/

    else
    {
        SetCursor (hOldCursor);
    } /* End if, else. */

} /* End of the WaitCursor function. */

/**==========================================================================**/
