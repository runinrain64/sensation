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
 @file  Sensor_Utils.cpp
 defines utility functions which are necessary in this project.
 
 @author    shjung
 @date      2017/09/08
 @attention Copyright (c)2017 LETUSTECH - All rights reserved
 */
 
#include "mbed.h"
#include "typedef.h"

Serial SpiDebug(PD_5, PD_6);  // Debug Serial Port
Mutex debug_mutex;

uint8_t CalCrc8( uint8_t inCrc, uint8_t inData );
uint8_t CalCrc8Block( uint8_t crc, uint8_t *data, uint8_t len );


uint8_t CalCrc8( uint8_t inCrc, uint8_t inData )
{
    uint8_t   i;
    uint8_t   data;

    data = inCrc ^ inData;

    for ( i = 0; i < 8; i++ )
    {
        if (( data & 0x80 ) != 0 )
        {
            data <<= 1;
            data ^= 0x07;
        }
        else
        {
            data <<= 1;
        }
    }
    return data;

} // Crc8


uint8_t CalCrc8Block( uint8_t crc, uint8_t *data, uint8_t len )
{
    while ( len > 0 )
    {
        crc = CalCrc8( crc, *data++ );
        len--;
    }

    return crc;

} // Crc8Block



