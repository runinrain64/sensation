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
 @file  Sensor_Probe_Handler.cpp
 defines sensor probe handler's functions
 
 @author    shjung
 @date      2017/09/08
 @attention Copyright (c)2017 LETUSTECH - All rights reserved
 */

#include "mbed.h"
#include "typedef.h"
#include "SN_Probe_Interface.h"
#include "SN_Utils.h"
#include "SN_Probe_Handler.h"

Serial SphDebug(USBTX, USBRX);

/*
    Probe Handler Class
        - number of mux
       - CStateMachine
       - SIO class
       - alive
       - sampling time
       - index of timer
*/

/**
    @brief  Construct CStateMachine
*/
CStateMachine::CStateMachine()
{
    runflag = false;
    successflag = false;
    
    pbuf = (uint8_t*)malloc(SPH_COM_BUF_SIZE);
    
    curstate = nxtstate = SPH_STATEMACHINE_IDLE;
}

/**
    @brief  Destruct CStateMachine
*/
CStateMachine::~CStateMachine()
{
    free(pbuf);
}

/**
    @brief  Reset the interal variable of Class
*/

void CStateMachine::reset( void )
{
    runflag = false;
    successflag = false;
    
    curstate = nxtstate = SPH_STATEMACHINE_IDLE;
}


/**
    @brief  make state-machine ready to run.
    @param  cmdtype command code to be run through the machine
    @param  numdat  number of data to be sent or recieved depending on 'direction' param
    @param  direction define the direction of communication. 
            true: node -> probe
            false: probe -> node 
*/
int16_t CStateMachine::ready( uint8_t cmdtype, uint8_t *payload, uint8_t numdat, bool direction )
{
    int16_t ret;
    
    if ( runflag == true )  // if already ruu=nning,
    {
        SphDebug.printf("It is already run!!\r\n");
        ret = -1;
    }
    else
    {
        nxtstate = SPH_STATEMACHINE_CMD;
        dirflag = direction;
        cmd = cmdtype;
        ndat = numdat;
        pbuf = payload;
        runflag = true;
           
        ret = 0;
    }
    
    return ret;
}

#define SPH_DEBUG

/**
    @brief  state-machine which can transcever data with sensor probe.
        It is not run based on thread.
        each instance of this class can remember its states.
*/
CStateMachine::SM_STAT CStateMachine::tranceiver( void )
{
    uint16_t nret, ndat;
    static uint16_t npayload;
    SM_STAT ret;
    uint8_t *payloadbuf, siobuf[2];
    
    if (runflag == true )
    {
        payloadbuf = this->pbuf;
        curstate = nxtstate;
        ret = run;

        switch (curstate)
        {
            case SPH_STATEMACHINE_CMD:
                siobuf[0] = cmd;
                ndat = 1;
                nret = SN_SIO_PutData( siobuf, ndat, 10 * ndat );      // timeout = 1second
                
                if ( nret == ndat ) // if communication is ok
                {
                    nxtstate = SPH_STATEMACHINE_CMDACK;
                }
                else
                    nxtstate = SPH_STATEMACHINE_ABORT;
                
#ifdef SPH_DEBUG
                SphDebug.printf("[SPH_CMD(%d:%d:%d)]\r\n", nret, ndat, nxtstate);
#endif              
                break;
            case SPH_STATEMACHINE_CMDACK:
                ndat = 2;
                nret = SN_SIO_GetData( siobuf, ndat, 10 * ndat);
                
                if ( nret == ndat && siobuf[0] == cmd ) // If communication is ok,
                {
                    acktype = siobuf[0];   // Get ack type
                    npayload = siobuf[1];  // Get data size
                    nxtstate = SPH_STATEMACHINE_DATA;
                }
                else
                {
                    nxtstate = SPH_STATEMACHINE_ABORT;
                }
#ifdef SPH_DEBUG
                SphDebug.printf("[SPH_CMDACK(%d:%d:%d:%d)-%d]\r\n", nret, ndat, acktype, nxtstate, npayload);
#endif
                break;
            case SPH_STATEMACHINE_DATA:
                ndat = npayload;
                nret = SN_SIO_GetData( payloadbuf, ndat, 10 * ndat );  // waiting time = number of data to be recieved.
                
                if ( nret == ndat ) // if all data is received,
                {
                    nxtstate = SPH_STATEMACHINE_END;
                }
                else
                {
                    nxtstate = SPH_STATEMACHINE_END;
                }
#ifdef SPH_DEBUG
                SphDebug.printf("[SPH_DATA(%d:%d:%d)]\r\n", nret, ndat, nxtstate);
#endif                    
                break;
            case SPH_STATEMACHINE_END:
                siobuf[0] = SPH_STATEMACHINE_MSG_END;
                siobuf[1] = CalCrc8Block( SENSOR_UTIL_CRC_INIT, pbuf, npayload);
                
                ndat = 2;
                nret = SN_SIO_PutData( siobuf, ndat, 10 * ndat );      // timeout = 1second
                if ( nret == ndat )
                {
                    nxtstate = SPH_STATEMACHINE_ENDACK;
                }
                else
                {
                    nxtstate = SPH_STATEMACHINE_ABORT;
                }     
#ifdef SPH_DEBUG
                SphDebug.printf("[SPH_END(%d:%d:%d)-%xh]\r\n", nret, ndat, nxtstate, siobuf[1]);
#endif       
                break;
            case SPH_STATEMACHINE_ENDACK:
                ndat = 1;
                nret = SN_SIO_GetData( siobuf, ndat, 10 * ndat );
                if ( nret == ndat && (siobuf[0] == SPH_STATEMACHINE_MSG_ACK || siobuf[0] == SPH_STATEMACHINE_MSG_NACK) )
                {
                    curstate = nxtstate = SPH_STATEMACHINE_IDLE;
                    runflag = false;
                    successflag = (siobuf[0] == SPH_STATEMACHINE_MSG_ACK) ? true:false;
                    ret = done;
                }
                else
                {
                    nxtstate = SPH_STATEMACHINE_ABORT;
                }                        
#ifdef SPH_DEBUG
                SphDebug.printf("[SPH_ENDACK(%d:%d:%d)-%xh]\r\n", nret, ndat, nxtstate, siobuf[0]);
#endif       
                break;
            case SPH_STATEMACHINE_ABORT:
#ifdef SPH_DEBUG
                SphDebug.printf("[SPH_ABORT]\r\n");
#endif
                runflag = false;
                successflag = false;
                ret = aborted;
                break;
            default:
                break;
        }
    }
    else
        ret = alreadyrun;
    
    return ret;           
}

    