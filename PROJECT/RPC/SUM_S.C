/* this ALWAYS GENERATED file contains the RPC server stubs */


/* File created by MIDL compiler version 3.01.76 */
/* at Tue Apr 28 15:35:23 1998
 */
/* Compiler settings for sum.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
*/
//@@MIDL_FILE_HEADING(  )

#include <string.h>
#include "sum.h"

#define TYPE_FORMAT_STRING_SIZE   1                                 
#define PROC_FORMAT_STRING_SIZE   5                                 

typedef struct _MIDL_TYPE_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ TYPE_FORMAT_STRING_SIZE ];
    } MIDL_TYPE_FORMAT_STRING;

typedef struct _MIDL_PROC_FORMAT_STRING
    {
    short          Pad;
    unsigned char  Format[ PROC_FORMAT_STRING_SIZE ];
    } MIDL_PROC_FORMAT_STRING;

extern const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString;
extern const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString;

/* Standard interface: sumup, ver. 1.0,
   GUID={0x55449658,0x4321,0x1234,{0x43,0x21,0x98,0x76,0x54,0x32,0x1C,0xBA}} */


extern RPC_DISPATCH_TABLE sumup_v1_0_DispatchTable;

static const RPC_SERVER_INTERFACE sumup___RpcServerInterface =
    {
    sizeof(RPC_SERVER_INTERFACE),
    {{0x55449658,0x4321,0x1234,{0x43,0x21,0x98,0x76,0x54,0x32,0x1C,0xBA}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    &sumup_v1_0_DispatchTable,
    0,
    0,
    0,
    0,
    0
    };
RPC_IF_HANDLE sumup_v1_0_s_ifspec = (RPC_IF_HANDLE)& sumup___RpcServerInterface;

extern const MIDL_STUB_DESC sumup_StubDesc;

void __RPC_STUB
sumup_SumUp(
    PRPC_MESSAGE _pRpcMessage )
{
    double _RetVal;
    MIDL_STUB_MESSAGE _StubMsg;
    short sumVal;
    RPC_STATUS _Status;
    
    ((void)(_Status));
    NdrServerInitializeNew(
                          _pRpcMessage,
                          &_StubMsg,
                          &sumup_StubDesc);
    
    RpcTryFinally
        {
        if ( (_pRpcMessage->DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[0] );
        
        sumVal = *(( short __RPC_FAR * )_StubMsg.Buffer)++;
        
        
        _RetVal = SumUp(sumVal);
        
        _StubMsg.BufferLength = 8U;
        _pRpcMessage->BufferLength = _StubMsg.BufferLength;
        
        _Status = I_RpcGetBuffer( _pRpcMessage ); 
        if ( _Status )
            RpcRaiseException( _Status );
        
        _StubMsg.Buffer = (unsigned char __RPC_FAR *) _pRpcMessage->Buffer;
        
        *(( double __RPC_FAR * )_StubMsg.Buffer)++ = _RetVal;
        
        }
    RpcFinally
        {
        }
    RpcEndFinally
    _pRpcMessage->BufferLength = 
        (unsigned int)((long)_StubMsg.Buffer - (long)_pRpcMessage->Buffer);
    
}


static const MIDL_STUB_DESC sumup_StubDesc = 
    {
    (void __RPC_FAR *)& sumup___RpcServerInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    0,
    0,
    0,
    0,
    0,
    __MIDL_TypeFormatString.Format,
    0, /* -error bounds_check flag */
    0x10001, /* Ndr library version */
    0,
    0x301004c, /* MIDL Version 3.1.76 */
    0,
    0,
    0,  /* Reserved1 */
    0,  /* Reserved2 */
    0,  /* Reserved3 */
    0,  /* Reserved4 */
    0   /* Reserved5 */
    };

static RPC_DISPATCH_FUNCTION sumup_table[] =
    {
    sumup_SumUp,
    0
    };
RPC_DISPATCH_TABLE sumup_v1_0_DispatchTable = 
    {
    1,
    sumup_table
    };

#if !defined(__RPC_WIN32__)
#error  Invalid build platform for this stub.
#endif

static const MIDL_PROC_FORMAT_STRING __MIDL_ProcFormatString =
    {
        0,
        {
			0x4e,		/* FC_IN_PARAM_BASETYPE */
			0x6,		/* FC_SHORT */
/*  2 */	0x53,		/* FC_RETURN_PARAM_BASETYPE */
			0xc,		/* FC_DOUBLE */

			0x0
        }
    };

static const MIDL_TYPE_FORMAT_STRING __MIDL_TypeFormatString =
    {
        0,
        {

			0x0
        }
    };
