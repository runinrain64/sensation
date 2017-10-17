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
 @file  Sensor_Node_Driver.cpp
 defines functions of Drivers which are necessary for Sensor Node Manager to operate
 
 @author    shjung
 @date      2017/09/08
 @attention Copyright (c)2017 LETUSTECH - All rights reserved
 */
 
#include "mbed.h"
#include "rtos.h"
#include "SN_Driver.h"

Serial sio_gps(PD_8, PD_9);
DigitalOut dout_gps_pwr_en(PE_15);

/**
    @brief  Enable power of GPS module.
	@param	onoff true = enable, false = disable
*/
void SNM_Drv_Gps_EnablePwr( bool onoff )
{
	dout_gps_pwr_en = (onoff) ? 1:0;
}

/**
    @brief  Initialize GPS module
*/
void SNM_Drv_Gps_Init( void )
{
		// Configure the serial port of host MCU
	sio_gps.format(8, SerialBase::None, 1);
	sio_gps.baud(9600);

		// Enable power
	SNM_Drv_Gps_EnablePwr( true );
}

/**
    @brief  check if there is available data in GPS module.
	@return	0 if no data, 1 if data available.
*/
int SNM_Drv_Gps_Readable( void )
{
	return sio_gps.readable();
}

/**
    @brief  read byte from GPS module.
	@return	data from GPS module.
*/
char SNM_Drv_Gps_ReadByte( void )
{
	return sio_gps.getc();
}


