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
 @file  Sensor_Node_Manager.cpp
 defines functions of Sensor Node Manager
 
 @author    shjung
 @date      2017/09/08
 @attention Copyright (c)2017 LETUSTECH - All rights reserved
 */
 
#include "mbed.h"
#include "rtos.h"
#include "SN_Probe_Interface.h"
#include "SN_Utils.h"
#include "SN_Probe_Handler.h"
#include "SN_Driver.h"

#define     MAX_SENSOR_PROBE    5

#define VSP_STATEMACHINE_IDLE       0x00
#define VSP_STATEMACHINE_CMD        0x01
#define VSP_STATEMACHINE_CMDRDACK   0x02
#define VSP_STATEMACHINE_CMDWRACK   0x03
#define VSP_STATEMACHINE_RDDATA     0x04
#define VSP_STATEMACHINE_WRDATA     0x05
#define VSP_STATEMACHINE_END        0x06
#define VSP_STATEMACHINE_ENDACK     0x07
#define VSP_STATEMACHINE_ABORT      0xff

#define MAX_SM_BUF_SIZE             0x100

#define NUMBER_SEND_DATA            0x10

uint8_t g_testdata[4][NUMBER_SEND_DATA] = { { 0x75, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f }
                                            , { 0x73, 0x01, 0x22, 0x33, 0x04, 0x05, 0xf6, 0xf7, 0x08, 0xf9, 0xaa, 0xab, 0x0c, 0x0d, 0x0e, 0xaf }
                                            , { 0x98, 0x01, 0x02, 0x23, 0x14, 0x05, 0x06, 0x11, 0x65, 0x21, 0xa2, 0xb0, 0x3c, 0xd1, 0xe0, 0x9f }
                                            , { 0x12, 0x21, 0x20, 0x13, 0xd4, 0x5a, 0x61, 0x72, 0x81, 0x03, 0x0f, 0x0b, 0x01, 0x1d, 0x1e, 0x2f } };
uint8_t g_spSMbuf[MAX_SM_BUF_SIZE];
uint8_t g_spSNMbuf[MAX_SM_BUF_SIZE];

Thread SNM_Thread_ProbeSampler;
Thread VirtualSensorProbe_Thread;

void SNM_ProbeSamplerFunc();
void VirtualSensorProbe_ThreadFunc();

typedef struct {
    unsigned char *pbuf;
    uint16_t msg_type;
} snm_message_t;
 
typedef struct {
    unsigned char *pbuf;
    uint16_t msg_type;
} sph_message_t;

CStateMachine *SPH[5];

MemoryPool<snm_message_t, 64> snm_mpool;
Queue<snm_message_t, 64> snm_queue;

MemoryPool<sph_message_t, 64> sph_mpool;
Queue<sph_message_t, 64> sph_queue;

extern void SNM_HwMon_MainMenu( void );

// main() runs in its own thread in the OS
int main() 
{
    int16_t idx;
        
        // Initialize serial driver
    SpiDebug.baud(115200);

		// Turn on Power LED
	SNM_Drv_EnablePwrLed(true);

		// set PS_HOLD to HIGH
	SNM_Drv_EnablePsHold( true);

	printf("\r\n\r\n Start (%s - %s)...!!\r\n", __DATE__, __TIME__);

	// Run Hardware Test Program
    SNM_HwMon_MainMenu();
         
    SN_SIO_Init();
    
    /* start threads */
    SNM_Thread_ProbeSampler.start(SNM_ProbeSamplerFunc);
    VirtualSensorProbe_Thread.start(VirtualSensorProbe_ThreadFunc);
    
    /* Create message queue */
    
    /* Create Sensor Probe State Machine */

    for ( idx = 0; idx < MAX_SENSOR_PROBE; idx++)
    {
        SPH[idx] = new CStateMachine;
    }
    
        
/*    
    while (true) {
        printf("!");
        snm_message_t *snm_msg = snm_mpool.alloc();
        snm_msg->pbuf = NULL;
        snm_msg->msg_type = 0x00;
        
        snm_queue.put(snm_msg);
    
        wait(5.0);
        printf("?");
        sph_message_t *sph_msg = sph_mpool.alloc();
        sph_msg->pbuf = NULL;
        sph_msg->msg_type = 0x01;
        
        sph_queue.put(sph_msg);
        
        wait(2.0);
    }
    */
    while(true);
}


void SNM_ProbeSamplerFunc()
{
    osEvent evt;
    int16_t iprobe;
    CStateMachine::SM_STAT smresult;
    
    iprobe = 0;
    
    while (true) // TODO - use the event of each timer of sensor probe
    {
        for (iprobe = 0; iprobe < MAX_SENSOR_PROBE; iprobe++)
        {
/*
            sph_message_t *sph_msg = sph_mpool.alloc();
            sph_msg->pbuf = NULL;
            sph_msg->msg_type = 0x01;
            sph_queue.put(sph_msg);
*/
			printf("Start State Machine[%d]---\r\n", iprobe);
            SPH[iprobe]->reset();
            SPH[iprobe]->ready(SPH_STATEMACHINE_MSG_RDCMD, g_spSNMbuf, 10, true);

            do
            {
                smresult = SPH[iprobe]->tranceiver();
                Thread::wait(100);
            }
            while ( smresult == CStateMachine::run );
            
            Thread::wait(2000);
        }
  /*      
        evt = snm_queue.get();
        
        if (evt.status == osEventMessage)
        {
            snm_message_t *msg = (snm_message_t*)evt.value.p;
            
            SnmDebug.printf("SNM_Thread(%d)!\n\r", msg->msg_type);
            snm_mpool.free(msg);
        }
    */
    }
}


void VirtualSensorProbe_ThreadFunc( void )
{
//    osEvent evt;
    uint8_t rxcmd, crc8, cnt;
    uint8_t curSMstate, nxtSMstate, ndat, nWRdat;
    uint16_t nret;
    bool crcvalid;
    
	printf("START SENSOR PROBE THREAD!!!!!..\r\n");
    
    curSMstate = nxtSMstate = SPH_STATEMACHINE_IDLE;
    
    while (true) 
    {
 /*
        evt = sph_queue.get();
        sph_message_t *msg = (sph_message_t*)evt.value.p;
        sph_mpool.free(msg);
 */       
        curSMstate = nxtSMstate;
        switch (curSMstate)
        {
            case VSP_STATEMACHINE_IDLE:     // Waiting for any command from Sensor Node
                nret = SP_SIO_GetData( g_spSMbuf, 1, 10 );
                if (nret == 1)
                {
                    rxcmd = g_spSMbuf[0]; // Get the command byte
                    nxtSMstate = VSP_STATEMACHINE_CMD;
                }
				printf("<SP_IDLE(%d:%xH:%d)>\r\n", nret, rxcmd, nxtSMstate);
                break;
            case VSP_STATEMACHINE_CMD:      // After getting command message, process it (read command or write command)
                nret = 0;
                nWRdat = 0;
                if (rxcmd == SPH_STATEMACHINE_MSG_RDCMD)    // if command is 'read'
                {
                    nxtSMstate = VSP_STATEMACHINE_CMDRDACK;
                }
                else if (rxcmd == SPH_STATEMACHINE_MSG_WRCMD)   // if command is 'write'
                {
                    nret = SP_SIO_GetData( g_spSMbuf, 1, 10 );    // Get number of data to be written by Sensor Node
                    if ( nret == 1 )
                    {
                        nWRdat = g_spSMbuf[0];        // store number of data written by SN
                        nxtSMstate = VSP_STATEMACHINE_CMDWRACK;
                    }
                    else    // something wrong in serial communcation
                    {
                        nxtSMstate = VSP_STATEMACHINE_ABORT;
                    }
                }
                else    // invalid message
                {
                    nxtSMstate = VSP_STATEMACHINE_ABORT;
                }
				printf("<SP_CMD(%d:%d:%d)>\r\n", nret, nWRdat, nxtSMstate);
                break;
            case VSP_STATEMACHINE_CMDRDACK: 
                // Process ACK to Read command by echoing "RD CMD byte" & sending "# of data sent to Sensor Node by Sensor Probe"
                g_spSMbuf[0] = rxcmd;
                g_spSMbuf[1] = NUMBER_SEND_DATA;  // number of data to send to Sensor Node. this value is just for test.
                ndat = 2;

                nret = SP_SIO_PutData(g_spSMbuf, ndat, 10 );
                if ( nret == ndat )
                {
                    nxtSMstate = VSP_STATEMACHINE_RDDATA;
                }
                else    // something wrong in serial communcation
                {
                    nxtSMstate = VSP_STATEMACHINE_ABORT;
                }
				printf("<SP_CMDRDACK(%d:%d:%d)>\r\n", nret, ndat, nxtSMstate);
                break;
            case VSP_STATEMACHINE_CMDWRACK: 
                // Process Write ACK by echoing "WR CMD byte" & "# of data written to Sensor Probe by Sensor Node" 
                g_spSMbuf[0] = rxcmd;
                g_spSMbuf[1] = nWRdat;  // number of data to send to Sensor Node. this value is just for test.
                ndat = 2;

                nret = SP_SIO_PutData(g_spSMbuf, ndat, 10 * ndat );
                if ( nret == ndat )
                {
                    nxtSMstate = VSP_STATEMACHINE_WRDATA;
                }
                else    // something wrong in serial communcation
                {
                    nxtSMstate = VSP_STATEMACHINE_ABORT;
                }
				printf("<SP_CMDWRACK(%d:%d:%d)>\r\n", nret, ndat, nxtSMstate);
                break;
            case VSP_STATEMACHINE_RDDATA:
                    // Sensor Probe will send data to Sensor Node 
                ndat = NUMBER_SEND_DATA;
                nret = SP_SIO_PutData( g_testdata[cnt++%4], ndat, 10 * ndat);
                
                if (nret == ndat)
                {
                    nxtSMstate = VSP_STATEMACHINE_END;
                }
                else    // something wrong in serial communcation
                {
                    nxtSMstate = VSP_STATEMACHINE_ABORT;
                }
				printf("<SP_RDDATA(%d:%d:%d)>\r\n", nret, ndat, nxtSMstate);
                break;
            case VSP_STATEMACHINE_WRDATA:
                // Sensor Probe will read data written by Sensor Node
                nret = SP_SIO_GetData( g_spSMbuf, nWRdat, 10 * nWRdat);
                
                if (nret != nWRdat)
                {
                    nxtSMstate = VSP_STATEMACHINE_ABORT;
                }
                else    // something wrong in serial communcation
                {
                    nxtSMstate = VSP_STATEMACHINE_ABORT;
                }
				printf("SP_WRDATA(%d:%d:%d)>\r\n", nret, nWRdat, nxtSMstate);
                break;
            case VSP_STATEMACHINE_END:
                // Waiting for ACK message & CRC value. In case of Read command, 
                // CRC8 will be calculated by Sensor Node using data sent by Sensor Probe.
                // In case of Write Command, CRC8 will be calculated by Sensor Node using data sent by Sensor Node. 
                ndat = 2;
                nret = SP_SIO_GetData(g_spSMbuf, ndat, 10 * ndat);  // Get ACK message & "CRC value"
                if (nret == ndat)
                {
                    if ( g_spSMbuf[0] == SPH_STATEMACHINE_MSG_END )
                    {
                        crc8 = g_spSMbuf[1];
                            /* TODO: Check CRC value */
                        nxtSMstate = VSP_STATEMACHINE_ENDACK;
                    }
                    else    // invalid message
                    {
                        nxtSMstate = VSP_STATEMACHINE_ABORT;
                    }
                }
                else    // something wrong in serial communcation
                {
                    nxtSMstate = VSP_STATEMACHINE_ABORT;
                }
				printf("<SP_END(%d:%xh/%xh:%d)-%xh>\r\n", nret, crc8, crc8, nxtSMstate,g_spSMbuf[0]);
                break;
            case VSP_STATEMACHINE_ENDACK:
                // According to the result of validating CRC, send ACK or NACK message
                g_spSMbuf[0] = (crcvalid == true) ? SPH_STATEMACHINE_MSG_ACK:SPH_STATEMACHINE_MSG_NACK ;
                ndat = 1;
                nret = SP_SIO_PutData( g_spSMbuf, ndat, 10 * ndat);
                if ( nret == ndat )
                {
                    nxtSMstate = VSP_STATEMACHINE_IDLE;
                }
                else    // something wrong in serial communcation
                {
                    nxtSMstate = VSP_STATEMACHINE_ABORT;
                }
				printf("<SP_ENDACK(%d:%d:%d)>\r\n", nret, ndat, nxtSMstate);
                break;
            case VSP_STATEMACHINE_ABORT:
				printf("<SM Aborted!>\r\n");
                nxtSMstate = VSP_STATEMACHINE_IDLE;
                break;
        }
/*
        evt = sph_queue.get();
        
        if (evt.status == osEventMessage)
        {
            sph_message_t *msg = (sph_message_t*)evt.value.p;
            
//            SnmDebug.printf("SPH_Thread(%d)!\n\r", msg->msg_type);
            sph_mpool.free(msg);
        }
    */
    }
}
