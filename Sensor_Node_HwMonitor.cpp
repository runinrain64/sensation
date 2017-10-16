#include "mbed.h"
#include "rtos.h"
#include "USBSerial.h"
#include "Sensor_Probe_IF.h"
#include "Sensor_Utils.h"
#include "Sensor_Probe_Handler.h"
#include "Sensor_Node_Driver.h"

extern AnalogIn ain_batt_v_check;
extern PwmOut pwm_buzz;
extern DigitalOut dout_relay_set;
extern DigitalOut dout_relay_reset;
//extern Serial sio_gps;

void SNM_HwMon_AnalogInMenu( void );
void SNM_HwMon_SerialMenu( void );
float SNM_HwMon_CalAnalogIn( int16_t nsamples, float analogin );
void flushSerialBuffer(void);

void SNM_HwMon_BuzzerMenu(void);
void SNM_HwMon_StartBuzzer(float);
void SNM_HwMon_StopBuzzer(void);

void SNM_HwMon_RelayTestMenu(void);
void SNM_HwMon_RelayTest_Set(bool boFlag);
void SNM_HwMon_RelayTest_Reset(bool boFlag);

void SNM_HwMon_GpsRxTest(void);

void SNM_HwMon_ProbeLedTest(void);
void SNM_HwMon_ProbePowerEnTest(void);
void SNM_HwMon_ProbeSerialTest(void);
void SNM_HwMon_TestUsbSerial(void);



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
        DbgPrint("\r\n\n\n");
        DbgPrint("***********<<Main Menu>>***********\r\n");
        DbgPrint("1. Serial Menu.\r\n");
        DbgPrint("2. Analogue In Menu.\r\n");
		DbgPrint("3. Buzzer Test.\r\n");
		DbgPrint("4. Relay Test.\r\n");
		DbgPrint("5. GPS Receiving Test.\r\n");
		DbgPrint("6. Test Probe's LED.\r\n");
		DbgPrint("7. Test Probe's Power Enable.\r\n");
		DbgPrint("8. Test Probe Serial Port.\r\n");
		DbgPrint("9. Test USB Serial Port.\r\n");
		DbgPrint("f. Exit.\r\n");
		DbgPrint("-------> Select Menu:");
        flushSerialBuffer();
        SpiDebug.scanf("%c", &cmenu);
        DbgPrint("\r\n\n");
    
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
				SNM_HwMon_GpsRxTest();
				break;
			case '6':
				SNM_HwMon_ProbeLedTest();
				break;
			case '7':
				SNM_HwMon_ProbePowerEnTest();
				break;
			case '8':
				SNM_HwMon_ProbeSerialTest();
				break;
			case '9':
				SNM_HwMon_TestUsbSerial();
				break;
			case 'f':
                inloop = false;
                DbgPrint("See you soon...\r\n");
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
        DbgPrint("\r\n");
        DbgPrint("------------[Serial Interface Test]------------\r\n");
        DbgPrint("> 1. Set to 4800 baud rate.\r\n");
        DbgPrint("> 2. Set to 9600 baud rate.\r\n");
        DbgPrint("> 3. Set to 14400 baud rate.\r\n");
        DbgPrint("> 4. Set to 19200 baud rate.\r\n");
        DbgPrint("> 5. Set to 38400 baud rate.\r\n");
        DbgPrint("> 6. Set to 57600 baud rate.\r\n");
        DbgPrint("> 7. Set to 115200 baud rate.\r\n");
        DbgPrint("> 8. Exit.\r\n");
        DbgPrint("-------> Select Menu:")
        flushSerialBuffer();
        SpiDebug.scanf("%c", &cmenu);
        DbgPrint("\r\n");
        
        switch (cmenu)
        {
            case '1':
                DbgPrint("Configure Serial Port to 4800 baud rate.\r\n");
                SpiDebug.baud(4800);
                break;
            case '2':
                DbgPrint("Configure Serial Port to 9600 baud rate.\r\n");
                SpiDebug.baud(9600);
                break;
            case '3':
                DbgPrint("Configure Serial Port to 14400 baud rate.\r\n");
                SpiDebug.baud(14400);
                break;
            case '4':
                DbgPrint("Configure Serial Port to 19200 baud rate.\r\n");
                SpiDebug.baud(19200);
                break;
            case '5':
                DbgPrint("Configure Serial Port to 38400 baud rate.\r\n");
                SpiDebug.baud(38400);
                break;
            case '6':
                DbgPrint("Configure Serial Port to 57600 baud rate.\r\n");
                SpiDebug.baud(57600);
                break;
            case '7':
                DbgPrint("Configure Serial Port to 115200 baud rate.\r\n");
                SpiDebug.baud(115200);
                break;
            case '8':
                DbgPrint("Return the top menu...\r\n\n");
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
        DbgPrint("\r\n");
        DbgPrint("------------[Analogue Input Test]------------\r\n");
        DbgPrint("> 1. Set Sampling Time( unit: ms)\r\n");
        DbgPrint("> 2. Set number of samples(1 ~ 100)\r\n");
        DbgPrint("> 3. Start Sampling Analogue Input\r\n");
        DbgPrint("> 4. Exit.\r\n");
        DbgPrint("-------> Select Menu:")
        flushSerialBuffer();
        SpiDebug.scanf("%c", &cmenu);
        DbgPrint("\r\n");
        
        switch (cmenu)
        {
            case '1':
                DbgPrint("> Enter the desired sampling time(mSec):\r\n");
                buffer_full = false;
                flushSerialBuffer();
                SpiDebug.scanf("%d", &samplingtime);
                DbgPrint("===> Set the sampling time to %dmS\r\n\n", samplingtime);
                break;
            case '2':
                DbgPrint("> Enter the desired number of samples:\r\n");
                buffer_full = false;
                flushSerialBuffer();
                SpiDebug.scanf("%d", &nsamples);
                if (nsamples < 0)
                    nsamples = 0;
                else if (nsamples > 100)
                    nsamples = 100;
                    
                DbgPrint("===> Set the number of samples to %d\r\n\n", nsamples);
                sample_index = 0;
                break;
            case '3':
                DbgPrint("Start sampling, hit any key to stop!\r\n");
                while (SpiDebug.readable() == 0)
                {
                    DbgPrint("Analogue Input = %.2fv\r\n", SNM_HwMon_CalAnalogIn(nsamples, ain_batt_v_check.read()) * 3.3 );
                    Thread::wait(samplingtime);
                }
                break;
            case '4':
                DbgPrint("Return the top menu...\r\n\n");
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
        
 //   DbgPrint("{%d-%d-%f}", sample_index, ndata, analogin);

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
		DbgPrint("\r\n");
		DbgPrint("------------[Buzzer Test]------------\r\n");

		DbgPrint("> 1. Input the frequency(1 KHz ~ 10 KHz)\r\n");
		DbgPrint("> 2. Start Buzzer(0.5 duty rate)\r\n");
		DbgPrint("> 3. Stop Buzzer\r\n");
		DbgPrint("> 4. Exit.\r\n");
		DbgPrint("-------> Select Menu:")
		flushSerialBuffer();
		SpiDebug.scanf("%c", &cmenu);
		DbgPrint("\r\n");
		switch (cmenu)
		{
			case '1':
				DbgPrint("> Enter the desired frequeny(1KHz ~ 10KHz):\r\n");
				flushSerialBuffer();
				SpiDebug.scanf("%d", &nfreq);
				if (nfreq < 0)
					nfreq = 0;
				else if (nfreq > 10)
					nfreq = 100;
				fPeriod = (float)(1 / (float)(nfreq*1000));
				pwm_buzz.period(fPeriod);
				DbgPrint("===> Set the frequency to %d(%fs)\r\n\n", nfreq, fPeriod);
				break;
			case '2':
				SNM_HwMon_StartBuzzer(0.5f);
				DbgPrint("==> Start Buzzer\r\n");
				break;
			case '3':
				SNM_HwMon_StopBuzzer();
				break;
			case '4':
				DbgPrint("Return the top menu...\r\n\n");
				inloop = false;
				flushSerialBuffer();
				break;
		}
	} 
	while (inloop);
}

void SNM_HwMon_StartBuzzer(float fPeriod)
{
	if (fPeriod > 1.0)
		fPeriod = 1.0;

	pwm_buzz.write(fPeriod);
}

void SNM_HwMon_StopBuzzer(void)
{
	pwm_buzz.write(0.0f);
}

void SNM_HwMon_RelayTestMenu(void)
{
	uint8_t cmenu;
	bool inloop;

	inloop = true;

	do
	{
		DbgPrint("\r\n");
		DbgPrint("------------[Relay Test]------------\r\n");
		DbgPrint("> 1. Set RELAY_SET to HIGH \r\n");
		DbgPrint("> 2. Set RELAY_SET to LOW\r\n");
		DbgPrint("> 3. Set RELAY_RESET to HIGH\r\n");
		DbgPrint("> 4. Set RELAY_RESET to LOW\r\n");
		DbgPrint("> 5. Exit.\r\n");
		DbgPrint("-------> Select Menu:")
			flushSerialBuffer();
		SpiDebug.scanf("%c", &cmenu);
		DbgPrint("\r\n");

		switch (cmenu)
		{
		case '1':
			SNM_HwMon_RelayTest_Set(true);
			break;
		case '2':
			SNM_HwMon_RelayTest_Set(false);
			break;
		case '3':
			SNM_HwMon_RelayTest_Reset(true);
			break;
		case '4':
			SNM_HwMon_RelayTest_Reset(false);
			break;
		case '5':
			DbgPrint("Return the top menu...\r\n\n");
			inloop = false;
			flushSerialBuffer();
			break;
		}
	} while (inloop);
}

void SNM_HwMon_RelayTest_Set(bool boFlag)
{
	if (boFlag == true)
		dout_relay_set = 1;
	else
		dout_relay_set = 0;
}

void SNM_HwMon_RelayTest_Reset(bool boFlag)
{
	if (boFlag == true)
		dout_relay_reset = 1;
	else
		dout_relay_reset = 0;
}


void SNM_HwMon_GpsRxTest(void)
{
	while (SpiDebug.readable() == 0)
	{
/*		if (sio_gps.readable() != 0)
			DbgPrint("%c", sio_gps.getc()); */
	} 
}

void SNM_HwMon_ProbeLedTest(void)
{
	DbgPrint("Turn On:");
	Thread::wait(1000);

	SNM_Drv_Do_LedEnCh1(true);	
	DbgPrint("+1");
	Thread::wait(1000);

	SNM_Drv_Do_LedEnCh2(true);
	DbgPrint("+2");
	Thread::wait(1000);

	SNM_Drv_Do_LedEnCh3(true);
	DbgPrint("+3");
	Thread::wait(1000);

	SNM_Drv_Do_LedEnCh4(true);
	DbgPrint("+4\r\n");
	Thread::wait(1000);

	DbgPrint("Turn Off:");
	Thread::wait(1000);
	
	SNM_Drv_Do_LedEnCh1(false);
	DbgPrint("-1");
	Thread::wait(1000);
	
	SNM_Drv_Do_LedEnCh2(false);
	DbgPrint("-2");
	Thread::wait(1000);
	
	SNM_Drv_Do_LedEnCh3(false);
	DbgPrint("-3");
	Thread::wait(1000);
	
	SNM_Drv_Do_LedEnCh4(false);
	DbgPrint("-4\r\n");
	Thread::wait(1000);
}

void SNM_HwMon_ProbePowerEnTest(void)
{
	DbgPrint("Enable Power:");
	Thread::wait(1000);

	SNM_Drv_Do_PwrEnCh1(true);
	DbgPrint("+1");
	Thread::wait(1000);

	SNM_Drv_Do_PwrEnCh2(true);
	DbgPrint("+2");
	Thread::wait(1000);

	SNM_Drv_Do_PwrEnCh3(true);
	DbgPrint("+3");
	Thread::wait(1000);

	SNM_Drv_Do_PwrEnCh4(true);
	DbgPrint("+4\r\n");
	Thread::wait(1000);

	DbgPrint("Disable Power:");
	Thread::wait(1000);

	SNM_Drv_Do_PwrEnCh1(false);
	DbgPrint("-1");
	Thread::wait(1000);

	SNM_Drv_Do_PwrEnCh2(false);
	DbgPrint("-2");
	Thread::wait(1000);

	SNM_Drv_Do_PwrEnCh3(false);
	DbgPrint("-3");
	Thread::wait(1000);

	SNM_Drv_Do_PwrEnCh4(false);
	DbgPrint("-4\r\n");
	Thread::wait(1000);
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

	sio_probe.format(8, SerialBase::None, 1);
	sio_probe.baud(9600);
	sio_gps.baud(4800);

	SNM_Drv_EnableProbeMux(true);
	SNM_Drv_MuxSelPort(0);	// Set default

	do
	{
		DbgPrint("\r\n");
		DbgPrint("------------[Probe Serial Port Test]------------\r\n");
		DbgPrint("> 1. Select Port(0 ~ 3) \r\n");
//		DbgPrint("> 2. Set Baud Rate\r\n");
		DbgPrint("> 3. Send A Character\r\n");
		DbgPrint("> 5. Exit.\r\n");
		DbgPrint("-------> Select Menu:");
		flushSerialBuffer();
		SpiDebug.scanf("%c", &cmenu);
		DbgPrint("\r\n");

		switch (cmenu)
		{
			case '1':
				DbgPrint("> Select the probe port(0 ~ 3):\r\n");
				flushSerialBuffer();
				SpiDebug.scanf("%d", &ucPort);
				SNM_Drv_MuxSelPort(ucPort);
				DbgPrint("Select %d Probe Port.\r\n", ucPort);
				break;
			case '2':
				DbgPrint("> Input Baudrate(4800/9600/38400/57600/115200):\r\n");
				flushSerialBuffer();
				SpiDebug.scanf("%d", &uiBaudrate);
//				sio_probe.baud(uiBaudrate);
//				sio_gps.baud(uiBaudrate);
				DbgPrint("Set %d Baudrate\r\n", uiBaudrate);
				break;
			case '3':
				while (SpiDebug.readable() == 0)
				{
					if (wink)
					{
						sio_probe.putc(0x55);
						wink = false;
					}
					else
					{
						sio_probe.putc(0xaa);
						wink = true;
					}

					DbgPrint("*");
					Thread::wait(500);
				}
			case '4':
				SNM_HwMon_RelayTest_Reset(false);
				break;
			case '5':
				DbgPrint("Return the top menu...\r\n\n");
				inloop = false;
				flushSerialBuffer();
				break;
		}
	} while (inloop);
}

void SNM_HwMon_TestUsbSerial(void)
{
	USBSerial vserial;
	bool wink = true;

	DbgPrint("Under Construction!!!\r\n");
	/*
	DbgPrint("USB Connected = %d\r\n", vserial.connected());

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

		DbgPrint("*");
		Thread::wait(500);
	}
	*/
}