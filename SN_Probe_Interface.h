#define SIO_CFG_BAUD_4800   0x00
#define SIO_CFG_BAUD_9600   0x01
#define SIO_CFG_BAUD_19200  0x02

#define SIO_CFG_DATA_5BITS  0x00
#define SIO_CFG_DATA_6BITS  0x01
#define SIO_CFG_DATA_7BTIS  0x02
#define SIO_CFG_DATA_8BTIS  0x03

#define SIO_CFG_PAR_NONE    0x00
#define SIO_CFG_PAR_EVEN    0x01
#define SIO_CFG_PAR_ODD     0x02

#define SIO_CFG_STOP_1BIT   0x00
#define SIO_CFG_STOP_2BIT   0x01

#define SIO_CFG_CNTR_OFF    0x00
#define SIO_CFG_CNTR_ON     0x01

#define SIO_EMUL            1


void SN_SIO_Init( void );

int16_t SN_SIO_Configure( uint8_t nprobe, uint8_t baudrate, uint8_t datlen, uint8_t stopbit, uint8_t parity, bool flowctrl);
uint16_t SN_SIO_PutData( uint8_t *pbuf, uint16_t ndat, uint16_t timeout);
uint16_t SN_SIO_GetData( uint8_t *pbuf, uint16_t maxndat, uint16_t timeout);

#ifdef  SIO_EMUL
    uint16_t SP_SIO_PutData( uint8_t *pbuf, uint16_t ndat, uint16_t timeout);
    uint16_t SP_SIO_GetData( uint8_t *pbuf, uint16_t maxndat, uint16_t timeout);
#endif
