#include <windows.h>    /* Required for all Windows applications. */
#include <stdlib.h>     /* malloc, free */

extern "C"
{
#include "dpdefs.h"     /* XPG parameters. */
#include "dpptypes.h"   /* XPG function prototypes. */
#include "dberrors.h"   /* XPG error codes and warnings definitions. */
}

#include "camera.h"      /* Specific to this program. */

/**========================================================================**/
/*
*  #defined constants
*/

#define P_TEXT_START_X  300
#define P_TEXT_START_Y  200

#define P_HIST_BOTTOM_LEFT_X      10
#define P_HIST_BOTTOM_LEFT_Y      P_DEFAULT_CAMERA_Y_SIZE - 100
#define P_HIST_IMAGE_HEIGHT       P_DEFAULT_CAMERA_Y_SIZE - 110
#define P_HIST_TEXT_COLOURF       200
#define P_HIST_TEXT_COLOURB       20

/**========================================================================**/
/*
* Module global variables.
*/

static BOOL M_CancelGrab;
static INT  M_iFrameCount = 30;
static LONG M_lContinuousGrab = FALSE;


VOID IDMTestImage (HWND hWnd)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    LONG    lStatus;    /* Error status. */

    /*-------------------------------------------------------------*/
    /* Draw the test image.  Cursor put into wait mode until done. */
    /*-------------------------------------------------------------*/

    WaitCursor (TRUE);
    if ((lStatus = DPT_TestImage (G_lGrabImage)) != P_SUCCESS)
    {
        wsprintf (G_pszErrMsg, "Error %ld: Unable to draw the test image.",
            lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        return;
    } /* End if. */
    WaitCursor (FALSE);

    /*-------------------------*/
    /* Display the test image. */
    /*-------------------------*/

    DisplayImage (hWnd, G_lGrabImage);

} /* End of the IDMTestImage function. */

/*============================================================================
*
*  Name        : IDMGrabImg
*  Author      : Pierre Landriau
*  Description : To grab an image in the current image and display it.
*
*  Parameters  :
*       hWnd            Handle to window.
*
*  Returns     : none
*
*===========================================================================*/

VOID IDMGrabImg (HWND hWnd, BOOL *pbSingleGrabFlag)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    LONG    lStatus;

    /*-------------------------------*/
    /* Grab an image and display it. */
    /*-------------------------------*/

    lStatus = DBF_SetFrameCount (P_DEFAULT_QGS, 1, P_SINGLE_IMAGE_GRAB);

    /*--------------------------------*/
    /* Grab through PPM or regularly. */
    /*--------------------------------*/

    if (G_lPPMActiveFlag == TRUE)
    {
        lStatus = DBF_QuickGrab (G_lQGSNumber, P_GRAB_NO_INIT, P_GRAB_WAIT);
    }
    else
    {
        lStatus = DBF_Grab (G_lGrabImage);
    } /* End if, else. */

    if (lStatus != P_SUCCESS)
    {
        wsprintf (G_pszErrMsg,
            "Error %ld: Unable to grab an image.", lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        return;
    } /* End if. */

    *pbSingleGrabFlag = FALSE;
    DisplayImage (hWnd, G_lGrabImage);
} /* End of the IDMGrabImg function. */

/*============================================================================
*
*  Name        : IDMSMulImgGrab
*  Author      : Pierre Landriau
*  Description : To grab and display continuously.
*
*  Parameters  :
*       hWnd            Handle to window.
*
*  Returns     : none
*
*===========================================================================*/

VOID IDMSMulImgGrab (HWND hWnd, BOOL *pbSingleGrabFlag)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    FARPROC lpProcNumFrames;

    LONG    i;
    LONG    lStatus;

    /*-----------------------------------*/
    /* Get the number of frames to grab. */
    /*-----------------------------------*/

    M_CancelGrab = FALSE;
    lpProcNumFrames = MakeProcInstance ((FARPROC) GetNumFrames, G_hInst);
    DialogBox (G_hInst, "GRABNFRAMES", hWnd, lpProcNumFrames);
    FreeProcInstance (lpProcNumFrames);
    if (M_CancelGrab)
    {
        return;
    } /* End if. */

    /*-------------------------------*/
    /* Grab an image and display it. */
    /*-------------------------------*/

    M_lContinuousGrab = TRUE;
    for (i = 0; i < M_iFrameCount; i++)
    {
        lStatus = DBF_SetFrameCount (P_DEFAULT_QGS, 1, P_SINGLE_IMAGE_GRAB);

        /*--------------------------------*/
        /* Grab through PPM or regularly. */
        /*--------------------------------*/

        if (G_lPPMActiveFlag == TRUE)
        {
            lStatus = DBF_QuickGrab (G_lQGSNumber, P_GRAB_NO_INIT,
                P_GRAB_WAIT);
        }
        else
        {
            lStatus = DBF_Grab (G_lGrabImage);
        } /* End if, else. */

        if (lStatus != P_SUCCESS)
        {
            wsprintf (G_pszErrMsg,
                "Error %ld: Unable to grab an image.", lStatus);
            fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
                MB_OK | MB_ICONINFORMATION);
            return;
        } /* End if. */
        DisplayImage (hWnd, G_lGrabImage);
    } /* End for. */

    M_lContinuousGrab = FALSE;
    *pbSingleGrabFlag = TRUE;

} /* End of the IDMSMulImgGrab function. */

/*============================================================================
*
*  Name        : AbortGrab
*  Author      : Pierre Landriau
*  Description : To abort the grab function.
*
*  Parameters  :
*       bSingleGrabFlag     Single grab or multigrab flag.
*       lQGSNumber          QGS for current grab.
*
*  Returns     : none
*
*===========================================================================*/

VOID IDMAbortGrab (BOOL bSingleGrabFlag, LONG lQGSNumber)
{
    LONG lStatus;
    if (!bSingleGrabFlag)
    {
        DBF_AbortGrab ();
        lStatus = DBF_QuickGrabStatus (P_GRAB_WAIT);
        lStatus = DBF_QuickGrabDeleteSetup (lQGSNumber);
    } /* End if. */
} /* End of the IDMAbortGrab function. */

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

VOID IDMHistogram (HWND hWnd)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    STATINFO    ImageStat;

    CHAR    pszHistogramText[30];   /* Text to write on histogram image. */
    LONG    lDecimal;               /* Fractional part of floating point value. */
    LONG    lHistogramBufferNumber; /* Histogram buffer number. */
    LONG    lInteger;               /* Integer part of floating point value. */
    LONG    lStatus;                /* Error status. */
    ULONG   *pulBuffer;             /* Histogram data buffer. */

    /*------------------------------*/
    /* Do a histogram of the image. */
    /*------------------------------*/

    if ((pulBuffer = (ULONG *) malloc (256 * sizeof (ULONG))) == NULL)
    {
        lStatus = P_ERROR_MALLOCING;
        wsprintf (G_pszErrMsg, "Error %ld: Unable to malloc.", lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        return;
    } /* End if. */

    if ((lStatus = DBI_CreateHistogramBuffer (P_DATA_SIZE_BYTE,
        &lHistogramBufferNumber)) != P_SUCCESS)
    {
        wsprintf (G_pszErrMsg,
            "Error %ld: Unable to create histogram buffer.", lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        free (pulBuffer);
        return;
    } /* End if. */

    if ((lStatus = DBI_Histogram (G_lGrabImage, lHistogramBufferNumber,
        pulBuffer)) < P_ERROR)
    {
        wsprintf (G_pszErrMsg,
            "Error %ld: Unable to do histogram.", lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        free (pulBuffer);
        return;
    } /* End if. */

	/*------------------------------------------------------*/
    /* Draw the histogram.   Do images statistics and write */
    /* them to the histogram image.                         */
    /*------------------------------------------------------*/

    if ((lStatus = DBI_Statistics (G_lGrabImage, &ImageStat)) != P_SUCCESS)
    {
        wsprintf (G_pszErrMsg,
            "Error %ld: Unable to do statistics.", lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        free (pulBuffer);
        return;
    } /* End if. */

    if ((lStatus = DBI_DrawHistogram (G_lOutputImage,
        lHistogramBufferNumber, P_HIST_BOTTOM_LEFT_X,
        P_HIST_BOTTOM_LEFT_Y, P_HIST_IMAGE_HEIGHT,
        P_HISTOGRAM_AUTO_SCALE, P_HISTOGRAM_DRAW_AXIS,
        P_HISTOGRAM_NOLABEL_AXIS)) != P_SUCCESS)
    {
        wsprintf (G_pszErrMsg,
            "Error %ld: Unable to draw histogram.", lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
        free (pulBuffer);
        return;
    } /* End if. */

    wsprintf (pszHistogramText, "Image Statistics");
    DBG_WriteText (G_lOutputImage, P_TEXT_START_X,
        P_TEXT_START_Y, P_HIST_TEXT_COLOURF,
        P_HIST_TEXT_COLOURB, 1, pszHistogramText);

    lInteger = (LONG) ImageStat.fMean;
    lDecimal = (LONG) ((ImageStat.fMean - (FLOAT) lInteger) * 100.0F);
    wsprintf (pszHistogramText, "Mean     = %ld.%ld", lInteger, lDecimal);
    DBG_WriteText (G_lOutputImage, P_TEXT_START_X,
        P_TEXT_START_Y + 12, P_HIST_TEXT_COLOURF,
        P_HIST_TEXT_COLOURB, 1, pszHistogramText);

    lInteger = (LONG) ImageStat.fVariance;
    lDecimal = (LONG) ((ImageStat.fVariance - (FLOAT) lInteger) * 100.0F);
    wsprintf (pszHistogramText, "Variance = %ld.%ld", lInteger, lDecimal);
    DBG_WriteText (G_lOutputImage, P_TEXT_START_X,
        P_TEXT_START_Y + 24, P_HIST_TEXT_COLOURF,
        P_HIST_TEXT_COLOURB, 1, pszHistogramText);

    wsprintf (pszHistogramText, "Median   = %ld", ImageStat.lMedian);
    DBG_WriteText (G_lOutputImage, P_TEXT_START_X,
        P_TEXT_START_Y + 36, P_HIST_TEXT_COLOURF,
        P_HIST_TEXT_COLOURB, 1, pszHistogramText);

    wsprintf (pszHistogramText, "Mode     = %ld", ImageStat.lMode);
    DBG_WriteText (G_lOutputImage, P_TEXT_START_X,
        P_TEXT_START_Y + 48, P_HIST_TEXT_COLOURF,
        P_HIST_TEXT_COLOURB, 1, pszHistogramText);

    wsprintf (pszHistogramText, "Maximum  = %ld", ImageStat.lMaximum);
    DBG_WriteText (G_lOutputImage, P_TEXT_START_X,
        P_TEXT_START_Y + 60, P_HIST_TEXT_COLOURF,
        P_HIST_TEXT_COLOURB, 1, pszHistogramText);

    wsprintf (pszHistogramText, "Minimum  = %ld", ImageStat.lMinimum);
    DBG_WriteText (G_lOutputImage, P_TEXT_START_X,
        P_TEXT_START_Y + 72, P_HIST_TEXT_COLOURF,
        P_HIST_TEXT_COLOURB, 1, pszHistogramText);

    /*----------------------*/
    /* Display the results. */
    /*----------------------*/

    DisplayImage (hWnd, G_lOutputImage);

} /* End of the Histogram function. */

/*============================================================================
*
*  Name        : IDMDiskToWindows
*  Author      : Pierre Landriau
*  Description : To copy an image from Windows to disk.
*
*  Parameters  :
*       hWnd            Handle to window.
*
*  Returns     : none
*
*===========================================================================*/

VOID IDMDiskToWindows (HWND hWnd)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    HDC     hDC;

    LONG    lStatus;

    /*-------------------------------------------*/
    /* Get the file name and load it to Windows. */
    /*-------------------------------------------*/

    if ((lStatus = OpenFileDialog (IMGFILEOPENDLG, (LPSTR) G_pszOpenName))
        == P_SUCCESS)
    {
        hDC = GetDC (hWnd);
        if ((lStatus = DPW_CopyDiskToWindows (hWnd, hDC, 0, 0, G_pszOpenName))
            != P_SUCCESS)
        {
            wsprintf (G_pszErrMsg,
                "Error %ld: copying disk to Windows.", lStatus);
            fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
                MB_OK | MB_ICONINFORMATION);
        } /* End if. */
        ReleaseDC (hWnd, hDC);
    } /* End if. */

} /* End of the IDMDiskToWindows function. */

/*============================================================================
*
*  Name        : IDMSaveImage
*  Author      : Pierre Landriau
*  Description : To copy an image from Dsp to disk.
*
*  Parameters  : none
*
*  Returns     : none
*
*===========================================================================*/

VOID IDMSaveImage (HWND hWnd)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    LONG    lStatus;

    /*------------------------------------------------------------*/
    /* Get the file name to save the image to and save the image. */
    /*------------------------------------------------------------*/

    if ((lStatus = SaveFileDialog (FILESAVEDLG, (LPSTR) G_pszSaveName))
        == P_SUCCESS)
    {
        if ((lStatus = DPC_CopyImageBufferToDisk (G_lCurrentImage,
            G_pszSaveName)) < P_ERROR)
        {
            wsprintf (G_pszErrMsg,
                "Error %ld: Unable to save the image.", lStatus);
            fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
                MB_OK | MB_ICONINFORMATION);
        }
        else if (lStatus != P_SUCCESS)
        {
            wsprintf (G_pszErrMsg,
                "Warning %ld: Bus Mastering not used.", lStatus);
            fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
                MB_OK | MB_ICONINFORMATION);
        } /* End if, else if. */
    } /* End if. */

} /* End of the IDMSaveImage function. */

/*============================================================================
*
*  Name        : IDMLoadImage
*  Author      : Pierre Landriau
*  Description : To copy an image from disk to Dsp.
*
*  Parameters  : none
*
*  Returns     : none
*
*===========================================================================*/

VOID IDMLoadImage (HWND hWnd)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    LONG    lStatus;

    /*-------------------------------------------*/
    /* Get the file name and load it to the DSP. */
    /*-------------------------------------------*/

    if ((lStatus = OpenFileDialog (IMGFILEOPENDLG, (LPSTR) G_pszOpenName))
        == P_SUCCESS)
    {
        if ((lStatus = DPC_CopyDiskToImageBuffer (G_lCurrentImage,
            G_pszOpenName)) < P_ERROR)
        {
            wsprintf (G_pszErrMsg,
                "Error %ld: Unable to load the image.", lStatus);
            fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
                MB_OK | MB_ICONINFORMATION);
            return;
        }
        else if (lStatus != P_SUCCESS)
        {
            wsprintf (G_pszErrMsg,
                "Warning %ld: Bus Mastering not used.", lStatus);
            fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
                MB_OK | MB_ICONINFORMATION);
        } /* End if, else if. */
    } /* End if. */
    DisplayImage (hWnd, G_lGrabImage);

} /* End of the IDMLoadImage function. */

/*============================================================================
*
*  Name        : IDMTiffToWindows
*  Author      : Pierre Landriau
*  Description : To copy a Tiff image from disk to Windows.
*
*  Parameters  :
*       hWnd            Handle to window.
*
*  Returns     : none
*
*===========================================================================*/

VOID IDMTiffToWindows (HWND hWnd)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    HDC     hDC;

    LONG    lStatus;

    /*---------------------------------*/
    /* Copy the Tiff image to Windows. */
    /*---------------------------------*/

    if ((lStatus = OpenFileDialog (TIFFFILEOPENDLG, (LPSTR) G_pszOpenName))
        == P_SUCCESS)
    {
        hDC = GetDC (hWnd);
        if ((lStatus = DPS_TiffCopyDiskToWindows (hWnd, hDC, 0, 0,
            G_pszOpenName)) != P_SUCCESS)
        {
            wsprintf (G_pszErrMsg,
                "Error %ld: displaying tiff file.", lStatus);
            fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
                MB_OK | MB_ICONINFORMATION);
        } /* End if. */
        ReleaseDC (hWnd, hDC);
    } /* End if. */

} /* End of the IDMTiffToWindows function. */

/*============================================================================
*
*  Name        : IDMWindowsToTiff
*  Author      : Pierre Landriau
*  Description : To copy a Tiff image from Windows to disk.
*
*  Parameters  :
*       hWnd            Handle to window.
*
*  Returns     : none
*
*===========================================================================*/

VOID IDMWindowsToTiff (HWND hWnd)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    HDC     hDC;

    LONG    lStatus;

    /*---------------------------------*/
    /* Copy the Windows Image to Tiff. */
    /*---------------------------------*/

    if ((lStatus = SaveFileDialog (TIFFFILESAVEDLG,
        (LPSTR) G_pszSaveName)) == P_SUCCESS)
    {
        hDC = GetDC (hWnd);
        if ((lStatus = DPS_TiffCopyWindowsToDisk (hWnd, hDC, 0, 0,
            P_DEFAULT_CAMERA_X_SIZE - 1, P_DEFAULT_CAMERA_Y_SIZE - 1,
            G_pszSaveName)) != P_SUCCESS)
        {
            wsprintf (G_pszErrMsg,
                "Error %ld: saving to tiff file.", lStatus);
            fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
                MB_OK | MB_ICONINFORMATION);
        } /* End if. */
        ReleaseDC (hWnd, hDC);
    } /* End if. */

} /* End of the IDMWindowsToTiff function. */

/*============================================================================
*
*  Name        : IDMTiffToDspImg
*  Author      : Pierre Landriau
*  Description : To copy a tiff image from the disk to the dsp.
*
*  Parameters  :
*       hWnd            Handle to window.
*
*  Returns     : none
*
*===========================================================================*/

VOID IDMTiffToDspImg (HWND hWnd)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    LONG    lStatus;

    /*------------------------------------------------*/
    /* Copy the Tiff image to the dsp and display it. */
    /*------------------------------------------------*/

    if ((lStatus = OpenFileDialog (TIFFFILEOPENDLG, (LPSTR) G_pszOpenName))
        == P_SUCCESS)
    {
        if ((lStatus = DPS_TiffCopyDiskToImageBuffer (G_lGrabImage,
            G_pszOpenName)) != P_SUCCESS)
        {
            wsprintf (G_pszErrMsg,
                "Error %ld: copying tiff file.", lStatus);
            fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
                MB_OK | MB_ICONINFORMATION);
        }
        else if (lStatus != P_SUCCESS)
        {
            wsprintf (G_pszErrMsg,
                "Warning %ld: Bus Mastering not used.", lStatus);
            fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
                MB_OK | MB_ICONINFORMATION);
        } /* End if, else if. */
    } /* End if. */

    DisplayImage (hWnd, G_lGrabImage);

} /* End of the IDMTiffToDspImg function. */

/*============================================================================
*
*  Name        : IDMDspImgToTiff
*  Author      : Pierre Landriau
*  Description : To copy an image from the dsp to the disk in Tiff.
*
*  Parameters  :
*       hWnd            Handle to window.
*
*  Returns     : none
*
*===========================================================================*/

VOID IDMDspImgToTiff (HWND hWnd)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    LONG    lStatus;

    /*-----------------------------------------------------------------*/
    /* Get the tiff file name to save the image to and save the image. */
    /*-----------------------------------------------------------------*/

    if ((lStatus = SaveFileDialog (TIFFFILESAVEDLG,
        (LPSTR) G_pszSaveName)) == P_SUCCESS)
    {
        if ((lStatus = DPS_TiffCopyImageBufferToDisk (G_lCurrentImage,
            G_pszSaveName)) != P_SUCCESS)
        {
            wsprintf (G_pszErrMsg,
                "Error %ld: Unable to save the image.", lStatus);
            fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
                MB_OK | MB_ICONINFORMATION);
        }
        else if (lStatus != P_SUCCESS)
        {
            wsprintf (G_pszErrMsg,
                "Warning %ld: Bus Mastering not used.", lStatus);
            fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
                MB_OK | MB_ICONINFORMATION);
        } /* End if, else if. */
    } /* End if. */

} /* End of the IDMDspImgToTiff function. */

/*============================================================================
*
*  Name        : DisplayImage
*  Author      : Pierre Landriau
*  Description : To display the specified image in the application's window.
*
*  Parameters  :
*       hWnd            Handle to window.
*       lImageNumber    Image to be displayed.
*
*  Returns     : none
*
*===========================================================================*/

VOID DisplayImage (HWND hWnd, LONG lImageNumber)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    HDC             hDC;        /* Handle to display context. */
    PROCESSINGINFO  ImageInfo;
    PROCESSINGINFO  tmpInfo;

    LONG    lStatus;    /* Error status. */


	//CStatic *cs = new CStatic();


    /*----------------------------------------*/
    /* Get the calculation window to save it. */
    /*----------------------------------------*/

    DBK_MmtGetImageInfo (lImageNumber, &ImageInfo);

    /*-------------------------------------*/
    /* Set the image calculation window to */
    /* the application display window size.*/
    /*-------------------------------------*/

    SizeImageDisplay (hWnd, lImageNumber, &G_iStartLine,
        &G_iStartPixel, &G_iEndLine, &G_iEndPixel);
    DBK_MmtSetCalculationWindow (lImageNumber, G_iStartPixel,
        G_iStartLine, G_iEndPixel, G_iEndLine);

    DBK_MmtGetImageInfo (lImageNumber, &tmpInfo);
    G_iStartPixel = (INT) tmpInfo.PRO_WindowStartPixel;
    G_iStartLine  = (INT) tmpInfo.PRO_WindowStartLine;
    G_iEndPixel   = (INT) tmpInfo.PRO_WindowEndPixel;
    G_iEndLine    = (INT) tmpInfo.PRO_WindowEndLine;

    /*--------------------*/
    /* Display the image. */
    /*--------------------*/

    hDC = GetDC (hWnd);
    if ((lStatus = DPW_DisplayImageInWindows (hWnd, hDC, lImageNumber,
        P_XSCREEN_POS, P_YSCREEN_POS)) < P_ERROR)
    {
        wsprintf (G_pszErrMsg,
            "Error %ld: Unable to display the image.", lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
    }
    else if (lStatus != P_SUCCESS && !M_lContinuousGrab)
    {
        wsprintf (G_pszErrMsg,
            "Warning %ld: Bus Mastering not used.", lStatus);
        fMessageBox (hWnd, G_pszErrMsg, G_pszMBTitleError,
            MB_OK | MB_ICONINFORMATION);
    } /* End if, else if. */
    DBK_MmtSetCalculationWindow (lImageNumber, ImageInfo.PRO_WindowStartPixel,
        ImageInfo.PRO_WindowStartLine, ImageInfo.PRO_WindowEndPixel,
        ImageInfo.PRO_WindowEndLine);
    ReleaseDC (hWnd, hDC);
    G_lCurrentImage = lImageNumber;

} /* End of the DisplayImage function. */

/*============================================================================
*
*  Name        : SizeImageDisplay
*  Author      : Hugh Parliament
*  Description : To generate a correct image size for the current display
*                window.
*
*  Parameters  :
*       hWnd            Handle to window.
*       lImageNumber    Image to be displayed.
*       piStartLine     Pointer to Start Line # variable (update)
*       piStartPixel    Pointer to Start Pixel # variable (update)
*       piEndLine       Pointer to End Line # variable (output)
*       piEndPixel      Pointer to End Pixel # variable (output)
*
*  Returns     : none
*
*===========================================================================*/

VOID SizeImageDisplay (HWND hWnd, LONG lImageNumber, INT *piStartLine,
        INT *piStartPixel, INT *piEndLine, INT *piEndPixel)
{
    /*------------------*/
    /*  Local Variables */
    /*------------------*/

    PROCESSINGINFO ImageInfo;
    RECT           rectRC;

    INT            iLineLength;
    INT            iScrollMin;
    INT            iScrollMax;

    /*----------------------------------------------*/
    /* Make sure we are dealing with a valid image. */
    /*----------------------------------------------*/

    if (lImageNumber == -1)
    {
        return;
    } /* End if. */

    /*---------------------------------------*/
    /* Get the image information to save it. */
    /*---------------------------------------*/

    DBK_MmtGetImageInfo (lImageNumber, &ImageInfo);

    /*---------------------------------*/
    /* Get the display area rectangle. */
    /* (Line length is multiple of 4)  */
    /*---------------------------------*/

    GetClientRect (hWnd, &rectRC);
    iLineLength = rectRC.right - rectRC.left + 1;
    iLineLength = 4 * (iLineLength >> 2);

    /*-----------------------------------------------------*/
    /* Calculate the start and end pixels to display based */
    /* on the image size and the currently desired display */
    /* area. Adjust the values to ensure proper display.   */
    /*-----------------------------------------------------*/

    *piStartPixel = (*piStartPixel < 0) ? 0 : *piStartPixel;
    if (*piStartPixel > (ImageInfo.PRO_Pixels - 4) )
    {
        *piStartPixel = (INT) ImageInfo.PRO_Pixels - 4;
    } /* End if. */

    *piEndPixel = *piStartPixel + iLineLength - 1;
    if (*piEndPixel > ImageInfo.PRO_Pixels)
    {
        *piEndPixel = (INT) ImageInfo.PRO_Pixels - 1;
    } /* End if. */

    *piEndPixel = (*piEndPixel < 4) ? 4: *piEndPixel;

    /*-----------------------------------------------------*/
    /* Calculate the start and end lines to display based  */
    /* on the image size and the currently desired display */
    /* area. Adjust the values to ensure proper display.   */
    /*-----------------------------------------------------*/

    *piStartLine = (*piStartLine < 0) ? 0 : *piStartLine;
    if (*piStartLine > (ImageInfo.PRO_Lines - 4) )
    {
        *piStartLine = (INT) ImageInfo.PRO_Lines - 4;
    } /* End if. */

    *piEndLine  = *piStartLine + rectRC.bottom - 1;
    if (*piEndLine > ImageInfo.PRO_Lines)
    {
        *piEndLine = (INT) ImageInfo.PRO_Lines - 1;
    } /* End if. */

    *piEndLine  = (*piEndLine < 1) ? 1 : *piEndLine;

    /*-----------------------------------------------------------*/
    /*  Check out the scroll bars and what we need to do to them */
    /*  (i.e. If the whole axis range fits in window,            */
    /*        disable scrolling.                                 */
    /*-----------------------------------------------------------*/

    if (((rectRC.right - rectRC.left) >= ImageInfo.PRO_Pixels) &&
        (*piStartPixel == 0))
    {
        iScrollMin = rectRC.left;
        iScrollMax = rectRC.right;
        EnableScrollBar (hWnd, SB_HORZ, ESB_DISABLE_BOTH);
    }
    else
    {
        iScrollMin = 0;
        iScrollMax = (INT) ImageInfo.PRO_Pixels - 1;
        EnableScrollBar (hWnd, SB_HORZ, ESB_ENABLE_BOTH);
    } /* End if, else. */

    SetScrollRange (hWnd, SB_HORZ, iScrollMin, iScrollMax, FALSE);

    if (((rectRC.bottom - rectRC.top) >= ImageInfo.PRO_Lines) &&
        (*piStartLine == 0))
    {
        iScrollMin = rectRC.top;
        iScrollMax = rectRC.bottom;
        EnableScrollBar (hWnd, SB_VERT, ESB_DISABLE_BOTH);
    }
    else
    {
        iScrollMin = 0;
        iScrollMax = (INT) ImageInfo.PRO_Lines - 1;
        EnableScrollBar (hWnd, SB_VERT, ESB_ENABLE_BOTH);
    } /* End if, else. */

    SetScrollRange (hWnd, SB_VERT, iScrollMin, iScrollMax, FALSE);
    return;

} /* End of the SizeImageDisplay function. */

/*============================================================================
*
*  Name        : GetNumFrames
*  Author      : Pierre Landriau
*  Description : Processes messages for "Get number of frames" dialog box
*
*  Parameters  :
*       hDlg                Window handle of the dialog box.
*       message             Type of message.  WM_INITDIALOG or WM_COMMAND.
*       wParam              Message-specific information.
*       lParam              More message-specific information.
*
*  Returns     : Error status
*
*  Context/Restrictions:
*===========================================================================*/

#ifdef WIN32
LRESULT APIENTRY GetNumFrames (HWND hDlg, unsigned uMessage, WORD wParam,
            LONG lParam)
#else
BOOL FAR PASCAL __export GetNumFrames (HWND hDlg, unsigned uMessage,
            WORD wParam, LONG lParam)
#endif
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    BOOL fbStatus;          /* Variable for error flag. */

    /*------------------------------*/
    /* Process dialog box messages. */
    /*------------------------------*/

    switch (uMessage)
    {
        /*------------------------*/
        /* Initialize dialog box. */
        /*------------------------*/

        case WM_INITDIALOG:
            SetDlgItemInt (hDlg, IDM_NUM_FRAMES, M_iFrameCount, FALSE);
            return (TRUE);

        /*---------------------*/
        /* Received a command. */
        /*---------------------*/

        case WM_COMMAND:
            /*-------------------------------------------------------------*/
            /* "Ok" or "Cancel" box selected or system menu close command? */
            /*-------------------------------------------------------------*/

            switch (wParam)
            {
                /*-----------------------*/
                /* Exits the dialog box. */
                /*-----------------------*/

                case IDOK:
                    EndDialog (hDlg, TRUE);
                    break;

                case IDCANCEL:
                    M_CancelGrab = TRUE;
                    EndDialog (hDlg, TRUE);
                    break;

                case IDM_NUM_FRAMES:
                    M_iFrameCount = GetDlgItemInt (hDlg, IDM_NUM_FRAMES,
                        (BOOL FAR *) &fbStatus, FALSE);
                    break;
            } /* End switch. */

            return (TRUE);
            break;
    } /* End switch. */

    /*---------------------------*/
    /* Didn't process a message. */
    /*---------------------------*/

    return (FALSE);

} /* End of the GetNumFrames function. */

/**========================================================================**/
