/////////////////////////////////////////////////////////////////////
// QuickSilver Controls, Inc Copyright(c) 1997
// Class QCICommDevice
//
// Abstract class used as an interface to all communication devices including
// Standard PC RS232, Cimetrics NBS-10,20, LCS 8880
/////////////////////////////////////////////////////////////////////

#ifndef QCICOMMDEVICE_H
#define QCICOMMDEVICE_H

#include <assert.h>

// Defines
#ifndef TRACE
#define TRACE(string) fprintf(stderr,"%s %d %s\n",__FILE__,__LINE__,string);
#endif

typedef unsigned long       DWORD;
typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned char       UCHAR;
typedef unsigned short      WORD;
typedef unsigned int        UINT;

class CQCICommDevice
{
public:

    ///////////////
    // Constructor/Destructor
    ///////////////
    CQCICommDevice(int maxMsgLen):
        m_init(FALSE),
        m_isAdrMsk(0),      // Default is no address mask
        m_startChar(0),
	    m_endChar(0),
        m_maxMsgLen(maxMsgLen),
        m_rxBuff(NULL),  
        m_rxIncrement(1),   // Default is increment one byte at a time
        m_valid(0xEADEAD)

        {
        };
    virtual ~CQCICommDevice() 
    {
        delete [] m_rxBuff;
    };

    ///////////////
    // Members
    ///////////////
    BOOL m_init;            // Indicates when the device is initialized
    UCHAR m_isAdrMsk;       // Mask used to find address bit
	UCHAR m_endChar;        // End Charactor
    UCHAR m_startChar;      // Start Charactor
    int m_maxMsgLen;        // Maximum allowable message to be received or transmitted
    BYTE * m_rxBuff;        // Receive buffer,  
    int m_rxIncrement;      // number of bytes to increment 
                            // through the RxBuff to get 
                            // to the next data element
    unsigned int m_valid;   // a unique number used to check validity of object


    

    ///////////////
    // Attributes
    ///////////////
    BOOL IsInit(void)
    {   assert(IsValid());
        return m_init; }

    BOOL IsValid(void)
    {   return m_valid == 0xEADEAD; }

    int GetRxIncrement(void)
    {   
        assert(IsInit());
        return m_rxIncrement; 
    }

    BYTE RxBuff(int rxBuffX)
    {   
        assert(IsInit());
        return m_rxBuff[rxBuffX]; 
    }

    BOOL IsEndChar(int rxBuffX)
    {
        assert(IsInit());
        return (m_rxBuff[rxBuffX] == m_endChar);
    }
    BOOL IsStartChar(int rxBuffX)
    {
        assert(IsInit());
        return (m_rxBuff[rxBuffX] == m_startChar);
    }
    ///////////////
    // Pure Virtual Attributes 
    // These functions must be define in the derived classes
    ///////////////
    virtual BOOL IsAdr(int rxBuffX) = 0;
    
    ///////////////
    // Pure Virtual Methods 
    // These functions must be define in the derived classes
    ///////////////

    // Runs any setup procedures for the device.  This is called after the device is
    // instanciated and before it is used.
    // Returns "" for success and an error str otherwise.
    virtual char * Init(void) = 0;

    virtual char * TxPacket(int size, char * data) = 0;
    virtual char * RxPacket(int maxMsgLen, DWORD timeout, int & numBytesRead) = 0;

};


#endif // QCICOMMDEVICE_H
