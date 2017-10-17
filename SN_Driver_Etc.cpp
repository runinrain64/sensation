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

/* SPI Driver */
SPI spi_rf(PA_7, PA_6, PA_5, PA_4); // MOSI, MISO, CLK, NSS

/* I2C Driver */
I2C i2c_rf(PB_14, PB_13);    // SDA, SCL

/* Digital Output */
DigitalOut dout_usb_disable(PC_4);
DigitalOut dout_ps_hold(PB_0);

DigitalOut dout_rf_pwr_en(PA_8);

DigitalOut dout_rf_reset_n(PE_0);
DigitalOut dout_rf_wake_up_n(PE_1);

DigitalOut dout_w_disable_n(PE_2);
DigitalOut dout_cb_reset_out_n(PE_3);
DigitalOut dout_cb_pwr_on(PE_4);
DigitalOut dout_backup_n(PE_5);

DigitalOut dout_rf_gpio_rsvd1(PE_13);

/* Digital Input */
DigitalIn din_usb_det(PC_5);
DigitalIn din_1pps(PE_14);

DigitalIn din_batt_chg_stat_n(PD_14);
DigitalIn din_chg_pgood_n(PD_15);

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

	if (freq < 0)
		freq = 0;
	else if (freq > 100)
		freq = 100;

	fPeriod = (float)(1 / (float)(freq*1000));

	pwm_buzz.period(fPeriod);
	pwm_buzz.write(0.5f);
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
