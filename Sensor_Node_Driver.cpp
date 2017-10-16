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
#include "Sensor_Probe_IF.h"
#include "Sensor_Utils.h"
#include "Sensor_Probe_Handler.h"

#define STM32L476VGT

/* UART Driver */
    // UART for LoRa
//Serial sio_lora(PC_1, PC_0);    // TX, RX
    // UART for Probe
//Serial sio_probe(PB_6, PB_7);
    // UART for GPS
//Serial sio_gps(PD_8, PD_9);

#ifdef STM32L476VGT

/* SPI Driver */
SPI spi_rf(PA_7, PA_6, PA_5, PA_4); // MOSI, MISO, CLK, NSS

/* I2C Driver */
I2C i2c_rf(PB_14, PB_13);    // SDA, SCL
I2C i2c_eeprom(PB_9, PB_8);

/* Analog Inputs */
AnalogIn ain_batt_v_check(PC_2);

/* Digital Output */

DigitalOut dout_usb_disable(PC_4);
DigitalOut dout_ps_hold(PB_0);

DigitalOut dout_ch1_led_en(PC_6);
DigitalOut dout_ch2_led_en(PC_7);
DigitalOut dout_ch3_led_en(PC_8);
DigitalOut dout_ch4_led_en(PC_9);

DigitalOut dout_relay_set(PB_10);
DigitalOut dout_relay_reset(PB_11);

DigitalOut dout_pwr_led_en(PB_12);

DigitalOut dout_rf_pwr_en(PA_8);

DigitalOut dout_probe_ch_sel0(PA_9);
DigitalOut dout_probe_ch_sel1(PA_10);

DigitalOut dout_uart_mux_en_n(PD_0);
DigitalOut dout_rf_reset_n(PE_0);
DigitalOut dout_rf_wake_up_n(PE_1);

DigitalOut dout_w_disable_n(PE_2);
DigitalOut dout_cb_reset_out_n(PE_3);
DigitalOut dout_cb_pwr_on(PE_4);
DigitalOut dout_backup_n(PE_5);

DigitalOut dout_ch1_wakeup_int(PE_7);
DigitalOut dout_ch2_wakeup_int(PE_8);
DigitalOut dout_ch3_wakeup_int(PE_9);
DigitalOut dout_ch4_wakeup_int(PE_10);

DigitalOut dout_ch1_pwr_en(PD_10);
DigitalOut dout_ch2_pwr_en(PD_11);
DigitalOut dout_ch3_pwr_en(PD_12);
DigitalOut dout_ch4_pwr_en(PD_13);

DigitalOut dout_rf_gpio_rsvd1(PE_13);

DigitalOut dout_gps_pwr_en(PE_15);

/* PWM Output */
PwmOut pwm_buzz(PE_11);

/* Digital Input */
DigitalIn din_ch2_det_n(PC_13);
DigitalIn din_ch1_det_n(PA_0);
DigitalIn din_ch4_det_n(PA_2);
DigitalIn din_usb_det(PC_5);

DigitalIn din_ch3_det_n(PE_6);

DigitalIn din_1pps(PE_14);

DigitalIn din_batt_chg_stat_n(PD_14);
DigitalIn din_chg_pgood_n(PD_15);
#endif