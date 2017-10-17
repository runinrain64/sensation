#define SPH_STATEMACHINE_IDLE       0x00
#define SPH_STATEMACHINE_CMD        0x01
#define SPH_STATEMACHINE_CMDACK     0x02
#define SPH_STATEMACHINE_DATA       0x03
#define SPH_STATEMACHINE_END        0x04
#define SPH_STATEMACHINE_ENDACK     0x05
#define SPH_STATEMACHINE_ABORT      0xff

#define SPH_STATEMACHINE_MSG_RDCMD  0x05
#define SPH_STATEMACHINE_MSG_WRCMD  0x0a
#define SPH_STATEMACHINE_MSG_END    0x50
#define SPH_STATEMACHINE_MSG_ACK    0xa5
#define SPH_STATEMACHINE_MSG_NACK   0x5a
#define SPH_COM_BUF_SIZE            0x100

#define SPH_STATEMACHINE_ALREAYRUN  -1
#define SPH_STATEMACHINE_RUN        0
#define SPH_STATEMACHINE_RUN        0


class CStateMachine
{
private:
    uint8_t curstate;
    uint8_t nxtstate;
    uint8_t acktype;
    uint8_t ndat;
    uint8_t cmd;
    uint8_t *pbuf;
    bool    dirflag;    // true - node -> probe
    bool    runflag;
    bool    successflag;
public:
    CStateMachine();
    ~CStateMachine();
    enum SM_STAT { aborted = -2, alreadyrun = -1, run = 0, done = 1 };
                
    void reset( void );
    int16_t ready( uint8_t cmdtype, uint8_t *payload, uint8_t numdat, bool direction );
    
    SM_STAT tranceiver( void );
};