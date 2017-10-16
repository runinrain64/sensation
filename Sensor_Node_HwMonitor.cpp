#include "mbed.h"
#include "rtos.h"
#include "Sensor_Probe_IF.h"
#include "Sensor_Utils.h"
#include "Sensor_Probe_Handler.h"

extern AnalogIn ain_batt_v_check;

void SNM_HwMon_AnalogInMenu( void );
void SNM_HwMon_SerialMenu( void );
float SNM_HwMon_CalAnalogIn( int16_t nsamples, float analogin );
void flushSerialBuffer(void);


float analogue_samples[100];
uint16_t sample_index;
bool buffer_full;

void SNM_HwMon_MainMenu( void )
{
    uint8_t cmenu;
    bool    inloop;
    
    inloop = true;
    
    do
    {   
        DbgPrint("\r\n\n\n");
        DbgPrint("***********<<Main Menu>>***********\r\n");
        DbgPrint("1. Serial Menu.\r\n");
        DbgPrint("2. Analogue In Menu.\r\n");
        DbgPrint("3. Exit.\r\n");
        DbgPrint("-------> Select Menu:")
        flushSerialBuffer();
        SpiDebug.scanf("%c", &cmenu);
        DbgPrint("\r\n\n");
    
        switch (cmenu)
        {
            case '1':
                SNM_HwMon_SerialMenu();
                break;
            case '2':
                SNM_HwMon_AnalogInMenu();
                break;
            case '3':
                inloop = false;
                DbgPrint("See you soon...\r\n");
                flushSerialBuffer();
                break;
        }
    }
    while( inloop );
}

void SNM_HwMon_SerialMenu( void )
{
    uint8_t cmenu;
    bool inloop;
    
    inloop = true;
    
    do
    {
        DbgPrint("\r\n");
        DbgPrint("------------[Serial Interface Test]------------\r\n");
        DbgPrint("> 1. Set to 4800 baud rate.\r\n");
        DbgPrint("> 2. Set to 9600 baud rate.\r\n");
        DbgPrint("> 3. Set to 14400 baud rate.\r\n");
        DbgPrint("> 4. Set to 19200 baud rate.\r\n");
        DbgPrint("> 5. Set to 38400 baud rate.\r\n");
        DbgPrint("> 6. Set to 57600 baud rate.\r\n");
        DbgPrint("> 7. Set to 115200 baud rate.\r\n");
        DbgPrint("> 8. Exit.\r\n");
        DbgPrint("-------> Select Menu:")
        flushSerialBuffer();
        SpiDebug.scanf("%c", &cmenu);
        DbgPrint("\r\n");
        
        switch (cmenu)
        {
            case '1':
                DbgPrint("Configure Serial Port to 4800 baud rate.\r\n");
                SpiDebug.baud(4800);
                break;
            case '2':
                DbgPrint("Configure Serial Port to 9600 baud rate.\r\n");
                SpiDebug.baud(9600);
                break;
            case '3':
                DbgPrint("Configure Serial Port to 14400 baud rate.\r\n");
                SpiDebug.baud(14400);
                break;
            case '4':
                DbgPrint("Configure Serial Port to 19200 baud rate.\r\n");
                SpiDebug.baud(19200);
                break;
            case '5':
                DbgPrint("Configure Serial Port to 38400 baud rate.\r\n");
                SpiDebug.baud(38400);
                break;
            case '6':
                DbgPrint("Configure Serial Port to 57600 baud rate.\r\n");
                SpiDebug.baud(57600);
                break;
            case '7':
                DbgPrint("Configure Serial Port to 115200 baud rate.\r\n");
                SpiDebug.baud(115200);
                break;
            case '8':
                DbgPrint("Return the top menu...\r\n\n");
                inloop = false;
                flushSerialBuffer();
                break;
        }
    }
    while (inloop);
}
void SNM_HwMon_AnalogInMenu( void )
{
    uint8_t cmenu;
    uint16_t samplingtime;
    int16_t nsamples;
    bool inloop;
    
    inloop = true;
    sample_index = 0;
    samplingtime = 1000;    // default value = 1sec
    nsamples = 1;   // default value
    buffer_full = false;
    
    do
    {
        DbgPrint("\r\n");
        DbgPrint("------------[Analogue Input Test]------------\r\n");
        DbgPrint("> 1. Set Sampling Time( unit: ms)\r\n");
        DbgPrint("> 2. Set number of samples(1 ~ 100)\r\n");
        DbgPrint("> 3. Start Sampling Analogue Input\r\n");
        DbgPrint("> 4. Exit.\r\n");
        DbgPrint("-------> Select Menu:")
        flushSerialBuffer();
        SpiDebug.scanf("%c", &cmenu);
        DbgPrint("\r\n");
        
        switch (cmenu)
        {
            case '1':
                DbgPrint("> Enter the desired sampling time(mSec):\r\n");
                buffer_full = false;
                flushSerialBuffer();
                SpiDebug.scanf("%d", &samplingtime);
                DbgPrint("===> Set the sampling time to %dmS\r\n\n", samplingtime);
                break;
            case '2':
                DbgPrint("> Enter the desired number of samples:\r\n");
                buffer_full = false;
                flushSerialBuffer();
                SpiDebug.scanf("%d", &nsamples);
                if (nsamples < 0)
                    nsamples = 0;
                else if (nsamples > 100)
                    nsamples = 100;
                    
                DbgPrint("===> Set the number of samples to %d\r\n\n", nsamples);
                sample_index = 0;
                break;
            case '3':
                DbgPrint("Start sampling, hit any key to stop!\r\n");
                while (SpiDebug.readable() == 0)
                {
                    DbgPrint("Analogue Input = %f\r\n", SNM_HwMon_CalAnalogIn(nsamples, ain_batt_v_check.read()) );
                    Thread::wait(samplingtime);
                }
                break;
            case '4':
                DbgPrint("Return the top menu...\r\n\n");
                inloop = false;
                flushSerialBuffer();
                break;
        }
    }
    while (inloop);
}

float SNM_HwMon_CalAnalogIn( int16_t nsamples, float analogin )
{
    uint16_t idx, ndata;
    float sum, average;
    
    sum = 0;
    analogue_samples[ sample_index++ ] = analogin;

    if ( buffer_full == true )
        ndata = nsamples;
    else
        ndata = sample_index;
        
 //   DbgPrint("{%d-%d-%f}", sample_index, ndata, analogin);

    for (idx = 0; idx < ndata; idx++)
    {
        sum += analogue_samples[idx];
    }
    
    if ( ndata > 0 )
        average = sum / ndata;
    else
        average = 0;
        
    if ( sample_index >= nsamples )
    {
        buffer_full = true;
        sample_index = 0;
    }
    
    return average;
}

void flushSerialBuffer(void) 
{ 
    char char1 = 0; 
    while (SpiDebug.readable()) 
    { 
        char1 = SpiDebug.getc(); 
    } 
}

