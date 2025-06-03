#include    <windows.h>
#include    <string.h>

extern "C"
{
#include    "dpdefs.h"
#include    "dpptypes.h"
#include    "dberrors.h"
}

#include    "camera.h"

static FCHAR       M_pszEnvPath[128];

static CHAR M_pszInCPFFilter[MAXFILTERLEN] = "CamParFiles\0*.cpf\0";
static CHAR M_pszInPPFFilter[MAXFILTERLEN] = "PPMParFiles\0*.ppf\0";
static CHAR M_pszInImgFilter[MAXFILTERLEN] = "Input Image files\0*.img\0";
static CHAR M_pszInTifFilter[MAXFILTERLEN] = "Input Tiff Image files\0*.tif\0";
static CHAR M_pszOutImgFilter[MAXFILTERLEN] = "Output Image files\0*.img\0";
static CHAR M_pszOutTifFilter[MAXFILTERLEN] = "Output Tiff Image files\0*.tif\0";

/*============================================================================
*
*  Name        : InitializeStruct
*  Author      : Pierre Landriau
*  Description : To initialize a structure for the current common dialog.
*                This routine is called just before the common dialogs API
*                is called.
*  Parameters  :
*       wCommDlgType    Open or save flag.
*       lpStruct        Pointer to structure.
*
*  Returns     : Error status
*
*  Context/Restrictions:
*
*===========================================================================*/

VOID InitializeStruct (WORD wCommDlgType, LPSTR lpStruct)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    LPFOCHUNK               lpFOChunk;
    LPFSCHUNK               lpFSChunk;
    FLOAT_LONG_STRINGFPTR   Value;

    /*-----------------------*/
    /* Get environment path. */
    /*-----------------------*/

    DPU_GetHostInfo (P_HOST_INFO_XPG_PATH, &Value.ulValue);
    strcpy ((char *)M_pszEnvPath, (const char *)Value.fpszString);

    /*-----------------------------*/
    /* Fill in common information. */
    /*-----------------------------*/

    lpFOChunk = (LPFOCHUNK) lpStruct;
    lpFSChunk = (LPFSCHUNK) lpStruct;
    switch (wCommDlgType)
    {
        case CPFFILEOPENDLG:
        case PPFFILEOPENDLG:
        case IMGFILEOPENDLG:
        case TIFFFILEOPENDLG:
            *(lpFOChunk->szFile)            = 0;
            *(lpFOChunk->szFileTitle)       = 0;
            lpFOChunk->of.lStructSize       = sizeof (OPENFILENAME);
            lpFOChunk->of.hwndOwner         = (HWND) G_hWndMain;
            lpFOChunk->of.hInstance         = (HANDLE) NULL;
            lpFOChunk->of.lpstrCustomFilter = (LPSTR) NULL;
            lpFOChunk->of.nMaxCustFilter    = 0L;
            lpFOChunk->of.nFilterIndex      = 1L;
            lpFOChunk->of.lpstrFile         = lpFOChunk->szFile;
            lpFOChunk->of.nMaxFile          = (DWORD) sizeof (lpFOChunk->szFile);
            lpFOChunk->of.lpstrFileTitle    = lpFOChunk->szFileTitle;
            lpFOChunk->of.nMaxFileTitle     = MAXFILTERLEN;
            lpFOChunk->of.lpstrTitle        = (LPSTR) NULL;
            lpFOChunk->of.Flags             = OFN_HIDEREADONLY |
                OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
            lpFOChunk->of.nFileOffset       = 0;
            lpFOChunk->of.nFileExtension    = 0;
            lpFOChunk->of.lCustData         = 0L;
            lpFOChunk->of.lpfnHook          = (unsigned int (__stdcall *)(void *,unsigned int,unsigned int,long)) NULL;
            lpFOChunk->of.lpTemplateName    = (LPCSTR) NULL;
            break;

        /*---------------------------*/
        /* Set up for saving a file. */
        /*---------------------------*/

        case FILESAVEDLG:
        case TIFFFILESAVEDLG:
            *(lpFSChunk->szFile)            = 0;
            *(lpFSChunk->szFileTitle)       = 0;
            lpFSChunk->of.lStructSize       = sizeof (OPENFILENAME);
            lpFSChunk->of.hwndOwner         = (HWND) G_hWndMain;
            lpFSChunk->of.hInstance         = (HANDLE) NULL;
            lpFSChunk->of.lpstrCustomFilter = (LPSTR) NULL;
            lpFSChunk->of.nMaxCustFilter    = 0L;
            lpFSChunk->of.nFilterIndex      = 1L;
            lpFSChunk->of.lpstrFile         = lpFSChunk->szFile;
            lpFSChunk->of.nMaxFile          = (DWORD) sizeof (lpFSChunk->szFile);
            lpFSChunk->of.lpstrFileTitle    = lpFSChunk->szFileTitle;
            lpFSChunk->of.nMaxFileTitle     = MAXFILTERLEN;
            lpFSChunk->of.lpstrTitle        = (LPSTR) NULL;
            lpFSChunk->of.Flags             = OFN_HIDEREADONLY
                | OFN_OVERWRITEPROMPT;
            lpFSChunk->of.nFileOffset       = 0;
            lpFSChunk->of.nFileExtension    = 0;
            lpFSChunk->of.lCustData         = 0L;
            lpFSChunk->of.lpfnHook          = (unsigned int (__stdcall *)(void *,unsigned int,unsigned int,long)) NULL;
            lpFSChunk->of.lpTemplateName    = (LPCSTR) NULL;
            break;
    } /* End switch. */

    /*--------------------------------------------------*/
    /* Fill in details according to file type required. */
    /*--------------------------------------------------*/

    switch (wCommDlgType)
    {
        /*--------------------------------*/
        /* Set up for opening a CPF file. */
        /*--------------------------------*/

        case CPFFILEOPENDLG:
            strcat ((char *)M_pszEnvPath, "\\cpf");
            lpFOChunk->of.lpstrFilter       = M_pszInCPFFilter;
            lpFOChunk->of.lpstrInitialDir   = (const char *)M_pszEnvPath;
            lpFOChunk->of.lpstrDefExt       = (LPSTR) "cpf";
            break;

        /*--------------------------------*/
        /* Set up for opening a PPF file. */
        /*--------------------------------*/

        case PPFFILEOPENDLG:
            strcat ((char *)M_pszEnvPath, "\\ppf");
            lpFOChunk->of.lpstrFilter       = M_pszInPPFFilter;
            lpFOChunk->of.lpstrInitialDir   = (const char *)M_pszEnvPath;
            lpFOChunk->of.lpstrDefExt       = (LPSTR) "ppf";
            break;

        /*--------------------------------*/
        /* Set up for opening a IMG file. */
        /*--------------------------------*/

        case IMGFILEOPENDLG:
            strcat ((char *)M_pszEnvPath, "\\img");
            lpFOChunk->of.lpstrFilter       = M_pszInImgFilter;
            lpFOChunk->of.lpstrInitialDir   = (LPSTR) M_pszEnvPath;
            lpFOChunk->of.lpstrDefExt       = (LPSTR) "img";
            break;

        /*-----------------------------*/
        /* Set up opening a Tiff file. */
        /*-----------------------------*/

        case TIFFFILEOPENDLG:
            strcat ((char *)M_pszEnvPath, "\\img");
            lpFOChunk->of.lpstrFilter       = M_pszInTifFilter;
            lpFOChunk->of.lpstrInitialDir   = (LPSTR) M_pszEnvPath;
            lpFOChunk->of.lpstrDefExt       = (LPSTR) "tif";
            break;

        /*-----------------------------------------*/
        /* Set up for save a file to Dipix format. */
        /*-----------------------------------------*/

        case FILESAVEDLG:
            strcat ((char *)M_pszEnvPath, "\\img");
            lpFSChunk->of.lpstrFilter       = M_pszOutImgFilter;
            lpFSChunk->of.lpstrInitialDir   = (LPSTR) M_pszEnvPath;
            lpFSChunk->of.lpstrDefExt       = (LPSTR) "img";
            break;

        /*---------------------------------*/
        /* Set up for save a file to Tiff. */
        /*---------------------------------*/

        case TIFFFILESAVEDLG:
            strcat ((char *)M_pszEnvPath, "\\img");
            lpFSChunk->of.lpstrFilter       = M_pszOutTifFilter;
            lpFSChunk->of.lpstrInitialDir   = (LPSTR) M_pszEnvPath;
            lpFSChunk->of.lpstrDefExt       = (LPSTR) "tif";
            break;
    } /* End switch. */

    return;

} /* End of the InitializeStruct function. */

/*============================================================================
*
*  Name        : OpenFileDialog
*  Author      : Pierre Landriau
*  Description : Initialize the OPENFILE structure and call the file open
*                common dialog.
*
*  Parameters  :
*       wCommDlgType        Dialog type, tiff or dipix.
*       lpszOpenName        Name of the input file.
*
*  Returns     : Error status
*
*  Context/Restrictions:
*
*===========================================================================*/

LONG OpenFileDialog (WORD wCommDlgType, LPSTR lpszOpenName)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    HANDLE    hChunk;
    LPFOCHUNK lpFOChunk;

    LONG    lStatus = P_SUCCESS;

    /*-----------------------*/
    /* Allocate some memory. */
    /*-----------------------*/

    hChunk = GlobalAlloc (GMEM_FIXED, sizeof (FOCHUNK));

    if (hChunk)
    {
        lpFOChunk = (LPFOCHUNK) GlobalLock (hChunk);
        if (!lpFOChunk)
        {
            GlobalFree (hChunk);
            lpFOChunk = NULL;
            return P_ERROR_WINDOWS;
        } /* End if. */
    }
    else
    {
        lpFOChunk = NULL;
        return P_ERROR_WINDOWS;
    } /* End if, else. */

    /*---------------------------*/
    /* Initialize the structure. */
    /*---------------------------*/

    InitializeStruct (wCommDlgType, (LPSTR) lpFOChunk);
    if (!GetOpenFileName (&(lpFOChunk->of)))
    {
        lStatus = P_ERROR_WINDOWS;
    } /* End if. */

    wsprintf (lpszOpenName, (LPSTR) "%s", lpFOChunk->of.lpstrFile);
    GlobalUnlock (hChunk);
    GlobalFree (hChunk);
    return lStatus;

} /* End of the OpenFileDialog function. */

/*============================================================================
*
*  Name        : SaveFileDialog
*  Author      : Pierre Landriau
*  Description : Initialize the OPENFILE structure and call the file open
*                common dialog.
*
*  Parameters  :
*       wCommDlgType        Dialog type, tiff or dipix.
*       lpszOpenName        Name of the output file.
*
*  Returns     : Error status
*
*  Context/Restrictions:
*
*===========================================================================*/

LONG SaveFileDialog (WORD wCommDlgType, LPSTR lpszSaveName)
{
    /*------------------*/
    /* Local variables. */
    /*------------------*/

    HANDLE    hChunk;
    LPFOCHUNK lpFOChunk;

    /*-----------------------*/
    /* Allocate some memory. */
    /*-----------------------*/

    hChunk = GlobalAlloc (GMEM_FIXED, sizeof (FOCHUNK));

    if (hChunk)
    {
        lpFOChunk = (LPFOCHUNK) GlobalLock (hChunk);
        if (!lpFOChunk)
        {
            GlobalFree (hChunk);
            lpFOChunk = NULL;
            return P_ERROR_WINDOWS;
        } /* End if. */
    }
    else
    {
        lpFOChunk = NULL;
        return P_ERROR_WINDOWS;
    } /* End if. */

    /*---------------------------*/
    /* Initialize the structure. */
    /*---------------------------*/

    InitializeStruct (wCommDlgType, (LPSTR) lpFOChunk);
    if (GetSaveFileName (&(lpFOChunk->of)))
    {
        wsprintf (lpszSaveName, (LPSTR) "%s", lpFOChunk->of.lpstrFile);
    } /* End if. */

    GlobalUnlock (hChunk);
    GlobalFree (hChunk);
    return (P_SUCCESS);

} /* End of the SaveFileDialog function. */

/**========================================================================**/
