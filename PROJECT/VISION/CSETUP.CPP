#include <windows.h>    /* Required for all Windows applications. */
#include <string.h>     /* strcmp */

extern "C"
{
#include "dpdefs.h"     /* XPG parameters. */
#include "dpptypes.h"   /* XPG function prototypes. */
#include "dberrors.h"   /* XPG error codes and warnings definitions. */
}

#include "camera.h"      /* Specific to this program. */

/**========================================================================**/
/*
*  Module Global Variables
*/

typedef struct IMAGES
{
    BOOL    bBoardInitFlag;
    BOOL    bCPFLoadedFlag;
    LONG    lCurrentImageNumber;
    LONG    lGrabImageNumber;
    LONG    lOutputImageNumber;
} IMAGES;
IMAGES     M_pqSavedParam[P_MAX_BOARDS];

static BOOL     M_bBoardSelectCancel;

static CHAR    *M_pszCoffFileName[] = {"", "xpg.out", "fpg.out", "lpg.out"};
static CHAR     M_pszBoardInfo[P_MAX_BOARDS+1][30];
static CHAR    *M_pszBoardType[] = {"", "XPG", "FPG", "LPG"};
static CHAR    *M_pszBModType[] = {"", "-Analog", "-Digital"};
static CHAR    *M_pszBusType[] = {"", "ISA/VL", "PCI", "PCI AO"};
static CHAR    *M_pszDisplayMode[] = {"SingleScreen", "DualScreen", "N/A"};
static CHAR    *M_pszDisplayType[] = {"NONE", "EDB"};

/**============================== CODE ====================================**/

#ifdef WIN32
LRESULT APIENTRY About (HWND hDlg, unsigned uMessage, WORD wParam,
            LONG lParam)
#else
BOOL FAR PASCAL __export About (HWND hDlg, unsigned uMessage, WORD wParam,
            LONG lParam)
#endif
{
    switch (uMessage)
    {
        /*------------------------*/
        /* Initialize dialog box. */
        /*------------------------*/

        case WM_INITDIALOG:
            return (TRUE);

        /*---------------------*/
        /* Received a command. */
        /*---------------------*/

        case WM_COMMAND:
            /*-------------------------------------------------*/
            /* "OK" box selected or system menu close command? */
            /*-------------------------------------------------*/

            if (wParam == IDOK || wParam == IDCANCEL)
            {
                /*-----------------------*/
                /* Exits the dialog box. */
                /*-----------------------*/

                EndDialog (hDlg, TRUE);
                return (TRUE);
            } /* End if. */
            break;
    } /* End switch. */

    /*---------------------------*/
    /* Didn't process a message. */
    /*---------------------------*/

    return (FALSE);

} /* End of the About function. */


VOID ConfigInfo (HWND hWnd)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    FREEMEMSTRUCT           qFreeMemInfo;   /* Free memory available. */

    FLOAT_LONG_STRINGFPTR   Value;  /* Environment info. */

    CHAR    pszVersionNumber[10];   /* To hold version number. */
    CHAR    pszConfigInfo[512];     /* Configuration information. */

    INT     iBModType;              /* Board module type. */
    LONG    lStatus;                /* Error status. */

    /*--------------------------*/
    /* Software version number. */
    /*--------------------------*/

    lStatus = DPU_GetHostInfo (P_HOST_INFO_VERSION_NUMBER, &Value.ulValue);
    sprintf (pszVersionNumber, "%4.2f", Value.fValue);

    lStatus = DPU_GetHostInfo (P_HOST_INFO_BMOD_TYPE, &Value.ulValue);
    iBModType = (INT) Value.ulValue;

    /*-------------------*/
    /* Environment path. */
    /*-------------------*/

    lStatus = DPU_GetHostInfo (P_HOST_INFO_XPG_PATH, &Value.ulValue);

    /*--------------------------------*/
    /* Get the free memory available. */
    /*--------------------------------*/

    if ((lStatus = DBK_MmtReportFreeMemory (&qFreeMemInfo)) != P_SUCCESS)
    {
        wsprintf (G_pszErrMsg,
            "Error %ld: Unable to get free memory information.", lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        return;
    } /* End if. */

    /*--------------------*/
    /* Encode the string. */
    /*--------------------*/

    wsprintf (pszConfigInfo,
        "Software Version: %s\n"
        "Board type      : %s%s\n"
        "Bus type        : %s\n"
        "Display type    : %s\n"
        "Display Mode    : %s\n"
        "Environment path: %s\n"
        "Free memory information (in 32 bit words):\n"
        "   Global Bus DRAM: 0x%08lx\n"
        "   Global Bus SRAM: 0x%08lx\n"
        "   Local  Bus DRAM: 0x%08lx\n"
        "   Local  Bus SRAM: 0x%08lx",
        pszVersionNumber, M_pszBoardType[(INT) G_lBoardType],
        M_pszBModType[iBModType], M_pszBusType[(INT) G_lBusType],
        M_pszDisplayType[(INT) G_lDisplayType],
        M_pszDisplayMode[(INT) G_lDisplayMode], Value.fpszString,
        qFreeMemInfo.GlobalBus.lTotalFree,
        qFreeMemInfo.GlobalBusSRAM.lTotalFree,
        qFreeMemInfo.LocalBus.lTotalFree, qFreeMemInfo.LocalBusSRAM.lTotalFree);
        fMessageBox (hWnd, pszConfigInfo, G_pszMBTitleInfo,
            MB_OK | MB_ICONINFORMATION);

} /* End of the ConfigInfo function. */




LONG IDMLoadCPF (HWND hWnd, LPSTR lpszCameraName,LONG *plGrabStartPixel, LONG *plGrabNumPixels,  //not use dfor a while.
        LONG *plGrabStartLine, LONG *plGrabNumLines)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    LONG    lDataSize;          /* Data size in bits. */
    LONG    lFrameCount;        /* Number of frames to grab. */
    LONG    lImagesCreated;     /* Number of images created. */
    LONG    lMultiImageGrab;    /* Multi grab flag. */
    LONG    lValidBits;         /* Number of valid bits per pixel. */
    LONG    lStatus;            /* Error status. */

   
	/*for any case delete old CPS*/
	DBF_FreeCPS(G_lCPSNumber);
	
	
	/*----------------------------------------------------------*/
    /* Reset the current image number to invalid and initialize */
    /* memory management.   Old images are deleted.             */
    /*----------------------------------------------------------*/

    DBK_MmtInitialize ();
    G_lCurrentImage = -1;


	/*---------------------------------------------*/
	/* check for sony xc-73 camera parameter files */
	/*---------------------------------------------*/
    
	//{
	//int i;
	//LPSTR CameraName[2] = {"sony1.cpf","sony2.cpf"};
	
	//for(i=0;i<2;i++)
	//{
		strcpy((LPSTR)G_pszOpenName,(LPSTR)lpszCameraName);
	    lStatus = DPF_LoadCPF (G_pszOpenName);
        if (lStatus < P_SUCCESS)
		{
	    /*---------------------*/
        /* Call file open dlg. */
        /*---------------------*/

          if ((lStatus = OpenFileDialog (CPFFILEOPENDLG, (LPSTR) G_pszOpenName))
              != P_SUCCESS)
		  { 
              return lStatus;
		  } /* End if. */
		
        /*--------------------*/
        /* Load the CPF file. */
        /*--------------------*/
          WaitCursor (TRUE);
          lStatus = DPF_LoadCPF (G_pszOpenName);
        }

		G_lCPSNumber = 0;
		WaitCursor (FALSE);
        
		if (lStatus < P_SUCCESS)
		{
            wsprintf (G_pszErrMsg, "Error %ld: Unable to load CPF file.",
                lStatus);
            fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
                MB_OK | MB_ICONINFORMATION);
            return P_ERROR_WINDOWS;
		} /* End if. */
        G_lCPSNumber = lStatus;
//        G_lCPSNumberSet[i] = G_lCPSNumber;

//	}
//	}


	/*--------------------*/
    /* Select the CPS.    */
    /*--------------------*/
//	G_lCPSNumber = G_lCPSNumberSet[0];
    if ((lStatus = DBF_SelectCPS (G_lCPSNumber)) < P_SUCCESS)
	{
        wsprintf (G_pszErrMsg, "Error %ld: Unable to selecting CPS.1",
            lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        return P_ERROR_WINDOWS;
    } /* End if. */


    /*----------------------------------------------*/
    /* Get the frame count and the multigrab value. */
    /*----------------------------------------------*/

    if ((lStatus = DBF_GetFrameCount (P_DEFAULT_QGS, &lFrameCount,
        &lMultiImageGrab)) != P_SUCCESS)
    {
        wsprintf (G_pszErrMsg, "Error %ld: Unable to get frame count.",
            lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        return P_ERROR_WINDOWS;
    } /* End if. */

    /*----------------------------------*/
    /* Get the grab window information. */
    /*----------------------------------*/

    if ((lStatus = DBF_GetGrabWindow (P_DEFAULT_QGS, plGrabStartPixel,
        plGrabNumPixels, plGrabStartLine, plGrabNumLines)) != P_SUCCESS)
    {
        wsprintf (G_pszErrMsg, "Error %ld: Unable to get grab window.",
            lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        return P_ERROR_WINDOWS;
    } /* End if. */

    /*----------------------------------*/
    /* Get the data size of the camera. */
    /*----------------------------------*/

    if ((lStatus = DBF_GetCameraDataSize (G_lCPSNumber, &lDataSize,
        &lValidBits)) != P_SUCCESS)
    {
        wsprintf (G_pszErrMsg, "Error %ld: Unable to get camera data size.",
            lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        return P_ERROR_WINDOWS;
    } /* End if. */


    
	/*---------------------------------------------------------*/
    /* Create images according to the size of the grab window. */
    /* If there is not enough free memory for the images then  */
    /* adjust the image size.                                  */
    /*---------------------------------------------------------*/

    
//	*plGrabNumPixels = P_FFT_SIZE_128;
//  *plGrabNumLines =  P_FFT_SIZE_128;

    lStatus = DBK_MmtCreateImage (*plGrabNumPixels, *plGrabNumLines,
              P_DATA_SIZE_BYTE, P_DATA_TYPE_INTEGER, lFrameCount + 1,
              &G_lGrabImage, &lImagesCreated);
              
//	DBF_SetGrabWindow (P_DEFAULT_QGS, *plGrabStartPixel,
//              *plGrabNumPixels, *plGrabStartLine, *plGrabNumLines);
              
	wsprintf (G_pszErrMsg, "Image size %ld pixels by %ld lines.\n"
              "Image reduced from CPF file values to fit available memory.",
              *plGrabNumPixels, *plGrabNumLines);
	
    while (lStatus != P_SUCCESS)  
	  if (lStatus == P_ERROR_NOT_ENOUGH_MEMORY)
	  {
              *plGrabNumPixels = (LONG) ((FLOAT) *plGrabNumPixels * 0.75F)
                  & (~0x3L);
              *plGrabNumLines = (LONG) ((FLOAT) *plGrabNumLines * 0.75F);
              lStatus = DBK_MmtCreateImage (*plGrabNumPixels, *plGrabNumLines,
                  P_DATA_SIZE_BYTE, P_DATA_TYPE_INTEGER, lFrameCount + 1,
                  &G_lGrabImage, &lImagesCreated);
              DBF_SetGrabWindow (P_DEFAULT_QGS, *plGrabStartPixel,
                  *plGrabNumPixels, *plGrabStartLine, *plGrabNumLines);
              wsprintf (G_pszErrMsg, "Image size %ld pixels by %ld lines.\n"
                  "Image reduced from CPF file values to fit available memory.",
                  *plGrabNumPixels, *plGrabNumLines);
	  } 
      else
	  {
              wsprintf (G_pszErrMsg, "Error %ld: Unable to create an image.",
                  lStatus);
              fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
                  MB_OK | MB_ICONINFORMATION);
              return P_ERROR_WINDOWS;
	  } /* End if, else. */
	

    fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleInfo,
        MB_OK | MB_ICONINFORMATION);
    DBI_SetImageBuffer (G_lGrabImage, 0x80808080L);
    M_pqSavedParam[G_iBoardNumber].bCPFLoadedFlag = TRUE;

    /*-------------------------------------*/
    /* Set the image calculation window to */
    /* the application window size.        */
    /*-------------------------------------*/

    G_iStartLine  = 0;
    G_iStartPixel = 0;
    G_iEndLine    = 4;
    G_iEndPixel   = 4;

    SizeImageDisplay (hWnd, G_lGrabImage, &G_iStartLine,
        &G_iStartPixel, &G_iEndLine, &G_iEndPixel);

    /*---------------------------------------*/
    /* Make the current image the grab image */
    /* and set up the output image number.   */
    /*---------------------------------------*/

    G_lCurrentImage = G_lGrabImage;
    G_lOutputImage = G_lGrabImage + lFrameCount;

    /*--------------------------*/
    /* Enable other menu items. */
    /*--------------------------*/

    if (G_lCPSNumber >= P_SUCCESS)
    {
        SetupItems (hWnd);
    } /* End if. */

    /*---------------------------*/
    /* Reload PPM after new CPF. */
    /*---------------------------*/

    if (G_lPPMActiveFlag == TRUE)
    {
        fMessageBox (hWnd, "Reload PPF file. ", G_pszMBTitleInfo,
            MB_OK | MB_ICONINFORMATION);
        IDMLoadPPF (hWnd);
    } /* End if. */

    return P_SUCCESS;

} /* End of the IDMLoadCPF function. */
  
  
  
/*============================================================================
*
*  Name        : IDMLoadCPF
*  Author      : Paul Dourbal
*  Description : Function to process the IDM_LOAD command.  The CPF file is
*                selected and loaded.  Images are created and menu items
*                enabled.  FFT buffer is created.
*
*  Parameters  :
*       hWnd                Handle to window.
*       plGrabStartPixel    First pixel to grab.
*       plGrabNumPixels     Number of pixels to grab.
*       plGrabStartLine     First line to grab.
*       plGrabNumLines      Number of lines to grab.
*
*  Returns     : Error status
*
*===========================================================================*/

LONG IDMLoadCPF_ (HWND hWnd, LONG *plGrabStartPixel, LONG *plGrabNumPixels,  //not use dfor a while.
        LONG *plGrabStartLine, LONG *plGrabNumLines)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    LONG    lDataSize;          /* Data size in bits. */
    LONG    lFrameCount;        /* Number of frames to grab. */
    LONG    lImagesCreated;     /* Number of images created. */
    LONG    lMultiImageGrab;    /* Multi grab flag. */
    LONG    lValidBits;         /* Number of valid bits per pixel. */
    LONG    lStatus;            /* Error status. */

    /*----------------------------------------------------------*/
    /* Reset the current image number to invalid and initialize */
    /* memory management.   Old images are deleted.             */
    /*----------------------------------------------------------*/

    DBK_MmtInitialize ();
    G_lCurrentImage = -1;


	/*---------------------------------------------*/
	/* check for sony xc-73 camera parameter files */
	/*---------------------------------------------*/
    
	{
	int i;
	LPSTR CameraName[2] = {"sony1.cpf","sony2.cpf"};
	
	for(i=0;i<2;i++)
	{
		strcpy((LPSTR)G_pszOpenName,(LPSTR)CameraName[i]);
	    lStatus = DPF_LoadCPF (G_pszOpenName);
        if (lStatus < P_SUCCESS)
		{
	    /*---------------------*/
        /* Call file open dlg. */
        /*---------------------*/

          if ((lStatus = OpenFileDialog (CPFFILEOPENDLG, (LPSTR) G_pszOpenName))
              != P_SUCCESS)
		  { 
              return lStatus;
		  } /* End if. */
		}
        /*--------------------*/
        /* Load the CPF file. */
        /*--------------------*/

        G_lCPSNumber = 0;
        WaitCursor (TRUE);
        lStatus = DPF_LoadCPF (G_pszOpenName);
        WaitCursor (FALSE);
        if (lStatus < P_SUCCESS)
		{
            wsprintf (G_pszErrMsg, "Error %ld: Unable to load CPF file.",
                lStatus);
            fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
                MB_OK | MB_ICONINFORMATION);
            return P_ERROR_WINDOWS;
		} /* End if. */
        G_lCPSNumber = lStatus;
        G_lCPSNumberSet[i] = G_lCPSNumber;

	}
	}


	/*--------------------*/
    /* Select the CPS # 1.*/
    /*--------------------*/
	G_lCPSNumber = G_lCPSNumberSet[0];
    if ((lStatus = DBF_SelectCPS (G_lCPSNumber)) < P_SUCCESS)
	{
        wsprintf (G_pszErrMsg, "Error %ld: Unable to selecting CPS.1",
            lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        return P_ERROR_WINDOWS;
    } /* End if. */


    /*----------------------------------------------*/
    /* Get the frame count and the multigrab value. */
    /*----------------------------------------------*/

    if ((lStatus = DBF_GetFrameCount (P_DEFAULT_QGS, &lFrameCount,
        &lMultiImageGrab)) != P_SUCCESS)
    {
        wsprintf (G_pszErrMsg, "Error %ld: Unable to get frame count.",
            lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        return P_ERROR_WINDOWS;
    } /* End if. */

    /*----------------------------------*/
    /* Get the grab window information. */
    /*----------------------------------*/

    if ((lStatus = DBF_GetGrabWindow (P_DEFAULT_QGS, plGrabStartPixel,
        plGrabNumPixels, plGrabStartLine, plGrabNumLines)) != P_SUCCESS)
    {
        wsprintf (G_pszErrMsg, "Error %ld: Unable to get grab window.",
            lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        return P_ERROR_WINDOWS;
    } /* End if. */

    /*----------------------------------*/
    /* Get the data size of the camera. */
    /*----------------------------------*/

    if ((lStatus = DBF_GetCameraDataSize (G_lCPSNumber, &lDataSize,
        &lValidBits)) != P_SUCCESS)
    {
        wsprintf (G_pszErrMsg, "Error %ld: Unable to get camera data size.",
            lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        return P_ERROR_WINDOWS;
    } /* End if. */


    
	
    /*-------------------------------------------*/
    /* Create a FFT buffer and get it's address. */
    /*-------------------------------------------*/
	
    if ((lStatus = DBI_CreateFftBuffer (P_FFT_SIZE_128, &G_lFftBufferNumber))
         != P_SUCCESS)
    {
        wsprintf (G_pszErrMsg, "Error %ld: Unable to create FFT buffer.",
            lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
    } 
	
    if ((lStatus = DBK_MmtReadBufferHeader (G_lFftBufferNumber, &G_FftBufHeader))
        != P_SUCCESS)
    {
        wsprintf (G_pszErrMsg, "Error %ld: Unable to obtain FFT buffer address.",
            lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
    }
	
	
	
	/*---------------------------------------------------------*/
    /* Create images according to the size of the grab window. */
    /* If there is not enough free memory for the images then  */
    /* adjust the image size.                                  */
    /*---------------------------------------------------------*/

    
	*plGrabNumPixels = P_FFT_SIZE_128;
    *plGrabNumLines =  P_FFT_SIZE_128;

    lStatus = DBK_MmtCreateImage (*plGrabNumPixels, *plGrabNumLines,
              P_DATA_SIZE_BYTE, P_DATA_TYPE_INTEGER, lFrameCount + 1,
              &G_lGrabImage, &lImagesCreated);
              
	DBF_SetGrabWindow (P_DEFAULT_QGS, *plGrabStartPixel,
              *plGrabNumPixels, *plGrabStartLine, *plGrabNumLines);
              
	wsprintf (G_pszErrMsg, "Image size %ld pixels by %ld lines.\n"
              "Image reduced from CPF file values to fit available memory.",
              *plGrabNumPixels, *plGrabNumLines);
	
    while (lStatus != P_SUCCESS)  
	  if (lStatus == P_ERROR_NOT_ENOUGH_MEMORY)
	  {
              *plGrabNumPixels = (LONG) ((FLOAT) *plGrabNumPixels * 0.75F)
                  & (~0x3L);
              *plGrabNumLines = (LONG) ((FLOAT) *plGrabNumLines * 0.75F);
              lStatus = DBK_MmtCreateImage (*plGrabNumPixels, *plGrabNumLines,
                  P_DATA_SIZE_BYTE, P_DATA_TYPE_INTEGER, lFrameCount + 1,
                  &G_lGrabImage, &lImagesCreated);
              DBF_SetGrabWindow (P_DEFAULT_QGS, *plGrabStartPixel,
                  *plGrabNumPixels, *plGrabStartLine, *plGrabNumLines);
              wsprintf (G_pszErrMsg, "Image size %ld pixels by %ld lines.\n"
                  "Image reduced from CPF file values to fit available memory.",
                  *plGrabNumPixels, *plGrabNumLines);
	  } 
      else
	  {
              wsprintf (G_pszErrMsg, "Error %ld: Unable to create an image.",
                  lStatus);
              fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
                  MB_OK | MB_ICONINFORMATION);
              return P_ERROR_WINDOWS;
	  } /* End if, else. */
	

    fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleInfo,
        MB_OK | MB_ICONINFORMATION);
    DBI_SetImageBuffer (G_lGrabImage, 0x80808080L);
    M_pqSavedParam[G_iBoardNumber].bCPFLoadedFlag = TRUE;

    /*-------------------------------------*/
    /* Set the image calculation window to */
    /* the application window size.        */
    /*-------------------------------------*/

    G_iStartLine  = 0;
    G_iStartPixel = 0;
    G_iEndLine    = 4;
    G_iEndPixel   = 4;

    SizeImageDisplay (hWnd, G_lGrabImage, &G_iStartLine,
        &G_iStartPixel, &G_iEndLine, &G_iEndPixel);

    /*---------------------------------------*/
    /* Make the current image the grab image */
    /* and set up the output image number.   */
    /*---------------------------------------*/

    G_lCurrentImage = G_lGrabImage;
    G_lOutputImage = G_lGrabImage + lFrameCount;

    /*--------------------------*/
    /* Enable other menu items. */
    /*--------------------------*/

    if (G_lCPSNumber >= P_SUCCESS)
    {
        SetupItems (hWnd);
    } /* End if. */

    /*---------------------------*/
    /* Reload PPM after new CPF. */
    /*---------------------------*/

    if (G_lPPMActiveFlag == TRUE)
    {
        fMessageBox (hWnd, "Reload PPF file. ", G_pszMBTitleInfo,
            MB_OK | MB_ICONINFORMATION);
        IDMLoadPPF (hWnd);
    } /* End if. */

    return P_SUCCESS;

} /* End of the IDMLoadCPF function. */





/*============================================================================
*
*  Name        : IDMLoadPPF
*  Author      : Andrzej Jaskielewicz
*  Description : Function to process the IDM_LOAD command.  The PPF file is
*                selected and loaded. The CPF file is require to be loaded
*                first.
*
*  Parameters  :
*       hWnd                Handle to window.
*
*  Returns     : Error status
*
*===========================================================================*/

LONG IDMLoadPPF (HWND hWnd)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    LONG    lStatus;               /* Error status. */

    /*---------------------------------------*/
    /* Set PPM active flag or clear old PPS. */
    /*---------------------------------------*/

    if (G_lPPMActiveFlag == TRUE)
    {
        DBP_FreePPS (G_lPPSNumber);
    }
    else
    {
        G_lPPMActiveFlag = TRUE;
    } /* End if. */

    /*---------------------*/
    /* Call file open dlg. */
    /*---------------------*/

    if ((lStatus = OpenFileDialog (PPFFILEOPENDLG, (LPSTR) G_pszOpenName))
        != P_SUCCESS)
    {
        return lStatus;
    } /* End if. */

    /*--------------------*/
    /* Load the PPF file. */
    /*--------------------*/

    WaitCursor (TRUE);
    lStatus = DPP_LoadPPF (G_pszOpenName, &G_lPPSNumber);
    WaitCursor (FALSE);
    if (lStatus < P_SUCCESS)
    {
        wsprintf (G_pszErrMsg, "Error %ld: Unable to load PPF file.",
            lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        return P_ERROR_WINDOWS;
    } /* End if. */

    /*-----------------------*/
    /* Select QGS structure. */
    /*-----------------------*/

    lStatus = DBF_QuickGrabAllocSetup (G_lCPSNumber);
    if (lStatus < P_SUCCESS)
    {
        wsprintf (G_pszErrMsg, "Error %ld: Unable to select QGS.\n"
                                    "Note:  CPF must be loaded first!",
            lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        return P_ERROR_WINDOWS;
    } /* End if. */

    G_lQGSNumber = lStatus;

    /*-----------------*/
    /* Select the CPS. */
    /*-----------------*/

    if ((lStatus = DBP_PPMSetGrabConfiguration (G_lQGSNumber,
        G_lPPSNumber)) < P_SUCCESS)
    {
        wsprintf (G_pszErrMsg, "Error %ld: Unable to set PPM configuration.",
            lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        return P_ERROR_WINDOWS;
    } /* End if. */

    /*---------------*/
    /* Init the QGS. */
    /*---------------*/

    if ((lStatus = DBF_QuickGrabSetup (G_lQGSNumber,
        G_lGrabImage)) < P_SUCCESS)
    {
        wsprintf (G_pszErrMsg, "Error %ld: Unable to setup QGS.",
            lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        return P_ERROR_WINDOWS;
    } /* End if. */

    /*---------------*/
    /* Init the PPM. */
    /*---------------*/

    WaitCursor (TRUE);
    lStatus = DBF_QuickGrab (G_lQGSNumber, P_GRAB_INIT, P_GRAB_WAIT);
    WaitCursor (FALSE);
    if (lStatus < P_SUCCESS)
    {
        wsprintf (G_pszErrMsg, "Error %ld: Unable to initialize PPM.",
            lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        return P_ERROR_WINDOWS;
    } /* End if. */

    return P_SUCCESS;

} /* End of the IDMLoadPPF function. */

/*============================================================================
*
*  Name        : FillBoardSelectionStrings
*  Author      : Pierre Landriau
*  Description : This function initializes the board selection strings with
*                a board number value, board type and bus type.
*
*  Parameters  : none
*
*  Returns     : none
*
*===========================================================================*/

VOID FillBoardSelectionStrings (INT iNumBoards)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    INT     i;                  /* Loop indice. */
    INT     iBoardType;         /* Type of board, XPG, FPG or LPG. */
    INT     iBusType;           /* Type of bus, ISA, PCI or PCI. */
    ULONG   ulValue;            /* Returned from DPU_GetHostInfo. */

    /*----------------------------------------------------------------*/
    /* Create an array of strings for the board selection dialog box. */
    /*----------------------------------------------------------------*/

    for (i = 0; i < iNumBoards; i++)
    {
        DPK_PCKSelectXPG (i);
        DPU_GetHostInfo (P_HOST_INFO_BOARD_TYPE, &ulValue);
        iBoardType = (INT) ulValue;
        DPU_GetHostInfo (P_HOST_INFO_BUS_TYPE, &ulValue);
        iBusType = (INT) ulValue;
        wsprintf (M_pszBoardInfo[i], "Board %1d: %s  %s", i,
            M_pszBoardType[iBoardType], M_pszBusType[iBusType]);
    } /* End for. */
    wsprintf (M_pszBoardInfo[i], "\0");

    /*---------------------------------------------------*/
    /* Set the board init flag array to not initialized. */
    /*---------------------------------------------------*/

    for (i = 0; i < iNumBoards; i++)
    {
        M_pqSavedParam[i].bBoardInitFlag = FALSE;
        M_pqSavedParam[i].bCPFLoadedFlag = FALSE;
    } /* End for. */

} /* End of the FillBoardSelectionStrings function. */

/*============================================================================
*
*  Name        : IDMSelectBoard
*  Author      : Pierre Landriau
*  Description : This function initializes the dialog box for selecting a
*                board to use.
*
*  Parameters  : hWnd: Handle to main application window
*
*  Returns     : none
*
*===========================================================================*/

VOID IDMSelectBoard (HWND hWnd)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    FARPROC     lpSelectBoardProc;

    INT         lFlag = IDOK;

    /*----------------------------*/
    /* Save current image values. */
    /*----------------------------*/

    if (G_iBoardNumber >= 0)
    {
        M_pqSavedParam[G_iBoardNumber].lGrabImageNumber =
            G_lGrabImage;
        M_pqSavedParam[G_iBoardNumber].lOutputImageNumber =
            G_lOutputImage;
        M_pqSavedParam[G_iBoardNumber].lCurrentImageNumber =
            G_lCurrentImage;
    } /* End if. */

    /*------------------------*/
    /* Put up the dialog box. */
    /*------------------------*/

    M_bBoardSelectCancel = FALSE;
    lpSelectBoardProc = MakeProcInstance ((FARPROC) SelectBoardDialog,
        G_hInst);
    DialogBox (G_hInst, "SELECTBOARDDIALOG", hWnd, lpSelectBoardProc);
    FreeProcInstance (lpSelectBoardProc);

    /*-----------------------------*/
    /* Select the board specified. */
    /*-----------------------------*/

    if (!M_bBoardSelectCancel)
    {
        /*----------------------------------------------------*/
        /* If the board selected has already been initialized */
        /* inform the user.   Init again or cancel.           */
        /*----------------------------------------------------*/

        if (M_pqSavedParam[G_iBoardNumber].bBoardInitFlag)
        {
            wsprintf (G_pszErrMsg, "This board is already initialized\n"
                "OK to re-initialize, CANCEL to select with no init");
            lFlag = fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleInfo,
                MB_OKCANCEL | MB_ICONINFORMATION);
        } /* End if. */

        InvalidateRect (hWnd, NULL, TRUE);
        UpdateWindow (hWnd);
        if (lFlag == IDOK)
        {
            SelectBoardProcess (hWnd, G_iBoardNumber);
        }
        else
        {
            G_lGrabImage =
                M_pqSavedParam[G_iBoardNumber].lGrabImageNumber;
            G_lOutputImage =
                M_pqSavedParam[G_iBoardNumber].lOutputImageNumber;
            G_lCurrentImage =
                M_pqSavedParam[G_iBoardNumber].lCurrentImageNumber;
            DPK_PCKSelectXPG (G_iBoardNumber);
            SetupItems (hWnd);
        } /* End if, else. */
    } /* End if. */

} /* End of the IDMSelectBoard function. */

/*============================================================================
*
*  Name        : SelectBoardDialog
*  Author      : Pierre Landriau
*  Description : This function processes the inputs to the Select board dialog
*                box.
*
*  Parameters  :
*
*  Returns     : TRUE or FALSE
*
*===========================================================================*/

#ifdef WIN32
LRESULT APIENTRY SelectBoardDialog (HWND hDlg, unsigned uMessage,
                WORD wParam, LONG lParam)
#else
BOOL FAR PASCAL __export SelectBoardDialog (HWND hDlg, unsigned uMessage,
                WORD wParam, LONG lParam)
#endif
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    CHAR    pszInput[30];       /* String for board selected. */

    INT     i;                  /* Loop index. */

    /*------------------------------*/
    /* Process dialog box messages. */
    /*------------------------------*/

    switch (uMessage)
    {
        /*------------------------*/
        /* Initialize dialog box. */
        /*------------------------*/

        case WM_INITDIALOG:
            /*--------------------------------------------------*/
            /* Fill in drop down menu with the board selection. */
            /* Highlight current selection.                     */
            /*--------------------------------------------------*/

            for (i = 0; i < G_iNumBoards; i++)
            {
                SendDlgItemMessage (hDlg, IDM_BOARD_SELECTION, CB_INSERTSTRING,
                    i, (LPARAM) ((LPCSTR) M_pszBoardInfo[i]));
            } /* End for. */
            if (G_iBoardNumber < 0)
            {
                SendDlgItemMessage (hDlg, IDM_BOARD_SELECTION, CB_SELECTSTRING,
                    0, (LPARAM) ((LPCSTR) M_pszBoardInfo[0]));
            }
            else
            {
                SendDlgItemMessage (hDlg, IDM_BOARD_SELECTION, CB_SELECTSTRING,
                    0, (LPARAM) ((LPCSTR) M_pszBoardInfo[G_iBoardNumber]));
            } /* End if, else. */
            break;

        case WM_COMMAND:
            switch (wParam)
            {
                /*----------------------------------------------------*/
                /* Get user selection, close the dialog box and exit. */
                /*----------------------------------------------------*/

                case IDOK:
                    GetDlgItemText (hDlg, IDM_BOARD_SELECTION, pszInput,
                        sizeof (pszInput));
                    for (i = 0; i < G_iNumBoards; i++)
                    {
                        if (strcmp (M_pszBoardInfo[i], pszInput) == 0)
                        {
                            G_iBoardNumber = i;
                            break;
                        } /* End if. */
                    } /* End for. */
                    EndDialog (hDlg, TRUE);
                    break;

                case IDCANCEL:
                    M_bBoardSelectCancel = TRUE;
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

} /* End of the SelectBoardDialog function. */

/*============================================================================
*
*  Name        : SelectBoardProcess
*  Author      : Pierre Landriau
*  Description : This function selects the board to be used.
*
*  Parameters  :
*   hWnd                Handle to window context.
*   iBoardNumber        Which board to use.
*
*  Returns     : Error status.
*
*===========================================================================*/

LONG SelectBoardProcess (HWND hWnd, INT iBoardNumber)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    DISP_STRUCT qDispParam;     /* Display board parameters. */
    BOARD_CONFIG_STRUCT pqBoardConfig;  /*  Holds information about */
                                        /*  system configuration. */

    CHAR    pszEDBFile[30];     /* EDB information. */

    INT     i;                  /* Loop indice. */
    INT     iBoardType;         /* Type of board. */
    INT     iStatus;            /* Error status from profile string functions. */

    LONG    lStatus;            /* Error status. */
    ULONG   ulValue;            /* Value returned from GetHostInfo. */

    /*-------------------*/
    /* Select the board. */
    /*-------------------*/

    M_pqSavedParam[iBoardNumber].bBoardInitFlag = TRUE;
    M_pqSavedParam[iBoardNumber].bCPFLoadedFlag = FALSE;
    G_lCurrentImage = -1;

    if ((lStatus = DPK_PCKSelectXPG (iBoardNumber)) != P_SUCCESS)
    {
        wsprintf (G_pszErrMsg, "Error selecting a board, status = %ld.",
            lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleInfo,
                MB_OK | MB_ICONINFORMATION);
    } /* End if. */

    /*------------------------------------------------*/
    /* Load the out file according to the board type. */
    /*------------------------------------------------*/

    DPU_GetHostInfo (P_HOST_INFO_BOARD_TYPE, &ulValue);
    G_lBoardType = (LONG) ulValue;
    iBoardType = (INT) ulValue;

    DPU_GetHostInfo (P_HOST_INFO_BUS_TYPE, &ulValue);
    G_lBusType = (LONG) ulValue;

    wsprintf (G_pszErrMsg, "Loading %s", M_pszCoffFileName[iBoardType]);
    fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleInfo,
            MB_OK | MB_ICONINFORMATION);
    WaitCursor (TRUE);
    lStatus = DPK_InitXPG (iBoardNumber, P_IFB_COFF_FLAG | P_IFB_REV_CHECK,
        M_pszCoffFileName[iBoardType]);
    WaitCursor (FALSE);
    if (lStatus < P_ERROR)
    {
        wsprintf (G_pszErrMsg,
            "Error %ld: Unable to initialize board.", lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        return lStatus;
    }
    else if (lStatus != P_SUCCESS)
    {
        wsprintf (G_pszErrMsg,
            "Warning %ld: DSP vs PC version mismatch.", lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
    } /* End if, else. */

    /*--------------------------------------------------------------*/
    /* If the board type is an LPG then skip the rest of the setup. */
    /*--------------------------------------------------------------*/

    if (G_lBoardType == P_LPG_BOARD)
    {
        G_lDisplayType = P_DISP_TYPE_NO_DISP;
        G_lDisplayMode = EDB_MODE_NA;
        SetupItems (hWnd);
        return lStatus;
    } /* End if. */

    /*---------------------------------------------------------------------*/
    /* If there is a display board available then get the mode information */
    /* from the ini file and initialize the display board in that mode.    */
    /*---------------------------------------------------------------------*/

    lStatus = DBD_DispGetType (&G_lDisplayType, &G_lDisplayMode);
    if (lStatus == P_SUCCESS) /* Not running with EDB yet. */
    {
        G_lDisplayType = P_DISP_TYPE_NO_DISP;
    } /* End if. */

    if (G_lDisplayType == P_DISP_TYPE_EDB)
    {
        iStatus = GetPrivateProfileString ("_INIT", "EDB_MODE",
            "800x600.edb", pszEDBFile, 30, "camera.ini");

        if ((lStatus = DPD_DispGetInit (pszEDBFile, &qDispParam)) != P_SUCCESS)
        {
            wsprintf (G_pszErrMsg,
                "Error %ld: Unable to get disp init file.", lStatus);
            fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
                MB_OK | MB_ICONINFORMATION);
            return P_ERROR_WINDOWS;
        } /* End if. */

        if ((lStatus = DBD_DispInit (&qDispParam)) != P_SUCCESS)
        {
            wsprintf (G_pszErrMsg,
                "Error %ld: Unable to load disp init file.", lStatus);
            fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
                MB_OK | MB_ICONINFORMATION);
            return P_ERROR_WINDOWS;
        } /* End if. */

        if ((lStatus = DBD_DispEnablePassthru (0, TRUE)) != P_SUCCESS)
        {
            wsprintf (G_pszErrMsg,
                "Error %ld: Failure to enable passthru mode.", lStatus);
            fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
                MB_OK | MB_ICONINFORMATION);
            return P_ERROR_WINDOWS;
        } /* End if. */
    } /* End if. */

    /*------------------------*/
    /* Search for PPM module. */
    /*------------------------*/

    if ((lStatus = DBK_GetBoardConfig (&pqBoardConfig)) != P_SUCCESS)
    {
         wsprintf (G_pszErrMsg,
                "Error %ld: Unable to get board configuration.", lStatus);
         fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
                MB_OK | MB_ICONINFORMATION);
         return P_ERROR_WINDOWS;
    } /* End if. */

    G_lPPMActiveFlag = P_ERROR_NOT_PPM_MODULE;
    for (i = 0; i < P_CONFIG_MAX_MODULES; i++)
    {
         if (pqBoardConfig.qModules[i].ulType == P_CONFIG_MODULE_TYPE_PPM_GEN)
         {
              G_lPPMActiveFlag = TRUE;      /* PPM present. */
              break;
         } /* End if. */
    } /* End for. */

    SetupItems (hWnd);
    return lStatus;

} /* End of the SelectBoardProcess function. */

/*============================================================================
*
*  Name        : SetupItems
*  Author      : Pierre Landriau
*  Description : This function disables some menu items at init time.  Mostly
*                in regards to ISA versus PCI bus differences.
*
*  Parameters  :
*   hWnd                Handle to window context.
*
*  Returns     : Error status.
*
*===========================================================================*/

VOID SetupItems (HWND hWnd)
{
    /*------------------------------------------------------------------*/
    /* If no board have been selected then disable all the items in the */
    /* top menu bar except for the Setup item.  In the setup submenu    */
    /* disable all item except board select, about and exit.            */
    /*------------------------------------------------------------------*/

    //if (G_iBoardNumber == -1)
    //{
        //EnableMenuItem (GetMenu (hWnd), IDM_LOAD_PPF, P_DISABLE_SUB_ITEM);
        //EnableMenuItem (GetMenu (hWnd), IDM_LOAD_CPF, P_DISABLE_SUB_ITEM);
        //EnableMenuItem (GetMenu (hWnd), IDM_TOGGLE_BUS_MASTERING,
        //    P_DISABLE_SUB_ITEM);
        //EnableMenuItem (GetMenu (hWnd), IDM_USE_2COMPORTS, P_DISABLE_SUB_ITEM);
        //EnableMenuItem (GetMenu (hWnd), IDM_TOGGLE_NOSYNC_MODE,
        //    P_DISABLE_SUB_ITEM);
        //EnableMenuItem (GetMenu (hWnd), IDM_TOGGLE_PIO_MODE,
        //    P_DISABLE_SUB_ITEM);
        //EnableMenuItem (GetMenu (hWnd), IDM_CONFIGURATION, P_DISABLE_SUB_ITEM);

        //EnableMenuItem (GetMenu (hWnd), IDM_IMAGE, P_DISABLE_ITEM);
        //EnableMenuItem (GetMenu (hWnd), IDM_FILTER_OPTIONS, P_DISABLE_ITEM);
        //EnableMenuItem (GetMenu (hWnd), IDM_PROCESS, P_DISABLE_ITEM);
        //EnableMenuItem (GetMenu (hWnd), IDM_DEBUG, P_DISABLE_ITEM);
    //}

    /*-------------------------------------------------*/
    /* A board has been selected.  Enable setup items. */
    /*-------------------------------------------------*/

    //else if (G_lBoardType != P_LPG_BOARD)
    //{
        //EnableMenuItem (GetMenu (hWnd), IDM_LOAD_CPF, P_ENABLE_SUB_ITEM);
        //EnableMenuItem (GetMenu (hWnd), IDM_CONFIGURATION, P_ENABLE_SUB_ITEM);

        /*-------------------------------------*/
        /* Enable items according to bus type. */
        /*-------------------------------------*/

        
		DPU_GetHostInfo (P_HOST_INFO_BUS_TYPE, (PULONG)(&G_lBusType));
        //if (G_lBusType == P_ISA_BUS)
        //{
        //    EnableMenuItem (GetMenu (hWnd), IDM_TOGGLE_BUS_MASTERING,
        //        P_DISABLE_SUB_ITEM);
        //    EnableMenuItem (GetMenu (hWnd), IDM_USE_2COMPORTS,
        //        P_DISABLE_SUB_ITEM);
        //    EnableMenuItem (GetMenu (hWnd), IDM_TOGGLE_NOSYNC_MODE,
        //        P_DISABLE_SUB_ITEM);
        //    EnableMenuItem (GetMenu (hWnd), IDM_TOGGLE_PIO_MODE,
        //        P_DISABLE_SUB_ITEM);
        //}
        //else
        //{
        //    EnableMenuItem (GetMenu (hWnd), IDM_TOGGLE_BUS_MASTERING,
        //        P_ENABLE_SUB_ITEM);
        //    EnableMenuItem (GetMenu (hWnd), IDM_USE_2COMPORTS,
        //        P_ENABLE_SUB_ITEM);
        //    EnableMenuItem (GetMenu (hWnd), IDM_TOGGLE_NOSYNC_MODE,
        //        P_ENABLE_SUB_ITEM);
        //    EnableMenuItem (GetMenu (hWnd), IDM_TOGGLE_PIO_MODE,
        //        P_ENABLE_SUB_ITEM);
        //} /* End if, else. */

        /*-----------------------*/
        /* Enable PPM menu item. */
        /*-----------------------*/

        //if (G_lPPMActiveFlag == P_ERROR_NOT_PPM_MODULE)
        //{
        //    EnableMenuItem (GetMenu (hWnd), IDM_LOAD_PPF, P_DISABLE_SUB_ITEM);
        //}
        //else
        //{
        //    EnableMenuItem (GetMenu (hWnd), IDM_LOAD_PPF, P_ENABLE_SUB_ITEM);
        //} /* End if, else. */

        /*--------------------------------------------------------*/
        /* Enable top menu bar item if an image has been created, */
        /* disable if not.                                        */
        /*--------------------------------------------------------*/

        //EnableMenuItem (GetMenu (hWnd), IDM_DEBUG, P_DISABLE_ITEM);
        //if (M_pqSavedParam[G_iBoardNumber].bCPFLoadedFlag)
        //{
        //    EnableMenuItem (GetMenu (hWnd), IDM_IMAGE, P_ENABLE_ITEM);
        //    EnableMenuItem (GetMenu (hWnd), IDM_FILTER_OPTIONS, P_ENABLE_ITEM);
        //    EnableMenuItem (GetMenu (hWnd), IDM_ARITH, P_ENABLE_ITEM);
        //    EnableMenuItem (GetMenu (hWnd), IDM_PROCESS, P_ENABLE_ITEM);
        //}
        //else
        //{
        //    EnableMenuItem (GetMenu (hWnd), IDM_IMAGE, P_DISABLE_ITEM);
        //    EnableMenuItem (GetMenu (hWnd), IDM_FILTER_OPTIONS, P_DISABLE_ITEM);
        //    EnableMenuItem (GetMenu (hWnd), IDM_ARITH, P_DISABLE_ITEM);
        //    EnableMenuItem (GetMenu (hWnd), IDM_PROCESS, P_DISABLE_ITEM);
        //} /* End if, else. */
    //}

    /*-------------------------------------------------------------*/
    /* Using an LPG, therefore only enable the configuration item. */
    /*-------------------------------------------------------------*/

    //else
    //{
    //    EnableMenuItem (GetMenu (hWnd), IDM_CONFIGURATION, P_ENABLE_SUB_ITEM);
    //} /* End if, else if, else. */
    DrawMenuBar (hWnd);

} /* End of the SetupItems function. */

/**========================================================================**/
