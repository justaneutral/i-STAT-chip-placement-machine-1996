/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 3.01.76 */
/* at Tue Apr 28 15:35:23 1998
 */
/* Compiler settings for sum.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
*/
//@@MIDL_FILE_HEADING(  )
#include "rpc.h"
#include "rpcndr.h"

#ifndef __sum_h__
#define __sum_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __sumup_INTERFACE_DEFINED__
#define __sumup_INTERFACE_DEFINED__

/****************************************
 * Generated header for interface: sumup
 * at Tue Apr 28 15:35:23 1998
 * using MIDL 3.01.76
 ****************************************/
/* [implicit_handle][version][uuid] */ 


double SumUp( 
    /* [in] */ short sumVal);


extern handle_t SumUpHandle;


extern RPC_IF_HANDLE sumup_v1_0_c_ifspec;
extern RPC_IF_HANDLE sumup_v1_0_s_ifspec;
#endif /* __sumup_INTERFACE_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
