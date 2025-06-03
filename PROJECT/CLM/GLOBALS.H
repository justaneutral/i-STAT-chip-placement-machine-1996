
#include "app.h"
#include "console.h"
#include "machineSettings.h"
#include "qci\\QCINBS10CommDevice.h"
#include "qci\\QCICommands.h"
#include "qci\\QCIAxes.h"
#include "smac\\SMAC.h"
#include "barcode\\barcode.h"
#include "io\\IOBoard.h"
#include "machineState.h"
#include "errorHandle.h"
#include "errorLog.h"
#include "../fileaccess/fileaccess.h"
#include "FileTimeoutDialog.h"
#include "ConsoleFake.h"


#define _PRINCETON_


typedef struct _CURRENT_USER
{
	BOOL      Restrictions;
	CString   UserID;
	CString   FullName;

}  CURRENT_USER;





#if defined __main_module__

BOOL machine_is_initialized = FALSE;
int indicator = 0;
CURRENT_USER g_current_user;
bool kbHit = false;
application theApp;
machineSettings* g_machine = NULL;
#if defined _PRINCETON_
CQCINBS10CommDevice g_Comm_Dev(256);
#endif
CQCICommands g_Cmnds;
CQCIAxes g_Axes;
CSMAC g_smac;
CBarcode g_barcode;
CIOBoard g_io(0x300);
CWnd* g_Wnd;
HANDLE g_in,g_out;
bool is_in_critical_section;
machineState g_machine_state;
errorLog g_error_log;
//errorHandle g_error_handle;
CConsoleFake g_wcons;
CEdit *g_text_output = NULL;
CString g_current_error = _T("");

#else

extern BOOL machine_is_initialized;
extern int indicator;
extern CURRENT_USER g_current_user;
extern	bool kbHit;
extern application theApp;
extern machineSettings* g_machine;
extern CQCICommands g_Cmnds;
extern CQCIAxes g_Axes;
extern CSMAC g_smac;
extern CBarcode g_barcode;
extern CIOBoard g_io;
extern CWnd* g_Wnd;
extern HANDLE g_in,g_out;
extern bool is_in_critical_section;
extern machineState g_machine_state;
extern errorLog g_error_log;
//extern errorHandle g_error_handle;
extern 	CConsoleFake g_wcons;
extern CEdit *g_text_output;
extern CString g_current_error;

#endif




//prototipes.
void doevents();