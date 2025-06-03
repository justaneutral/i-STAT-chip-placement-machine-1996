// DOUBLE.cpp : implementation file
//

#include "stdafx.h"
#include "app.h"
#include "DOUBLE.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DOUBLE

IMPLEMENT_DYNCREATE(DOUBLE, CEdit)

DOUBLE::DOUBLE()
{
	EnableAutomation();
	
	// To keep the application running as long as an OLE automation 
	//	object is active, the constructor calls AfxOleLockApp.
	
	AfxOleLockApp();
}

DOUBLE::~DOUBLE()
{
	// To terminate the application when all objects created with
	// 	with OLE automation, the destructor calls AfxOleUnlockApp.
	
	AfxOleUnlockApp();
}

void DOUBLE::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CEdit::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(DOUBLE, CEdit)
	//{{AFX_MSG_MAP(DOUBLE)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(DOUBLE, CEdit)
	//{{AFX_DISPATCH_MAP(DOUBLE)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IDOUBLE to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {7982E461-D53B-11D1-8C4D-00C04FAD4034}
static const IID IID_IDOUBLE =
{ 0x7982e461, 0xd53b, 0x11d1, { 0x8c, 0x4d, 0x0, 0xc0, 0x4f, 0xad, 0x40, 0x34 } };

BEGIN_INTERFACE_MAP(DOUBLE, CEdit)
	INTERFACE_PART(DOUBLE, IID_IDOUBLE, Dispatch)
END_INTERFACE_MAP()

// {7982E462-D53B-11D1-8C4D-00C04FAD4034}
IMPLEMENT_OLECREATE(DOUBLE, "App.DOUBLE", 0x7982e462, 0xd53b, 0x11d1, 0x8c, 0x4d, 0x0, 0xc0, 0x4f, 0xad, 0x40, 0x34)

/////////////////////////////////////////////////////////////////////////////
// DOUBLE message handlers
