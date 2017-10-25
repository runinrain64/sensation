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

SPI spi_rf(PA_7, PA_6, PA_5, PA_4); // MOSI, MISO, CLK, NSS
I2C i2c_rf(PB_14, PB_13);    // SDA, SCL

DigitalOut dout_rf_wake_up_n(PE_1);
DigitalOut dout_rf_gpio_rsvd1(PE_13);

DigitalOut dout_w_disable_n(PE_2);
DigitalOut dout_cb_reset_out_n(PE_3);
DigitalOut dout_cb_pwr_on(PE_4);
DigitalOut dout_backup_n(PE_5);

DigitalIn din_mcu_wake_up(PC_5);

/*
DbgPrint("> 1. Set RF_PWR_EN to LOW.\r\n");
DbgPrint("> 2. Set RF_PWR_EN to HIGH.\r\n");
DbgPrint("> 3. Set RF_RESET_N to LOW.\r\n");
DbgPrint("> 4. Set RF_RESET_N to HIGH.\r\n");
*/

void SNM_Drv_LoRa_Initialize(void)
{
	SNM_Drv_LoRa_SetSioFormat(8, 0, 1);
	SNM_Drv_LoRa_SetSioBaudrate(9600);
}

int SNM_Drv_LoRa_SetSioBaudrate(int baudrate)
{
	Serial sio_lora(PC_1, PC_0);

	if ((baudrate < 2400) || (baudrate > 115200))
	{
		baudrate = 9600;	// set default value
	}
	
	sio_lora.baud(baudrate);	// configure baudrate

	return 0;
}

int SNM_Drv_LoRa_SioReadable(void)
{
	Serial sio_lora(PC_1, PC_0);

	sio_lora.readable();
}

char SNM_Drv_LoRa_SioReadByte(void)
{
	Serial sio_lora(PC_1, PC_0);
	
	return (sio_lora.getc());
}


int SNM_Drv_LoRa_SetSioFormat(int databits, int stopbits, int iparity)
{
	bool cfgvalid;
	int iret;
	SerialBase::Parity parity ;

	Serial sio_lora(PC_1, PC_0);

	cfgvalid = true;

	if ((databits != 7) && (databits != 8))
	{
		cfgvalid = false;
	}

	if ((stopbits != 1) && (stopbits != 2))
	{
		cfgvalid = false;
	}
	switch (iparity)
	{
		case 0:
				parity = SerialBase::None;
				break;
		case 1:
				parity = SerialBase::Odd;
				break;
		case 2:
				parity = SerialBase::Even;
				break;
		case 3:
				parity = SerialBase::Forced1;
				break;
		case 4:
				parity = SerialBase::Forced0;
				break;
		default:
				cfgvalid = false;
				break;
	}

	if (cfgvalid == true)
	{
		iret = 1;
		sio_lora.format(databits, parity, stopbits);
	}
	else
		iret = -1;

	return iret;
}

void SNM_Drv_LoRa_SetPwrEn(bool onoff)
{
	DigitalOut dout_rf_pwr_en(PA_8);

	dout_rf_pwr_en = (onoff == true) ? 1 : 0;
}

void SNM_Drv_LoRa_SetResetLine(bool onoff)
{
	DigitalOut dout_rf_reset_n(PE_0);

	dout_rf_reset_n = (onoff == true) ? 1 : 0;
}
