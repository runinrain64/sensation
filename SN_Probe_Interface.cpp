/**
@mainpage (c)LETUSTECH Software
This software is developed on mbed

@section INTRO
 - introduction :   LESUSTECH Sensation Project
@section CREATINFO
 - Author   : shjung
 - date :   2017/09/08
 @section   ADDINFO
 - codeveloped with INNOFOS
 @section Links
 
 */ 
 
 /**
@page (c)LETUSTECH Software
This software is developed on mbed

@section profile
 - introduction :   LESUSTECH Sensation Project
@code
 - name   : shjung
 - date :   2017/09/08 
 */ 
 
 /**
 @file  Sensor_Probe_IF.cpp
 defines interfaces to sensor probe
 
 @author    shjung
 @date      2017/09/08
 @attention Copyright (c)2017 LETUSTECH - All rights reserved
 */
 
#include "mbed.h"
#include "rtos.h"
#include "typedef.h"
#include "SN_Utils.h"
#include <SerialBase.h>
#include "SN_Probe_Interface.h"

#ifdef  SIO_EMUL
#define SIO_EMUL_BUF_SIZE       0x100
#define SIO_TIMEOUT             2000

uint8_t g_siotxbuf[SIO_EMUL_BUF_SIZE]; // Virtual SIO Tx Buffer of Sensor Node
uint16_t    g_siotxbuf_wr;  // Write pointer of Sensor Node's SIO Tx Buffer
uint16_t    g_siotxbuf_rd;  // Read pointer of Sensor Node's SIO Tx Buffer

uint8_t g_siorxbuf[SIO_EMUL_BUF_SIZE];  // Virtual SIO Rx Buffer of Sensor Node
uint16_t    g_siorxbuf_wr;  // Write pointer of Sensor Node's SIO Rx Buffer
uint16_t    g_siorxbuf_rd;  // Read pointer of Sensor Node's SIO Rx Buffer
#endif

Serial  *pCSio;

/**
    @brief  initialize the virtual serial port of Sensor Node
*/
void SN_SIO_Init( void )
{
#ifdef  SIO_EMUL
    g_siotxbuf_wr = 0;
    g_siotxbuf_rd = 0;

    g_siorxbuf_wr = 0;
    g_siorxbuf_rd = 0;
#endif 

}


/**
    @brief  configure the serial controller of sensor node module.
    @param  nprobe number of probe to be accessed
    @param  baudrate baud rate, 4800/9600/19200/...
    @param  datlen data length, 5bits/6bits/7bits/8bits
    @param  stopbit stop bits, 1bit/2bits
    @param  parity parity bits, none/odd/even
    @flowctrl flow control 
*/
int16_t SN_SIO_Configure( uint8_t nprobe, uint8_t baudrate, uint8_t datlen, uint8_t stopbit, uint8_t parity, bool flowctrl)
{
#ifndef SIO_EMUL
    SerialBase::Parity cfgparity;
    int cfgbaud;
    int cfgbits;
    int cfgstopbits;
    SerialBase::Flow flowtype;
    PinName flow1, flow2;
    
    int16_t ret = 0;

    pCSio = new Serial(USBTX, USBRX);
  
    switch (baudrate)
    {
        case SIO_CFG_BAUD_4800:
            cfgbaud = 4800;
            break;
        case SIO_CFG_BAUD_9600:
            cfgbaud = 9600;
            break;
        case SIO_CFG_BAUD_19200:
            cfgbaud = 19200;
            break;
        default:
            ret = -1;
    }
    
    switch (datlen)
    {
        case SIO_CFG_DATA_5BITS:
            cfgbits = 5;
            break;
        case SIO_CFG_DATA_6BITS:
            cfgbits = 6;
            break;
        case SIO_CFG_DATA_7BTIS:
            cfgbits = 7;
            break;
        case SIO_CFG_DATA_8BTIS:
            cfgbits = 8;
            break;
        default:
            ret = -1;
    }

    switch (stopbit)
    {
        case SIO_CFG_STOP_1BIT:
            cfgstopbits = 1;
            break;
        case SIO_CFG_STOP_2BIT:
            cfgstopbits = 2;
            break;
        default:
            ret = -1;
    }
    
    switch (parity)
    {
        case SIO_CFG_PAR_NONE:
            cfgparity = SerialBase::None;
            break;
        case SIO_CFG_PAR_EVEN:
            cfgparity = SerialBase::Even;
            break;
        case SIO_CFG_PAR_ODD:
            cfgparity = SerialBase::Odd;
            break;
        default:
            ret = -1;
    }
    
    switch (flowctrl)
    {
        case SIO_CFG_CNTR_OFF:
            flowtype = SerialBase::Disabled;
            flow1 = NC;
            flow2 = NC;
            break;
        case SIO_CFG_CNTR_ON:
            flowtype = SerialBase::RTSCTS;
            flow1 = PA_1;
            flow2 = PA_0;
            break;
        default:
            ret = -1;
    }
    
    if (ret == 0)   // if valid configuration, set the serial driver accordingly.
    {
        pCSio->baud( cfgbaud );
        pCSio->format( cfgbits, cfgparity, cfgstopbits );
        pCSio->set_flow_control(flowtype, flow1, flow2);
    }

    return ret;
#else
    return 0;
#endif
}
 

/**
    @brief  write data into SIO buffer of Sensor Node.
    @param  pbuf pointer of data buffer to be sent
    @param  ndat number of data to be sent
    @param  timeout timeout value to wait for finishing sending one byte.
*/
uint16_t SN_SIO_PutData( uint8_t *pbuf, uint16_t ndat, uint16_t timeout)
{
    int16_t retry = 0;
    uint16_t retndat = 0;  // number of data which is really sent
#ifdef  SIO_EMUL
    uint16_t nemptiness;
 
    while (retndat < ndat && retry < timeout )
    {
        if ( g_siotxbuf_wr >= g_siotxbuf_rd)
            nemptiness = (SIO_EMUL_BUF_SIZE - g_siotxbuf_wr) + g_siotxbuf_rd;
        else
            nemptiness = g_siotxbuf_rd - g_siotxbuf_wr;
    
//        printf("SN Serial Driver: empty buffer = %d(%d-%d)\r\n", nemptiness, g_siotxbuf_wr, g_siotxbuf_rd);
        
        if (nemptiness > 0)
        {
            g_siotxbuf[g_siotxbuf_wr++] = *pbuf++;
            retndat++;
            
            g_siotxbuf_wr = (g_siotxbuf_wr >= SIO_EMUL_BUF_SIZE) ? 0:g_siotxbuf_wr;
        }
        else
        {
            Thread::wait( SIO_TIMEOUT );    // wait 100ms
            retry++;
        }
    }  
#else
    if (pCSio != NULL)
    {
        while (retndat < ndat && retry < timeout )
        {
            if (pCSio->writeable() == true)
            {
                pCSio->putc(*pbuf++);
                retndat++;
            }
            else
            {
                Thread::wait( 100 );    // wait 100ms
                retry++;
            }
        }
    }
#endif

    return retndat;
}

/*
    timeout
*/

/**
    @brief  read data from the serial buffer of sensor node
    @param  pbuf pointer of buffer to store data
    @param  maxndat max. number of data to be read
    @param  datlen data length, 5bits/6bits/7bits/8bits
    @param  timeout timeout value to wait for finishing reading one byte.
*/
uint16_t SN_SIO_GetData( uint8_t *pbuf, uint16_t maxndat, uint16_t timeout)
{
    int16_t retry = 0;
    uint16_t retndat = 0;  // number of data which is really sent
#ifdef  SIO_EMUL
    uint16_t nfullness;
    
    while (retndat < maxndat && retry < timeout )
    {
        if ( g_siorxbuf_wr >= g_siorxbuf_rd)
            nfullness = g_siorxbuf_wr - g_siorxbuf_rd;
        else
            nfullness = (SIO_EMUL_BUF_SIZE - g_siorxbuf_rd) + g_siorxbuf_wr;

 //       printf("SNSIO: full buffer = %d(%d-%d)\r\n", nfullness, g_siorxbuf_wr, g_siorxbuf_rd);
   
        if (nfullness > 0)
        {
            *pbuf++ = g_siorxbuf[g_siorxbuf_rd++];
            retndat++;
            
            g_siorxbuf_rd = (g_siorxbuf_rd >= SIO_EMUL_BUF_SIZE) ? 0:g_siorxbuf_rd;
        }
        else
        {
            Thread::wait( SIO_TIMEOUT );    // wait 100ms
            retry++;
        }
    }  
#else    
    if (pCSio != NULL)
    {
        while (retndat < maxndat && retry < timeout )
        {
            if (pCSio->readable() == true)
            {
                *pbuf++ = pCSio->getc();
            }
            else
            {
                Thread::wait( 100 );    // wait 100ms
                retry++;
            }
        }
    }
#endif

    return retndat;
}

#ifdef SIO_EMUL
/**
    @brief  write data to SIO Rx buffer by Sensor Probe
    @param  pbuf pointer of data buffer to be sent
    @param  ndat number of data to send
    @param  timeout value to wait for finishing sending one byte.
*/
uint16_t SP_SIO_PutData( uint8_t *pbuf, uint16_t ndat, uint16_t timeout)
{
    int16_t retry = 0;
    uint16_t retndat = 0;  // number of data which is really sent
    uint16_t nemptiness;
 
  
    while (retndat < ndat && retry < timeout )
    {
        if ( g_siorxbuf_wr >= g_siorxbuf_rd)
            nemptiness = (SIO_EMUL_BUF_SIZE - g_siorxbuf_wr) + g_siorxbuf_rd;
        else
            nemptiness = g_siorxbuf_rd - g_siorxbuf_wr;
    
 //       printf("SPSIO: empty buffer = %d(%d-%d)\r\n", nemptiness, g_siorxbuf_wr, g_siorxbuf_rd);
        
        if (nemptiness > 0)
        {
            g_siorxbuf[g_siorxbuf_wr++] = *pbuf++;
            retndat++;
            
            g_siorxbuf_wr = (g_siorxbuf_wr >= SIO_EMUL_BUF_SIZE) ? 0:g_siorxbuf_wr;
        }
        else
        {
            Thread::wait( SIO_TIMEOUT );    // wait 100ms
            retry++;
        }
    } 

    return retndat;
}

/**
    @brief  read data from SIO Tx buffer by Sensor Probe
    @param  pbuf pointer of buffer to store
    @param  maxndat number of data to read
    @param  timeout value to wait for finishing reading one byte.
*/
uint16_t SP_SIO_GetData( uint8_t *pbuf, uint16_t maxndat, uint16_t timeout)
{
    int16_t retry = 0;
    uint16_t retndat = 0;  // number of data which is really sent
    uint16_t nfullness;

    while (retndat < maxndat && retry < timeout )
    {
        if ( g_siotxbuf_wr >= g_siotxbuf_rd)
            nfullness = g_siotxbuf_wr - g_siotxbuf_rd;
        else
            nfullness = (SIO_EMUL_BUF_SIZE - g_siotxbuf_rd) + g_siotxbuf_wr;

 //       printf(">>> Serial Probe SIO Driver: fullness buffer = %d(%d-%d)\r\n", nfullness, g_siotxbuf_wr, g_siotxbuf_rd);
    
        if (nfullness > 0)
        {
            *pbuf++ = g_siotxbuf[g_siotxbuf_rd++];
            retndat++;
            
            g_siotxbuf_rd = (g_siotxbuf_rd >= SIO_EMUL_BUF_SIZE) ? 0:g_siotxbuf_rd;
        }
        else
        {
            Thread::wait( SIO_TIMEOUT );    // wait 100ms
            retry++;
        }
    }  

    return retndat;
}
#endif
