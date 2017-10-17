#include "typedef.h"

class SensorProbeClass
{
    private:
        INT     iSensorType;    // CO2 = 0, Temperature = 1, Moisture = 2...
        INT     iInterfaceType; // UART = 0, I2C = 1, SPI = 2, GPIO = 3
        BOOL    bSafeBufferMode;
        UINT32  uiSamplingInterval; // seconds
        UINT32  uiBufLen;
        UINT32  uiRdPtr;    // Read Pointer of Safe Buffer
        UINT32  uiWrPtr;    // Write Pointer of Safe Buffer
    
    public:
        InterfaceClass  *pcInterface;
        UCHAR   *pucSafBuf;
        INT     SaveSafBuf(CHAR *pcBuf, UINT32 uiLength);   // Return value: real number of data stored in the buffer
        INT     LoadSafBuf(CHAR *pcBuf, UINT32 uiLength);   // Return value: real number of data stored in the buffer
        void    ResetSafBuf( void );
        
        void    SetSensorType( INT iSensor );
        INT     GetSensorType( void );
        void    SetInterfaceType( INT iType );
        INT     GetInterfaceType( void );
        BOOL    GetSafeMode( void );
        void    SetSamplingInterval( UINT32 );
        UINT32  GetSamplingInterval( void );
}


class InterfaceClass
{
    private:
        INT     iInterfaceType;
        UINT32  uiMaxBW;
        UINT32  uiMinBW;
    public:
        void    SetInterfaceType( INT iType );
        INT     GetInterfaceType( void );
        void    SetBW( UINT32 uiMin, UINT32 uiMax );
        void    GetBW( UINT32 *puiMin, UINT32 *puiMax );
        
        INT     SetConfiguration( UCHAR *pucBuf );
        INT     TxData(UCHAR *pcBuf, UINT32 uiLength);      // Return value: real number of data stored in the buffer
        INT     RxData(UCHAR *pcBuf, UINT32 uiLength);      // Return value: real number of data stored in the buffer
}

        