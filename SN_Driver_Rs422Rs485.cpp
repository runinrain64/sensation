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

enum RS422_RS485_OPMODE	g_rs422_rs485_opmode;

Serial sio_rs422_rs485(PC_12, PD_2);

DigitalOut rs422_rs485_rx_re(PB_5);
DigitalOut rs422_rs485_rx_de(PD_7);
DigitalOut rs422_rs485_tx_re(PD_4);
DigitalOut rs422_rs485_tx_de(PD_3);

void SNM_Drv_Rs422Rs485_Init(void)
{
		/* Configure the serial port of RS422 & RS484 */
	sio_rs422_rs485.format(8, SerialBase::None, 1);
	sio_rs422_rs485.baud(9600);

		/* Set RS422 & RS485 control lines */
	SNM_Drv_Rs422Rs485_SetOpMode(shutdownmode);
}

int	SNM_Drv_Rs422Rs485_SetOpMode(enum RS422_RS485_OPMODE opmode)
{
	int iret;

	switch (opmode)
	{
	case shutdownmode:
			rs422_rs485_rx_re = 0x1;
			rs422_rs485_rx_de = 0x0;
			rs422_rs485_tx_re = 0x1;
			rs422_rs485_tx_de = 0x0;
			g_rs422_rs485_opmode = opmode;

			iret = 0;
			break;
	case txmode:
			rs422_rs485_rx_re = 0x0;
			rs422_rs485_rx_de = 0x0;
			rs422_rs485_tx_re = 0x1;
			rs422_rs485_tx_de = 0x1;
			g_rs422_rs485_opmode = opmode;

			iret = 0;
			break;
	case rxmode:
			rs422_rs485_rx_re = 0x0;
			rs422_rs485_rx_de = 0x0;
			rs422_rs485_tx_re = 0x0;
			rs422_rs485_tx_de = 0x0;
			g_rs422_rs485_opmode = opmode;

			iret = 0;
			break;
/*
	case txrxmode:
			rs422_rs485_rx_re = 0x0;
			rs422_rs485_rx_de = 0x0;
			rs422_rs485_tx_re = 0x1;
			rs422_rs485_tx_de = 0x1;
			g_rs422_rs485_opmode = opmode;

			iret = 0;
			break;
*/
	default:
			iret = -1;
			break;
	}

	return iret;
}

enum RS422_RS485_OPMODE SNM_Drv_Rs422Rs485_GetOpMode(void)
{
	return g_rs422_rs485_opmode;
}

char SNM_Drv_Rs422Rs485_ReadByte(void)
{
	char cret;

	cret = sio_rs422_rs485.getc();

	return cret;
}

int SNM_Drv_Rs422Rs485_Readable(void)
{
	return (sio_rs422_rs485.readable());
}

void SNM_Drv_Rs422Rs485_WriteByte(char cbyte)
{
	while (sio_rs422_rs485.writeable() == 0)
		Thread::wait(1);
	
	sio_rs422_rs485.putc(cbyte);
}

void SNM_Drv_Rs422Rs485_WriteString(char *cstr)
{
	sio_rs422_rs485.printf("%s", cstr);
}
