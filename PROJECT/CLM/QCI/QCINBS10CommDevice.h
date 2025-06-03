/////////////////////////////////////////////////////////////////////
// QuickSilver Controls, Inc Copyright(c) 1997
//
// QCINBS10CommDevice.h: interface for the QCINBS10CommDevice class.
//
// This is a Comm Device for a Cimetrics NBS10 ISA PC board.
//
//////////////////////////////////////////////////////////////////////

#ifndef QCINBS10CommDevice_H
#define QCINBS10CommDevice_H

#ifdef _WINNT

#include "QCICommDevice.h"
#include "QCINBS10Defines.h"

#define NBS10_MAX_MSG_LEN 256

class CQCINBS10CommDevice : public CQCICommDevice
{
public:

    ///////////////
    // Members
    ///////////////
    public:
    HANDLE m_hNBS10;
    BYTE m_txBuff[NBS10_MAX_MSG_LEN * 2];

    ///////////////
    // Constructor/Destructor
    ///////////////
    CQCINBS10CommDevice(
        int maxMsgLen
        ):
        m_hNBS10(0),
        CQCICommDevice(maxMsgLen)    // parent constructor

    {
        m_rxIncrement = 2;
        m_isAdrMsk = 1;

        // It is 4x larger
        // than the max message length because
        // there are 3 auxillary byte give with every
        // real byte when we do a read from a port
        // using this driver.
        m_rxBuff = new BYTE[maxMsgLen * 4];
        
    }

	~CQCINBS10CommDevice()
    {
        if(m_hNBS10 != 0())
            CloseNbs10(m_hNBS10);
    }


    ///////////////////////////////////
    // Base Class virtual procedures
    ///////////////////////////////////
    public:
    BOOL IsAdr(int rxBuffX)
    {   assert(IsInit());
        return (m_rxBuff[rxBuffX + 1] & m_isAdrMsk); 
    }

    char * TxPacket(int size, char * data);
    char * RxPacket(int maxMsgLen, DWORD timeout, int & numBytesRead);
    char * Init(void);

    ///////////////////////////////////
    // Virtual procedures for child classes
    ///////////////////////////////////


    ///////////////
    // Attributes
    ///////////////

    ///////////////
    // Methods 
    ///////////////

    // NBS10 specific

    // Setup/Initialization methods
    BOOL InitializeNBS10(HANDLE & hNbs10);
    BOOL nbs10Open(LPCSTR lpszNbs10Name, HANDLE & hNbs10);
    BOOL nbs10SetLineParameters(HANDLE hNbs10, UCHAR wordLength, UCHAR parity, UCHAR stopBits);
    BOOL nbs10SetBaudRate(HANDLE hNbs10, NBS10_BAUD_RATE baudRate);
    BOOL nbs10SetTimeouts(HANDLE hNbs10);
    VOID nbs10Close(HANDLE hNbs10);
    BOOL nbs10StartDeviceDriver();

    // Transmit/Receive Methods
    BOOL nbs10TransmitString(HANDLE hNbs10, LPVOID str, DWORD strLen);
    BOOL nbs10ReceiveString(HANDLE hNbs10, LPVOID str, DWORD strLen, LPDWORD bytesRcvd);
    BOOL nbs10TransmitNineBitCharString(HANDLE hNbs10, LPVOID str, DWORD strLen);
    BOOL nbs10TransmitNineBitChar(HANDLE hNbs10, WORD c);
    INT nbs10ReceiveNineBitChar(HANDLE hNbs10);

    //  Handshaking Line Functions
    BOOL nbs10SetRTS(HANDLE hNbs10);
    BOOL nbs10SetDTR(HANDLE hNbs10);
    INT nbs10ReadCTS(HANDLE hNbs10);
    BOOL nbs10CTSHandshakeMode(HANDLE hNbs10);
    BOOL nbs10PurgeAll(HANDLE hNbs10);
    BOOL nbs10WaitForReceivedCharacter(HANDLE hNbs10);
    BOOL nbs10DisplayProperties(HANDLE hNbs10);
    BOOL nbs10SetInternalRcvBufferSize(HANDLE hNbs10, ULONG newSize);
    BOOL nbs10DisplayDeviceStatus(HANDLE hNbs10);
    BOOL nbs10ErrorAbortMode(HANDLE hNbs10);

    // Wrapper Functions for compatibility with Cimetrics 95 driver
    BOOL WINAPI SendNbs10(
        HANDLE hNbs10,          // Handle to device driver
        LPBYTE lpBuffer,        // 2 byte wide buffer (MSB=9th bit, LSB=data)
        WORD wBufferSize,       // Length of buffer in bytes (data length = 1/2 this value)
        LPWORD lpwBytesWritten  // Bytes actually writter (provided for backward compatibility)
        );
    BOOL WINAPI ReceiveNbs10(
        HANDLE hNbs10, 
        LPBYTE lpBuffer, 
        WORD wBufferSize, 
        LPWORD lpwBytesReceived
        );
    BOOL WINAPI CloseNbs10(HANDLE hNbs10);


};

#endif // WINNT
#endif // QCINBS10CommDevice_H
