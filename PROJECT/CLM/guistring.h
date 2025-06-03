// guistring.h: interface for the guistring class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GUISTRING_H__7982E464_D53B_11D1_8C4D_00C04FAD4034__INCLUDED_)
#define AFX_GUISTRING_H__7982E464_D53B_11D1_8C4D_00C04FAD4034__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#include "EditString.h"

class guistring  
{
public:
	guistring(int x, int y, int w, int h);
	virtual ~guistring();


    CEditString *wnd;
	CStatic *stat;

    int m_x;
    int m_y;
    int m_w;
    int m_h;

	void name(char *name);
    void type(char *string);
    bool accept(char *string);
	void destroy();
};

#endif // !defined(AFX_GUISTRING_H__7982E464_D53B_11D1_8C4D_00C04FAD4034__INCLUDED_)
