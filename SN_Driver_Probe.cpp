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

    // UART for Probe
Serial sio_probe(PB_6, PB_7);

DigitalOut dout_ch1_wakeup_int(PE_7);
DigitalOut dout_ch2_wakeup_int(PE_8);
DigitalOut dout_ch3_wakeup_int(PE_9);
DigitalOut dout_ch4_wakeup_int(PE_10);

DigitalOut dout_ch1_led_en(PC_6);
DigitalOut dout_ch2_led_en(PC_7);
DigitalOut dout_ch3_led_en(PC_8);
DigitalOut dout_ch4_led_en(PC_9);

DigitalOut dout_ch1_pwr_en(PD_10);
DigitalOut dout_ch2_pwr_en(PD_11);
DigitalOut dout_ch3_pwr_en(PD_12);
DigitalOut dout_ch4_pwr_en(PD_13);

DigitalIn din_ch1_det_n(PA_0);
DigitalIn din_ch2_det_n(PC_13);
DigitalIn din_ch3_det_n(PE_6);
DigitalIn din_ch4_det_n(PA_2);

/**
    @brief  Select the serial communication path between MCU and sensor probe #1/2/3/4
    @param  ouiSelPort number of sensor probe to be selected. (0 = CH1 Probe, 1 = CH2 Probe, 2 = CH3 Probe, 3 = CH4 Probe
	@return result, 0 = success, -1 = failure
*/
int16_t SNM_Drv_MuxSelPort(int16_t iSelPort)
{
	DigitalOut dout_probe_ch_sel0(PA_9);
	DigitalOut dout_probe_ch_sel1(PA_10);
	int16_t iret;

	iret = 0;

	if (iSelPort >= 0 && iSelPort <= 3)	// Check if port number is in rnage
	{
		dout_probe_ch_sel0 = ((unsigned)iSelPort & 0x01) ? 1 : 0;
		dout_probe_ch_sel1 = ((unsigned)iSelPort & 0x02) ? 1 : 0;
	}
	else
		iret = -1;

	return iret;
}

/**
    @brief  Enable muxer of sensor probe. To send data to the probe, the mux shall be enabled.
    @param  onoff 
			true - enable muxer
			false - disable muxer
*/
void SNM_Drv_EnableProbeMux(bool onoff)
{
	DigitalOut dout_uart_mux_en_n(PD_0);

	dout_uart_mux_en_n = (onoff == true) ? 0 : 1;
}

/**
    @brief  Turn on LED of channel #1 probe.
	@param	iChannel number of probe channel
    @param  onoff 
			true - turn on LED
			false - turn off LED
*/
void SNM_Drv_Probe_TurnLedOn(int16_t iChannel, bool onoff)
{
	DigitalOut *pDout;

	if ( iChannel == 0)
	{
		pDout = &dout_ch1_led_en;
	}
	else if ( iChannel == 1 )
	{
		pDout = &dout_ch2_led_en;
	}
	else if ( iChannel == 2 )
	{
		pDout = &dout_ch3_led_en;
	}
	else if ( iChannel == 3 )
	{
		pDout = &dout_ch3_led_en;
	}
	else
	{
		pDout = NULL;
	}
	
	if ( pDout != NULL )
	{
		*pDout = (onoff == true) ? 1 : 0;
	}
}



/**
    @brief  Enable Power of of channel #1 probe.
	@param	iChannel number of probe channel
	@param  onoff 
			true - Enable
			false - Disable
*/
void SNM_Drv_Probe_EnablePwr(int16_t iChannel, bool onoff)
{
	DigitalOut *pDout;

	if ( iChannel == 0)
	{
		pDout = &dout_ch1_pwr_en;
	}
	else if ( iChannel == 1 )
	{
		pDout = &dout_ch2_pwr_en;
	}
	else if ( iChannel == 2 )
	{
		pDout = &dout_ch3_pwr_en;
	}
	else if ( iChannel == 3 )
	{
		pDout = &dout_ch3_pwr_en;
	}
	else
	{
		pDout = NULL;
	}
	
	if ( pDout != NULL )
	{
		*pDout = (onoff == true) ? 1 : 0;
	}
}

/**
    @brief  Read Probe's Det Line.
	@param	iChannel number of probe channel
	@retun  value of Det Line. ( -1 = error, 0 or 1 = success) 
*/
char SNM_Drv_Probe_ReadDetLine(int16_t iChannel)
{
	DigitalIn *pDin;
	char	ret;

	if ( iChannel == 0)
		pDin = &din_ch1_det_n;
	else if ( iChannel == 1 )
		pDin = &din_ch2_det_n;
	else if ( iChannel == 2 )
		pDin = &din_ch3_det_n;
	else if ( iChannel == 3 )
		pDin = &din_ch4_det_n;
	else
		pDin = NULL;
	
	ret = (pDin != NULL) ? *pDin:-1;

	return ret;
}

