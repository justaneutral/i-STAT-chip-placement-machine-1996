// barcode.h: interface for the CBarcode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BARCODE_H__AE99DC61_AED3_11D1_B2BB_00C04FAD4084__INCLUDED_)
#define AFX_BARCODE_H__AE99DC61_AED3_11D1_B2BB_00C04FAD4084__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CBarcode  
{
public:
	CBarcode();
	virtual ~CBarcode();

//members.

//methods.
void Trigger_On();
void Trigger_Off();
CString & Read();
CString Get_File_Name();
};


#endif // !defined(AFX_BARCODE_H__AE99DC61_AED3_11D1_B2BB_00C04FAD4084__INCLUDED_)
