//***************************************************************
// From the book "Win32 System Services: The Heart of Windows 95
// and Windows NT"
// by Marshall Brain
// Published by Prentice Hall
//
// Copyright 1995, by Prentice Hall.
//
// This code implements the RPC client.
//***************************************************************

// conclnt.cpp

/*
This program calls an RPC function 
using a manual handle.
*/

#include <windows.h>
#include <iostream.h>
#include <rpc.h>
#include <stdlib.h>
#include "sum.h"
#include "memstub"

INT main(VOID)
{
  CHAR sumUpToStr[10];
  long theSum;
  UCHAR *stringBinding;                   

  cout << "Enter a value to compute running sum: ";
  cin.getline(sumUpToStr, 10);

  // put together string binding
  if (RpcStringBindingCompose(NULL,            
    (UCHAR *) "ncacn_ip_tcp", 
    (UCHAR *) "localhost",
    (UCHAR *) "55449", NULL, &stringBinding))
  {
    cerr << 
      "ERROR: Unable to compose string binding"
      << endl;
    return(1);
  }

  // bind to server using string binding info
  if (RpcBindingFromStringBinding(stringBinding,
    &SumUpHandle))
  {
    cerr << "ERROR: Unable to bind" << endl;
    return(1);
  }

  // free the string binding info
  if (RpcStringFree(&stringBinding))
  {
    cerr 
      << "ERROR: Unable to free string binding" 
      << endl;
    return(1);
  }

  theSum=(long)SumUp((short)atoi(sumUpToStr));

  cout << "The running sum is: " << theSum << endl;

  // release binding to server
  if (RpcBindingFree(&SumUpHandle))
  {
    cerr 
      << "ERROR: Unable to free binding" 
      << endl;
    return(1);
  }

  return(0);
}
