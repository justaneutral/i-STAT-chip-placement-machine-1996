/* this ALWAYS GENERATED file contains the RPC client stubs */


/* File created by MIDL compiler version 3.01.76 */
/* at Tue Apr 28 15:35:23 1998
 */
/* Compiler settings for sum.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: none
*/
//@@MIDL_FILE_HEADING(  )

#include <string.h>
#if defined( _ALPHA_ )
#include <stdarg.h>
#endif

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

handle_t SumUpHandle;


static const RPC_CLIENT_INTERFACE sumup___RpcClientInterface =
    {
    sizeof(RPC_CLIENT_INTERFACE),
    {{0x55449658,0x4321,0x1234,{0x43,0x21,0x98,0x76,0x54,0x32,0x1C,0xBA}},{1,0}},
    {{0x8A885D04,0x1CEB,0x11C9,{0x9F,0xE8,0x08,0x00,0x2B,0x10,0x48,0x60}},{2,0}},
    0,
    0,
    0,
    0,
    0,
    0
    };
RPC_IF_HANDLE sumup_v1_0_c_ifspec = (RPC_IF_HANDLE)& sumup___RpcClientInterface;

extern const MIDL_STUB_DESC sumup_StubDesc;

static RPC_BINDING_HANDLE sumup__MIDL_AutoBindHandle;


double SumUp( 
    /* [in] */ short sumVal)
{

    RPC_BINDING_HANDLE _Handle	=	0;
    
    double _RetVal;
    
    RPC_MESSAGE _RpcMessage;
    
    MIDL_STUB_MESSAGE _StubMsg;
    
    RpcTryFinally
        {
        NdrClientInitializeNew(
                          ( PRPC_MESSAGE  )&_RpcMessage,
                          ( PMIDL_STUB_MESSAGE  )&_StubMsg,
                          ( PMIDL_STUB_DESC  )&sumup_StubDesc,
                          0);
        
        
        _Handle = SumUpHandle;
        
        
        _StubMsg.BufferLength = 2U;
        NdrGetBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg, _StubMsg.BufferLength, _Handle );
        
        *(( short __RPC_FAR * )_StubMsg.Buffer)++ = sumVal;
        
        NdrSendReceive( (PMIDL_STUB_MESSAGE) &_StubMsg, (unsigned char __RPC_FAR *) _StubMsg.Buffer );
        
        if ( (_RpcMessage.DataRepresentation & 0X0000FFFFUL) != NDR_LOCAL_DATA_REPRESENTATION )
            NdrConvert( (PMIDL_STUB_MESSAGE) &_StubMsg, (PFORMAT_STRING) &__MIDL_ProcFormatString.Format[0] );
        
        _RetVal = *(( double __RPC_FAR * )_StubMsg.Buffer)++;
        
        }
    RpcFinally
        {
        NdrFreeBuffer( (PMIDL_STUB_MESSAGE) &_StubMsg );
        
        }
    RpcEndFinally
    
    return _RetVal;
}


static const MIDL_STUB_DESC sumup_StubDesc = 
    {
    (void __RPC_FAR *)& sumup___RpcClientInterface,
    MIDL_user_allocate,
    MIDL_user_free,
    &SumUpHandle,
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
