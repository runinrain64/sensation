enum RS422_RS485_OPMODE
{
	shutdownmode,
	txmode,
	rxmode,
};

int16_t SNM_Drv_MuxSelPort(int16_t iSelPort);
void SNM_Drv_EnableProbeMux(bool onoff);
void SNM_Drv_EnablePwrLed(bool onoff);
void SNM_Drv_EnablePsHold(bool onoff);
int SNM_Drv_ReadBattStatus( void );

void SNM_Drv_SetRelayResetLine(bool onoff);
void SNM_Drv_SetRelaySetLine(bool onoff);

float SNM_Drv_ReadBatVoltage( void );
void SNM_Drv_SetBuzzerFreq( float freq );


void SNM_Drv_Gps_EnablePwr( bool onoff );
void SNM_Drv_Gps_Init( void );
int SNM_Drv_Gps_Readable( void );
char SNM_Drv_Gps_ReadByte( void );

void SNM_Drv_Probe_TurnLedOn(int16_t iChannel, bool onoff);
void SNM_Drv_Probe_EnablePwr(int16_t iChannel, bool onoff);
char SNM_Drv_Probe_ReadDetLine(int16_t iChannel);
void SNM_Drv_Probe_Initialize( void );
int SNM_Drv_Probe_Readable( void );
char SNM_Drv_Probe_ReadByte( void );
void SNM_Drv_Probe_WriteByte( char cData );
int SNM_Drv_Probe_SetSioBaudrate(int baudrate);
int SNM_Drv_Probe_SetSioFormat(int databits, int stopbits, int iparity);


void SNM_Drv_EEPROM_Init(void);
void SNM_Drv_EEPROM_Read(uint16_t addr, char * buf, uint16_t nsize);
void SNM_Drv_EEPROM_Write(uint16_t addr, char * buf, uint16_t nsize);
void SNM_Drv_EEPROM_Test(void);

void SNM_Drv_Rs422Rs485_Init(void);
int	SNM_Drv_Rs422Rs485_SetOpMode(enum RS422_RS485_OPMODE opmode);
enum RS422_RS485_OPMODE SNM_Drv_Rs422Rs485_GetOpMode(void);
int SNM_Drv_Rs422Rs485_SetSioBaudrate(int baudrate);
int SNM_Drv_Rs422Rs485_SetSioFormat(int databits, int stopbits, int iparity);

char SNM_Drv_Rs422Rs485_ReadByte(void);
int SNM_Drv_Rs422Rs485_Readable(void);
void SNM_Drv_Rs422Rs485_WriteByte(char cbyte);
void SNM_Drv_Rs422Rs485_WriteString(char *cstr);

void SNM_Drv_LoRa_Initialize(void);
int SNM_Drv_LoRa_SetSioBaudrate(int baudrate);
int SNM_Drv_LoRa_SioReadable(void);
char SNM_Drv_LoRa_SioReadByte(void);
int SNM_Drv_LoRa_SetSioFormat(int databits, int stopbits, int iparity);
void SNM_Drv_LoRa_SetPwrEn(bool onoff);
void SNM_Drv_LoRa_SetResetLine(bool onoff);




