void SNM_Drv_Do_LedEnCh1(bool onoff);
void SNM_Drv_Do_LedEnCh2(bool onoff);
void SNM_Drv_Do_LedEnCh3(bool onoff);
void SNM_Drv_Do_LedEnCh4(bool onoff);

void SNM_Drv_Do_PwrEnCh1(bool onoff);
void SNM_Drv_Do_PwrEnCh2(bool onoff);
void SNM_Drv_Do_PwrEnCh3(bool onoff);
void SNM_Drv_Do_PwrEnCh4(bool onoff);

int16_t SNM_Drv_MuxSelPort(uint16_t uiSelPort);
void SNM_Drv_EnableProbeMux(bool onoff);
void SNM_Drv_EnablePwrLed(bool onoff);

void SNM_Drv_EEPROM_Read(uint16_t addr, int8_t * buf, uint16_t nsize);
void SNM_Drv_EEPROM_Write(uint16_t addr, int8_t * buf, uint16_t nsize);


