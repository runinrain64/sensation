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
#include "SN_Utils.h"
#include "SN_Driver.h"

/* Digital Output */
DigitalOut dout_usb_disable(PC_4);

/*
SPI spi_rf(PA_7, PA_6, PA_5, PA_4); // MOSI, MISO, CLK, NSS
I2C i2c_rf(PB_14, PB_13);    // SDA, SCL

DigitalOut dout_rf_pwr_en(PA_8);

DigitalOut dout_rf_reset_n(PE_0);
DigitalOut dout_rf_wake_up_n(PE_1);

DigitalOut dout_w_disable_n(PE_2);
DigitalOut dout_cb_reset_out_n(PE_3);
DigitalOut dout_cb_pwr_on(PE_4);
DigitalOut dout_backup_n(PE_5);

DigitalOut dout_rf_gpio_rsvd1(PE_13);
*/

/* Digital Input */
DigitalIn din_1pps(PE_14);

/*	DEFINTIONS 
*/
#define		ADC_REF_VOLTAGE		(float)3.3

/**
    @brief  Set the frequency of buzzer. 0Hz stops buzzer.
    @param	freq	frequency (unit: KHz)
*/
void SNM_Drv_SetBuzzerFreq( float freq )
{
	PwmOut pwm_buzz(PE_11);
	float fPeriod;

	if (freq <= 0)	// stop buzzer
	{
		pwm_buzz.write(0.0f);
	}
	else
	{	
		
		freq = (freq > 100) ? 100:freq;
		fPeriod = (float)(1 / (float)(freq*1000));
		pwm_buzz.period(fPeriod);
		pwm_buzz.write(0.5f);
	}
}

/**
    @brief  Read battery voltage level
    @return battery voltage
*/
float SNM_Drv_ReadBatVoltage(void)
{
	float ret;
	AnalogIn ain_batt_v_check(PC_2);

	ret = ain_batt_v_check.read() *  ADC_REF_VOLTAGE;

	return ret;
}

/**
    @brief  set Delay Reset line.
    @param  onoff
			true - set to '1'
			false - set to '0'
*/
void SNM_Drv_SetRelayResetLine(bool onoff)
{
	DigitalOut dout_relay_reset(PB_11);

	dout_relay_reset = (onoff) ? 1:0;
}

/**
    @brief  set Delay Set line.
    @param  onoff
			true - set to '1'
			false - set to '0'
*/
void SNM_Drv_SetRelaySetLine(bool onoff)
{
	DigitalOut dout_relay_set(PB_10);

	dout_relay_set = (onoff) ? 1:0;
}

/**
    @brief  turn on Power LED.
    @param  onoff
			true - turn on LED
			false - turn off LED
*/
void SNM_Drv_EnablePwrLed(bool onoff)
{
	DigitalOut dout_pwr_led_en(PB_12);

		// turn on Power LED accroding to onoff flag
	dout_pwr_led_en = (onoff) ? 1 : 0;
}

/**
    @brief  set PS Hold line.
    @param  onoff
			true - set to HIGH
			false - set to LOW
*/
void SNM_Drv_EnablePsHold(bool onoff)
{
	DigitalOut dout_ps_hold(PB_0);

		// set PS Hold accroding to onoff flag
	dout_ps_hold = (onoff) ? 1 : 0;
}

/**
    @brief  read battery charge status.
    @return
			0 - "None"
			1 - "in charging"
			2 - "fully charged"
			3 - "no good"
			-1 - unknown status
*/
int SNM_Drv_ReadBattStatus( void )
{
	int iret, statusb, statchg, statpgood;

	DigitalIn din_usb_det(PC_5);
	DigitalIn din_batt_chg_stat_n(PD_14);
	DigitalIn din_chg_pgood_n(PD_15);

	statusb = din_usb_det;
	statchg = din_chg_pgood_n;
	statpgood = din_batt_chg_stat_n;
	DbgPrint("[USB_DET:CHG_PGOOD_N:BATT_CHG_STAT_N = %d:%d:%d]\r\n", statusb, statchg, statpgood);

	if (statusb == 0 )
	{
		iret = 0;
	}
	else if (statchg == 0 )
	{
		iret = (statpgood == 0 ) ? 1: 2;
	}
	else
	{
		iret = 3;
	}

	return iret;
}






