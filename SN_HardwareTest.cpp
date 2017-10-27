#include "mbed.h"
#include "rtos.h"
#include "USBSerial.h"
#include "SN_Probe_Interface.h"
#include "SN_Utils.h"
#include "SN_Probe_Handler.h"
#include "SN_Driver.h"


void SNM_HwMon_AnalogInMenu( void );
void SNM_HwMon_SerialMenu( void );
float SNM_HwMon_CalAnalogIn( int16_t nsamples, float analogin );
void flushSerialBuffer(void);

void SNM_HwMon_BuzzerMenu(void);

void SNM_HwMon_RelayTestMenu(void);

void SNM_HwMon_ProbeTestMenu(void);
void SNM_HwMon_ProbeLedTest(void);
void SNM_HwMon_ProbePowerEnTest(void);
void SNM_HwMon_ProbeSerialTest(void);
void SNM_HwMon_ProbeDetLineTest(void);

void SNM_HwMon_TestUsbSerial(void);
void SNM_HwMon_TestGps(void);
void SNM_HwMon_TestEeprom(void);
void SNM_HwMon_TestRS422_485(void);
void SNM_HwMon_ConfigureRs422485SerialPort(void);


void SNM_HwMon_TestLoRa(void);
void SNM_HwMon_ConfigureProbeSerialPort(void);

void SNM_HwMon_TestPowerControlModule(void);



float analogue_samples[100];
uint16_t sample_index;
bool buffer_full;

void SNM_HwMon_MainMenu( void )
{
    uint8_t cmenu;
    bool    inloop;
    
    inloop = true;
    
    do
    {   
        printf("\r\n\n\n");
		printf("***********<<Main Menu>>***********\r\n");
		printf("1. Debug Serial Port Test.\r\n");
		printf("2. Battery Voltage ADC Test.\r\n");
		printf("3. Buzzer Test.\r\n");
		printf("4. Relay Test.\r\n");
		printf("5. Probe I/F Test.\r\n");
		printf("6. USB Serial Port(under construction).\r\n");
		printf("7. GPS module Test.\r\n");
		printf("8. EEPROM R/W Test.\r\n");
		printf("9. RS422_RS485 Test.\r\n");
		printf("a. LoRa Module Test.\r\n");
		printf("b. Power Module Test.\r\n");
		printf("f. Exit.\r\n");
		printf("-------> Select Menu:");
        flushSerialBuffer();
        scanf("%c", &cmenu);
		printf("%c\r\n", cmenu);
    
        switch (cmenu)
        {
            case '1':
                SNM_HwMon_SerialMenu();
                break;
            case '2':
                SNM_HwMon_AnalogInMenu();
                break;
			case '3':
				SNM_HwMon_BuzzerMenu();
				break;
			case '4':
				SNM_HwMon_RelayTestMenu();
				break;
			case '5':
				SNM_HwMon_ProbeTestMenu();
				break;
			case '6':
				SNM_HwMon_TestUsbSerial();
				break;
			case '7':
				SNM_HwMon_TestGps();
				break;
			case '8':
				SNM_HwMon_TestEeprom();
				//SNM_Drv_EEPROM_Test();
				break;
			case '9':
				SNM_HwMon_TestRS422_485();
				break;
			case 'a':
				SNM_HwMon_TestLoRa();
				break;
			case 'b':
				SNM_HwMon_TestPowerControlModule();
				break;
			case 'f':
                inloop = false;
				printf("See you soon...\r\n");
                flushSerialBuffer();
                break;
        }
    }
    while( inloop );
}

void SNM_HwMon_SerialMenu( void )
{
    uint8_t cmenu;
    bool inloop;
    
    inloop = true;
    
    do
    {
		printf("\r\n");
		printf("------------[Serial Interface Test]------------\r\n");
		printf("> 1. Set to 4800 baud rate.\r\n");
		printf("> 2. Set to 9600 baud rate.\r\n");
		printf("> 3. Set to 14400 baud rate.\r\n");
		printf("> 4. Set to 19200 baud rate.\r\n");
		printf("> 5. Set to 38400 baud rate.\r\n");
		printf("> 6. Set to 57600 baud rate.\r\n");
		printf("> 7. Set to 115200 baud rate.\r\n");
		printf("> 8. Exit.\r\n");
		printf("-------> Select Menu:");
        flushSerialBuffer();
        scanf("%c", &cmenu);
		printf("%c\r\n", cmenu);
        
        switch (cmenu)
        {
            case '1':
				printf("Configure Serial Port to 4800 baud rate.\r\n");
                SpiDebug.baud(4800);
                break;
            case '2':
				printf("Configure Serial Port to 9600 baud rate.\r\n");
                SpiDebug.baud(9600);
                break;
            case '3':
				printf("Configure Serial Port to 14400 baud rate.\r\n");
                SpiDebug.baud(14400);
                break;
            case '4':
				printf("Configure Serial Port to 19200 baud rate.\r\n");
                SpiDebug.baud(19200);
                break;
            case '5':
				printf("Configure Serial Port to 38400 baud rate.\r\n");
                SpiDebug.baud(38400);
                break;
            case '6':
				printf("Configure Serial Port to 57600 baud rate.\r\n");
                SpiDebug.baud(57600);
                break;
            case '7':
				printf("Configure Serial Port to 115200 baud rate.\r\n");
                SpiDebug.baud(115200);
                break;
            case '8':
				printf("Return the top menu...\r\n\n");
                inloop = false;
                flushSerialBuffer();
                break;
        }
    }
    while (inloop);
}
void SNM_HwMon_AnalogInMenu( void )
{
    uint8_t cmenu;
    uint16_t samplingtime;
    int16_t nsamples;
    bool inloop;
    
    inloop = true;
    sample_index = 0;
    samplingtime = 1000;    // default value = 1sec
    nsamples = 1;   // default value
    buffer_full = false;
    
    do
    {
		printf("\r\n");
		printf("------------[Analogue Input Test]------------\r\n");
		printf("> 1. Set Sampling Time( unit: ms)\r\n");
		printf("> 2. Set number of samples(1 ~ 100)\r\n");
		printf("> 3. Start Sampling Analogue Input\r\n");
		printf("> 4. Exit.\r\n");
		printf("-------> Select Menu:");
        flushSerialBuffer();
        scanf("%c", &cmenu);
		printf("%c\r\n", cmenu);
        
        switch (cmenu)
        {
            case '1':
				printf("> Enter the desired sampling time(mSec):\r\n");
                buffer_full = false;
                flushSerialBuffer();
                scanf("%d", &samplingtime);
				printf("===> Set the sampling time to %dmS\r\n\n", samplingtime);
                break;
            case '2':
				printf("> Enter the desired number of samples:\r\n");
                buffer_full = false;
                flushSerialBuffer();
                scanf("%d", &nsamples);
                if (nsamples < 0)
                    nsamples = 0;
                else if (nsamples > 100)
                    nsamples = 100;
                    
				printf("===> Set the number of samples to %d\r\n\n", nsamples);
                sample_index = 0;
                break;
            case '3':
				printf("Start sampling, hit any key to stop!\r\n");
                sample_index = 0;
                while (SpiDebug.readable() == 0)
                {
					printf("Analogue Input = %.2fv\r\n", SNM_HwMon_CalAnalogIn(nsamples, SNM_Drv_ReadBatVoltage() ) );
                    Thread::wait(samplingtime);
                }
                break;
            case '4':
				printf("Return the top menu...\r\n\n");
                inloop = false;
                flushSerialBuffer();
                break;
        }
    }
    while (inloop);
}

float SNM_HwMon_CalAnalogIn( int16_t nsamples, float analogin )
{
    uint16_t idx, ndata;
    float sum, average;
    
    sum = 0;
    analogue_samples[ sample_index++ ] = analogin;
    if ( buffer_full == true )
        ndata = nsamples;
    else
        ndata = sample_index;
        
 //   printf("{%d-%d-%f}", sample_index, ndata, analogin);

    for (idx = 0; idx < ndata; idx++)
    {
        sum += analogue_samples[idx];
    }
    
    if ( ndata > 0 )
        average = sum / ndata;
    else
        average = 0;
        
    if ( sample_index >= nsamples )
    {
        buffer_full = true;
        sample_index = 0;
    }
    
    return average;
}

void flushSerialBuffer(void) 
{ 
    char char1 = 0; 
    while (SpiDebug.readable()) 
    { 
        char1 = SpiDebug.getc(); 
    } 
}


void SNM_HwMon_BuzzerMenu(void)
{
	uint8_t cmenu;
	int16_t nfreq;
	float fPeriod;
	bool inloop;

	inloop = true;

	do
	{
		printf("\r\n");
		printf("------------[Buzzer Test]------------\r\n");

		printf("> 1.To run, Input the frequency(1 KHz ~ 10 KHz)\r\n");
		printf("> 2. Stop Buzzer\r\n");
		printf("> 3. Exit.\r\n");
		printf("-------> Select Menu:");
		flushSerialBuffer();
		scanf("%c", &cmenu);
		printf("%c\r\n", cmenu);

		switch (cmenu)
		{
			case '1':
				printf("> Enter the desired frequeny(1KHz ~ 10KHz):\r\n");
				flushSerialBuffer();
				scanf("%d", &nfreq);
				SNM_Drv_SetBuzzerFreq((float)nfreq);
				printf("===> Set the frequency to %dKHz\r\n\n", nfreq);
				break;
			case '2':
				SNM_Drv_SetBuzzerFreq(0.0f);
				printf("==> Stop Buzzer\r\n");
				break;
			case '3':
				printf("Return the top menu...\r\n\n");
				inloop = false;
				flushSerialBuffer();
				break;
		}
	} 
	while (inloop);
}


void SNM_HwMon_RelayTestMenu(void)
{
	uint8_t cmenu;
	bool inloop;

	inloop = true;

	do
	{
		printf("\r\n");
		printf("------------[Relay Test]------------\r\n");
		printf("> 1. Set RELAY_SET to HIGH \r\n");
		printf("> 2. Set RELAY_SET to LOW\r\n");
		printf("> 3. Set RELAY_RESET to HIGH\r\n");
		printf("> 4. Set RELAY_RESET to LOW\r\n");
		printf("> 5. Exit.\r\n");
		printf("-------> Select Menu:");
		flushSerialBuffer();
		scanf("%c", &cmenu);
		printf("%c\r\n", cmenu);

		switch (cmenu)
		{
		case '1':
			SNM_Drv_SetRelaySetLine(true);
			break;
		case '2':
			SNM_Drv_SetRelaySetLine(false);
			break;
		case '3':
			SNM_Drv_SetRelayResetLine(true);
			break;
		case '4':
			SNM_Drv_SetRelayResetLine(false);
			break;
		case '5':
			printf("Return the top menu...\r\n\n");
			inloop = false;
			flushSerialBuffer();
			break;
		}
	} while (inloop);
}

void SNM_HwMon_ProbeTestMenu(void)
{
	uint8_t cmenu;
	bool inloop;

	inloop = true;

	do
	{
		printf("\r\n");
		printf("------------[Relay Test]------------\r\n");
		printf("> 1. Probe I/F LED.\r\n");
		printf("> 2. Probe I/F Power Enable.\r\n");
		printf("> 3. Read Probe Channels' Det Lines.\r\n");
		printf("> 4. Probe I/F Serial Port.\r\n");
		printf("> 5. Exit.\r\n");
		printf("-------> Select Menu:");
		flushSerialBuffer();
		scanf("%c", &cmenu);
		printf("%c\r\n", cmenu);

		switch (cmenu)
		{
		case '1':
			SNM_HwMon_ProbeLedTest();
			break;
		case '2':
			SNM_HwMon_ProbePowerEnTest();
			break;
		case '3':
			SNM_HwMon_ProbeDetLineTest();
			break;
		case '4':
			SNM_HwMon_ProbeSerialTest();
			break;
		case '5':
			printf("Return the top menu...\r\n\n");
			inloop = false;
			flushSerialBuffer();
			break;
		}
	} while (inloop);
}

void SNM_HwMon_ProbeLedTest(void)
{
	int16_t	idx;

	printf("Turn On LED's:");
	Thread::wait(1000);

	for ( idx = 0; idx < 4; idx++)
	{
		SNM_Drv_Probe_TurnLedOn(idx, true);
		printf("+%d", idx);
		Thread::wait(1000);
	}

	for ( idx = 0; idx < 4; idx++)
	{
		SNM_Drv_Probe_TurnLedOn(idx, false);
		printf("-%d", idx);
		Thread::wait(1000);
	}
}

void SNM_HwMon_ProbePowerEnTest(void)
{
	int16_t	idx;

	printf("Enable Power:");
	Thread::wait(1000);

	for ( idx = 0; idx < 4; idx++)
	{
		SNM_Drv_Probe_EnablePwr(idx, true);
		printf("+%d", idx);
		Thread::wait(1000);
	}

	for ( idx = 0; idx < 4; idx++)
	{
		SNM_Drv_Probe_EnablePwr(idx, false);
		printf("-%d", idx);
		Thread::wait(1000);
	}
}

void SNM_HwMon_ProbeDetLineTest(void)
{
	printf("To stop testing, het any key!\r\n");

	while (SpiDebug.readable() == 0)
	{
		printf("Ch0_Det = %x, Ch1_Det = %x, Ch2_Det = %x, Ch3_Det = %x \r\n"
			, SNM_Drv_Probe_ReadDetLine(0), SNM_Drv_Probe_ReadDetLine(1), SNM_Drv_Probe_ReadDetLine(2), SNM_Drv_Probe_ReadDetLine(3));
		Thread::wait(500);
	}

}


void SNM_HwMon_ProbeSerialTest(void)
{
	uint8_t cmenu, ucPort;
	uint16_t uiBaudrate;
	bool inloop, wink;
	Serial sio_probe(PB_6, PB_7);
	Serial sio_gps(PD_8, PD_9);

	inloop = true;
	wink = true;

	
	SNM_Drv_Probe_Initialize();

	SNM_Drv_EnableProbeMux(true);
	SNM_Drv_MuxSelPort(0);	// Set default

	do
	{
		printf("\r\n");
		printf("------------[Probe Serial Port Test]------------\r\n");
		printf("> 1. Select Port(0 ~ 3) \r\n");
		printf("> 2. Test serial communication\r\n");
		printf("> 3. Exit.\r\n");
		printf("-------> Select Menu:");
		flushSerialBuffer();
		scanf("%c", &cmenu);
		printf("%c\r\n", cmenu);

		switch (cmenu)
		{
			case '1':
				printf("> Select the probe port(0 ~ 3):\r\n");
				flushSerialBuffer();
				scanf("%d", &ucPort);
				SNM_Drv_MuxSelPort(ucPort);
				printf("Select %d Probe Port.\r\n", ucPort);
				break;
			case '2':
				printf(">> To test, open the serial terminal for probe serial port!!!\r\n");
				sio_probe.printf("Type any character in the termianl.\r\n");
				sio_probe.printf("You should find that the typed character is echoed on the terminal.\r\n");

				while (SpiDebug.readable() == 0)
				{
					if (SNM_Drv_Probe_Readable())
						SNM_Drv_Probe_WriteByte(SNM_Drv_Probe_ReadByte());
					else
						Thread::wait(1);
				}
				break;
			case '3':
				printf("Return the top menu...\r\n\n");
				inloop = false;
				flushSerialBuffer();
				break;
		}
	} while (inloop);
}

void SNM_HwMon_TestGps(void)
{
	uint8_t cmenu, ucPort;
	bool inloop, wink;
	DigitalOut dout_gps_pwr_en(PE_15);
	Serial sio_gps(PD_8, PD_9);

	inloop = true;
	wink = true;

	SNM_Drv_Gps_Init();

	do
	{
		printf("\r\n");
		printf("------------[GPS module Test]------------\r\n");
		printf("> 1. Enable GPS power.\r\n");
		printf("> 2. Disable GPS power.\r\n");
		printf("> 3. Receive GPS data\r\n");
		printf("> 4. Exit.\r\n");
		printf("-------> Select Menu:");
		flushSerialBuffer();
		scanf("%c", &cmenu);
		printf("%c\r\n", cmenu);

		switch (cmenu)
		{
		case '1':
			SNM_Drv_Gps_EnablePwr(true);
			printf("Enable GPS module power !!!\r\n");
			break;
		case '2':
			SNM_Drv_Gps_EnablePwr(false);
			printf("Disable GPS module power !!!\r\n");
			break;
		case '3':
			while (SpiDebug.readable() == 0)
			{
				while (SNM_Drv_Gps_Readable())
				{
					printf("%c", SNM_Drv_Gps_ReadByte());
				}
			}
			break;
		case '4':
			printf("Return the top menu...\r\n\n");
			inloop = false;
			flushSerialBuffer();
			break;
		}
	} while (inloop);
}


void SNM_HwMon_TestUsbSerial(void)
{
	USBSerial vserial( 0x1f00, 0x2012, 0x0001, false);
	bool wink = true;

	printf("USB Connected = %d\r\n", vserial.connected());

	while (SpiDebug.readable() == 0)
	{
		if (wink)
		{
			vserial.printf("+");
			wink = false;
		}
		else
		{
			vserial.printf("-");
			wink = true;
		}

		printf("*");
		Thread::wait(500);
	}
}

void SNM_HwMon_TestEeprom(void)
{
	char rdbuf[0x10];
	char wrbuf[0x10];
	int idx;

	SNM_Drv_EEPROM_Init();

	memset(rdbuf, 0xff, 0x10);
	for (idx = 0; idx < 0x10; idx++)
	{
		wrbuf[idx] = 0xb0 + idx;
	}

	SNM_Drv_EEPROM_Write(0x00, wrbuf, 0x10);
	Thread::wait(100);

	SNM_Drv_EEPROM_Read(0x00, rdbuf, 0x10);
	printf("--- Compare written and read data \r\n");

	for (idx = 0; idx < 0x10; idx++)
	{
		printf(" %x - %x\r\n", wrbuf[idx], rdbuf[idx]);
	}
}

void SNM_HwMon_TestRS422_485(void)
{
	uint8_t cmenu, ucPort;
	bool inloop;

	inloop = true;

	SNM_Drv_Rs422Rs485_Init();

	do
	{
		printf("\r\n");
		printf("------------[RS-422 RS485 module Test]------------\r\n");
		printf("> 1. Set Op. Mode to Shut Down Mode.\r\n");
		printf("> 2. Set Op. Mode to RS-485 Tx Mode.\r\n");
		printf("> 3. Set Op. Mode to RS-485 Rx Mode.\r\n");
		printf("> 4. Configure Serial Port.\r\n");
		printf("> 5. Rx Mode Test -- Hit key, stop test.\r\n");
		printf("> 6. Tx Mode Test -- Hit key, stop test.\r\n");
		printf("> 7. Exit.\r\n");
		printf("-------> Select Menu:");
		flushSerialBuffer();
		scanf("%c", &cmenu);
		printf("%c\r\n", cmenu);

		switch (cmenu)
		{
			case '1':
				printf("===> Set Shutdown Mode!\r\n");
				SNM_Drv_Rs422Rs485_SetOpMode(shutdownmode);
				break;
			case '2':
				printf("===> Set Tx Mode!\r\n");
				SNM_Drv_Rs422Rs485_SetOpMode(txmode);
				break;
			case '3':
				printf("===> Set Rx Mode!\r\n");
				SNM_Drv_Rs422Rs485_SetOpMode(rxmode);
				break;
			case '4':
				printf("===> Configure Serial Port of RS422/RS485 device...\r\n");
				SNM_HwMon_ConfigureRs422485SerialPort();
				break;
			case '5':
				printf("===> Waiting for data from RS422/RS485 device...\r\n");
				while (SpiDebug.readable() == 0)
				{
					if (SNM_Drv_Rs422Rs485_Readable() == 1)
					{
						printf("%c", SNM_Drv_Rs422Rs485_ReadByte());
					}
					else
						Thread::wait(1);
				}

				printf("\r\n\n");
				break;
			case '6':
				printf("===> Sending \"Hello World!!!\" message to RS422/RS485 device repeatedly...\r\n");
				while (SpiDebug.readable() == 0)
				{
					SNM_Drv_Rs422Rs485_SetOpMode(txmode);	// set to Tx Mode
					Thread::wait(100);

					SNM_Drv_Rs422Rs485_WriteString("Hello World !!!\r\n");
					Thread::wait(1);

					SNM_Drv_Rs422Rs485_SetOpMode(rxmode);	// return to Default mode (Rx Mode)
				}

				printf("\r\n\n");
				break;
			case '7':
				printf("Return the top menu...\r\n\n");
				inloop = false;
				flushSerialBuffer();
				break;

		}
	} while (inloop);
}

void SNM_HwMon_ConfigureRs422485SerialPort(void)
{
	uint8_t cmenu, ucPort;
	static int baudrate = 9600;
	static int databits = 8;
	static int stopbits = 1;
	static int parity = 0;
	bool inloop;

	inloop = true;

	do
	{
		printf("\r\n");
		printf("------------[Configure RS422 & 485 Serial Port]------------\r\n");
		printf("> 1. Set Baud Rate(2400 ~ 115200baud)\r\n");
		printf("> 2. Set Data Bits(7bits/8bits)\r\n");
		printf("> 3. Set Stop Bits(1bit/2bits)\r\n");
		printf("> 4. Set Parity Bits(0 = none, 1 = odd, 2 = even, 3 = mark, 4 = space)\r\n");
		printf("> 5. Exit.\r\n");
		printf("-------> Select Menu:");
		flushSerialBuffer();
		scanf("%c", &cmenu);
		printf("%c\r\n", cmenu);

		switch (cmenu)
		{
		case '1':
			printf("	- Input baud rate(default = 9600baud): ");
			flushSerialBuffer();
			scanf("%d", &baudrate);
			if ((baudrate < 2400) || (baudrate > 115200))
			{
				baudrate = 9600;	// set default value
			}

			SNM_Drv_Rs422Rs485_SetSioBaudrate(baudrate);
			printf("%d \r\n", baudrate);

			break;
		case '2':
			printf("	- Input databits(7 or 8, default = 8): ");
			flushSerialBuffer();
			scanf("%d", &databits);
			if ((databits != 7) && (databits != 8))
			{
				databits = 8;	// set default value
			}

			SNM_Drv_Rs422Rs485_SetSioFormat(databits, stopbits, parity);

			printf("%d\r\n", databits);
			break;
		case '3':
			printf("	- input stopbits(1 or 2, default = 1): ");
			flushSerialBuffer();
			scanf("%d", &stopbits);
			if ((stopbits != 1) && (stopbits != 2))
			{
				stopbits = 1;	// set default value
			}

			SNM_Drv_Rs422Rs485_SetSioFormat(databits, stopbits, parity);
			printf("%d\r\n", stopbits);
			break;
		case '4':
			printf("	- Input paritybit(0 = none, 1 = odd, 2 = even, 3 = mark or 4 = space, default = 0): ");
			flushSerialBuffer();
			scanf("%d", &parity);
			SNM_Drv_Rs422Rs485_SetSioFormat(databits, stopbits, parity);
			printf("%d\r\n", parity);
			break;
		case '5':
			printf("Return the top menu...\r\n\n");
			inloop = false;
			flushSerialBuffer();
			break;
		}
	} while (inloop);
}

void SNM_HwMon_TestLoRa(void)
{
	uint8_t cmenu, ucPort;
	bool inloop;

	SNM_Drv_LoRa_Initialize();

	inloop = true;

	do
	{
		printf("\r\n");
		printf("------------[LoRa module Test]------------\r\n");
		printf("> 1. Set RF_PWR_EN to LOW.\r\n");
		printf("> 2. Set RF_PWR_EN to HIGH.\r\n");
		printf("> 3. Set RF_RESET_N to LOW.\r\n");
		printf("> 4. Set RF_RESET_N to HIGH.\r\n");
		printf("> 5. Read byte from LoRa module via Serail Connection.\r\n");
		printf("> 6. Exit.\r\n");
		printf("-------> Select Menu:");
		flushSerialBuffer();
		scanf("%c", &cmenu);
		printf("%c\r\n", cmenu);

		switch (cmenu)
		{
			case '1':
				SNM_Drv_LoRa_SetPwrEn(false);
				break;
			case '2':
				SNM_Drv_LoRa_SetPwrEn(true);
				break;
			case '3':
				SNM_Drv_LoRa_SetResetLine(false);
				break;
			case '4':
				SNM_Drv_LoRa_SetResetLine(true);
				break;
			case '5':
				while (SpiDebug.readable() == 0)
				{
					if (SNM_Drv_LoRa_SioReadable() == 1)
					{				
						printf("%c", SNM_Drv_LoRa_SioReadByte());
					}
					else
						Thread::wait(1);
				}

				printf("\r\n\n");
				break;
			case '6':
				printf("Return the top menu...\r\n\n");
				inloop = false;
				flushSerialBuffer();
				break;

		}
	} while (inloop);
}

void SNM_HwMon_ConfigureProbeSerialPort(void)
{
	uint8_t cmenu, ucPort;
	static int baudrate = 9600;
	static int databits = 8;
	static int stopbits = 1;
	static int parity = 0;
	bool inloop;

	inloop = true;

	do
	{
		printf("\r\n");
		printf("------------[Configure Probe Serial Port]------------\r\n");
		printf("> 1. Set Baud Rate(2400 ~ 115200baud)\r\n");
		printf("> 2. Set Data Bits(7bits/8bits)\r\n");
		printf("> 3. Set Stop Bits(1bit/2bits)\r\n");
		printf("> 4. Set Parity Bits(0 = none, 1 = odd, 2 = even, 3 = mark, 4 = space)\r\n");
		printf("> 5. Exit.\r\n");
		printf("-------> Select Menu:");
		flushSerialBuffer();
		scanf("%c", &cmenu);
		printf("%c\r\n", cmenu);

		switch (cmenu)
		{
			case '1':
				printf("	- Input baud rate(default = 9600baud): ");
				flushSerialBuffer();
				scanf("%d", &baudrate);
				if ((baudrate < 2400) || (baudrate > 115200))
				{
					baudrate = 9600;	// set default value
				}

				SNM_Drv_Probe_SetSioBaudrate(baudrate);
				printf("%d \r\n", baudrate);

				break;
			case '2':
				printf("	- Input databits(7 or 8, default = 8): ");
				flushSerialBuffer();
				scanf("%d", &databits);
				if ((databits != 7) && (databits != 8))
				{
					databits = 8;	// set default value
				}

				SNM_Drv_Probe_SetSioFormat(databits, stopbits, parity);

				printf("%d\r\n", databits);
				break;
			case '3':
				printf("	- input stopbits(1 or 2, default = 1): ");
				flushSerialBuffer();
				scanf("%d", &stopbits);
				if ((stopbits != 1) && (stopbits != 2))
				{
					stopbits = 1;	// set default value
				}

				SNM_Drv_Probe_SetSioFormat(databits, stopbits, parity);
				printf("%d\r\n", stopbits);
				break;
			case '4':
				printf("	- Input paritybit(0 = none, 1 = odd, 2 = even, 3 = mark or 4 = space, default = 0): ");
				flushSerialBuffer();
				scanf("%d", &parity);

				SNM_Drv_Probe_SetSioFormat(databits, stopbits, parity);
				printf("%d\r\n", parity);
				break;
			case '5':
				printf("Return the top menu...\r\n\n");
				inloop = false;
				flushSerialBuffer();
				break;
		}
	} while (inloop);
}

void SNM_HwMon_TestPowerControlModule(void)
{
	bool inloop;
	char cmenu;
	int battstatus;

	inloop = true;

	do
	{
		printf("\r\n");
		printf("------------[Power Control Line Test]------------\r\n");
		printf("> 1. Set PS_HOLD to LOW.\r\n");
		printf("> 2. Set PS_HOLD to HIGH.\r\n");
		printf("> 3. Read Battery Status.\r\n");
		printf("> 4. Enter Power Sleep mode.\r\n");
		printf("> 5. Enter Power Deep Sleep mode.\r\n");
		printf("> 6. Exit.\r\n");
		printf("-------> Select Menu:");
		flushSerialBuffer();
		scanf("%c", &cmenu);
		printf("%c\r\n", cmenu);

		switch (cmenu)
		{
			case '1':
				SNM_Drv_EnablePsHold(false);
				break;
			case '2':
				SNM_Drv_EnablePsHold(true);
				break;
			case '3':
				battstatus = SNM_Drv_ReadBattStatus();
				if ( battstatus == 0 )
				{
					printf("Battery Status = None.\r\n");
				}
				else if ( battstatus == 1)
				{
					printf("Battery Status = In Charging.\r\n");
				}
				else if ( battstatus == 2)
				{
					printf("Battery Status = Fully Charged.\r\n");
				}
				else if ( battstatus == 3)
				{
					printf("Battery Status = No Good.\r\n");
				}

				break;
			case '4':
				printf("Entering Power Sleepmode after 1sec.. To return to run mode, hit any key on keyboard!\r\n");
				Thread::wait(500);
				sleep();
				break;
			case '5':
				printf("Entering Power Sleepmode after 1sec.. To return to run mode, hit any key on keyboard!\r\n");
				Thread::wait(500);
				deepsleep();
				break;
			case '6':
				printf("Return the top menu...\r\n\n");
				inloop = false;
				flushSerialBuffer();
				break;

		}
	} while (inloop);
}
