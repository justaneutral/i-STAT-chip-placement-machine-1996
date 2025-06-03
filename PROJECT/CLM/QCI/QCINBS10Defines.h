/******************************************************************************\
*                                                                              *
*  Copyright (c) 1993-1994 Cimetrics Technology. All rights reserved.          *
*                                                                              *
*  File Name:                                                                  *
*                                                                              *
*    ddk\src\nbs10\ntdbs10.h                                                   *
*                                                                              *
*  Description:                                                                *
*                                                                              *
*    This is the include file that defines all constants and types for         *
*  accessing the NBS-10 Windows NT device driver.                              *
*                                                                              *
*  Author:                                                                     *
*                                                                              *
*    Vladimir B. Rozov (E-mail: vrozov@classic.iki.rssi.ru)                    *
*                                                                              *
*  Environment:                                                                *
*                                                                              *
*    Windows NT Kernel NBS-10 Device Driver                                    *
*                                                                              *
*  Revision History:                                                           *
*                                                                              *
\******************************************************************************/

#ifndef _NTDNBS10
#define _NTDNBS10

#ifndef CTL_CODE
#include <winioctl.h>
#endif /* CTL_CODE */

/*
   NtDeviceIoControlFile IoControlCode values for the NBS-10 Device Driver.
*/
#define IOCTL_NBS10_SET_BAUD_RATE         CTL_CODE(FILE_DEVICE_SERIAL_PORT, 1,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_SET_QUEUE_SIZE        CTL_CODE(FILE_DEVICE_SERIAL_PORT, 2,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_SET_LINE_CONTROL      CTL_CODE(FILE_DEVICE_SERIAL_PORT, 3,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_SET_BREAK_ON          CTL_CODE(FILE_DEVICE_SERIAL_PORT, 4,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_SET_BREAK_OFF         CTL_CODE(FILE_DEVICE_SERIAL_PORT, 5,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_IMMEDIATE_CHAR        CTL_CODE(FILE_DEVICE_SERIAL_PORT, 6,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_SET_TIMEOUTS          CTL_CODE(FILE_DEVICE_SERIAL_PORT, 7,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_GET_TIMEOUTS          CTL_CODE(FILE_DEVICE_SERIAL_PORT, 8,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_SET_DTR               CTL_CODE(FILE_DEVICE_SERIAL_PORT, 9,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_CLR_DTR               CTL_CODE(FILE_DEVICE_SERIAL_PORT,10,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_RESET_DEVICE          CTL_CODE(FILE_DEVICE_SERIAL_PORT,11,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_SET_RTS               CTL_CODE(FILE_DEVICE_SERIAL_PORT,12,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_CLR_RTS               CTL_CODE(FILE_DEVICE_SERIAL_PORT,13,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_SET_XOFF              CTL_CODE(FILE_DEVICE_SERIAL_PORT,14,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_SET_XON               CTL_CODE(FILE_DEVICE_SERIAL_PORT,15,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_GET_WAIT_MASK         CTL_CODE(FILE_DEVICE_SERIAL_PORT,16,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_SET_WAIT_MASK         CTL_CODE(FILE_DEVICE_SERIAL_PORT,17,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_WAIT_ON_MASK          CTL_CODE(FILE_DEVICE_SERIAL_PORT,18,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_PURGE                 CTL_CODE(FILE_DEVICE_SERIAL_PORT,19,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_GET_BAUD_RATE         CTL_CODE(FILE_DEVICE_SERIAL_PORT,20,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_GET_LINE_CONTROL      CTL_CODE(FILE_DEVICE_SERIAL_PORT,21,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_GET_CHARS             CTL_CODE(FILE_DEVICE_SERIAL_PORT,22,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_SET_CHARS             CTL_CODE(FILE_DEVICE_SERIAL_PORT,23,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_GET_HANDFLOW          CTL_CODE(FILE_DEVICE_SERIAL_PORT,24,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_SET_HANDFLOW          CTL_CODE(FILE_DEVICE_SERIAL_PORT,25,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_GET_MODEMSTATUS       CTL_CODE(FILE_DEVICE_SERIAL_PORT,26,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_GET_DEVICE_STATUS     CTL_CODE(FILE_DEVICE_SERIAL_PORT,27,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_XOFF_COUNTER          CTL_CODE(FILE_DEVICE_SERIAL_PORT,28,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_GET_PROPERTIES        CTL_CODE(FILE_DEVICE_SERIAL_PORT,29,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_GET_DTRRTS            CTL_CODE(FILE_DEVICE_SERIAL_PORT,30,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_LSRMST_INSERT         CTL_CODE(FILE_DEVICE_SERIAL_PORT,31,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_SET_FIFOLEVEL         CTL_CODE(FILE_DEVICE_SERIAL_PORT,32,METHOD_BUFFERED,FILE_ANY_ACCESS)
#define IOCTL_NBS10_GET_FIFOLEVEL         CTL_CODE(FILE_DEVICE_SERIAL_PORT,33,METHOD_BUFFERED,FILE_ANY_ACCESS)


/*
   NtDeviceIoControlFile InputBuffer/OutputBuffer structures definitions for the
   NBS-10 device driver.
*/


/*
   This structure used to resize the input/output buffers. It should be used only
   with the IOCTL_NBS10_SET_QUEUE_SIZE Ioctl. The driver reserves the right to
   allocate a larger buffer.
*/
typedef struct _NBS10_QUEUE_SIZE {
  ULONG InSize;
  ULONG OutSize;
} NBS10_QUEUE_SIZE, *PNBS10_QUEUE_SIZE;

/*
   This structure used to set/get line parameters. It should be used only with
   the IOCTL_NBS10_SET_LINE_CONTROL and IOCTL_NBS10_GET_LINE_CONTROL.
*/
typedef struct _NBS10_LINE_CONTROL {
  UCHAR StopBits;
  UCHAR Parity;
  UCHAR WordLength;
} NBS10_LINE_CONTROL,*PNBS10_LINE_CONTROL;

/*
   Possible settings for the StopBits in the NBS10_LINE_CONTROL structure.
*/
#define STOP_BIT_1      (UCHAR)0
#define STOP_BITS_1_5   (UCHAR)1 // Valid only for the Character Length 5
#define STOP_BITS_2     (UCHAR)2 // Valid only for the Character Length 6-9

/*
   Possible settings for the Parity in the NBS10_LINE_CONTROL structure.
*/
#define NO_PARITY        (UCHAR)0
#define ODD_PARITY       (UCHAR)1
#define EVEN_PARITY      (UCHAR)2
#define MARK_PARITY      (UCHAR)3
#define SPACE_PARITY     (UCHAR)4

/*
   This structure used to set/get Timeouts. It should be used only with the
   IOCTL_NBS10_SET_TIMEOUTS and IOCTL_NBS10_GET_TIMEOUTS.
*/
typedef struct _NBS10_TIMEOUTS {
  ULONG ReadIntervalTimeout;
  ULONG ReadTotalTimeoutMultiplier;
  ULONG ReadTotalTimeoutConstant;
  ULONG WriteTotalTimeoutMultiplier;
  ULONG WriteTotalTimeoutConstant;
} NBS10_TIMEOUTS,*PNBS10_TIMEOUTS;

/*
   This type definition used to set/get Baud Rate. It should be used only with
   the IOCTL_NBS10_SET_BAUD_RATE and IOCTL_NBS10_GET_BAUD_RATE.
*/
typedef ULONG NBS10_BAUD_RATE, *PNBS10_BAUD_RATE;

/*
   Bitmask definitions of the Waitable Events for the NBS-10 Device Driver.
*/
#define NBS10_EV_RXCHAR           0x0001  // Character received
#define NBS10_EV_TXEMPTY          0x0004  // Tx Machine Idle
#define NBS10_EV_CTS              0x0008  // CTS changed state
#define NBS10_EV_ERR              0x0080  // Line status error

/*
   Bitmask definitions for the IOCTL_NBS10_PURGE.
*/
#define NBS10_PURGE_TXABORT 0x00000001   // Implies the current and all pending writes.
#define NBS10_PURGE_RXABORT 0x00000002   // Implies the current and all pending reads.
#define NBS10_PURGE_TXCLEAR 0x00000004   // Implies the transmit buffer if exists.
#define NBS10_PURGE_RXCLEAR 0x00000008   // Implies the receive buffer if exists.

/*
   This structure is used to set/get Special Characters. Note that this feature
   was not yet implemented for the NBS-10 Device Driver and it was introduced
   for the compatibility with the Windows NT Serial Device Driver.
*/
typedef struct _NBS10_CHARS {
  UCHAR EofChar;
  UCHAR ErrorChar;
  UCHAR BreakChar;
  UCHAR EventChar;
  UCHAR XonChar;
  UCHAR XoffChar;
} NBS10_CHARS, *PNBS10_CHARS;

/*
   This structure is used to set/get current settings of the Handshake and Flow
   Control. It should be used only with the IOCTL_NBS10_SET_HANDFLOW and
   IOCTL_NBS10_GET_HANDFLOW. Note that Xon/Xoff feature was not yet implemented
   for the NBS-10 Device Driver.
*/
typedef struct _NBS10_HANDFLOW {
  ULONG ControlHandShake;
  ULONG FlowReplace;
  LONG XonLimit;
  LONG XoffLimit;
} NBS10_HANDFLOW,*PNBS10_HANDFLOW;

/*
   Possible settings for the ControlHandShake field of the NBS10_HANDFLOW.
*/
/*
   ControlHadshake Field settings.
*/
#define NBS10_DTR_CONTROL         ((ULONG)0x01)       // DTR always on, not implemented
#define NBS10_DTR_HANDSHAKE       ((ULONG)0x02)       // DTR Handshaking, not implemented
#define NBS10_DTR_TRANSMIT_TOGGLE ((ULONG)0x03)       // DTR Transmit Toggle, hardware settings
#define NBS10_DTR_MASK            ((ULONG)0x03)       // DTR Mask in the Handshake
#define NBS10_CTS_HANDSHAKE       ((ULONG)0x08)       // CTS Handshake
#define NBS10_DSR_HANDSHAKE       ((ULONG)0x10)       // DSR Handshake, not implemented
#define NBS10_DCD_HANDSHAKE       ((ULONG)0x20)       // DCD Handshake, not implemented
#define NBS10_OUT_HANDSHAKEMASK   ((ULONG)0x38)       // Handshake Mask
#define NBS10_DSR_SENSITIVITY     ((ULONG)0x40)       // DSR sensitivity, not implemented
#define NBS10_ERROR_ABORT         ((ULONG)0x80000000) // Abort all Irps
/*
   Flow Replace Field settings.
*/
#define NBS10_AUTO_TRANSMIT       ((ULONG)0x01)       // Not implemented
#define NBS10_AUTO_RECEIVE        ((ULONG)0x02)       // Not implemented
#define NBS10_ERROR_CHAR          ((ULONG)0x04)       // Not implemented
#define NBS10_NULL_STRIPPING      ((ULONG)0x08)       // Not implemented
#define NBS10_BREAK_CHAR          ((ULONG)0x10)       // Not implemented
#define NBS10_RTS_CONTROL         ((ULONG)0x40)       // Rts always on, not implemented
#define NBS10_RTS_HANDSHAKE       ((ULONG)0x80)       // Rts Handshake, not implemented
#define NBS10_RTS_TRANSMIT_TOGGLE ((ULONG)0xc0)       // Rts Transmit Toggle, hardware settings
#define NBS10_RTS_MASK            ((ULONG)0xc0)       // Rts Mask in the Flow Replace
#define NBS10_XOFF_CONTINUE       ((ULONG)0x80000000) // Not implemented

/*
   Reasons for the Transmit holding state.
*/
#define NBS10_TX_WAITING_FOR_CTS      ((ULONG)0x00000001)

/*
   Pendings errors on the device.
*/
#define NBS10_ERROR_BREAK             (0x00000001)
#define NBS10_ERROR_FRAMING           (0x00000002)
#define NBS10_ERROR_OVERRUN           (0x00000004)
#define NBS10_ERROR_QUEUEOVERRUN      (0x00000008)
#define NBS10_ERROR_PARITY            (0x00000010)

/*
   This structure is used to get the current error and general status of the
   device. It should be used only with the IOCTL_NBS10_GET_STATUS.
*/
typedef struct _NBS10_DEVICE_STATUS {
  ULONG Errors;                       // Communication errors occurred since last call
  ULONG HoldReasons;                  // Transmit Hold reason.
  ULONG AmountInInQueue;              // Amount of Bytes in Internal Read Buffer
  ULONG AmountInOutQueue;             // Amount of Bytes in Internal Write Buffer. Always zero for NBS-10
  BOOLEAN EofReceived;                // End Of File Received. Always FALSE for NBS-10
  BOOLEAN WaitForImmediate;           // Waiting to transmit Immediate. Always zero for NBS-10
} NBS10_DEVICE_STATUS, *PNBS10_DEVICE_STATUS;

/*
   The following structure (and defines) are passed back by the NBS-10 device
   driver in response to the get properties ioctl.
*/
#define NBS10_DP_SERIALDEVICE         ((ULONG)0x00000001)

/*
   Provider subtypes.
*/
#define NBS10_DP_UNSPECIFIED       ((ULONG)0x00000000)
#define NBS10_DP_RS232             ((ULONG)0x00000001)
#define NBS10_DP_PARALLEL          ((ULONG)0x00000002)
#define NBS10_DP_RS422             ((ULONG)0x00000003)
#define NBS10_DP_RS423             ((ULONG)0x00000004)
#define NBS10_DP_RS449             ((ULONG)0x00000005)
#define NBS10_DP_RS485             ((ULONG)0x00000006)

/*
   Provider capabilities flags.
*/
#define NBS10_PCF_DTRDSR        ((ULONG)0x0001)
#define NBS10_PCF_RTSCTS        ((ULONG)0x0002)
#define NBS10_PCF_CD            ((ULONG)0x0004)
#define NBS10_PCF_PARITY_CHECK  ((ULONG)0x0008)
#define NBS10_PCF_XONXOFF       ((ULONG)0x0010)
#define NBS10_PCF_SETXCHAR      ((ULONG)0x0020)
#define NBS10_PCF_TOTALTIMEOUTS ((ULONG)0x0040)
#define NBS10_PCF_INTTIMEOUTS   ((ULONG)0x0080)
#define NBS10_PCF_SPECIALCHARS  ((ULONG)0x0100)
#define NBS10_PCF_9BITMODE      ((ULONG)0x0200)

/*
   NBS-10 provider settable parameters.
*/
#define NBS10_SP_PARITY         ((ULONG)0x0001)
#define NBS10_SP_BAUD           ((ULONG)0x0002)
#define NBS10_SP_DATABITS       ((ULONG)0x0004)
#define NBS10_SP_STOPBITS       ((ULONG)0x0008)
#define NBS10_SP_HANDSHAKING    ((ULONG)0x0010)
#define NBS10_SP_PARITY_CHECK   ((ULONG)0x0020)
#define NBS10_SP_CARRIER_DETECT ((ULONG)0x0040)

/*
   Settable baud rates in the provider.
*/
#define NBS10_BAUD_USER         ((ULONG)0x10000000)

/*
   Settable Data Bits
*/
#define NBS10_DATABITS_5        ((USHORT)0x0001)
#define NBS10_DATABITS_6        ((USHORT)0x0002)
#define NBS10_DATABITS_7        ((USHORT)0x0004)
#define NBS10_DATABITS_8        ((USHORT)0x0008)
#define NBS10_DATABITS_9        ((USHORT)0x0010)

/*
   Settable Stop and Parity bits.
*/
#define NBS10_STOPBITS_10       ((USHORT)0x0001)
#define NBS10_STOPBITS_15       ((USHORT)0x0002)
#define NBS10_STOPBITS_20       ((USHORT)0x0004)
#define NBS10_PARITY_NONE       ((USHORT)0x0100)
#define NBS10_PARITY_ODD        ((USHORT)0x0200)
#define NBS10_PARITY_EVEN       ((USHORT)0x0400)
#define NBS10_PARITY_MARK       ((USHORT)0x0800)
#define NBS10_PARITY_SPACE      ((USHORT)0x1000)

typedef struct _NBS10_DEVICE_PROPERTIES {
  USHORT PacketLength;
  USHORT PacketVersion;
  ULONG ServiceMask;
  ULONG Reserved1;
  ULONG MaxTxQueue;
  ULONG MaxRxQueue;
  ULONG MaxBaud;
  ULONG ProvSubType;
  ULONG ProvCapabilities;
  ULONG SettableParams;
  ULONG SettableBaud;
  USHORT SettableData;
  USHORT SettableStopParity;
  ULONG CurrentTxQueue;
  ULONG CurrentRxQueue;
  ULONG ProvSpec1;
  ULONG ProvSpec2;
  WCHAR ProvChar[1];
} NBS10_DEVICE_PROPERTIES, *PNBS10_DEVICE_PROPERTIES;


#endif  // _NTDNBS10
