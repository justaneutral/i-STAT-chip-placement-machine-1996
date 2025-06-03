// guistring.cpp: implementation of the guistring class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "app.h"
#include "guistring.h"
#include "globals.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


extern void doevents();


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

guistring::guistring(int x, int y, int w, int h)
{
   stat = NULL;
   wnd = NULL;
   m_x = x;
   m_y = y;
   m_w = w;
   m_h = h;
}

guistring::~guistring()
{
   destroy();
}

void guistring::name(char *name)
{
   if(stat == NULL)
   {
     stat = new CStatic();
   	 stat->Create("",WS_CHILD|WS_VISIBLE,CRect(m_x,m_y-20,m_x+m_w,m_y+m_h-20),g_Wnd);
   }
   stat->SetWindowText(name);
}

void guistring::type(char *string)
{
   if(wnd == NULL)
   {
	   wnd = new CEditString();
       wnd->Create(WS_CHILD|WS_VISIBLE|WS_BORDER,CRect(m_x,m_y,m_x+m_w,m_y+m_h),g_Wnd,0);
   }
   wnd->SetWindowText(string);
}

bool guistring::accept(char *string)
{
	static char str[80];
	bool st = true;
	
	doevents();
	
	if(wnd != NULL)
	{
		wnd->GetWindowText(string,80);
		strcpy(str,wnd->string);
		if(!(wnd->ready_f)) st = false;
	}
	
	strcpy(string,str);
    return st;
}

void guistring::destroy()
{
   if(wnd != NULL) delete wnd;
   wnd = NULL;
   if(stat != NULL) delete stat;
   stat = NULL;
}