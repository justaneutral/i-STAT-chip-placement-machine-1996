// This code implements the RPC server body.
// conserv.cpp

#include <windows.h>
#include <iostream.h>
#include <rpc.h>
#include "sum.h"
#include "memstub"

BOOL start_rpc_server(VOID)
{
  // use the specified protocol and endpoint
  if (RpcServerUseProtseqEp(
    (UCHAR *) "ncacn_ip_tcp", 1,
    (UCHAR *) "55449", NULL))
  return FALSE;


  // register the interface
  if (RpcServerRegisterIf(sumup_v1_0_s_ifspec,
    NULL, NULL))
  return FALSE;

  // listen for and service RPC requests
  RpcServerListen(1, 5, TRUE);
  return TRUE;
}
