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


#define	EEPROM_ADDRESS_RD		(const char)0xA0
#define	EEPROM_ADDRESS_WR		(const char)0xA1

I2C i2c_eeprom(PB_9, PB_8);

void SNM_Drv_EEPROM_Init(void)
{
	i2c_eeprom.frequency(400000);	// set frequency as 400KHz
}

void SNM_Drv_EEPROM_Read(uint16_t addr,char * pcbuf, uint16_t nsize)
{
	char cDevAddr[2];

	cDevAddr[0] = (addr >> 16) & 0xff;
	cDevAddr[1] = (addr & 0xff);

	i2c_eeprom.write(EEPROM_ADDRESS_WR, cDevAddr, 2);
	i2c_eeprom.read(EEPROM_ADDRESS_RD, pcbuf, nsize);
}

void SNM_Drv_EEPROM_Write(uint16_t addr, char * pcbuf, uint16_t nsize)
{
	char *pcDevAddr;

	pcDevAddr = (char*)malloc(nsize + 2);

	if (pcDevAddr != NULL)
	{
		pcDevAddr[0] = (addr >> 16) & 0xff;
		pcDevAddr[1] = (addr & 0xff);
		memcpy(pcDevAddr + 2, pcbuf, nsize);

		i2c_eeprom.write(EEPROM_ADDRESS_WR, pcDevAddr, 2 + nsize);
	}
}

