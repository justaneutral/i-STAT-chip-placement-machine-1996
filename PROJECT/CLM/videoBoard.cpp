// videoBoard.cpp : implementation file
//

#include "stdafx.h"
#include "globals.h"
#include "videoBoard.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern "C"
{
#include "dpdefs.h"     /* XPG parameters. */
#include "dpptypes.h"   /* XPG function prototypes. */
#include "dberrors.h"   /* XPG error codes and warnings definitions. */
}

#ifdef WIN32
    #include    "dpgioctl.h"
#endif

/////////////////////////////////////////////////////////////////////////////
// videoBoard



LONG videoBoard::SelectBoardProcess ()
{

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

    G_lCurrentImage = -1;

    if ((lStatus = DPK_PCKSelectXPG (0)) != P_SUCCESS)
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



videoBoard::init()
{
    HDC         hDC;            /* Handle to device context. */
    HWND        hWnd;           /* Main window handle. */

    INT     iVgaMaxColours;     /* Maximum number of colours for vga. */

    LONG    lStatus;            /* Error status. */
    ULONG   ulValue;            /* For getting info back from DPU_GetHostInfo. */

	/*--------------------------------------*/
    /* Initialize the communication kernel. */
    /* Ignore warnings.                     */
    /*--------------------------------------*/

    if ((lStatus = DPK_InitPCK (1)) >= P_ERROR)
    {
      DPK_XCCSetWaitMode (P_WAIT_COMPLETE);
      if ((lStatus = DPU_GetEnvironmentVar ()) == P_SUCCESS)
	  {
        if ((lStatus = SelectBoardProcess (hWnd, 0))== P_SUCCESS)
		{
          if ((hDC = GetDC (hWnd)) != (HDC) NULL)
          {
			 iVgaMaxColours = GetDeviceCaps (hDC, BITSPIXEL);
			 if ((lStatus = DPW_LoadWindowsPalette (hDC, "palette.0"))== P_SUCCESS)
			 {
			 }
             ReleaseDC (hWnd, hDC);
		  }
		}
	  }
	}
}

videoBoard::~videoBoard()
{
}

